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
    void SortCategories();
    void SorterHelper(QString prefix);
    QString ConvertStringToCiv4(QString string);
    void ConvertCiv4ToUTF8(QString file);
    void ConvertUTF8ToCiv4(QString file);
    void FindDuplicates();

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
    qDebug() << "\nMain menu:\n----------\n1 - Export all languages [Civ 4 XML -> Language XML]\n2 - Export a specific language [Civ 4 XML -> Language XML]\n3 - Import language strings to ALL files [Language XML -> Civ 4 XML]\n4 - Import language strings to SAME files [Language XML -> Civ 4 XML]\n5 (Disabled) - Sort files by categories [Civ 4 XML]\n6 (Disabled) - Find duplicates files\n6 - Exit program\n\n";
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
            xml->FindDuplicates();
            break;

        case 5 :
            std::cout << "Which language do you want to IMPORT (en, fr, ge, it, sp, po) ?\nNB: You can only import one language at a time.\n";
            std::cin >> lang;
            if(lang == "en" && lang == "fr" && lang == "ge" && lang == "it" && lang == "sp" && lang == "po")
            {
                qDebug() << "\nA valid language code is required";
                break;
            }
            xml->ImportDocumentToSameName(QString::fromStdString(lang));
            break;

        case 6 :
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
    /*QString language;
    QStringList languages_list;
    languages_list << "English" << "French" << "German" << "Italian" << "Spanish" << "Polish";

    // List all root files
    qDebug() << "Configuration...";
    XMLDocument xml_input;
    XMLDocument xml_export;

    QFile::remove("export/bigfile.xml");
    QDir root_dir(".");
    QDir export_dir;

    export_dir.mkdir("export/");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    files = root_dir.entryList(xml_filter, QDir::Files);
    QStringList tags;

    // Creation of the big file
    qDebug() << "Gathering all values...";
    xml_export.LoadFile("export/bigfile.xml");
    xml_export.InsertFirstChild(xml_export.NewDeclaration());
    XMLNode *root = xml_export.NewElement("Civ4GameText");
    xml_export.InsertEndChild(root);

    const char* null_value = "";
    for (QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QString current = *it;

        // Begin iteration

        xml_input.LoadFile(current.toStdString().c_str());
        XMLElement* tag_orig = xml_input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
        for(tag_orig;tag_orig != NULL;tag_orig = tag_orig->NextSiblingElement())
        {
            //qDebug() << current << " - " << tag_orig->FirstChildElement("Tag")->GetText();

            // Create xml structure

            XMLElement* text_value = xml_export.NewElement("TEXT");
            root->InsertEndChild(text_value);
            XMLElement* tag_export = xml_export.NewElement("Tag");
            text_value->InsertEndChild(tag_export);

            // Read elements and check if NULL

            tags << tag_orig->FirstChildElement("Tag")->GetText();
            tag_export->SetText(tag_orig->FirstChildElement("Tag")->GetText());

            foreach (language, languages_list) {
                text_value->InsertEndChild(xml_export.NewElement(language.toStdString().c_str()));
                if(tag_orig->FirstChildElement(language.toStdString().c_str()) == NULL)
                {
                    //qDebug() << "No " << language << " element.";
                    const char* null_value = "";
                    text_value->FirstChildElement(language.toStdString().c_str())->SetText(null_value);
                }
                else if (tag_orig->FirstChildElement(language.toStdString().c_str())->GetText() == NULL)
                {
                    //qDebug() << "No " << language << " text.";
                    if(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Text") != NULL)
                    {
                        XMLElement* sub_text = xml_export.NewElement("Text");
                        XMLElement* sub_gender = xml_export.NewElement("Gender");
                        XMLElement* sub_plural = xml_export.NewElement("Plural");
                        text_value->FirstChildElement(language.toStdString().c_str())->InsertEndChild(sub_text);
                        text_value->FirstChildElement(language.toStdString().c_str())->InsertEndChild(sub_gender);
                        text_value->FirstChildElement(language.toStdString().c_str())->InsertEndChild(sub_plural);
                        sub_text->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Text")->GetText());
                        sub_gender->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Gender")->GetText());
                        sub_plural->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Plural")->GetText());
                    }
                }
                else
                {
                    text_value->FirstChildElement(language.toStdString().c_str())->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->GetText());
                }
            }
        }

    }
    xml_export.SaveFile("export/bigfile.xml");

    // Fix encoding before process
    qDebug() << "Fixing encoding...";
    QFile file_in("export/bigfile.xml");
    QFile file_out("export/__temp__bigfile.xml");
    file_in.open(QIODevice::ReadOnly | QIODevice::Text);
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream in_enc(&file_in);
    QTextStream out_enc(&file_out);
    out_enc.setCodec("UTF-8");
    while(!in_enc.atEnd())
    {
        QString line = in_enc.readLine();
        out_enc << line;
    }
    file_in.close();
    file_out.close();
    QFile::remove("export/bigfile.xml");
    QFile::rename("export/__temp__bigfile.xml","export/bigfile.xml");



    // Sort tags by category
    int total_tags = tags.size();
    qDebug() << "Sorting" << total_tags << "tags into categories...";
    XMLDocument xml_sort;
    xml_export.LoadFile("export/bigfile.xml");
    tags.sort();

    QString tag_search;
    QStringList sort_categories;
    QString category;*/

    //* --- Known categories --- *//
    /*sort_categories << "AI_" << "INTERFACE_" << "USER_" << "TXT_";
                    //<< "TXT_KEY_GREAT_PERSON" << "TXT_KEY_BUILDING" << "TXT_MAIN_MENU" << "TXT_KEY_UNIT";

    QStringList tags_process = tags;

    foreach(category, sort_categories)
    {
        // Creation of each file
        QString category_file = "export/" + category + ".xml";
        xml_sort.LoadFile(category_file.toStdString().c_str());
        xml_sort.InsertFirstChild(xml_sort.NewDeclaration());
        XMLNode *root = xml_sort.NewElement("Civ4GameText");
        xml_sort.InsertEndChild(root);

        foreach(tag_search, tags)
        {

            if(tag_search.startsWith(category))
            {
                // Look for the correct tag
                XMLElement* tag_orig = xml_export.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();

                for(tag_orig;tag_orig != NULL;tag_orig = tag_orig->NextSiblingElement())
                {
                    // Create xml structure
                    const char* tag_value = tag_orig->FirstChildElement("Tag")->GetText();

                    if(!std::strcmp(tag_value,tag_search.toStdString().c_str()))
                    {
                        //qDebug() << "Found " << tag_value;
                        XMLElement* text_value = xml_sort.NewElement("TEXT");
                        root->InsertEndChild(text_value);
                        XMLElement* tag_export = xml_sort.NewElement("Tag");
                        text_value->InsertEndChild(tag_export);
                        tag_export->SetText(tag_value);

                        // Script to adapt
                        foreach (language, languages_list) {
                            text_value->InsertEndChild(xml_sort.NewElement(language.toStdString().c_str()));
                            if(tag_orig->FirstChildElement(language.toStdString().c_str()) == NULL)
                            {
                                //qDebug() << "No " << language << " element.";
                                const char* null_value = "";
                                text_value->FirstChildElement(language.toStdString().c_str())->SetText(null_value);
                            }
                            else if (tag_orig->FirstChildElement(language.toStdString().c_str())->GetText() == NULL)
                            {
                                //qDebug() << "No " << language << " text.";
                                if(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Text") != NULL)
                                {
                                    XMLElement* sub_text = xml_sort.NewElement("Text");
                                    XMLElement* sub_gender = xml_sort.NewElement("Gender");
                                    XMLElement* sub_plural = xml_sort.NewElement("Plural");
                                    text_value->FirstChildElement(language.toStdString().c_str())->InsertEndChild(sub_text);
                                    text_value->FirstChildElement(language.toStdString().c_str())->InsertEndChild(sub_gender);
                                    text_value->FirstChildElement(language.toStdString().c_str())->InsertEndChild(sub_plural);
                                    sub_text->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Text")->GetText());
                                    sub_gender->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Gender")->GetText());
                                    sub_plural->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->FirstChildElement("Plural")->GetText());
                                    xml_export.DeleteNode(tag_orig);
                                }

                            }
                            else
                            {
                                text_value->FirstChildElement(language.toStdString().c_str())->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->GetText());
                                xml_export.DeleteNode(tag_orig);
                            }
                        }

                        // Until here
                    }
                }
            }
        }

        xml_sort.SaveFile(category_file.toStdString().c_str());
    }

    xml_export.SaveFile("export/OTHERS.xml");

    // Count exported tags
    qDebug() << "Checking exported tags...";
    int tags_counter = 0;
    QDir dir_export("export/");
    QStringList folders;
    folders = dir_export.entryList(xml_filter, QDir::Files);
    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        XMLDocument temp;
        QString current = "export/" + *it;
        temp.LoadFile(current.toStdString().c_str());
        XMLElement* root = temp.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->FirstChildElement("Tag")->ToElement();
        for(root;root != NULL;root = root->NextSiblingElement())
        {
            tags_counter++;
        }
    }
    qDebug() << "Found" << tags_counter << "tags !";

    // Export tags to file
    qDebug() << "Exporting tags to a list...";
    QString tag;
    QFile tag_list("export/tag_list.txt");

    if ( tag_list.open(QIODevice::ReadWrite) )
    {
        QTextStream stream( &tag_list );

        foreach(tag, tags)
        {
            stream << tag << endl;
        }
    }
    tag_list.close();

    qDebug() << "Find duplicates tags...";
    languages::FindDuplicates();

    QFile::remove("export/bigfile.xml");*/
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
    QDomElement read_text = read.firstChildElement("Civ4GameText").firstChildElement("TEXT");
    //qDebug() << read_text.firstChildElement().tagName();

    // Create the output file
    QDomDocument write;
    QFile file_out("rewrite.xml");
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDomNode declaration = write.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"ISO-8859-1\""));
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
    QFile::rename("rewrite.xml",file);
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

