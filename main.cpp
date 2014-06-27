#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QtXml/QtXml>
#include "dll_finder.h"

namespace constants {
const QString VERSION = "0.9";
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
    void FindUnusedTags();
    QString ConvertLatin1ToRussian(QString string);
    QString ConvertRussianToLatin1(QString string);
    void SorterHelper(QString prefix);
    QString ConvertStringToCiv4(QString string);
    void ConvertCiv4ToUTF8(QString file);
    void ConvertUTF8ToCiv4(QString file);
    QStringList CheckImportLanguages();
    void RemoveLanguage();
    QStringList ListLanguages(QString dir);
    QString AutomaticLanguageDetection(QString dir);

private:
    QString language;
    const char* lang_file;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    languages *xml = new languages();
    QDir dir_parse("lang/");

    if(argc > 1)
    {
        QString arg1 = argv[1];
        QString arg2 = argv[2];
        if(arg1 == "export")
        {
            if(arg2 == "all")
            {
                qDebug() << "Exporting all languages";
                dir_parse.removeRecursively();
                xml->ParseAllFiles("en");
                xml->ParseAllFiles("fr");
                xml->ParseAllFiles("de");
                xml->ParseAllFiles("it");
                xml->ParseAllFiles("es");
                xml->ParseAllFiles("pl");
                xml->ParseAllFiles("ru");
                xml->ParseAllFiles("fi");
                xml->ParseAllFiles("hu");
                return 0;
            }
            else if(arg2 == "en")
            {
                qDebug() << "Exporting English";
                xml->ParseAllFiles("en");
                return 0;
            }
            else if(arg2 == "fr")
            {
                qDebug() << "Exporting French";
                xml->ParseAllFiles("fr");
                return 0;
            }
            else if(arg2 == "de")
            {
                qDebug() << "Exporting German";
                xml->ParseAllFiles("de");
                return 0;
            }
            else if(arg2 == "it")
            {
                qDebug() << "Exporting Italian";
                xml->ParseAllFiles("it");
                return 0;
            }
            else if(arg2 == "es")
            {
                qDebug() << "Exporting Spanish";
                xml->ParseAllFiles("es");
                return 0;
            }
            else if(arg2 == "pl")
            {
                qDebug() << "Exporting Polish";
                xml->ParseAllFiles("pl");
                return 0;
            }
            else if(arg2 == "ru")
            {
                qDebug() << "Exporting Russian";
                xml->ParseAllFiles("ru");
                return 0;
            }
            else if(arg2 == "fi")
            {
                qDebug() << "Exporting Finnish";
                xml->ParseAllFiles("fi");
                return 0;
            }
            else if(arg2 == "hu")
            {
                qDebug() << "Exporting Hungarian";
                xml->ParseAllFiles("hu");
                return 0;
            }
            else
            {
                qDebug() << "Language not recognized";
            }
        }
        else if(arg1 == "import")
        {
            if(arg2 == "all")
            {
                qDebug() << "Importing files to all documents";
                xml->ImportAllDocuments(1);
                return 0;
            }
            else if(arg2 == "same")
            {
                qDebug() << "Importing files to documents with same name only";
                xml->ImportAllDocuments(0);
                return 0;
            }
            else
            {
                qDebug() << "Unknown argument for import (all / same)";
            }
        }
        else if (arg1 == "clean") { xml->CleanFiles(); return 0;}
        else if (arg1 == "sort") { xml->SortCategories(); return 0;}
        else
        {
            qDebug() << "Unknown argument (all / <lang>)";
        }
    }
    else
    {
        qDebug() << QString("Civilization IV : XML translation tool v") + QString(constants::VERSION) + QString("\n-------------------------------------------\nBy dbkblk (A New Dawn team)\nNOTA: This executable must be in the same folder than xml files");

        int ch;
        int z = 0;

        do {
        qDebug() << "\nMain menu:\n----------\n1 - Export all languages [Civ 4 XML -> Language XML]\n2 - Export a specific language [Civ 4 XML -> Language XML]\n3 - Import language strings to ALL files [Language XML -> Civ 4 XML]\n4 - Import language strings to SAME files [Language XML -> Civ 4 XML]\n5 - Clean files [Civ 4 XML]\n6 - Sort tags in categories [Civ 4 XML]\n7 - Remove a specific language [Civ 4 XML]\n8 - Find unused tags in files [WARNING: Experimental]\n9 - Exit program\n\n";
        std::cin >> ch;
        std::string lang;
        QString lang_remove;
        switch (ch)
        {
            case 1 :
                dir_parse.removeRecursively();
                xml->ParseAllFiles("en");
                xml->ParseAllFiles("fr");
                xml->ParseAllFiles("de");
                xml->ParseAllFiles("it");
                xml->ParseAllFiles("es");
                xml->ParseAllFiles("pl");
                xml->ParseAllFiles("ru");
                xml->ParseAllFiles("fi");
                xml->ParseAllFiles("hu");
                break;

            case 2 :
                dir_parse.removeRecursively();
                lang_remove = xml->AutomaticLanguageDetection(".");
                if (lang_remove == "error"){break;}
                xml->ParseAllFiles(lang_remove);
                break;

            case 3 :
                xml->ImportAllDocuments(1);
                break;

            case 4 :
                xml->ImportAllDocuments(0);
                break;


            case 5 :
                xml->CleanFiles();
                break;

            case 6 :
                xml->SortCategories();
                break;

            case 7:
                xml->RemoveLanguage();
                break;

            case 8:
                xml->FindUnusedTags();
                break;

            case 9:
                return 0;
                break;

            default :
                break;

        }
        } while (z=1);
    }

