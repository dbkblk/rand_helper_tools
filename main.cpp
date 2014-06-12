#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QtXml/QtXml>

namespace constants {
const QString VERSION = "0.7";
}

class languages
{
public:
    void ParseDocument(QString input_file, QString language);
    void ParseAllFiles(QString language);
    void ImportAllDocuments(bool all_files);
    void ImportDocumentToAll(QString int_lang, QStringList file_list);
    void ImportDocumentToSameName(QString int_lang, QStringList file_list);
    QStringList ListTags();
    void CleanFiles();
    void SortCategories();
    void FindUnusedTags();
    void SorterHelper(QString prefix);
    QString ConvertStringToCiv4(QString string);
    void ConvertCiv4ToUTF8(QString file);
    void ConvertUTF8ToCiv4(QString file);
    QStringList CheckImportLanguages();

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
                xml->ParseAllFiles("zh");
                xml->ParseAllFiles("pl");
                xml->ParseAllFiles("ja");
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
            else if(arg2 == "zh")
            {
                qDebug() << "Exporting Chinese";
                xml->ParseAllFiles("zh");
                return 0;
            }
            else if(arg2 == "pl")
            {
                qDebug() << "Exporting Polish";
                xml->ParseAllFiles("pl");
                return 0;
            }
            else if(arg2 == "ja")
            {
                qDebug() << "Exporting Japanese";
                xml->ParseAllFiles("ja");
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
        qDebug() << "Civilization IV : XML translation tool v" + QString(constants::VERSION) + "\n-------------------------------------------\nNOTA: This executable must be in the same folder than xml files.";

        int ch;
        int z = 0;

        do {
        qDebug() << "\nMain menu:\n----------\n1 - Export all languages [Civ 4 XML -> Language XML]\n2 - Export a specific language [Civ 4 XML -> Language XML]\n3 - Import language strings to ALL files [Language XML -> Civ 4 XML]\n4 - Import language strings to SAME files [Language XML -> Civ 4 XML]\n5 - Clean files [Civ 4 XML]\n6 - Sort tags in categories [Civ 4 XML]\n7 (WIP) - Find unused tags [Civ 4 XML]\n8 - Exit program\n\n";
        std::cin >> ch;
        std::string lang;
        switch (ch)
        {
            case 1 :
                dir_parse.removeRecursively();
                xml->ParseAllFiles("en");
                xml->ParseAllFiles("fr");
                xml->ParseAllFiles("de");
                xml->ParseAllFiles("it");
                xml->ParseAllFiles("es");
                xml->ParseAllFiles("zh");
                xml->ParseAllFiles("pl");
                xml->ParseAllFiles("ja");
                xml->ParseAllFiles("ru");
                xml->ParseAllFiles("fi");
                xml->ParseAllFiles("hu");
                break;

            case 2 :
                dir_parse.removeRecursively();
                std::cout << "Which language do you want to EXPORT (en, fr, de, it, es, pl, ja, zh, ru, fi, hu) ?\n";
                std::cin >> lang;
                xml->ParseAllFiles(QString::fromStdString(lang));
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
                std::cout << "Function disabled. Still work in progress\n";
                //xml->FindUnusedTags();
                break;

            case 8:
                return 0;
                break;

            default :
                break;

        }
        } while (z=1);
    }

    return a.exec();
}

void languages::ParseAllFiles(QString language)
{
    // List all files
    qDebug() << "Preparing files...";
    QDir root(".");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList folders;
    folders = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        ParseDocument(*it,language);
    }

    // Language settings
    QString int_lang;
    if(language == "en"){int_lang = "English";}
    else if(language == "fr"){int_lang = "French";}
    else if(language == "de"){int_lang = "German";}
    else if(language == "it"){int_lang = "Italian";}
    else if(language == "es"){int_lang = "Spanish";}
    else if(language == "pl"){int_lang = "Polish";}
    else if(language == "ja"){int_lang = "Japanese";}
    else if(language == "zh"){int_lang = "Chinese";}
    else if(language == "ru"){int_lang = "Russian";}
    else if(language == "fi"){int_lang = "Finnish";}
    else if(language == "hu"){int_lang = "Hungarian";}
    else { qDebug() << "Invalid language code. Aborting..."; return;}
    QString output_dir = "lang/" + language + "/";

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

