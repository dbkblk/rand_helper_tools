#include <iostream>
#include <QtCore>
#include <QtWidgets>
#include <QtXml>
#include <QDebug>
#include "w_main.h"

namespace constants {
const QString VERSION = "2.0.0";
}

class languages
{
public:
    void ParseDocument(QString input_file, QString int_lang);
    void ParseAllFiles(QString language);
    void ImportAllDocuments(bool all_files);
    void ImportDocumentToAll(QString int_lang, QStringList file_list);
    void ImportDocumentToSameName(QString int_lang, QStringList file_list);
    QStringList ListTags(QString dir);
    void CleanFiles();
    void SortCategories();
    void SortCategoriesExperimental();
    QStringList FindUnusedTags();
    QString ConvertLatin1ToCharset(QString language, QString string);
    QString ConvertCharsetToLatin1(QString language, QString string);
    void SorterHelper(QString prefix);
    QString ConvertStringToCiv4(QString string);
    void ConvertCiv4ToUTF8(QString file);
    void ConvertUTF8ToCiv4(QString file);
    QStringList CheckImportLanguages();
    void RemoveLanguage();
    QStringList ListLanguages(QString dir);
    QString AutomaticLanguageDetection(QString dir);
    void RemoveUnusedTags();
    QString test();

private:
    QString language;
    const char* lang_file;
};

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w_main w;
    w.show();

    return a.exec();
}