    return a.exec();
}

void languages::ParseAllFiles(QString int_lang)
{
    // Language settings
    QString language_code;
    if(int_lang == "English"){language_code = "en";}
    else if(int_lang == "French"){language_code = "fr";}
    else if(int_lang == "German"){language_code = "de";}
    else if(int_lang == "Italian"){language_code = "it";}
    else if(int_lang == "Spanish"){language_code = "es";}
    else if(int_lang == "Polish"){language_code = "pl";}
    else if(int_lang == "Russian"){language_code = "ru";}
    else if(int_lang == "Finnish"){language_code = "fi";}
    else if(int_lang == "Hungarian"){language_code = "hu";}
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
    else if(int_lang == "French"){language_code = "fr";}
    else if(int_lang == "German"){language_code = "de";}
    else if(int_lang == "Italian"){language_code = "it";}
    else if(int_lang == "Spanish"){language_code = "es";}
    else if(int_lang == "Polish"){language_code = "pl";}
    else if(int_lang == "Russian"){language_code = "ru";}
    else if(int_lang == "Finnish"){language_code = "fi";}
    else if(int_lang == "Hungarian"){language_code = "hu";}

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

                                // Check if there is a sub language marker
                                if (!tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().isNull() || !tag_orig.firstChildElement("English").firstChildElement("Text").firstChild().isNull())
                                {
                                    //qDebug() << value_text_tr;
                                    QString new_value_text = value_text_tr;
                                    QString new_value_gender;
                                    QString new_value_plural;

                                    // Check if attributes exists in translated file, then in original file, then use english
                                    if(!tag_tr.attribute("gender").isNull())
                                    {
                                        new_value_gender = tag_tr.attribute("gender");
                                    }
                                    else if(!tag_orig.firstChildElement(tag_lang).firstChildElement("Gender").firstChild().isNull())
                                    {
                                        new_value_gender = tag_orig.firstChildElement(tag_lang).firstChildElement("Gender").firstChild().nodeValue();
                                    }
                                    else
                                    {
                                        new_value_gender = tag_orig.firstChildElement("English").firstChildElement("Gender").firstChild().nodeValue();
                                    }
                                    if(!tag_tr.attribute("plural").isNull())
                                    {
                                        new_value_plural = tag_tr.attribute("plural");
                                    }
                                    else if(!tag_orig.firstChildElement(tag_lang).firstChildElement("Plural").firstChild().isNull())
                                    {
                                        new_value_plural = tag_orig.firstChildElement(tag_lang).firstChildElement("Plural").firstChild().nodeValue();
                                    }
                                    else
                                    {
                                        new_value_plural = tag_orig.firstChildElement("English").firstChildElement("Plural").firstChild().nodeValue();
                                    }

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
                                // Check if the original direct language marker doesn't exist
                                else if(tag_orig.firstChildElement(tag_lang).firstChild().isNull())
                                {
                                    QDomNode new_tag = input.createElement(tag_lang);
                                    tag_orig.appendChild(new_tag);
                                    QDomText new_value = input.createTextNode(value_text_tr);
                                    new_tag.appendChild(new_value);
                                }

                                // Check if there is a direct language marker
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

                                // Check if there is a sub language marker
                                if (!tag_orig.firstChildElement(tag_lang).firstChildElement("Text").firstChild().isNull() || !tag_orig.firstChildElement("English").firstChildElement("Text").firstChild().isNull())
                                {
                                    //qDebug() << value_text_tr;
                                    QString new_value_text = value_text_tr;
                                    QString new_value_gender;
                                    QString new_value_plural;

                                    // Check if attributes exists in translated file, then in original file, then use english
                                    if(!tag_tr.attribute("gender").isNull())
                                    {
                                        new_value_gender = tag_tr.attribute("gender");
                                    }
                                    else if(!tag_orig.firstChildElement(tag_lang).firstChildElement("Gender").firstChild().isNull())
                                    {
                                        new_value_gender = tag_orig.firstChildElement(tag_lang).firstChildElement("Gender").firstChild().nodeValue();
                                    }
                                    else
                                    {
                                        new_value_gender = tag_orig.firstChildElement("English").firstChildElement("Gender").firstChild().nodeValue();
                                    }
                                    if(!tag_tr.attribute("plural").isNull())
                                    {
                                        new_value_plural = tag_tr.attribute("plural");
                                    }
                                    else if(!tag_orig.firstChildElement(tag_lang).firstChildElement("Plural").firstChild().isNull())
                                    {
                                        new_value_plural = tag_orig.firstChildElement(tag_lang).firstChildElement("Plural").firstChild().nodeValue();
                                    }
                                    else
                                    {
                                        new_value_plural = tag_orig.firstChildElement("English").firstChildElement("Plural").firstChild().nodeValue();
                                    }

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
                                // Check if the original direct language marker doesn't exist
                                else if(tag_orig.firstChildElement(tag_lang).firstChild().isNull())
                                {
                                    QDomNode new_tag = input.createElement(tag_lang);
                                    tag_orig.appendChild(new_tag);
                                    QDomText new_value = input.createTextNode(value_text_tr);
                                    new_tag.appendChild(new_value);
                                }

                                // Check if there is a direct language marker
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

    /* Collect all tags in a bigfile */
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

    /* Sorting */

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

void languages::ConvertCiv4ToUTF8(QString file)
{
    /* Read all the file
     * Save the content to another file
     * Replace the other file with the first */

    // First, pure read to remove faulty characters
    QFile amp_read(file);
    QFile amp_write(file + "_amptemp_");
    amp_read.open(QIODevice::ReadOnly);
    amp_write.open(QIODevice::WriteOnly);
    QTextStream amp_in(&amp_read);
    QTextStream amp_out(&amp_write);
    while(!amp_in.atEnd())
    {
        QString line = amp_in.readLine();
        line.replace("&#xd;","").replace(QLatin1Char('&'), "&amp;").replace("&amp;amp;","&amp;").replace("&amp;#","&#").replace("&amp;lt;","&lt;").replace("&amp;gt;","&gt;");
        line.simplified();
        amp_out << line;
    }
    amp_read.close();
    amp_write.close();


    // Open the input file
    QDomDocument read;
    QFile file_in(file + "_amptemp_");
    file_in.open(QIODevice::ReadOnly);
    read.setContent(&file_in);
    QDomElement read_text = read.firstChildElement("Civ4GameText").firstChildElement("TEXT");
    //qDebug() << read_text.firstChildElement().tagName();

    // Create the output file
    QDomDocument write;
    QFile file_out("rewrite.xml");
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDomNode declaration = write.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    write.insertBefore(declaration,write.firstChild());
    QDomNode write_root = write.createElement("Civ4GameText");
    write.appendChild(write_root);

   // Loop the input file
    for (read_text;!read_text.isNull();read_text = read_text.nextSiblingElement())
    {
        //qDebug() << read_text.firstChildElement().tagName();
        QDomElement read_element = read_text.firstChildElement();
        QDomElement write_node = write.createElement("TEXT");
        write_root.appendChild(write_node);

        for (read_element;!read_element.isNull();read_element = read_element.nextSiblingElement())
        {
            //qDebug() << read_element.tagName();
            if(!read_element.firstChildElement("Text").isNull())
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QDomElement write_element_text = write.createElement("Text");
                QDomElement write_element_gender = write.createElement("Gender");
                QDomElement write_element_plural = write.createElement("Plural");
                write_element.appendChild(write_element_text);
                write_element.appendChild(write_element_gender);
                write_element.appendChild(write_element_plural);

                QString write_text_value = read_element.firstChildElement("Text").firstChild().nodeValue();
                QString write_gender_value = read_element.firstChildElement("Gender").firstChild().nodeValue();
                QString write_plural_value = read_element.firstChildElement("Plural").firstChild().nodeValue();

                // Check russian
                if(read_element.tagName() == "Russian")
                {
                    write_text_value = ConvertLatin1ToRussian(write_text_value);
                }

                QDomText write_text = write.createTextNode(write_text_value);
                QDomText write_gender = write.createTextNode(write_gender_value);
                QDomText write_plural = write.createTextNode(write_plural_value);
                write_element_text.appendChild(write_text);
                write_element_gender.appendChild(write_gender);
                write_element_plural.appendChild(write_plural);
            }
            else
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QString write_text_value = read_element.firstChild().nodeValue();

                // Check russian
                if(read_element.tagName() == "Russian")
                {
                    write_text_value = ConvertLatin1ToRussian(write_text_value);
                }

                QDomText write_text = write.createTextNode(write_text_value);
                write_element.appendChild(write_text);
            }
        }
    }
    QTextStream ts(&file_out);
    write.save(ts, 3);
    file_out.close();
    file_in.close();
    QFile::remove(file + "_amptemp_");
    QFile::remove(file);
    QFile::rename("rewrite.xml",file);
}

void languages::ConvertUTF8ToCiv4(QString file)
{
    /* Read all the file
     * Save the content to another file
     * Replace the other file with the first */

    // Open the input file
    QDomDocument read;
    QFile file_in(file);
    file_in.open(QIODevice::ReadOnly);
    read.setContent(&file_in);
    QDomNode read_text = read.firstChildElement("Civ4GameText").firstChildElement();
    //qDebug() << read_text.firstChildElement().tagName();

    // Create the output file
    QDomDocument write;
    QFile file_out(file + "_tempconvert_");
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDomNode declaration = write.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"ISO-8859-1\""));
    write.insertBefore(declaration,write.firstChild());
    QDomNode write_root = write.createElement("Civ4GameText");
    write.appendChild(write_root);
    //qDebug() << write.firstChildElement().tagName();

   // Loop the input file
    for (read_text;!read_text.isNull();read_text = read_text.nextSibling())
    {
        //qDebug() << read_text.firstChildElement().tagName();
        QDomElement read_element = read_text.firstChildElement();
        QDomNode write_node = write.createElement("TEXT");
        write_root.appendChild(write_node);

        for (read_element;!read_element.isNull();read_element = read_element.nextSiblingElement())
        {
            //qDebug() << read_element.tagName();
            if(!read_element.firstChildElement("Text").isNull())
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QDomElement write_element_text = write.createElement("Text");
                QDomElement write_element_gender = write.createElement("Gender");
                QDomElement write_element_plural = write.createElement("Plural");
                write_element.appendChild(write_element_text);
                write_element.appendChild(write_element_gender);
                write_element.appendChild(write_element_plural);

                QString write_text_value = read_element.firstChildElement("Text").firstChild().nodeValue();
                QString write_gender_value = read_element.firstChildElement("Gender").firstChild().nodeValue();
                QString write_plural_value = read_element.firstChildElement("Plural").firstChild().nodeValue();

                // Check russian
                if(read_element.tagName() == "Russian")
                {
                    write_text_value = ConvertRussianToLatin1(write_text_value);
                }

                write_text_value = ConvertStringToCiv4(write_text_value);
                write_gender_value = ConvertStringToCiv4(write_gender_value);
                write_plural_value = ConvertStringToCiv4(write_plural_value);

                QDomText write_text = write.createTextNode(write_text_value);
                QDomText write_gender = write.createTextNode(write_gender_value);
                QDomText write_plural = write.createTextNode(write_plural_value);

                write_element_text.appendChild(write_text);
                write_element_gender.appendChild(write_gender);
                write_element_plural.appendChild(write_plural);
            }
            else
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);

                QString write_text_value = read_element.firstChild().nodeValue();

                // Check russian
                if(read_element.tagName() == "Russian")
                {
                     write_text_value = ConvertRussianToLatin1(write_text_value);
                }

                write_text_value = ConvertStringToCiv4(write_text_value);

                QDomText write_text = write.createTextNode(write_text_value);
                write_element.appendChild(write_text);
            }
        }
    }

    QTextStream ts(&file_out);
    QString docString = write.toString().replace("&amp;", "&").replace(QLatin1Char('&'), "&amp;").replace("&amp;#","&#").replace("&amp;lt;","&lt;").replace("&amp;gt;","&gt;").replace("&amp;amp;","&amp;");
    ts << docString;
    //write.save(ts, 3);
    file_out.close();
    file_in.close();

    QFile::remove(file);
    QFile::rename(file + "_tempconvert_",file);
}

