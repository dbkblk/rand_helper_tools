#include <iostream>
#include <QtCore>
#include "lib/tinyxml2.h"
#include <QDebug>

using namespace tinyxml2;

class languages
{
public:
    void ParseDocument(QString input_file, QString language);
    void ParseAllFiles(QString language);
    void ImportDocument(QString language);
    void SortCategories();
    void SorterHelper(QString prefix);
    void ConvertToUTF8();
    void ConvertToISO8859();
    void FindDuplicates();

private:
    XMLDocument input;
    XMLDocument output;
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

    int ch;
    int z = 0;
    do {
    qDebug() << "\nWhat do you want to do ?\n1 - Export all language\n2 - Export a specific language\n3 - Import a language\n4 - Sort files by categories\n5 - Convert all root files from ISO8859-1 to UTF8\n6 - Convert all root files from UTF8 to ISO8859-1\n7 - Find duplicates files\n8 - Exit program\n\n";
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
            xml->ImportDocument(QString::fromStdString(lang));
            break;

        case 4 :
            xml->SortCategories();
            break;

        case 5 :
            std::cout << "This will convert all files in the root directory from encoding ISO8859-1 to UTF8. Are you sure to continue (Y/N) ?\n";
            std::cin >> lang;
            if(lang == "Y")
            {
                xml->ConvertToUTF8();
                break;
            }
            else
                break;

        case 6 :
            std::cout << "This will convert all files in the root directory from encoding UTF8 to ISO8859-1. Are you sure to continue (Y/N) ?\n";
            std::cin >> lang;
            if(lang == "Y")
            {
                xml->ConvertToISO8859();
                break;
            }
            else
                break;

        case 7 :
            xml->FindDuplicates();
            break;

        case 8 :
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
    XMLDocument input;
    XMLDocument output;

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
    QString output_dir = "lang/" + int_lang + "/";
    QString output_file = output_dir + input_file;
    QDir dir;
    dir.mkpath(output_dir);
    bool save = false;

    // Check encoding before process
    QFile file_in(input_file);
    QFile file_out(input_temp_file);
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

    // Begin process

    input.LoadFile(input_temp_file.toStdString().c_str());
    output.LoadFile(output_file.toStdString().c_str());

    XMLDeclaration *declaration = output.NewDeclaration("xml version=\"1.0\" encoding=\"ISO-8859-1\"");
    //output.InsertFirstChild(declaration);
    XMLNode *root = output.NewElement("resources");
    output.InsertEndChild(root);

    // Loop process
    XMLElement* tag_el = input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
    const char* value_en;

    for(tag_el;tag_el != NULL;tag_el = tag_el->NextSiblingElement())
    {
        // Read tags

        const char* value_tag = tag_el->FirstChildElement("Tag")->GetText();

        // Prepare new tag
        XMLElement *new_tag = output.NewElement("string");
        root->InsertEndChild(new_tag);
        new_tag->SetAttribute("name", value_tag);

        // Check for NULL values or substrings
        if(tag_el->FirstChildElement(int_lang.toStdString().c_str()) == NULL)
        {
            new_tag->SetText("");
        }
        else if (tag_el->FirstChildElement(int_lang.toStdString().c_str())->GetText() == NULL)
        {
            if(tag_el->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Text") != NULL)
            {
                new_tag->SetText(tag_el->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Text")->GetText());
                new_tag->SetAttribute("Gender", tag_el->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Gender")->GetText());
                new_tag->SetAttribute("Plural", tag_el->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Plural")->GetText());
            }
        }
        else
        {
            new_tag->SetText(tag_el->FirstChildElement(int_lang.toStdString().c_str())->GetText());
        }


        //new_tag->NextSiblingElement();
    }

    output.SaveFile(output_file.toStdString().c_str());

    // Save input if modified
    if(save)
    {
        dir.mkdir("modif");
        input_file = "modif/" + input_file;
        input.SaveFile(input_temp_file.toStdString().c_str());
    }
    QFile::remove(input_temp_file);
}

void languages::ImportDocument(QString language)
{
    XMLDocument input;
    XMLDocument input_tr;

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

    // Translated files
    QDir translated("import/");
    QStringList files_translated;
    files_translated = translated.entryList(xml_filter, QDir::Files);

    // Print list

    QStringList print_list;

    // Entering loop

    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        int s = 0;

        QString current = *it;
        QString current_copy = "imported/" + current;

        qDebug() << "Checking " << current;

        // Check encoding before process
        QFile file_in(current);
        QFile file_out(current_copy);
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

        input.LoadFile(current_copy.toStdString().c_str());

        // Comparing to each new file
        for(QStringList::Iterator tr = files_translated.begin(); tr != files_translated.end(); tr++)
        {
            QString current_new = "import/" + *tr;
            input_tr.LoadFile(current_new.toStdString().c_str());

            XMLElement* tag_orig = input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
            for(tag_orig;tag_orig != NULL;tag_orig = tag_orig->NextSiblingElement())
            {
                // Compare each occurence with each tag of the translated file
                const char* value_tag = tag_orig->FirstChildElement("Tag")->GetText();
                //qDebug() << value_tag;
                XMLElement* tag_tr = input_tr.FirstChildElement("resources")->FirstChildElement("string")->ToElement();
                for(tag_tr;tag_tr != NULL;tag_tr = tag_tr->NextSiblingElement())
                {
                    const char* value_tag_tr = tag_tr->Attribute("name");

                    if (!std::strcmp(value_tag,value_tag_tr))
                    {
                        s++;
                        const char* value_text_tr = tag_tr->GetText();
                        const char* value_text;

                        // Checking tag presence

                        if(tag_orig->FirstChildElement(int_lang.toStdString().c_str()) == NULL)
                        {
                            //qDebug() << "No tag";
                            value_text = "";
                        }

                        else if (tag_orig->FirstChildElement(int_lang.toStdString().c_str())->GetText() == NULL)
                        {
                            if(tag_orig->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Text") != NULL)
                            {
                                //qDebug() << "No value";
                                value_text = tag_orig->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Text")->GetText();
                            }
                            else
                            {
                                value_text = "";
                            }
                        }

                        else
                        {
                            //qDebug() << "Value set";
                            value_text = tag_orig->FirstChildElement(int_lang.toStdString().c_str())->GetText();
                        }

                        //qDebug() << value_tag << " : " << value_text << " / " << value_text_tr;

                        // Normalize value
                        if(value_text_tr == NULL)
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
                                qDebug() << operation;
                            }

                            // Major cases
                            else if(strcmp(value_text,value_text_tr))
                            {
                                QString operation;
                                operation = "FILE: " + current + " | TAG: " + value_tag + " | OLD: " + value_text + " -> NEW: " + value_text_tr;
                                print_list << operation;
                                qDebug() << operation;

                                // Checking if tag is not subtag, again...
                                if(tag_orig->FirstChildElement(int_lang.toStdString().c_str()) == NULL)
                                {
                                }
                                else if (tag_orig->FirstChildElement(int_lang.toStdString().c_str())->GetText() == NULL)
                                {
                                    if(tag_orig->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Text") != NULL)
                                    {
                                        tag_orig->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Text")->SetText(value_text_tr);
                                        // Check for additionnal attributes
                                        if(tag_tr->Attribute("Gender") != NULL)
                                        {
                                            tag_orig->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Gender")->SetText(tag_tr->Attribute("Gender"));
                                        }
                                        if(tag_tr->Attribute("Plural") != NULL)
                                        {
                                            tag_orig->FirstChildElement(int_lang.toStdString().c_str())->FirstChildElement("Plural")->SetText(tag_tr->Attribute("Plural"));
                                        }
                                    }
                                }
                                else
                                {
                                    tag_orig->FirstChildElement(int_lang.toStdString().c_str())->SetText(value_text_tr);
                                }
                            }


                        }
                    }
                }

            }
        }
        input.SaveFile(current_copy.toStdString().c_str());
        if(s == 0)
        {
            QFile::remove(current_copy);
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
    QString language;
    QStringList languages_list;
    languages_list << "English" << "French" << "German" << "Italian" << "Spanish" << "Polish";

    // List all root files
    qDebug() << "Preparing the sorting...";
    XMLDocument xml_input;
    XMLDocument xml_export;

    QFile::remove("export/bigfile.xml");
    QDir root_dir(".");
    QDir export_dir;
    //export_dir.removeRecursively();
    export_dir.mkdir("export/");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    files = root_dir.entryList(xml_filter, QDir::Files);
    QStringList tags;

    // Creation of the big file
    qDebug() << "Creation of export file...";
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
    qDebug() << "Begin sorting...";
    XMLDocument xml_sort;
    xml_export.LoadFile("export/bigfile.xml");
    tags.sort();

    QString tag_search;
    QStringList sort_categories;
    QString category;

    //* --- Known categories --- *//
    sort_categories << "AI_" << "INTERFACE_" << "TXT_KEY_GREAT_PERSON" << "TXT_KEY_BUILDING" << "USER_" << "TXT_MAIN_MENU" << "TXT_KEY_UNIT";



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
                                }
                            }
                            else
                            {
                                text_value->FirstChildElement(language.toStdString().c_str())->SetText(tag_orig->FirstChildElement(language.toStdString().c_str())->GetText());
                            }
                        }

                        // Until here
                    }
                }
            }
        }

        xml_sort.SaveFile(category_file.toStdString().c_str());

    }

    // Export tags to file
    qDebug() << "Exporting tags...";
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
}