/*

void languages::ParseAllFiles(QString int_lang)
{
    // Language settings
    QString language_code;
    if(int_lang == "English"){language_code = "en";}
    else if(int_lang == "Chinese"){language_code = "zh";}
    else if(int_lang == "Finnish"){language_code = "fi";}
    else if(int_lang == "French"){language_code = "fr";}
    else if(int_lang == "German"){language_code = "de";}
    else if(int_lang == "Hungarian"){language_code = "hu";}
    else if(int_lang == "Italian"){language_code = "it";}
    else if(int_lang == "Japanese"){language_code = "ja";}
    else if(int_lang == "Polish"){language_code = "pl";}
    else if(int_lang == "Russian"){language_code = "ru";}
    else if(int_lang == "Spanish"){language_code = "es";}
    else { qDebug() << "Language not supported. Please report on the forum. Aborting..."; return;}
    QString output_dir = "lang/" + language_code + "/";

    // List all files
    qDebug() << "Preparing files...";
    QDir root(".");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList folders;
    folders = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        QFile::copy(*it,*it + "_TEMP_");
        ConvertCiv4ToUTF8(*it);
        ParseDocument(*it,int_lang);
        QFile::remove(*it);
        QFile::rename(*it + "_TEMP_",*it);
    }

    // Check output files
    QStringList output_files;
    QDir output(output_dir);
    output_files = output.entryList(xml_filter, QDir::Files);
    for(QStringList::Iterator it = output_files.begin(); it != output_files.end(); it++)
    {
        QString current = output_dir + *it;
        int not_empty_counter = 0;
        QDomDocument check;
        QFile file_check(current);
        file_check.open(QIODevice::ReadOnly);
        check.setContent(&file_check);
        file_check.close();
        QDomElement node_check = check.firstChildElement("resources").firstChildElement("string");
        for(node_check;!node_check.isNull();node_check = node_check.nextSiblingElement())
        {
            if(!node_check.firstChild().nodeValue().isEmpty())
            {
                not_empty_counter++;
            }
        }

        if(not_empty_counter == 0)
        {
            file_check.remove();
        }
        check.clear();

    }


    qDebug() << int_lang << " language successfully exported to " << output_dir;
}

void languages::ParseDocument(QString input_file, QString int_lang)
{
    QDomDocument input;
    QDomDocument output;

    QString language_code;
    if(int_lang == "English"){language_code = "en";}
    else if(int_lang == "Chinese"){language_code = "zh";}
    else if(int_lang == "Finnish"){language_code = "fi";}
    else if(int_lang == "French"){language_code = "fr";}
    else if(int_lang == "German"){language_code = "de";}
    else if(int_lang == "Hungarian"){language_code = "hu";}
    else if(int_lang == "Italian"){language_code = "it";}
    else if(int_lang == "Japanese"){language_code = "ja";}
    else if(int_lang == "Polish"){language_code = "pl";}
    else if(int_lang == "Russian"){language_code = "ru";}
    else if(int_lang == "Spanish"){language_code = "es";}

    // Getting filenames
    input_file.replace(".XML", ".xml", Qt::CaseSensitive);
    QString input_temp_file = "__temp__" + input_file;
    QFile::copy(input_file,input_temp_file);
    QString output_dir = "lang/" + language_code + "/";
    QString output_file = output_dir + input_file;
    QDir dir;
    dir.mkpath(output_dir);
    bool save = false;

    // Begin process
    QFile file_input(input_temp_file);
    QFile file_output(output_file);
    file_input.open(QIODevice::ReadOnly);
    file_output.open(QIODevice::Truncate | QIODevice::WriteOnly);
    input.setContent(&file_input);
    file_input.close();

    QDomNode declaration = output.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    output.insertBefore(declaration,output.firstChild());

    // Check file integrity
    if (input.firstChildElement("Civ4GameText").firstChildElement("TEXT").isNull())
    {
        qDebug() << input_file << " is not properly formatted";
        file_input.close();
        file_input.remove();
        return;
    }

    // Parsing
    QDomNode root = output.createElement("resources");
    output.appendChild(root);

    // Loop process
    QDomElement tag_el = input.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();

    for(tag_el;!tag_el.isNull();tag_el = tag_el.nextSiblingElement())
    {
        // Read tags

        QString value_tag = tag_el.firstChildElement("Tag").firstChild().nodeValue();

        // Prepare new tag
        QDomElement new_tag = output.createElement("string");
        root.appendChild(new_tag);

        // Check for NULL values or substrings
        if(tag_el.firstChildElement(int_lang).isNull())
        {
            QDomText write_empty_node = output.createTextNode("");
            new_tag.appendChild(write_empty_node);
            new_tag.setAttribute("name", value_tag);
        }
        else if (!tag_el.firstChildElement(int_lang).firstChildElement("Text").firstChild().isNull())
        {
            QString node_text_value = tag_el.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue();
            node_text_value.replace("\"","\\\"");

            new_tag.appendChild(output.createTextNode(node_text_value));
            new_tag.setAttribute("name", value_tag);
            new_tag.setAttribute("gender", tag_el.firstChildElement(int_lang).firstChildElement("Gender").firstChild().nodeValue());
            new_tag.setAttribute("plural", tag_el.firstChildElement(int_lang).firstChildElement("Plural").firstChild().nodeValue());


        }
        else
        {
            QString node_text_value = tag_el.firstChildElement(int_lang).firstChild().nodeValue();
            node_text_value.replace("\"","\\\"");

            QDomText node_value = output.createTextNode(node_text_value);
            new_tag.appendChild(node_value);
            new_tag.setAttribute("name", value_tag);
        }
    }

    file_output.write(output.toByteArray());
    file_input.close();
    file_output.close();

    // Save input if modified
    if(save)
    {
        dir.mkdir("modif");
        input_file = "modif/" + input_file;
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
        file_input.close();
        file_input.rename(input_file);
    }
    QFile::remove(input_temp_file);
}

QStringList languages::CheckImportLanguages()
{
    qDebug() << "Checking directories in 'import/'...";
    QDir dir_import;
    dir_import.setPath("import/");
    QStringList compatible_languages;
    compatible_languages << "fr" << "de" << "it" << "es" << "pl" << "en" << "ja" << "zh" << "ru" << "fi" << "hu";
    QStringList language_list;
    language_list = dir_import.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList language_found;
    for(QStringList::Iterator it = language_list.begin(); it !=language_list.end();it++)
    {
        if(compatible_languages.contains(*it))
        {
            qDebug() << "Found" << *it << "language in 'Import/'";
            language_found << *it;
        }
        else
        {
            qDebug() << "Dir" << *it << "isn't recognized or is incompatible language";
        }
    }
    return language_found;
}

void languages::ImportAllDocuments(bool all_files)
{
    // Prepare files
    qDebug() << "Preparing files...";
    QDir dir_import("imported/");
    dir_import.removeRecursively();
    dir_import.mkpath(".");
    QStringList xml_filter;
    xml_filter << "*.xml";

    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    QDir import_dir("imported/");
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"imported/"+*it);
        ConvertCiv4ToUTF8("imported/"+*it);
    }
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Check languages
    QStringList list_languages = CheckImportLanguages();
    QString language;
    foreach(language, list_languages)
    {
        qDebug() << "Importing" << language << "...";
        if(all_files)
        {
            ImportDocumentToAll(language, import_files);
        }
        else
        {
            ImportDocumentToSameName(language, import_files);
        }

    }

    // Clean folder
    qDebug() << "Cleaning folder...";
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QString imported_path = "imported/" + *it;
        ConvertUTF8ToCiv4(imported_path);

        // Generate hash of original file
        QCryptographicHash orig_crypto(QCryptographicHash::Md5);
        QFile orig_file(*it);
        orig_file.open(QFile::ReadOnly);
        while(!orig_file.atEnd()){
        orig_crypto.addData(orig_file.read(8192));
        }
        QByteArray orig_hash = orig_crypto.result().toHex();
        QString orig_hash_string = orig_hash;
        //qDebug() << "Generated hash " << orig_hash_string << " of " << *it;
        orig_file.close();

        // Generate hash of imported file
        QCryptographicHash imported_crypto(QCryptographicHash::Md5);
        QFile imported_file(imported_path);
        imported_file.open(QFile::ReadOnly);
        while(!imported_file.atEnd()){
        imported_crypto.addData(imported_file.read(8192));
        }
        QByteArray imported_hash = imported_crypto.result().toHex();
        QString imported_hash_string = imported_hash;
        //qDebug() << "Generated hash " << imported_hash_string << " of " << imported_path;
        orig_file.close();
        if(orig_hash_string == imported_hash_string)
        {
            //qDebug() << "Identical" << *it;
            imported_file.remove();
        }
    }
}

void languages::ImportDocumentToAll(QString int_lang, QStringList file_list)
{
    // Translated files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QString translated_files = "import/" + int_lang + "/";
    QDir translated(translated_files);
    QStringList files_translated;
    files_translated = translated.entryList(xml_filter, QDir::Files);

    // Check language
    QString tag_lang = "English";
    if(int_lang == "fr"){tag_lang = "French";};
    if(int_lang == "de"){tag_lang = "German";};
    if(int_lang == "it"){tag_lang = "Italian";};
    if(int_lang == "es"){tag_lang = "Spanish";};
    if(int_lang == "pl"){tag_lang = "Polish";};
    if(int_lang == "ja"){tag_lang = "Japanese";};
    if(int_lang == "zh"){tag_lang = "Chinese";};
    if(int_lang == "ru"){tag_lang = "Russian";};
    if(int_lang == "fi"){tag_lang = "Finnish";};
    if(int_lang == "hu"){tag_lang = "Hungarian";};

    // Print list

    QStringList print_list;

    // Entering loop

    for(QStringList::Iterator it = file_list.begin(); it != file_list.end(); it++)
    {
        int s = 0;

        QString current = "imported/" + *it;

        qDebug() << "Checking " << current;

        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        // Check file integrity
        if (input.firstChildElement("Civ4GameText").firstChildElement("TEXT").isNull())
        {
            qDebug() << current << " is not properly formatted";
            QFile::remove(current);
        }
        else
        {

        // Comparing to each new file
        for(QStringList::Iterator tr = files_translated.begin(); tr != files_translated.end(); tr++)
        {
            QString current_new = translated_files + *tr;
            QDomDocument input_tr;
            QFile file_tr(current_new);
            file_tr.open(QIODevice::ReadOnly);
            input_tr.setContent(&file_tr);
            file_tr.close();

            QDomElement root = input_tr.documentElement();

            QDomElement tag_orig = input.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
            for(tag_orig;!tag_orig.isNull();tag_orig = tag_orig.nextSiblingElement())
            {
                // Compare each occurence with each tag of the translated file
                QString value_tag = tag_orig.firstChildElement("Tag").firstChild().nodeValue();
                QDomElement tag_tr = root.firstChildElement("string").toElement();
                //qDebug() << tag_tr.attribute("name");

                for(tag_tr;!tag_tr.isNull();tag_tr = tag_tr.nextSiblingElement())
                {
                    QString value_tag_tr = tag_tr.attribute("name");
                    //qDebug() << value_tag_tr;

                    if (value_tag == value_tag_tr)
                    {
                        s++;
                        int skip = 0;
                        QString value_text_tr = tag_tr.firstChild().nodeValue();
                        value_text_tr.replace("\\'","\'").replace("\\\"","\"").replace("\\t","[TAB]");
                        QString value_text;

                        if(value_text_tr == tag_orig.firstChildElement("English").firstChild().nodeValue())
                        {
                            skip++;
                        }

                        // Checking tag presence
                        // Sub language marker
                        if (!tag_orig.firstChildElement(tag_lang).firstChildElement().firstChild().nodeValue().isNull())
                        {
                            value_text = tag_orig.firstChildElement(tag_lang).firstChildElement().firstChild().nodeValue();
                        }
                        // No language marker
                        else if(tag_orig.firstChildElement(tag_lang).isNull())
                        {
                            value_text = "";
                        }

                        // Direct language marker
                        else
                        {
                            value_text = tag_orig.firstChildElement(tag_lang).firstChild().nodeValue();
                        }

                        //qDebug() << value_tag << " : " << value_text << " / " << value_text_tr;

                        // Normalize value
                        if(value_text_tr.isEmpty())
                        {
                            value_text_tr = "";
                        }

                        // Compare original and modified strings
                        if(value_text != value_text_tr && skip == 0)
                        {
                                QString operation;
                                operation = "FILE: " + *it + " | TAG: " + value_tag + " | OLD: " + value_text + " -> NEW: " + value_text_tr;
                                print_list << operation;
                                //qDebug() << operation;

                                // Case: original direct value, new subtag value
                                if (tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().isNull() && !tag_tr.attribute("gender").isNull())
                                {
                                    QString new_value_text = value_text_tr;
                                    QString new_value_gender = tag_tr.attribute("gender");
                                    QString new_value_plural = tag_tr.attribute("plural");

                                    // Empty text value, then add subtags
                                    tag_orig.removeChild(tag_orig.firstChildElement(tag_lang).firstChild());
                                    QDomNode new_node_text = input.createElement("Text");
                                    QDomNode new_node_gender = input.createElement("Gender");
                                    QDomNode new_node_plural = input.createElement("Plural");
                                    tag_orig.firstChildElement(tag_lang).firstChild().appendChild(new_node_text);
                                    tag_orig.firstChildElement(tag_lang).firstChild().appendChild(new_node_gender);
                                    tag_orig.firstChildElement(tag_lang).firstChild().appendChild(new_node_plural);

                                    QDomText new_node_text_value = input.createTextNode(new_value_text);
                                    QDomText new_node_gender_value = input.createTextNode(new_value_gender);
                                    QDomText new_node_plural_value = input.createTextNode(new_value_plural);
                                    new_node_text.appendChild(new_node_text_value);
                                    new_node_gender.appendChild(new_node_gender_value);
                                    new_node_plural.appendChild(new_node_plural_value);

                                }
                                // Case: original, new subtag value
                                else if(!tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().isNull()){
                                    tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().setNodeValue(value_text_tr);
                                    // Set gender and plural
                                    if (!tag_tr.attribute("gender").isNull()){
                                    QString new_value_gender = tag_tr.attribute("gender");
                                    tag_orig.firstChildElement(tag_lang).firstChildElement("Gender").firstChild().setNodeValue(new_value_gender);}
                                    if(!tag_tr.attribute("plural").isNull()){
                                    QString new_value_plural = tag_tr.attribute("plural");
                                    tag_orig.firstChildElement(tag_lang).firstChildElement("Plural").firstChild().setNodeValue(new_value_plural);
                                    }
                                }

                                // Case: no original
                                else if(tag_orig.firstChildElement(tag_lang).firstChild().isNull())
                                {
                                    // New subtag value
                                    if (value_text == "" && !tag_tr.attribute("gender").isNull()){
                                        QString new_value_text = value_text_tr;
                                        QString new_value_gender = tag_tr.attribute("gender");
                                        QString new_value_plural = tag_tr.attribute("plural");

                                        // Append nodes
                                        QDomNode new_node_language = input.createElement(tag_lang);
                                        tag_orig.appendChild(new_node_language);
                                        QDomNode new_node_text = input.createElement("Text");
                                        QDomNode new_node_gender = input.createElement("Gender");
                                        QDomNode new_node_plural = input.createElement("Plural");
                                        new_node_language.appendChild(new_node_text);
                                        new_node_language.appendChild(new_node_gender);
                                        new_node_language.appendChild(new_node_plural);

                                        QDomText new_node_text_value = input.createTextNode(new_value_text);
                                        QDomText new_node_gender_value = input.createTextNode(new_value_gender);
                                        QDomText new_node_plural_value = input.createTextNode(new_value_plural);
                                        new_node_text.appendChild(new_node_text_value);
                                        new_node_gender.appendChild(new_node_gender_value);
                                        new_node_plural.appendChild(new_node_plural_value);
                                    }

                                    // New direct value
                                    else {
                                    QDomNode new_tag = input.createElement(tag_lang);
                                    tag_orig.appendChild(new_tag);
                                    QDomText new_value = input.createTextNode(value_text_tr);
                                    new_tag.appendChild(new_value);
                                    }
                                }

                                // Case: original, direct value
                                else
                                {
                                    //qDebug() << "here";
                                    tag_orig.firstChildElement(tag_lang).firstChild().setNodeValue(value_text_tr);
                                }
                            }
                        }
                    }

                }
            }
        }

        // Save to file
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream ts(&file_input);
        input.save(ts, 3);
        file_input.close();
    }

    // Output modified list
    QString print_value;
    QString print_file_name = "imported/imported_" + int_lang + "_values.txt";
    QFile print_file(print_file_name);

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, print_list)
            {
                stream << print_value << endl;
            }
        }
        print_file.close();

        qDebug() << "\n\nFiles successfully processed. Modified files are in 'imported/'.\nA report of the modified values have been generated in 'imported/imported_values.txt'";

}

void languages::ImportDocumentToSameName(QString int_lang, QStringList file_list)
{
    // Translated files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QString translated_files = "import/" + int_lang + "/";
    QDir translated(translated_files);
    QStringList files_translated;
    files_translated = translated.entryList(xml_filter, QDir::Files);

    // Check language
    QString tag_lang = "English";
    if(int_lang == "fr"){tag_lang = "French";};
    if(int_lang == "de"){tag_lang = "German";};
    if(int_lang == "it"){tag_lang = "Italian";};
    if(int_lang == "es"){tag_lang = "Spanish";};
    if(int_lang == "pl"){tag_lang = "Polish";};
    if(int_lang == "ja"){tag_lang = "Japanese";};
    if(int_lang == "zh"){tag_lang = "Chinese";};
    if(int_lang == "ru"){tag_lang = "Russian";};
    if(int_lang == "fi"){tag_lang = "Finnish";};
    if(int_lang == "hu"){tag_lang = "Hungarian";};

    // Print list

    QStringList print_list;

    // Entering loop

    for(QStringList::Iterator it = file_list.begin(); it != file_list.end(); it++)
    {
        int s = 0;

        QString current = "imported/" + *it;

        qDebug() << "Checking " << current;

        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        // Check file integrity
        if (input.firstChildElement("Civ4GameText").firstChildElement("TEXT").isNull())
        {
            qDebug() << current << " is not properly formatted";
            QFile::remove(current);
        }
        else
        {

        // Comparing to each new file
        for(QStringList::Iterator tr = files_translated.begin(); tr != files_translated.end(); tr++)
        {
            if(*it == *tr){
            QString current_new = translated_files + *tr;
            QDomDocument input_tr;
            QFile file_tr(current_new);
            file_tr.open(QIODevice::ReadOnly);
            input_tr.setContent(&file_tr);
            file_tr.close();

            QDomElement root = input_tr.documentElement();

            QDomElement tag_orig = input.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
            for(tag_orig;!tag_orig.isNull();tag_orig = tag_orig.nextSiblingElement())
            {
                // Compare each occurence with each tag of the translated file
                QString value_tag = tag_orig.firstChildElement("Tag").firstChild().nodeValue();
                QDomElement tag_tr = root.firstChildElement("string").toElement();
                //qDebug() << tag_tr.attribute("name");

                for(tag_tr;!tag_tr.isNull();tag_tr = tag_tr.nextSiblingElement())
                {
                    QString value_tag_tr = tag_tr.attribute("name");
                    //qDebug() << value_tag_tr;

                    if (value_tag == value_tag_tr)
                    {
                        s++;
                        int skip = 0;
                        QString value_text_tr = tag_tr.firstChild().nodeValue();
                        value_text_tr.replace("\\'","\'").replace("\\\"","\"").replace("\\t","[TAB]");
                        QString value_text;

                        if(value_text_tr == tag_orig.firstChildElement("English").firstChild().nodeValue())
                        {
                            skip++;
                        }

                        // Checking tag presence
                        // Sub language marker
                        if (!tag_orig.firstChildElement(tag_lang).firstChildElement().firstChild().nodeValue().isNull())
                        {
                            value_text = tag_orig.firstChildElement(tag_lang).firstChildElement().firstChild().nodeValue();
                        }
                        // No language marker
                        else if(tag_orig.firstChildElement(tag_lang).isNull())
                        {
                            value_text = "";
                        }

                        // Direct language marker
                        else
                        {
                            value_text = tag_orig.firstChildElement(tag_lang).firstChild().nodeValue();
                        }

                        //qDebug() << value_tag << " : " << value_text << " / " << value_text_tr;

                        // Normalize value
                        if(value_text_tr.isEmpty())
                        {
                            value_text_tr = "";
                        }

                        // Compare original and modified strings
                        if(value_text != value_text_tr && skip == 0)
                        {
                                QString operation;
                                operation = "FILE: " + *it + " | TAG: " + value_tag + " | OLD: " + value_text + " -> NEW: " + value_text_tr;
                                print_list << operation;
                                //qDebug() << operation;

                                // Case: original direct value, new subtag value
                                if (tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().isNull() && !tag_tr.attribute("gender").isNull())
                                {
                                    QString new_value_text = value_text_tr;
                                    QString new_value_gender = tag_tr.attribute("gender");
                                    QString new_value_plural = tag_tr.attribute("plural");

                                    // Empty text value, then add subtags
                                    tag_orig.removeChild(tag_orig.firstChildElement(tag_lang).firstChild());
                                    QDomNode new_node_text = input.createElement("Text");
                                    QDomNode new_node_gender = input.createElement("Gender");
                                    QDomNode new_node_plural = input.createElement("Plural");
                                    tag_orig.firstChildElement(tag_lang).firstChild().appendChild(new_node_text);
                                    tag_orig.firstChildElement(tag_lang).firstChild().appendChild(new_node_gender);
                                    tag_orig.firstChildElement(tag_lang).firstChild().appendChild(new_node_plural);

                                    QDomText new_node_text_value = input.createTextNode(new_value_text);
                                    QDomText new_node_gender_value = input.createTextNode(new_value_gender);
                                    QDomText new_node_plural_value = input.createTextNode(new_value_plural);
                                    new_node_text.appendChild(new_node_text_value);
                                    new_node_gender.appendChild(new_node_gender_value);
                                    new_node_plural.appendChild(new_node_plural_value);

                                }
                                // Case: original, new subtag value
                                else if(!tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().isNull()){
                                    tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().setNodeValue(value_text_tr);
                                    // Set gender and plural
                                    if (!tag_tr.attribute("gender").isNull()){
                                    QString new_value_gender = tag_tr.attribute("gender");
                                    tag_orig.firstChildElement(tag_lang).firstChildElement("Gender").firstChild().setNodeValue(new_value_gender);}
                                    if(!tag_tr.attribute("plural").isNull()){
                                    QString new_value_plural = tag_tr.attribute("plural");
                                    tag_orig.firstChildElement(tag_lang).firstChildElement("Plural").firstChild().setNodeValue(new_value_plural);
                                    }
                                }

                                // Case: no original
                                else if(tag_orig.firstChildElement(tag_lang).firstChild().isNull())
                                {
                                    // New subtag value
                                    if (value_text == "" && !tag_tr.attribute("gender").isNull()){
                                        QString new_value_text = value_text_tr;
                                        QString new_value_gender = tag_tr.attribute("gender");
                                        QString new_value_plural = tag_tr.attribute("plural");

                                        // Append nodes
                                        QDomNode new_node_language = input.createElement(tag_lang);
                                        tag_orig.appendChild(new_node_language);
                                        QDomNode new_node_text = input.createElement("Text");
                                        QDomNode new_node_gender = input.createElement("Gender");
                                        QDomNode new_node_plural = input.createElement("Plural");
                                        new_node_language.appendChild(new_node_text);
                                        new_node_language.appendChild(new_node_gender);
                                        new_node_language.appendChild(new_node_plural);

                                        QDomText new_node_text_value = input.createTextNode(new_value_text);
                                        QDomText new_node_gender_value = input.createTextNode(new_value_gender);
                                        QDomText new_node_plural_value = input.createTextNode(new_value_plural);
                                        new_node_text.appendChild(new_node_text_value);
                                        new_node_gender.appendChild(new_node_gender_value);
                                        new_node_plural.appendChild(new_node_plural_value);
                                    }

                                    // New direct value
                                    else {
                                    QDomNode new_tag = input.createElement(tag_lang);
                                    tag_orig.appendChild(new_tag);
                                    QDomText new_value = input.createTextNode(value_text_tr);
                                    new_tag.appendChild(new_value);
                                    }
                                }

                                // Case: original, direct value
                                else
                                {
                                    //qDebug() << "here";
                                    tag_orig.firstChildElement(tag_lang).firstChild().setNodeValue(value_text_tr);
                                }
                            }
                        }
                    }

                }
            }
        }
        }

        // Save to file
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream ts(&file_input);
        input.save(ts, 3);
        file_input.close();
    }

    // Output modified list
    QString print_value;
    QString print_file_name = "imported/imported_" + int_lang + "_values.txt";
    QFile print_file(print_file_name);

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, print_list)
            {
                stream << print_value << endl;
            }
        }
        print_file.close();

        qDebug() << "\n\nFiles successfully processed. Modified files are in 'imported/'.\nA report of the modified values have been generated in 'imported/imported_values.txt'";

}

void languages::SortCategories()
{
    /* Sorting logic */
    /* Check parse file presence
     * Prepare original files by converting them to UTF8
     * List all tags in a QStringList
     * Initialize all XML files by category plus MISC. Add a _PEDIA and _REMOVED_ for all categories
     * For each tag (until the end of the list):
        * Check category
        * Open category file
        * Check all files looking for the tag
        * Check if the tag has already been found
        * Add the tag to the category file
        * Save category file
    * Convert the files back to Civ4
    * Print the duplicate list */