QString languages::ConvertStringToCiv4(QString string)
{
    QString encode;
    for(int i=0;i<string.size();++i)
    {
        QChar ch = string.at(i);
        if(ch.unicode() > 127)
        {
            encode += QChar(38);
            encode += QString("#%1;").arg((int)ch.unicode());
        }
        else
        {
            encode += ch;
        }
    }
    encode.replace("&amp;", "&").replace(QLatin1Char('&'), "&amp;").replace("&amp;#","&#").replace("&amp;lt;","&lt;").replace("&amp;gt;","&gt;");
    return encode;
}

QString languages::ConvertLatin1ToRussian(QString string)
{
    // Convert from C0 (latin1) to B0 (latin5) charsets (8859-1 to 8859-5)

    QString encode;
    for(int i=0;i<string.size();++i)
    {
        QChar ch = string.at(i);

        // Conversion table (see ods file)
        if(ch == QChar(192)){ch = QChar(1040);};
        if(ch == QChar(193)){ch = QChar(1041);};
        if(ch == QChar(194)){ch = QChar(1042);};
        if(ch == QChar(195)){ch = QChar(1043);};
        if(ch == QChar(196)){ch = QChar(1044);};
        if(ch == QChar(197)){ch = QChar(1045);};
        if(ch == QChar(198)){ch = QChar(1046);};
        if(ch == QChar(199)){ch = QChar(1047);};
        if(ch == QChar(200)){ch = QChar(1048);};
        if(ch == QChar(201)){ch = QChar(1049);};
        if(ch == QChar(202)){ch = QChar(1050);};
        if(ch == QChar(203)){ch = QChar(1051);};
        if(ch == QChar(204)){ch = QChar(1052);};
        if(ch == QChar(205)){ch = QChar(1053);};
        if(ch == QChar(206)){ch = QChar(1054);};
        if(ch == QChar(207)){ch = QChar(1055);};
        if(ch == QChar(208)){ch = QChar(1056);};
        if(ch == QChar(209)){ch = QChar(1057);};
        if(ch == QChar(210)){ch = QChar(1058);};
        if(ch == QChar(211)){ch = QChar(1059);};
        if(ch == QChar(212)){ch = QChar(1060);};
        if(ch == QChar(213)){ch = QChar(1061);};
        if(ch == QChar(214)){ch = QChar(1062);};
        if(ch == QChar(215)){ch = QChar(1063);};
        if(ch == QChar(216)){ch = QChar(1064);};
        if(ch == QChar(217)){ch = QChar(1065);};
        if(ch == QChar(218)){ch = QChar(1066);};
        if(ch == QChar(219)){ch = QChar(1067);};
        if(ch == QChar(220)){ch = QChar(1068);};
        if(ch == QChar(221)){ch = QChar(1069);};
        if(ch == QChar(222)){ch = QChar(1070);};
        if(ch == QChar(223)){ch = QChar(1071);};
        if(ch == QChar(224)){ch = QChar(1072);};
        if(ch == QChar(225)){ch = QChar(1073);};
        if(ch == QChar(226)){ch = QChar(1074);};
        if(ch == QChar(227)){ch = QChar(1075);};
        if(ch == QChar(228)){ch = QChar(1076);};
        if(ch == QChar(229)){ch = QChar(1077);};
        if(ch == QChar(230)){ch = QChar(1078);};
        if(ch == QChar(231)){ch = QChar(1079);};
        if(ch == QChar(232)){ch = QChar(1080);};
        if(ch == QChar(233)){ch = QChar(1081);};
        if(ch == QChar(234)){ch = QChar(1082);};
        if(ch == QChar(235)){ch = QChar(1083);};
        if(ch == QChar(236)){ch = QChar(1084);};
        if(ch == QChar(237)){ch = QChar(1085);};
        if(ch == QChar(238)){ch = QChar(1086);};
        if(ch == QChar(239)){ch = QChar(1087);};
        if(ch == QChar(240)){ch = QChar(1088);};
        if(ch == QChar(241)){ch = QChar(1089);};
        if(ch == QChar(242)){ch = QChar(1090);};
        if(ch == QChar(243)){ch = QChar(1091);};
        if(ch == QChar(244)){ch = QChar(1092);};
        if(ch == QChar(245)){ch = QChar(1093);};
        if(ch == QChar(246)){ch = QChar(1094);};
        if(ch == QChar(247)){ch = QChar(1095);};
        if(ch == QChar(248)){ch = QChar(1096);};
        if(ch == QChar(249)){ch = QChar(1097);};
        if(ch == QChar(250)){ch = QChar(1098);};
        if(ch == QChar(251)){ch = QChar(1099);};
        if(ch == QChar(252)){ch = QChar(1100);};
        if(ch == QChar(253)){ch = QChar(1101);};
        if(ch == QChar(254)){ch = QChar(1102);};
        if(ch == QChar(255)){ch = QChar(1103);};
        encode += ch;
    }

    return encode;
}

