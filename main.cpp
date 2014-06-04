#include <iostream>
#include <QtCore>
#include <QDebug>
#include <QtXml/QtXml>

class languages
{
public:
    void ParseDocument(QString input_file, QString language);
    void ParseAllFiles(QString language);
    void ImportDocumentToAll(QString language);
    void ImportDocumentToSameName(QString language);
    QStringList ListTags();
    void CleanFiles();
    void SortCategories();
    void SorterHelper(QString prefix);
    QString ConvertStringToCiv4(QString string);
    void ConvertCiv4ToUTF8(QString file);
    void ConvertUTF8ToCiv4(QString file);

private:
    QString language;
    const char* lang_file;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    languages *xml = new languages();

    QDir dir("lang/");
    dir.removeRecursively();
    QDir dir_export("export/");
    dir_export.removeRecursively();
    QDir import_dir;

    qDebug() << "Civilization IV : XML translation tool v0.6\n-------------------------------------------\nNOTA: This executable must be in the same folder than xml files.";

    int ch;
    int z = 0;

    do {
    qDebug() << "\nMain menu:\n----------\n1 - Export all languages [Civ 4 XML -> Language XML]\n2 - Export a specific language [Civ 4 XML -> Language XML]\n3 - Import language strings to ALL files [Language XML -> Civ 4 XML]\n4 - Import language strings to SAME files [Language XML -> Civ 4 XML]\n5 - Clean files [Civ 4 XML]\n6 - Sort tags in categories [Civ 4 XML]\n7 - Exit program\n\n";
    std::cin >> ch;
    std::string lang;
    switch (ch)
    {
        case 1 :
            xml->ParseAllFiles("en");
            xml->ParseAllFiles("fr");
            xml->ParseAllFiles("ge");
            xml->ParseAllFiles("it");
            xml->ParseAllFiles("sp");
            xml->ParseAllFiles("po");
            break;

        case 2 :
            std::cout << "Which language do you want to EXPORT (en, fr, ge, it, sp, po) ?\n";
            std::cin >> lang;
            if(lang == "en" && lang == "fr" && lang == "ge" && lang == "it" && lang == "sp" && lang == "po")
            {
                qDebug() << "\nA valid language code is required";
                break;
            }
            xml->ParseAllFiles(QString::fromStdString(lang));
            break;

        case 3 :
            std::cout << "Which language do you want to IMPORT (en, fr, ge, it, sp, po) ?\nNB: You can only import one language at a time.\n";
            std::cin >> lang;
            if(lang == "en" && lang == "fr" && lang == "ge" && lang == "it" && lang == "sp" && lang == "po")
            {
                qDebug() << "\nA valid language code is required";
                break;
            }
            xml->ImportDocumentToAll(QString::fromStdString(lang));
            break;

        case 4 :
            std::cout << "Which language do you want to IMPORT (en, fr, ge, it, sp, po) ?\nNB: You can only import one language at a time.\n";
            std::cin >> lang;
            if(lang == "en" && lang == "fr" && lang == "ge" && lang == "it" && lang == "sp" && lang == "po")
            {
                qDebug() << "\nA valid language code is required";
                break;
            }
            xml->ImportDocumentToSameName(QString::fromStdString(lang));
            break;


        case 5 :
            xml->CleanFiles();
            break;

        case 6 :
            xml->SortCategories();
            break;

        case 7:
            return 0;
            break;

        default :
            break;

    }
    } while (z=1);

    return a.exec();
}

void languages::ParseAllFiles(QString language)
{
    // List all files

    QDir root(".");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList folders;
    folders = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        ParseDocument(*it,language);
    }

    // For debug purpose
    QString int_lang;
    int_lang = "English";
    if(language == "fr"){int_lang = "French";};
    if(language == "ge"){int_lang = "German";};
    if(language == "it"){int_lang = "Italian";};
    if(language == "sp"){int_lang = "Spanish";};
    if(language == "po"){int_lang = "Polish";};
    QString output_dir = "lang/" + int_lang + "/";
    qDebug() << int_lang << " language successfully exported to " << output_dir;
}