void languages::ParseDocument(QString input_file, QString language)
{
    QDomDocument input;
    QDomDocument output;

    QString int_lang;

    // Check language
    int_lang = "English";
    if(language == "fr"){int_lang = "French";};
    if(language == "de"){int_lang = "German";};
    if(language == "it"){int_lang = "Italian";};
    if(language == "es"){int_lang = "Spanish";};
    if(language == "pl"){int_lang = "Polish";};
    if(language == "ja"){int_lang = "Japanese";};
    if(language == "zh"){int_lang = "Chinese";};
    if(language == "ru"){int_lang = "Russian";};
    if(language == "fi"){int_lang = "Finnish";};
    if(language == "hu"){int_lang = "Hungarian";};

    // Getting filenames
    input_file.replace(".XML", ".xml", Qt::CaseSensitive);
    QString input_temp_file = "__temp__" + input_file;
    QFile::copy(input_file,input_temp_file);
    QString output_dir = "lang/" + language + "/";
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
    /*// Translated files
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
            if(*it = *tr)
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
                        value_text_tr.replace("\\'","\'");
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
                            value_text = "(empty)";
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
                            value_text_tr = "(empty)";
                        }

                        // Compare original and modified strings
                        if(value_text != value_text_tr && skip == 0)
                        {
                            // Do not overwrite if the translated file has an empty value
                            if(value_text != "(empty)" && value_text_tr == "(empty)")
                            {
                                //QString operation;
                                //operation = "--WARNING-- FILE:" + QString(*it) + " | TAG: " + QString(value_tag) + " | OLD: " + value_text + " -> NEW: (empty)";
                                //print_list << operation;
                                //qDebug() << operation;
                            }

                            // If both have different values
                            else if(value_text != value_text_tr)
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
        }

        // Save to file
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
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

        qDebug() << "\n\nFiles successfully processed. Modified files are in 'imported/'.\nA report of the modified values have been generated in 'imported/imported_values.txt'";*/

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

    /* New logic:
     * =Check parse file presence
     * Prepare original files by converting them to UTF8
     * List all tags in a QStringList
     * Initialize all XML files by category plus MISC. Add a _PEDIA and _REMOVED_ for all categories
     * Open all XMLDocuments categories in memory
     * For each tag (until the end of the list):
        * Check category
        * Check all files looking for the tag
        * Check if the tag has already been found
        * Add the tag to the XML Document
    * Save all XML Documents
    * Convert the files back to Civ4
    * Print the duplicate list */

    // Check requirements
    QFile categories("_categories.parse");
    if(!categories.exists())
    {
       qDebug() << "The parser need a '_categories.parse' file to continue. Aborting...";
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

    // List all tags
    qDebug() << "Listing tags...";
    QStringList duplicate_tags;
    QStringList tags = ListTags();
    tags.removeDuplicates();
    int tags_total_counter = tags.count();
    qDebug() << "Detected" << tags_total_counter << "tags";

    // Check categories
    qDebug() << "Checking categories...";
    QDomDocument xml_categories;
    categories.open(QIODevice::ReadOnly);
    xml_categories.setContent(&categories);
    categories.close();

    // Initialize all XML classics, pedia and removed
    QDomElement read_categories = xml_categories.firstChildElement().firstChildElement();
    for(read_categories;!read_categories.isNull(); read_categories = read_categories.nextSiblingElement())
    {
        QDomDocument xml_temp;
        xml_temp.insertBefore(xml_temp.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_temp.firstChild());
        xml_temp.appendChild(xml_temp.createElement("Civ4GameText"));

        QString filename = read_categories.attribute("file");
        QString filename_pedia = "sorted/new/" + filename + "_PEDIA.xml";
        QString filename_removed = "sorted/new/_REMOVED_TAGS_" + filename + ".xml";
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

    /* For each tag in the list, determine category, then check each file and each tag
     * For each tag, check category and copy all the elements to the corresponding file
     * If a tag has already been found, copy it in "removed_category" file */

    QDir import_dir("sorted/");
    QStringList import_files;
    import_files = import_dir.entryList(xml_filter, QDir::Files);
    QString tag;
    int tags_counter = 1;
    foreach(tag,tags)
    {
        int tag_occurence = 0;
        //qDebug() << "Checking " << tag;
        // Check category of the tag
        QDomElement xml_category = xml_categories.firstChildElement().firstChildElement();
        QString category = "MISC";
        QString category_file;
        for(xml_category;!xml_category.isNull();xml_category = xml_category.nextSiblingElement())
        {
            //qDebug() << xml_category.attribute("file");
            QDomElement xml_category_tag = xml_category.firstChildElement();
            for(xml_category_tag;!xml_category_tag.isNull();xml_category_tag = xml_category_tag.nextSiblingElement())
            {
                //qDebug() << "Comparing " << tag << " to " << xml_category_tag.firstChild().nodeValue();
                if(tag.startsWith(xml_category_tag.firstChild().nodeValue()))
                {
                    category = xml_category.attribute("file");

                }
            }
        }
        if(tag.contains("_PEDIA"))
        {
            category_file = "sorted/new/" + category + "_PEDIA.xml";
        }
        else
        {
            category_file = "sorted/new/" + category + ".xml";
        }

        //qDebug() << category_file;

        qDebug() << "Sorting tag" << tags_counter << "of" << tags_total_counter << ":" << tag;

        // Looking for tag in files
        for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
        {
            QString current = "sorted/" + *it;
            QDomDocument input;
            QFile file_input(current);
            file_input.open(QIODevice::ReadOnly);
            input.setContent(&file_input);
            file_input.close();

            QDomElement input_text_node = input.firstChildElement().firstChildElement();
            for(input_text_node;!input_text_node.isNull();input_text_node = input_text_node.nextSiblingElement())
            {
                if(tag == input_text_node.firstChildElement("Tag").firstChild().nodeValue())
                {
                    //qDebug() << "Occurence is " << tag_occurence;
                    QDomDocument xml_detected;
                    // If tag has already been checked, move the tag to removed file
                    if(tag_occurence > 0)
                    {
                        qDebug() << "Duplicate detected";
                        QString operation = "TAG:" + tag + " | FILE:" + category + ".xml";
                        duplicate_tags << operation;
                        category_file = "sorted/new/_REMOVED_TAGS_" + category + ".xml";
                        QFile file_cat(category_file);
                        file_cat.open(QIODevice::ReadOnly);
                        xml_detected.setContent(&file_cat);
                        file_cat.close();
                    }
                    else
                    {
                        // If it's first time, do it the classical way
                        QFile file_cat(category_file);
                        file_cat.open(QIODevice::ReadOnly);
                        xml_detected.setContent(&file_cat);
                        file_cat.close();
                    }

                    // Write the node to the correct category
                    QDomElement input_element_node = input_text_node.firstChildElement();
                    QDomElement xml_detected_node = xml_detected.firstChildElement();
                    QDomNode xml_text_node = xml_detected.createElement("Text");
                    //qDebug() << xml_detected.firstChildElement().tagName();
                    xml_detected_node.appendChild(xml_text_node);
                    //qDebug() << xml_detected_node.firstChildElement().tagName();
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

                    // Save the xml
                    if(tag_occurence > 0)
                    {
                        category_file = "sorted/new/_REMOVED_TAGS_" + category + ".xml";
                        QFile file_cat_temp(category_file + "_TEMP_.xml");
                        QFile::remove(category_file + "_TEMP_.xml");
                        file_cat_temp.open(QIODevice::WriteOnly);
                        QTextStream ts(&file_cat_temp);
                        xml_detected.save(ts,4);
                        file_cat_temp.close();
                        QFile::remove(category_file);
                        file_cat_temp.rename(category_file);
                    }
                    else
                    {
                        QFile file_cat_temp("sorted/new/_TEMP_.xml");
                        QFile::remove("sorted/new/_TEMP_.xml");
                        file_cat_temp.open(QIODevice::WriteOnly);
                        QTextStream ts(&file_cat_temp);
                        xml_detected.save(ts,4);
                        file_cat_temp.close();
                        QFile::remove(category_file);
                        file_cat_temp.rename(category_file);
                    }
                    xml_detected.clear();
                    tag_occurence++;
                }
            }
            input.clear();
        }
        tags_counter++;
    }

    // Clean up folder and encoding back
    qDebug() << "Clean up and preparing files";
    import_files = import_dir.entryList(xml_filter, QDir::Files);
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

                QDomText write_text = write.createTextNode(ConvertStringToCiv4(read_element.firstChildElement("Text").firstChild().nodeValue()));
                QDomText write_gender = write.createTextNode(ConvertStringToCiv4(read_element.firstChildElement("Gender").firstChild().nodeValue()));
                QDomText write_plural = write.createTextNode(ConvertStringToCiv4(read_element.firstChildElement("Plural").firstChild().nodeValue()));

                write_element_text.appendChild(write_text);
                write_element_gender.appendChild(write_gender);
                write_element_plural.appendChild(write_plural);
            }
            else
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QDomText write_text = write.createTextNode(ConvertStringToCiv4(read_element.firstChild().nodeValue()));
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

QStringList languages::ListTags()
{
    QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    QDir root(".");
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

void languages::FindUnusedTags()
{
    // Check the directory
    QDir root(".");
    if(!root.exists("../../../Assets/"))
    {
        qDebug() << "The parser need to be in 'Assets/XML/Text/' folder for this function to work. Aborting...";
        return;
    }

    // List tags
    qDebug() << "Listing tags...";
    QStringList list_tags = ListTags();
    QString tag;
    list_tags.removeDuplicates();
    int tags_total_counter = list_tags.count();

    // List files
    qDebug() << "Checking Python and XML files for tags...";
    QStringList list_ext;
    list_ext << "*.xml" << "*.py";

    QStringList exclusion_list;
    exclusion_list << "Assets/XML/Text/";
    root.setPath("../../../");
    QDirIterator iterator(root.absolutePath(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    foreach(tag,list_tags)
    {
        int found = 0;
        do
        {
            qDebug() << iterator.fileName();
            QFile file(iterator.filePath());
            /*if(!file.open(QFile::ReadOnly))
            {
                qDebug() << "Can't open" << iterator.filePath();
            }
            QTextStream ts(&file);
            while(!ts.atEnd())
            {
                QString tag_check = ts.readLine();
                if(tag_check.contains(tag))
                {
                    qDebug() << "Tag:" << tag << "found in" << iterator.filePath();
                    found++;
                }
            }*/


        }while (iterator.hasNext() || found == 0);
    }


}