QString languages::ConvertRussianToLatin1(QString string)
{
    // Convert from C0 (latin1) to B0 (latin5) charsets (8859-1 to 8859-5)

    QString encode;
    for(int i=0;i<string.size();++i)
    {
        QChar ch = string.at(i);

        // Conversion table (see ods file)
        if(ch == QChar(1040)){ch = QChar(192);};
        if(ch == QChar(1041)){ch = QChar(193);};
        if(ch == QChar(1042)){ch = QChar(194);};
        if(ch == QChar(1043)){ch = QChar(195);};
        if(ch == QChar(1044)){ch = QChar(196);};
        if(ch == QChar(1045)){ch = QChar(197);};
        if(ch == QChar(1046)){ch = QChar(198);};
        if(ch == QChar(1047)){ch = QChar(199);};
        if(ch == QChar(1048)){ch = QChar(200);};
        if(ch == QChar(1049)){ch = QChar(201);};
        if(ch == QChar(1050)){ch = QChar(202);};
        if(ch == QChar(1051)){ch = QChar(203);};
        if(ch == QChar(1052)){ch = QChar(204);};
        if(ch == QChar(1053)){ch = QChar(205);};
        if(ch == QChar(1054)){ch = QChar(206);};
        if(ch == QChar(1055)){ch = QChar(207);};
        if(ch == QChar(1056)){ch = QChar(208);};
        if(ch == QChar(1057)){ch = QChar(209);};
        if(ch == QChar(1058)){ch = QChar(210);};
        if(ch == QChar(1059)){ch = QChar(211);};
        if(ch == QChar(1060)){ch = QChar(212);};
        if(ch == QChar(1061)){ch = QChar(213);};
        if(ch == QChar(1062)){ch = QChar(214);};
        if(ch == QChar(1063)){ch = QChar(215);};
        if(ch == QChar(1064)){ch = QChar(216);};
        if(ch == QChar(1065)){ch = QChar(217);};
        if(ch == QChar(1066)){ch = QChar(218);};
        if(ch == QChar(1067)){ch = QChar(219);};
        if(ch == QChar(1068)){ch = QChar(220);};
        if(ch == QChar(1069)){ch = QChar(221);};
        if(ch == QChar(1070)){ch = QChar(222);};
        if(ch == QChar(1071)){ch = QChar(223);};
        if(ch == QChar(1072)){ch = QChar(224);};
        if(ch == QChar(1073)){ch = QChar(225);};
        if(ch == QChar(1074)){ch = QChar(226);};
        if(ch == QChar(1075)){ch = QChar(227);};
        if(ch == QChar(1076)){ch = QChar(228);};
        if(ch == QChar(1077)){ch = QChar(229);};
        if(ch == QChar(1078)){ch = QChar(230);};
        if(ch == QChar(1079)){ch = QChar(231);};
        if(ch == QChar(1080)){ch = QChar(232);};
        if(ch == QChar(1081)){ch = QChar(233);};
        if(ch == QChar(1082)){ch = QChar(234);};
        if(ch == QChar(1083)){ch = QChar(235);};
        if(ch == QChar(1084)){ch = QChar(236);};
        if(ch == QChar(1085)){ch = QChar(237);};
        if(ch == QChar(1086)){ch = QChar(238);};
        if(ch == QChar(1087)){ch = QChar(239);};
        if(ch == QChar(1088)){ch = QChar(240);};
        if(ch == QChar(1089)){ch = QChar(241);};
        if(ch == QChar(1090)){ch = QChar(242);};
        if(ch == QChar(1091)){ch = QChar(243);};
        if(ch == QChar(1092)){ch = QChar(244);};
        if(ch == QChar(1093)){ch = QChar(245);};
        if(ch == QChar(1094)){ch = QChar(246);};
        if(ch == QChar(1095)){ch = QChar(247);};
        if(ch == QChar(1096)){ch = QChar(248);};
        if(ch == QChar(1097)){ch = QChar(249);};
        if(ch == QChar(1098)){ch = QChar(250);};
        if(ch == QChar(1099)){ch = QChar(251);};
        if(ch == QChar(1100)){ch = QChar(252);};
        if(ch == QChar(1101)){ch = QChar(253);};
        if(ch == QChar(1102)){ch = QChar(254);};
        if(ch == QChar(1103)){ch = QChar(255);};
        encode += ch;
    }

    return encode;
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
            tags << xml_tag.firstChildElement("Tag").text();
        }
        file_in.close();
    }

    tags.sort();

    return tags;
}