void languages::FindDuplicates()
{
    qDebug() << "Checking files...";
    QDir root(".");
    root.rmdir("duplicates/");
    root.mkdir("duplicates/");
    QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
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

    // Output modified list
    qDebug() << "Printing list of tags...";
    QString print_value;
    QFile print_file("duplicates/_list_of_tags.txt");

    tags.sort();

    if ( print_file.open(QIODevice::Truncate | QIODevice::WriteOnly))
        {
            QTextStream stream( &print_file );

            foreach(print_value, tags)
            {
                stream << print_value << endl;
            }
        }
    print_file.close();


    // Double loop to find duplicates
    qDebug() << "Checking duplicated tags...";
    QString loop1;
    QString loop2;
    QStringList duplicated_tags;

    foreach(loop1,tags)
    {
        int c = 0;
        foreach(loop2,tags)
        {
            if(loop1 == loop2)
            {
               c++;
            }
        }
        if (c > 1)
        {
            duplicated_tags << loop1;
        }
    }

    duplicated_tags.removeDuplicates();

    QString print_value2;
    QFile print_file2("duplicates/_duplicated_tags.txt");

    if ( print_file2.open(QIODevice::Truncate | QIODevice::WriteOnly))
        {
            QTextStream stream2( &print_file2 );

            foreach(print_value2, duplicated_tags)
            {
                stream2 << print_value2 << endl;
            }
        }
    print_file2.close();

    // Copy identified files
    qDebug() << "Copying identified files...";
    QString tag_value;

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
            QString tag = xml_tag.firstChildElement("Tag").text();
            foreach (tag_value, duplicated_tags) {
                if(tag == tag_value)
                {
                    QFile::copy(*it,"duplicates/"+*it);
                }
            }
        }

        file_in.close();
    }

    // Generate a report
    qDebug() << "Generating report...";
    QStringList file_list;
    QDir duplicate_dir("duplicates/");
    QStringList duplicate_files;
    duplicate_files = duplicate_dir.entryList(xml_filter, QDir::Files);

    foreach(tag_value,duplicated_tags)
    {
        for(QStringList::Iterator it = duplicate_files.begin(); it != duplicate_files.end(); it++)
        {
            QFile file_in(*it);
            file_in.open(QIODevice::ReadOnly);
            QDomDocument xml;
            xml.setContent(&file_in);
            QDomElement xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();

            if(xml_tag.isNull())
            {
                file_in.close();
            }

            for(xml_tag;!xml_tag.isNull();xml_tag = xml_tag.nextSiblingElement())
            {
                QString tag_value;
                //qDebug() << xml_tag.firstChildElement("Tag").text();

                if (tag_value == xml_tag.firstChildElement("Tag").text());
                QString operation;
                operation = "TAG: " + tag_value + " found in " + *it;
                //qDebug() << operation;
                file_list << operation;

            }
            file_in.close();
        }
    }

    QString print_value3;
    QFile print_file3("duplicates/_report.txt");

    file_list.sort();

    if ( print_file3.open(QIODevice::Truncate | QIODevice::WriteOnly))
        {
            QTextStream stream3( &print_file3 );

            foreach(print_value3, file_list)
            {
                stream3 << print_value3 << endl;
            }
        }
    print_file3.close();

    /* Old function
     * // Find duplicate tags
    QString dupl_old = "";
    QString dupl_new;
    QFile dupl_in("export/tag_list.txt");
    QFile dupl_out("export/duplicates.txt");
    dupl_in.open(QIODevice::ReadOnly | QIODevice::Text);
    dupl_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream in_dupl(&dupl_in);
    QTextStream out_dupl(&dupl_out);
    while(!in_dupl.atEnd())
    {
        dupl_new = in_dupl.readLine();
        if(!strcmp(dupl_new.toStdString().c_str(),dupl_old.toStdString().c_str()))
        {
            //qDebug() << dupl_new;
            out_dupl << dupl_new << endl;
        }
        dupl_old = dupl_new;
    }
    dupl_in.close();
    dupl_out.close();

    qDebug() << "Duplicates tags have been gathered in 'export/duplicates.txt'";*/
}

void languages::CleanFiles()
{

}