void languages::ParseDocument(QString input_file, QString language)
{
    QDomDocument input;
    QDomDocument output;

    QString int_lang;
    int_lang = "English";

    // Check language
    if(language == "fr"){int_lang = "French";};
    if(language == "ge"){int_lang = "German";};
    if(language == "it"){int_lang = "Italian";};
    if(language == "sp"){int_lang = "Spanish";};
    if(language == "po"){int_lang = "Polish";};

    // Getting filenames
    input_file.replace(".XML", ".xml", Qt::CaseSensitive);
    QString input_temp_file = "__temp__" + input_file;
    QFile::copy(input_file,input_temp_file);
    QString output_dir = "lang/" + int_lang + "/";
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
            new_tag.firstChild().setNodeValue("");
            new_tag.setAttribute("name", value_tag);
        }
        else if (!tag_el.firstChildElement(int_lang).firstChildElement("Text").firstChild().isNull())
        {
            new_tag.appendChild(output.createTextNode(tag_el.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue()));
            new_tag.setAttribute("plural", tag_el.firstChildElement(int_lang).firstChildElement("Plural").firstChild().nodeValue());
            new_tag.setAttribute("gender", tag_el.firstChildElement(int_lang).firstChildElement("Gender").firstChild().nodeValue());
            new_tag.setAttribute("name", value_tag);
        }
        else
        {
            QDomText node_value = output.createTextNode(tag_el.firstChildElement(int_lang).firstChild().nodeValue());
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

void languages::ImportDocumentToAll(QString language)
{
    QDir dir_import;
    dir_import.rmdir("imported/");
    dir_import.mkdir("imported");

    // Check language
    QString int_lang;
    if(language == "fr"){int_lang = "French";};
    if(language == "ge"){int_lang = "German";};
    if(language == "it"){int_lang = "Italian";};
    if(language == "sp"){int_lang = "Spanish";};
    if(language == "po"){int_lang = "Polish";};
    if(language == "en"){int_lang = "English";};
    qDebug() << "Selected language is " << int_lang;

    /* Search the tag trough existing files and replace with the new value if found */
    QStringList xml_filter;
    xml_filter << "*.xml";

    // Root files
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Import files
    QDir import_dir("imported/");
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"imported/"+*it);
        ConvertCiv4ToUTF8("imported/"+*it);
    }
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Translated files
    QDir translated("import/");
    QStringList files_translated;
    files_translated = translated.entryList(xml_filter, QDir::Files);

    // Print list

    QStringList print_list;

    // Entering loop

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
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
            QString current_new = "import/" + *tr;
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
                        QString value_text_tr = tag_tr.text();
                        QString value_text;

                        // Checking tag presence

                        if(tag_orig.firstChildElement(int_lang).isNull())
                        {
                            //qDebug() << "No tag";
                            value_text = "";
                        }

                        else if (tag_orig.firstChildElement(int_lang).firstChild().nodeValue().isNull())
                        {
                            if(!tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue().isNull())
                            {
                                //qDebug() << "No value";
                                value_text = tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue();
                            }
                            else
                            {
                                value_text = "";
                            }
                        }

                        else
                        {
                            //qDebug() << "Value set";
                            value_text = tag_orig.firstChildElement(int_lang).firstChild().nodeValue();
                        }

                        //qDebug() << value_tag << " : " << value_text << " / " << value_text_tr;

                        // Normalize value
                        if(value_text_tr.isNull())
                        {
                            value_text_tr = "";
                        }

                        // Compare original and modified strings
                        if(value_text != value_text_tr)
                        {
                            // Check if the modified file has a value
                            if(value_text != "" && value_text_tr == "")
                            {
                                QString operation;
                                operation = "--WARNING-- TAG:" + QString(value_tag)+ " | ORIGINAL FILE: " + QString(current) + " | IMPORT FILE: " + QString(current_new) + " has empty value.";
                                print_list << operation;
                                //qDebug() << operation;
                            }

                            // Major cases
                            else if(value_text != value_text_tr)
                            {
                                QString operation;
                                operation = "FILE: " + current + " | TAG: " + value_tag + " | OLD: " + value_text + " -> NEW: " + value_text_tr;
                                print_list << operation;
                                //qDebug() << operation;

                                // Checking if tag is not subtag, again...
                                if(tag_orig.firstChildElement(int_lang).isNull())
                                {
                                }
                                else if (tag_orig.firstChildElement(int_lang).firstChild().nodeValue().isNull())
                                {
                                    if(!tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue().isNull())
                                    {
                                        tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().setNodeValue(value_text_tr);
                                        // Check for additionnal attributes
                                        if(!tag_tr.attribute("gender").isNull())
                                        {
                                            tag_orig.firstChildElement(int_lang).firstChildElement("Gender").firstChild().setNodeValue(tag_tr.attribute("gender"));
                                        }
                                        if(!tag_tr.attribute("plural").isNull())
                                        {
                                            tag_orig.firstChildElement(int_lang).firstChildElement("Plural").firstChild().setNodeValue(tag_tr.attribute("plural"));
                                        }
                                    }
                                }
                                else
                                {
                                    tag_orig.firstChildElement(int_lang).firstChild().setNodeValue(value_text_tr);
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
    QFile print_file("imported/imported_values.txt");

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

void languages::ImportDocumentToSameName(QString language)
{
    QDir dir_import;
    dir_import.rmdir("imported/");
    dir_import.mkdir("imported");

    // Check language
    QString int_lang;
    if(language == "fr"){int_lang = "French";};
    if(language == "ge"){int_lang = "German";};
    if(language == "it"){int_lang = "Italian";};
    if(language == "sp"){int_lang = "Spanish";};
    if(language == "po"){int_lang = "Polish";};
    if(language == "en"){int_lang = "English";};
    qDebug() << "Selected language is " << int_lang;

    /* Search the tag trough existing files and replace with the new value if found */
    QStringList xml_filter;
    xml_filter << "*.xml";

    // Root files
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Import files
    QDir import_dir("imported/");
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"imported/"+*it);
        ConvertCiv4ToUTF8("imported/"+*it);
    }
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Translated files
    QDir translated("import/");
    QStringList files_translated;
    files_translated = translated.entryList(xml_filter, QDir::Files);

    // Print list

    QStringList print_list;

    // Entering loop

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
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
            //qDebug() << "Comparing " << *it << " to " << *tr;
            if (*it == *tr)
            {
                QString current_new = "import/" + *tr;
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
                            QString value_text_tr = tag_tr.firstChild().nodeValue();
                            QString value_text;

                            // Checking tag presence

                            if(tag_orig.firstChildElement(int_lang).isNull())
                            {
                                //qDebug() << "No tag";
                                value_text = "";
                            }

                            else if (tag_orig.firstChildElement(int_lang).firstChild().nodeValue().isNull())
                            {
                                if(!tag_orig.firstChildElement(int_lang).firstChildElement("Text").isNull())
                                {
                                    //qDebug() << "No value";
                                    value_text = tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue();
                                }
                                else
                                {
                                    value_text = "";
                                }
                            }

                            else
                            {
                                //qDebug() << "Value set";
                                value_text = tag_orig.firstChildElement(int_lang).firstChild().nodeValue();
                            }

                            //qDebug() << value_tag << " : " << value_text << " / " << value_text_tr;

                            // Normalize value
                            if(value_text_tr.isNull())
                            {
                                value_text_tr = "";
                            }

                            // Compare original and modified strings
                            if(value_text != value_text_tr)
                            {
                                // Check if the modified file has a value
                                if(value_text != "" && value_text_tr == "")
                                {
                                    QString operation;
                                    operation = "--WARNING-- TAG:" + QString(value_tag)+ " | ORIGINAL FILE: " + QString(current) + " | IMPORT FILE: " + QString(current_new) + " has empty value.";
                                    print_list << operation;
                                    //qDebug() << operation;
                                }

                                // Major cases
                                else if(value_text != value_text_tr)
                                {
                                    QString operation;
                                    operation = "FILE: " + current + " | TAG: " + value_tag + " | OLD: " + value_text + " -> NEW: " + value_text_tr;
                                    print_list << operation;
                                    //qDebug() << operation;

                                    // Checking if tag is not subtag, again...
                                    if(tag_orig.firstChildElement(int_lang).isNull())
                                    {
                                    }
                                    else if (tag_orig.firstChildElement(int_lang).firstChild().nodeValue().isNull())
                                    {
                                        if(!tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().nodeValue().isNull())
                                        {
                                            tag_orig.firstChildElement(int_lang).firstChildElement("Text").firstChild().setNodeValue(value_text_tr);
                                            // Check for additionnal attributes
                                            if(!tag_tr.attribute("gender").isNull())
                                            {
                                                tag_orig.firstChildElement(int_lang).firstChildElement("Gender").firstChild().setNodeValue(tag_tr.attribute("gender"));
                                            }
                                            if(!tag_tr.attribute("plural").isNull())
                                            {
                                                tag_orig.firstChildElement(int_lang).firstChildElement("Plural").firstChild().setNodeValue(tag_tr.attribute("plural"));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        tag_orig.firstChildElement(int_lang).firstChild().setNodeValue(value_text_tr);
                                    }
                                }


                            }
                        }
                    }

                }
            }
            else
            {

            }
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
    QFile print_file("imported/imported_values.txt");

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
    // Listing root files
    qDebug() << "Listing files...";
    QDir dir_import;
    dir_import.rmdir("sorted/");
    dir_import.mkpath("sorted/new/");
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
    QFile categories("categories.parse");
    if(!categories.exists())
    {
       qDebug() << "The parser need a 'categories.parse' file to continue. Aborting...";
       return;
    }
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

    // Open the input file
    QDomDocument read;
    QFile file_in(file);
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

                QDomText write_text = write.createTextNode(read_element.firstChildElement("Text").firstChild().nodeValue());
                QDomText write_gender = write.createTextNode(read_element.firstChildElement("Gender").firstChild().nodeValue());
                QDomText write_plural = write.createTextNode(read_element.firstChildElement("Plural").firstChild().nodeValue());
                write_element_text.appendChild(write_text);
                write_element_gender.appendChild(write_gender);
                write_element_plural.appendChild(write_plural);
            }
            else
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QDomText write_text = write.createTextNode(read_element.firstChild().nodeValue());
                write_element.appendChild(write_text);
            }
        }
    }
    file_out.write(write.toByteArray());
    file_out.close();
    file_in.close();
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

    QTextStream TextStream(&file_out);
    QString docString = write.toString().replace("&amp;", "&");
    TextStream << docString;
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
    QDir dir_import;
    dir_import.rmdir("cleaned/");
    dir_import.mkdir("cleaned");

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

    // Entering loop

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

        QDomElement input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");

        // Check file integrity
        if (input_node.text().isNull())
        {
            qDebug() << current << " is not properly formatted";
            QFile::remove(current);
        }
        else
        {
            // Check english tag
            for(input_node;!input_node.isNull();input_node = input_node.nextSiblingElement())
            {
                QString english_tag = input_node.firstChildElement("English").firstChild().nodeValue();
                QDomElement element = input_node.firstChildElement();
                int wait = 0;
                QDomNode remove_element;
                for(element;!element.isNull();element = element.nextSiblingElement())
                {
                    // Remove the previous element in the loop if it has been detected
                    if (wait > 0)
                    {
                        remove_element = element.previousSiblingElement();
                        element.previousSiblingElement().parentNode().removeChild(remove_element);
                        wait = 0;
                    }
                    if(element.firstChild().nodeValue() == english_tag && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName();
                        print_list << operation;
                        wait++;
                        s++;
                        remove_element = element.previousSiblingElement();
                    }
                    else if (element.firstChildElement("Text").isNull() && element.firstChild().nodeValue().isEmpty() && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (empty)";
                        print_list << operation;
                        wait++;
                        s++;
                        remove_element = element.previousSiblingElement();
                    }
                }
                // Remove the last checked element if it has been detected
                if (wait > 0)
                {
                    input_node.removeChild(remove_element);
                    wait = 0;
                }
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

        qDebug() << "\n\nFiles successfully processed. Modified files are in 'imported/'.\nA report of the modified values have been generated in 'imported/imported_values.txt'";
}