QStringList languages::ListLanguages(QString dir)
{
    QStringList tags;
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

    /* Search the tag trough existing files and replace with the new value if found */
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

QString languages::AutomaticLanguageDetection(QString dir)
{
    // Automatic language detection / menu
    QStringList known_languages = ListLanguages(dir);
    QString lang;
    QString lang_check;
    std::string answer;
    int lang_counter = 0;

    qDebug("\nWhich language do you want to process ?\n");

    foreach(lang, known_languages)
    {
        lang_counter++;
        qDebug() << lang_counter << ")" << lang;
    }
    qDebug() << lang_counter+1 << ")  Exit";

    std::cin >> answer;
    lang_counter = 0;

    if(QString::fromStdString(answer).toInt() > known_languages.count()+1)
    {
        qDebug("Invalid selection");
        return "error";
    }
    else if (QString::fromStdString(answer).toInt() == known_languages.count()+1)
    {
        return "error";
    }

    foreach(lang, known_languages)
    {
        lang_counter++;
        if(lang_counter == QString::fromStdString(answer).toInt())
        {
            qDebug() << "Processing" << lang;
            lang_check = lang;
        }
    }
    return lang_check;
}

void languages::FindUnusedTags()
{
    // Check the directory
    QDir root(".");
    if(!root.exists("../../../Assets/"))
    {
        qDebug() << "The parser need to be in 'Assets/XML/Text/' folder for this function to work. Aborting...";
        return;
    }

    // Getting game path
    QFile categories("_xml_parser.config");
    if(!categories.exists())
    {
       qDebug() << "The parser need a '_xml_parser.config' file to continue. Aborting...";
       return;
    }
    QDomDocument xml_categories;
    categories.open(QIODevice::ReadOnly);
    xml_categories.setContent(&categories);
    categories.close();
    QString gamepath = xml_categories.firstChildElement().firstChildElement("gamepath").firstChild().nodeValue();
    if (gamepath.isEmpty())
    {
        qDebug() << "You need to add the base game path to the '_xml_parser.config' file";
        return;
    }

    // List tags
    qDebug() << "Listing tags...";
    QStringList list_tags = ListTags(".");
    QString tag;
    list_tags.removeDuplicates();
    int tags_total_counter = list_tags.count() + 1;

    qDebug() << "Collecting Python and XML files...";
    QStringList list_ext;
    list_ext << "*.xml" << "*.py";

    // List files in mod folder
    QStringList exclusion_list;
    exclusion_list << "Assets/XML/Text/";
    root.setCurrent("../../../");
    QDir game_folder(gamepath);

    /* Collect all files in a bigfile */
    QDirIterator mod_iterator(root.absolutePath(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QDirIterator game_iterator(game_folder.path(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QFile output("TEMPFILE");
    output.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&output);
    int file_counter = 0;
    while (mod_iterator.hasNext())
    {
        // Check if it isn't in the text folder
        if(mod_iterator.filePath().contains("Assets/XML/Text"))
        {
            mod_iterator.next();
        }

        // Check for dll

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
        if(game_iterator.filePath().contains("Assets/XML/Text"))
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

            game_iterator.next();
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

    // Checking DLL
    qDebug() << "Checking DLL";
    foreach(tag,list_tags)
    {
        int temp = 0;
        while(temp == 0)
        {
            // Mod
            if(FindTagsInDLL(tag.toStdString(), QString("Assets/CvGameCoreDLL.dll").toStdString()))
            {
                list_tags.removeAll(tag);
                found++;
                tags_total_counter--;
                if(tags_total_counter%50==0)
                {
                    qDebug() << "Still" << tags_total_counter << "tags to process";
                }
				temp++;
            }

            // Classic game
            QString game_path_std = game_folder.path() + "/Assets/CvGameCoreDLL.dll";
            if(FindTagsInDLL(tag.toStdString(), game_path_std.toStdString()))
            {
                list_tags.removeAll(tag);
                tags_total_counter--;
                if(tags_total_counter%50==0)
                {
                    qDebug() << "Still" << tags_total_counter << "tags to process";
                }
                temp++;
            }

            // Beyond the sword
            game_path_std = game_folder.path() + "/Beyond the Sword/Assets/CvGameCoreDLL.dll";
            if(FindTagsInDLL(tag.toStdString(), game_path_std.toStdString()))
            {
                list_tags.removeAll(tag);
                tags_total_counter--;
                if(tags_total_counter%50==0)
                {
                    qDebug() << "Still" << tags_total_counter << "tags to process";
                }
                temp++;
            }

            // Warlords
            game_path_std = game_folder.path() + "/Warlords/Assets/CvGameCoreDLL.dll";
            if(FindTagsInDLL(tag.toStdString(), game_path_std.toStdString()))
            {
                list_tags.removeAll(tag);
                tags_total_counter--;
                if(tags_total_counter%50==0)
                {
                    qDebug() << "Still" << tags_total_counter << "tags to process";
                }
                temp++;
            }
            temp++;
        }

    }

    qDebug() << tags_total_counter << "have not been found. Printing list: _tags_not_found.txt in text folder";

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
}