void languages::ConvertToUTF8()
{
    XMLDocument temp;
    QDir root_dir(".");
    QStringList files;
    QStringList xml_filter;
    xml_filter << "*.xml";
    files = root_dir.entryList(xml_filter, QDir::Files);

    for (QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QString string_in = *it;
        QString string_out = "__temp__" + *it;
        QFile file_in(string_in);
        QFile file_out(string_out);
        file_in.open(QIODevice::ReadOnly | QIODevice::Text);
        file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream in_enc(&file_in);
        QTextStream out_enc(&file_out);
        in_enc.setCodec("ISO 8859-1");
        out_enc.setCodec("UTF-8");
        while(!in_enc.atEnd())
        {
            QString line = in_enc.readLine();
            line.replace("ISO-8859-1","utf-8",Qt::CaseInsensitive);
            out_enc << line;
        }
        file_in.close();
        file_out.close();
        temp.LoadFile(string_out.toStdString().c_str());
        temp.SaveFile(string_out.toStdString().c_str());
        QFile::remove(string_in);
        QFile::rename(string_out,string_in);
    }
}

void languages::ConvertToISO8859()
{
    XMLDocument temp;
    QDir root_dir(".");
    QStringList files;
    QStringList xml_filter;
    xml_filter << "*.xml";
    files = root_dir.entryList(xml_filter, QDir::Files);

    for (QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QString string_in = *it;
        QString string_out = "__temp__" + *it;
        QFile file_in(string_in);
        QFile file_out(string_out);
        file_in.open(QIODevice::ReadOnly | QIODevice::Text);
        file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream in_enc(&file_in);
        QTextStream out_enc(&file_out);
        in_enc.setCodec("UTF-8");
        out_enc.setCodec("ISO 8859-1");
        while(!in_enc.atEnd())
        {
            QString line = in_enc.readLine();
            line.replace("utf-8","ISO-8859-1",Qt::CaseInsensitive);
            out_enc << line;
        }
        file_in.close();
        file_out.close();
        temp.LoadFile(string_out.toStdString().c_str());
        temp.SaveFile(string_out.toStdString().c_str());
        QFile::remove(string_in);
        QFile::rename(string_out,string_in);
    }
}

void languages::FindDuplicates()
{

    // Find duplicate tags
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

    qDebug() << "Duplicates tags have been gathered in 'export/duplicates.txt'";
}