/*
    // Check requirements
    QFile categories("_xml_parser.config");
    if(!categories.exists())
    {
       qDebug() << "The parser need a '_xml_parser.config' file to continue. Aborting...";
       return;
    }

    // Listing root files
    qDebug() << "Listing files...";
    QDir dir_import("sorted/");
    dir_import.removeRecursively();
    dir_import.mkpath("new/");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Import files
    qDebug() << "Copying files...";
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"sorted/"+*it);
        ConvertCiv4ToUTF8("sorted/"+*it);
    }

    // Collect all tags in a bigfile
    qDebug() << "Collecting all tags to a file";
    QDomDocument xml_collect;
    QFile xml_collect_file("sorted/TEMP");
    xml_collect_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    // Create the output file
    QDomNode declaration = xml_collect.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    xml_collect.insertBefore(declaration,xml_collect.firstChild());
    QDomNode collect_root = xml_collect.createElement("Civ4GameText");
    xml_collect.appendChild(collect_root);

   // Loop the input file
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QString current = "sorted/" + *it;
        QDomDocument read;
        QFile file_in(current);
        file_in.open(QIODevice::ReadOnly);
        read.setContent(&file_in);
        QDomNode read_text = read.firstChildElement("Civ4GameText").firstChildElement();

        for (read_text;!read_text.isNull();read_text = read_text.nextSiblingElement())
        {
            QDomElement read_element = read_text.firstChildElement();
            QDomElement collect_node = xml_collect.createElement("TEXT");
            collect_root.appendChild(collect_node);

            for (read_element;!read_element.isNull();read_element = read_element.nextSiblingElement())
            {
                //qDebug() << read_element.tagName();
                if(!read_element.firstChildElement("Text").isNull())
                {
                    QDomElement collect_element = xml_collect.createElement(read_element.tagName());
                    collect_node.appendChild(collect_element);
                    QDomElement collect_element_text = xml_collect.createElement("Text");
                    QDomElement collect_element_gender = xml_collect.createElement("Gender");
                    QDomElement collect_element_plural = xml_collect.createElement("Plural");
                    collect_element.appendChild(collect_element_text);
                    collect_element.appendChild(collect_element_gender);
                    collect_element.appendChild(collect_element_plural);

                    QString collect_text_value = read_element.firstChildElement("Text").firstChild().nodeValue();
                    QString collect_gender_value = read_element.firstChildElement("Gender").firstChild().nodeValue();
                    QString collect_plural_value = read_element.firstChildElement("Plural").firstChild().nodeValue();

                    QDomText collect_text = xml_collect.createTextNode(collect_text_value);
                    QDomText collect_gender = xml_collect.createTextNode(collect_gender_value);
                    QDomText collect_plural = xml_collect.createTextNode(collect_plural_value);
                    collect_element_text.appendChild(collect_text);
                    collect_element_gender.appendChild(collect_gender);
                    collect_element_plural.appendChild(collect_plural);
                }
                else
                {
                    QDomElement collect_element = xml_collect.createElement(read_element.tagName());
                    collect_node.appendChild(collect_element);

                    QString collect_text_value = read_element.firstChild().nodeValue();

                    QDomText collect_text = xml_collect.createTextNode(collect_text_value);
                    collect_element.appendChild(collect_text);
                }
            }
        }
        file_in.close();
    }
    QTextStream ts(&xml_collect_file);
    xml_collect.save(ts, 3);
    xml_collect_file.close();

    // List all tags
    qDebug() << "Listing tags...";
    QStringList duplicate_tags;
    QStringList tags = ListTags(".");
    tags.removeDuplicates();
    int tags_total_counter = tags.count();
    qDebug() << "Detected" << tags_total_counter << "tags";

    // Check categories
    qDebug() << "Checking categories...";
    QStringList categories_list;
    QDomDocument xml_categories;
    categories.open(QIODevice::ReadOnly);
    xml_categories.setContent(&categories);
    categories.close();

    // Initialize all XML classics, pedia and removed
    QDomElement read_categories = xml_categories.firstChildElement().firstChildElement("category");
    for(read_categories;!read_categories.isNull(); read_categories = read_categories.nextSiblingElement())
    {
        QDomDocument xml_temp;
        xml_temp.insertBefore(xml_temp.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_temp.firstChild());
        xml_temp.appendChild(xml_temp.createElement("Civ4GameText"));

        QString filename = read_categories.attribute("file");
        QString filename_pedia = "sorted/new/" + filename + "_PEDIA.xml";
        QString filename_removed = "sorted/new/_REMOVED_TAGS_" + filename + ".xml";
        QString filename_removed_pedia = "sorted/new/_REMOVED_TAGS_" + filename + "_PEDIA.xml";
        categories_list << filename << filename + "_PEDIA";
        filename = "sorted/new/" + filename + ".xml";
        QFile temp(filename);
        temp.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_temp(&temp);
        xml_temp.save(ts_temp,4);
        temp.close();
        QFile temp_pedia(filename_pedia);
        temp_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_pedia(&temp_pedia);
        xml_temp.save(ts_pedia,4);
        temp_pedia.close();
        QFile temp_removed(filename_removed);
        temp_removed.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_removed(&temp_removed);
        xml_temp.save(ts_removed,4);
        temp_removed.close();
        QFile temp_removed_pedia(filename_removed_pedia);
        temp_removed_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_removed_pedia(&temp_removed_pedia);
        xml_temp.save(ts_removed_pedia,4);
        temp_removed_pedia.close();

    }
    // Create MISC classic, pedia, and removed
    QDomDocument xml_misc;
    xml_misc.insertBefore(xml_misc.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_misc.firstChild());
    xml_misc.appendChild(xml_misc.createElement("Civ4GameText"));
    QFile misc("sorted/new/MISC.xml");
    misc.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp(&misc);
    xml_misc.save(ts_temp,4);
    misc.close();
    QFile misc_pedia("sorted/new/MISC_PEDIA.xml");
    misc_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_pedia(&misc_pedia);
    xml_misc.save(ts_temp_pedia,4);
    misc_pedia.close();
    QFile misc_removed("sorted/new/_REMOVED_TAGS_MISC.xml");
    misc_removed.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed(&misc_removed);
    xml_misc.save(ts_temp_removed,4);
    misc_removed.close();
    QFile misc_removed_pedia("sorted/new/_REMOVED_TAGS_MISC_PEDIA.xml");
    misc_removed_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed_pedia(&misc_removed_pedia);
    xml_misc.save(ts_temp_removed_pedia,4);
    misc_removed_pedia.close();
    categories_list << "MISC" << "MISC_PEDIA";
    categories_list.sort();

    // Sorting

    QString tag;
    QString category;
    int tags_counter = 1;
    int category_total_counter = categories_list.count();
    int category_counter = 1;
    QDir import_dir("sorted/");
    QStringList import_files;
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Open input file
    xml_collect_file.open(QIODevice::ReadOnly);
    QDomDocument input;
    input.setContent(&xml_collect_file);

    // For each category, make a list of the concerned tags
    foreach(category,categories_list)
    {
        qDebug() << "Checking category" << category_counter << "of" << category_total_counter << " : " << category;
        QStringList category_tags_list;

        // For each tag in the list, check the category. If it's the same, the add the tag to the list
        foreach (tag,tags)
        {
            QDomElement xml_category = xml_categories.firstChildElement().firstChildElement();
            QString category_value = "MISC";
            for(xml_category;!xml_category.isNull();xml_category = xml_category.nextSiblingElement())
            {
                QDomElement xml_category_tag = xml_category.firstChildElement();
                for(xml_category_tag;!xml_category_tag.isNull();xml_category_tag = xml_category_tag.nextSiblingElement())
                {
                    if(tag.startsWith(xml_category_tag.firstChild().nodeValue()))
                    {
                        category_value = xml_category.attribute("file");
                    }
                }
            }
            if(tag.contains("_PEDIA"))
            {
                category_value = category_value + "_PEDIA";
            }

            if(category == category_value)
            {
                category_tags_list << tag;
            }
        }
        category_tags_list.removeDuplicates();

        // Open the category files
        QFile file_category("sorted/new/" + category + ".xml");
        QFile file_removed("sorted/new/_REMOVED_TAGS_" + category + ".xml");
        QDomDocument xml_detected;
        QDomDocument xml_removed;
        file_category.open(QIODevice::ReadOnly);
        file_removed.open(QIODevice::ReadOnly);
        xml_detected.setContent(&file_category);
        xml_removed.setContent(&file_removed);
        file_category.close();
        file_removed.close();

        // For each tag in the category, populate the xml.
        foreach(tag,category_tags_list)
        {
            int tag_occurence = 0;

            // Check the counter !!!!
            //qDebug() << "Sorting tag" << tags_counter << "of" << tags_total_counter << ":" << tag;

            // Looking for tag in the file
                QDomElement input_text_node = input.firstChildElement().firstChildElement();
                for(input_text_node;!input_text_node.isNull();input_text_node = input_text_node.nextSiblingElement())
                {
                    if(tag == input_text_node.firstChildElement("Tag").firstChild().nodeValue())
                    {
                        // If tag has already been checked, move the tag to removed file
                        if(tag_occurence > 0)
                        {
                            qDebug() << "Duplicate detected : " << tag;
                            QString operation = "TAG:" + tag + " | FILE:" + category + ".xml";
                            duplicate_tags << operation;

                            // Write the node to the correct category
                            QDomElement input_element_node = input_text_node.firstChildElement();
                            QDomElement xml_removed_node = xml_removed.firstChildElement();
                            QDomNode xml_text_node = xml_removed.createElement("Text");
                            xml_removed_node.appendChild(xml_text_node);
                            for(input_element_node;!input_element_node.isNull();input_element_node = input_element_node.nextSiblingElement())
                            {
                                // Check if the node is a plural form
                                if (!input_element_node.firstChildElement("Text").isNull())
                                {
                                    //qDebug() << "Multiple tag detected";
                                    QString element_name = input_element_node.tagName();
                                    QString element_text_name = input_element_node.firstChildElement("Text").tagName();
                                    QString element_gender_name = input_element_node.firstChildElement("Gender").tagName();
                                    QString element_plural_name = input_element_node.firstChildElement("Plural").tagName();

                                    QDomNode node_element_name = xml_removed.createElement(element_name);
                                    QDomElement node_element_text_name = xml_removed.createElement(element_text_name);
                                    QDomElement node_element_gender_name = xml_removed.createElement(element_gender_name);
                                    QDomElement node_element_plural_name = xml_removed.createElement(element_plural_name);

                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(node_element_text_name);
                                    node_element_name.appendChild(node_element_gender_name);
                                    node_element_name.appendChild(node_element_plural_name);

                                    QString element_text_value = input_element_node.firstChildElement("Text").firstChild().nodeValue();
                                    QString element_gender_value = input_element_node.firstChildElement("Gender").firstChild().nodeValue();
                                    QString element_plural_value = input_element_node.firstChildElement("Plural").firstChild().nodeValue();

                                    node_element_text_name.appendChild(xml_removed.createTextNode(element_text_value));
                                    node_element_gender_name.appendChild(xml_removed.createTextNode(element_gender_value));
                                    node_element_plural_name.appendChild(xml_removed.createTextNode(element_plural_value));
                                }
                                else
                                {
                                    QString element_name = input_element_node.tagName();
                                    QString element_value = input_element_node.firstChild().nodeValue();
                                    QDomElement node_element_name = xml_removed.createElement(element_name);
                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(xml_removed.createTextNode(element_value));
                                }
                            }
                        }
                        else
                        {
                            // Write the node to the correct category
                            QDomElement input_element_node = input_text_node.firstChildElement();
                            QDomElement xml_detected_node = xml_detected.firstChildElement();
                            QDomNode xml_text_node = xml_detected.createElement("Text");
                            xml_detected_node.appendChild(xml_text_node);
                            for(input_element_node;!input_element_node.isNull();input_element_node = input_element_node.nextSiblingElement())
                            {
                                // Check if the node is a plural form
                                if (!input_element_node.firstChildElement("Text").isNull())
                                {
                                    //qDebug() << "Multiple tag detected";
                                    QString element_name = input_element_node.tagName();
                                    QString element_text_name = input_element_node.firstChildElement("Text").tagName();
                                    QString element_gender_name = input_element_node.firstChildElement("Gender").tagName();
                                    QString element_plural_name = input_element_node.firstChildElement("Plural").tagName();

                                    QDomNode node_element_name = xml_detected.createElement(element_name);
                                    QDomElement node_element_text_name = xml_detected.createElement(element_text_name);
                                    QDomElement node_element_gender_name = xml_detected.createElement(element_gender_name);
                                    QDomElement node_element_plural_name = xml_detected.createElement(element_plural_name);

                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(node_element_text_name);
                                    node_element_name.appendChild(node_element_gender_name);
                                    node_element_name.appendChild(node_element_plural_name);

                                    QString element_text_value = input_element_node.firstChildElement("Text").firstChild().nodeValue();
                                    QString element_gender_value = input_element_node.firstChildElement("Gender").firstChild().nodeValue();
                                    QString element_plural_value = input_element_node.firstChildElement("Plural").firstChild().nodeValue();

                                    node_element_text_name.appendChild(xml_detected.createTextNode(element_text_value));
                                    node_element_gender_name.appendChild(xml_detected.createTextNode(element_gender_value));
                                    node_element_plural_name.appendChild(xml_detected.createTextNode(element_plural_value));
                                }
                                else
                                {
                                    QString element_name = input_element_node.tagName();
                                    QString element_value = input_element_node.firstChild().nodeValue();
                                    QDomElement node_element_name = xml_detected.createElement(element_name);
                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(xml_detected.createTextNode(element_value));
                                }
                            }
                        }
                        tag_occurence++;
                    }
                }
            tags_counter++;
        }

        // Save both xml
        QFile file_detected_write("sorted/new/_TEMPWRITE_.xml");
        QFile file_removed_write("sorted/new/_TEMPREMOVED_.xml");
        QFile::remove("sorted/new/_TEMPWRITE_.xml");
        QFile::remove("sorted/new/_TEMPREMOVED_.xml");
        file_detected_write.open(QIODevice::WriteOnly);
        file_removed_write.open(QIODevice::WriteOnly);
        QTextStream ts_detected(&file_detected_write);
        QTextStream ts_removed(&file_removed_write);
        xml_detected.save(ts_detected,4);
        xml_removed.save(ts_removed,4);
        file_detected_write.close();
        file_removed_write.close();
        file_category.remove();
        file_removed.remove();
        file_detected_write.rename("sorted/new/" + category + ".xml");
        file_removed_write.rename("sorted/new/_REMOVED_TAGS_" + category + ".xml");
        category_counter++;
    }

    // Clean up folder and encoding back
    qDebug() << "Clean up and preparing files";
    import_files = import_dir.entryList(xml_filter, QDir::Files);
    xml_collect_file.close();
    QFile::remove("sorted/TEMP");
    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        QString file_remove = "sorted/" + *it;
        QFile::remove(file_remove);
    }
    QDir dir_sorted("sorted/new/");
    QStringList dir_sorted_list;
    dir_sorted_list = dir_sorted.entryList(xml_filter, QDir::Files);
    for(QStringList::Iterator it = dir_sorted_list.begin(); it != dir_sorted_list.end(); it++)
    {
        QString old_file = "sorted/new/" + *it;
        QString new_file = "sorted/" + *it;
        QFile::rename(old_file,new_file);
        QDomDocument doc_tester;
        QFile file_tester(new_file);
        file_tester.open(QIODevice::ReadOnly);
        doc_tester.setContent(&file_tester);
        file_tester.close();
        if (doc_tester.firstChildElement("Civ4GameText").firstChildElement().isNull())
        {
            QFile::remove(new_file);
        }
        else
        {
            ConvertUTF8ToCiv4(new_file);
        }
    }
    dir_sorted.setPath("sorted/");
    dir_sorted.rmdir("new/");

    // Print duplicate list
    qDebug() << "Creating duplicates list";
    QString print_value;
    QFile print_file("sorted/_duplicates_list.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, duplicate_tags)
            {
                stream << print_value << endl;
            }
        }
    print_file.close();

    QStringList tags_output = ListTags("sorted/");
    tags_output.removeDuplicates();
    int tags_output_counter = tags_output.count();

    qDebug() << "Original tag number:" << tags_total_counter << "| New tag number:" << tags_output_counter;
}



QStringList languages::ListTags(QString dir)
{
    QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    QDir root(dir);
    files = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QFile file_in(root.path()+"/"+*it);
        //qDebug() << root.path()+"/"+*it;
        file_in.open(QIODevice::ReadOnly);
        QDomDocument xml;
        xml.setContent(&file_in);
        QDomElement xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        if(xml_tag.isNull())
        {
            qDebug() << *it << " is not properly formatted";
            file_in.close();
        }

        for(xml_tag;!xml_tag.isNull();xml_tag = xml_tag.nextSiblingElement())
        {
            tags << xml_tag.firstChildElement("Tag").text();
        }
        file_in.close();
    }

	// Sort unsensitive
	QMap<QString, QString> strMap;
    foreach ( QString str, tags ) {
        strMap.insert( str.toLower(), str );
    }
    tags = strMap.values();

    return tags;
}

QStringList languages::ListLanguages(QString dir)
{
    /*QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    QDir root(dir);
    files = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QFile file_in(*it);
        file_in.open(QIODevice::ReadOnly);
        QDomDocument xml;
        xml.setContent(&file_in);
        QDomElement xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        if(xml_tag.isNull())
        {
            qDebug() << *it << " is not properly formatted";
            file_in.close();
        }

        for(xml_tag;!xml_tag.isNull();xml_tag = xml_tag.nextSiblingElement())
        {
            QDomElement xml_language = xml_tag.firstChildElement();
            for(xml_language;!xml_language.isNull();xml_language = xml_language.nextSiblingElement())
            {
                QString tag;
                tag = xml_language.tagName();
                if(tag == "English" || tag == "Tag" || tag == "Text" || tag == "Gender" || tag == "Plural") {}
                else {
                    //qDebug() << xml_language.tagName();
                    tags << tag;
                }
            }

        }
        file_in.close();
    }

    tags.sort();
    tags.removeDuplicates();

    return tags;
}

void languages::CleanFiles()
{
    qDebug() << "Preparing files...";
    QDir dir_import("cleaned/");
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // Search the tag trough existing files and replace with the new value if found
    QStringList xml_filter;
    xml_filter << "*.xml";

    // Root files
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Import files
    QDir import_dir("cleaned/");
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"cleaned/"+*it);
        ConvertCiv4ToUTF8("cleaned/"+*it);
    }
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Print list

    QStringList print_list;

    // Loop through all files

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        int s = 0;

        QString current = "cleaned/" + *it;

        qDebug() << "Cleaning " << current;

        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomNode input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");

        // Check file integrity
        if (input.firstChildElement("Civ4GameText").firstChildElement("TEXT").isNull())
        {
            qDebug() << current << " is not properly formatted";
            QFile::remove(current);
        }
        else
        {
            // Check english tag
            while(!input_node.isNull())
            {
                QString english_tag;
                // Check plural form english
                if(!input_node.firstChildElement("English").firstChildElement("Text").isNull())
                {
                    english_tag = input_node.firstChildElement("English").firstChildElement("Text").firstChild().nodeValue();
                    //qDebug() << "Found special node" << english_tag;
                }
                else
                {
                    english_tag = input_node.firstChildElement("English").firstChild().nodeValue();
                }

                // Compare all elements to english
                QDomElement element = input_node.firstChildElement();
                QString list_element;
                QStringList list_removal;
                while(!element.isNull())
                {
                    // Case: Language subtag with value equal to english
                    if(!element.firstChildElement("Text").isNull() && element.tagName() != "English" && english_tag == element.firstChildElement("Text").firstChild().nodeValue())
                    {
                        // Do nothing
                    }
                    // Case: Normal tag with same value as english
                    else if(element.firstChild().nodeValue() == english_tag && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (identical)";
                        print_list << operation;
                        s++;
                        list_removal << element.tagName();
                    }
                    // Case: Language tag is normal and empty
                    else if (element.firstChildElement("Text").isNull() && element.firstChild().nodeValue().isEmpty() && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (empty)";
                        print_list << operation;
                        s++;
                        list_removal << element.tagName();
                    }
                    // Case: Language tag is subtag and empty
                    else if (!element.firstChildElement("Text").isNull() && element.firstChildElement("Text").firstChild().nodeValue().isEmpty() && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (empty subtag)";
                        print_list << operation;
                        s++;
                        list_removal << element.tagName();
                    }
                    element = element.nextSiblingElement();
                }

            foreach (list_element, list_removal)
            {
                QDomNode to_remove = input_node.firstChildElement(list_element);
                //qDebug() << input_node.firstChildElement(list_element).tagName();
                to_remove.parentNode().removeChild(to_remove);
            }

            input_node = input_node.nextSibling();

            }
        }

        // Save to file
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
        file_input.close();

        if(s == 0)
        {
            QFile::remove(current);
        }
        else
        {
            ConvertUTF8ToCiv4(current);
        }
    }

    // Output modified list
    QString print_value;
    QFile print_file("cleaned/_cleaned_values.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, print_list)
            {
                stream << print_value << endl;
            }
        }
        print_file.close();

        qDebug() << "\n\nFiles successfully processed. Modified files are in 'cleaned/'.\nA report of the modified values have been generated in 'cleaned/_cleaned_values.txt'";
}

void languages::RemoveLanguage()
{
    QString lang_remove = AutomaticLanguageDetection(".");
    if (lang_remove == "error"){return;}

    // Prepare folder
    qDebug() << "Preparing files...";
    QDir dir_import("backup/");
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // List root files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Copy and convert files to backup dir
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"backup/"+*it);
        ConvertCiv4ToUTF8("backup/"+*it);
    }
    import_files = dir_import.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        QString current = "backup/" + *it;
        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomNode input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");

        int counter = 0;
        while(!input_node.isNull())
        {
            if(!input_node.firstChildElement(lang_remove).isNull())
            {
                input_node.removeChild(input_node.firstChildElement(lang_remove));
                counter++;
            }

            input_node = input_node.nextSibling();
        }
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
        file_input.close();

        if(counter == 0)
        {
            QFile::remove(current);
        }
        else
        {
            ConvertUTF8ToCiv4(current);
        }
    }

    qDebug() << "All modified files are in the \"backup\" folder";
}



QStringList languages::FindUnusedTags()
{
    // Check the directory
    QDir root(".");
    QStringList empty;
    if(!root.exists("../../../Assets/"))
    {
        qDebug() << "The parser need to be in 'Assets/XML/Text/' folder for this function to work. Aborting...";
        return empty;
    }

    // Getting game path and source path
    QFile categories("_xml_parser.config");
    if(!categories.exists())
    {
       qDebug() << "The parser need a '_xml_parser.config' file to continue. Aborting...";
       return empty;
    }
    QDomDocument xml_categories;
    categories.open(QIODevice::ReadOnly);
    xml_categories.setContent(&categories);
    categories.close();
    QString gamepath = xml_categories.firstChildElement().firstChildElement("gamepath").firstChild().nodeValue();
    if (gamepath.isEmpty())
    {
        qDebug() << "You need to add the base game path to the '_xml_parser.config' file";
        return empty;
    }
    QString srcpath = xml_categories.firstChildElement().firstChildElement("sourcepath").firstChild().nodeValue();
    if (srcpath.isEmpty())
    {
        qDebug() << "You need to add the dll source path to the '_xml_parser.config' file";
        return empty;
    }
    gamepath.replace("\\","/");
    srcpath.replace("\\","/");
	
    // List tags
    qDebug() << "Listing tags...";
    QStringList list_tags = ListTags(QDir::currentPath());
    QString tag;
	QString base_tag;
    list_tags.removeDuplicates();
	
	// List base game tags
	QStringList list_base_tags = ListTags(gamepath+"Assets/XML/Text/");
	QStringList list_ext_tags = ListTags(gamepath+"Beyond the Sword/Assets/XML/Text/");

    // Remove exceptions from the tag list

    foreach(tag, list_tags)
    {
        //qDebug() << tag;
        QDomElement exceptions = xml_categories.firstChildElement().firstChildElement("exceptions").firstChildElement();
        for(exceptions;!exceptions.isNull();exceptions = exceptions.nextSiblingElement())
        {
            QString value = exceptions.firstChild().nodeValue();
            if(tag.startsWith(value))
            {
                list_tags.removeAll(tag);
            }
        }
    }
	
	// Remove base game tags from the tag list
	foreach(tag,list_tags)
	{
		foreach(base_tag,list_base_tags)
		{
			if (tag == base_tag)
			{
				list_tags.removeAll(tag);
			}
		}
		foreach(base_tag,list_ext_tags)
		{
			if (tag == base_tag)
			{
				list_tags.removeAll(tag);
			}
		}
	}		

    int tags_total_counter = list_tags.count();

    qDebug() << "Collecting files...";
    QStringList list_ext;
    list_ext << "*.xml" << "*.py" << "*.CivBeyondSwordWBSave" << "*.Civ4WorldBuilderSave" << "*.CivWarlordsWBSave";
    QStringList list_cpp;
    list_cpp << "*.cpp" << "*.h";


    // List files in mod folder
    QStringList exclusion_list;
    exclusion_list << "Assets/XML/Text/";
    root.setCurrent("../../../");
    QDir game_folder(gamepath);
    QDir source_folder(srcpath);

    // Collect all files in a bigfile
    QDirIterator mod_iterator(root.absolutePath(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QDirIterator game_iterator(game_folder.path(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QDirIterator source_iterator(source_folder.path(), list_cpp, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QFile output("TEMPFILE");
    output.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&output);
    int file_counter = 0;
    while (mod_iterator.hasNext())
    {
        // Check if it isn't in the text folder
        if(mod_iterator.filePath().contains("Assets/XML/Text") || mod_iterator.filePath() == "")
        {
            mod_iterator.next();
        }

        else {
            file_counter++;
            QFile file(mod_iterator.filePath());

            file.open(QIODevice::ReadOnly);
            QTextStream temp(&file);
            while(!temp.atEnd())
            {
                QString line = temp.readLine();
                stream << line;
            }
            file.close();

            mod_iterator.next();
        }
    }
    while (game_iterator.hasNext())
    {
        // Check if it isn't in the text folder
        if(game_iterator.filePath().contains("Assets/XML/Text") || game_iterator.filePath().contains("Mods/") || game_iterator.filePath() == "")
        {
            game_iterator.next();
        }
        else {
            file_counter++;
            QFile file(game_iterator.filePath());

            file.open(QIODevice::ReadOnly);
            QTextStream temp(&file);
            while(!temp.atEnd())
            {
                QString line = temp.readLine();
                stream << line;
            }
            file.close();
            //qDebug() << game_iterator.filePath();
            game_iterator.next();
        }
    }
    while (source_iterator.hasNext())
    {
        if(source_iterator.filePath() == "")
        {
            source_iterator.next();
        }
        else {
            file_counter++;
            QFile file(source_iterator.filePath());

            file.open(QIODevice::ReadOnly);
            QTextStream temp(&file);
            while(!temp.atEnd())
            {
                QString line = temp.readLine();
                stream << line;
            }
            file.close();
            //qDebug() << source_iterator.filePath();
            source_iterator.next();
        }
    }
    output.close();

    // Looking for tags
    int found = 0;
    qDebug() << "Looking for tags...";
    output.open(QIODevice::ReadOnly);
    QTextStream read_stream(&output);
    while (!read_stream.atEnd())
    {
        QString line = read_stream.readLine();
        foreach(tag,list_tags)
        {
            if (line.contains(tag))
            {
                //qDebug() << "Found" << tag;
                list_tags.removeAll(tag);
                found++;
                tags_total_counter--;
                if(tags_total_counter%100==0)
                {
                    qDebug() << "Still" << tags_total_counter << "tags to process";
                }
            }
        }

    }
    output.close();
    output.remove();

    qDebug() << tags_total_counter << "tags have not been found. Printing list: _tags_not_found.txt in text folder";

    root.setCurrent("Assets/XML/Text/");

    // Print not found list
    QString print_value;
    QFile print_file("_tags_not_found.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, list_tags)
            {
                stream << print_value << endl;
            }
        }
    print_file.close();

    return list_tags;
}

void languages::RemoveUnusedTags()
{
    // Test code
    QFile test("_tags_not_found.txt");
    QStringList list_tags;
    test.open(QIODevice::ReadOnly);
    QTextStream read_stream(&test);
    while (!read_stream.atEnd())
    {
        list_tags << read_stream.readLine();
    }
    test.close();

    // Prepare folder
    qDebug() << "Preparing files...";
    QDir dir_import("unused/");
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // List root files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Copy and convert files to backup dir
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"unused/"+*it);
        ConvertCiv4ToUTF8("unused/"+*it);
    }
    import_files = dir_import.entryList(xml_filter, QDir::Files);

    QString tags;
    int counter = 0;
    int file_number = 1;
    QStringList operation_list;

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        qDebug() << "Processing file" << file_number << "of" << import_files.count();
        QString current = "unused/" + *it;
        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomNode input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");
        int remove_tag = 0;
        int remove_file = 0;
        while(!input_node.isNull())
        {
            QString tag_value = input_node.firstChildElement("Tag").firstChild().nodeValue();
            //qDebug() << tag_value;
            remove_tag = 0;
            foreach(tags,list_tags)
            {
                if(tag_value == tags)
                {
                    list_tags.removeAll(tags);
                    QString operation = "FILE: " + *it + " | REMOVED TAG: " + tags;
                    operation_list << operation;
                    counter++;
                    remove_file++;
                    remove_tag++;
                }
            }
            input_node = input_node.nextSibling();
            if(remove_tag == 1)
            {
                input.firstChildElement("Civ4GameText").removeChild(input_node.previousSibling());
            }
        }
        if(remove_tag == 1)
        {
            input.firstChildElement("Civ4GameText").removeChild(input.firstChildElement("Civ4GameText").lastChildElement());
        }
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
        file_input.close();

        if(remove_file == 0)
        {
            QFile::remove(current);
        }
        else
        {
            ConvertUTF8ToCiv4(current);
        }
        file_number++;
    }

    // Output modified list
    QString print_value;
    QString print_file_name = "unused/_unused_tags_report.txt";
    QFile print_file(print_file_name);

    if ( print_file.open(QIODevice::Truncate | QIODevice::WriteOnly) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, operation_list)
            {
                stream << print_value << endl;
            }
        }
        print_file.close();

    qDebug() << "All modified files are in the \"unused\" folder. Removed" << counter << "tags. A detailled report has been print in _unused_tags_report.txt";
    return;
}
*/
