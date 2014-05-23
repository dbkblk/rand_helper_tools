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

    // Clean directory
    QDir dir("lang/");
    dir.removeRecursively();
    QDir dir2("modif/");
    dir2.removeRecursively();

    int ch;
    int z = 0;
    do {
    qDebug() << "\nWhat do you want to do ?\n1 - Export all language\n2 - Export a specific language\n3 - Import a language\n4 - Sort files by categories\n5 - Exit program\n\n";
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
            std::cout << "Please specify which language (en, fr, ge, it, sp, po) ?";
            std::cin >> lang;
            if(lang == "en" && lang == "fr" && lang == "ge" && lang == "it" && lang == "sp" && lang == "po")
            {
                qDebug() << "A valid language code is required";
                break;
            }
            xml->ParseAllFiles(QString::fromStdString(lang));
            break;

        case 3 :
            qDebug() << "This is not yet implemented";
            xml->ImportDocument("fr");
            break;

        case 4 :
            xml->SortCategories();

        case 5 :
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

    qDebug() << "Export finished. All translations are into subfolders 'lang/language/'";
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
    qDebug() << "Processing " << output_file;
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
        // qDebug() << int_lang << " : " << value_tag;

        // Check for NULL values
        if(tag_el->FirstChildElement(int_lang.toStdString().c_str()) == NULL)
        {
            qDebug() << "Element is NULL";
            XMLElement *new_lang = input.NewElement(int_lang.toStdString().c_str());
            tag_el->InsertEndChild(new_lang);
            save = true;
        }
        if(tag_el->FirstChildElement(int_lang.toStdString().c_str())->GetText() == NULL)
        {
            value_en = "";
        }
        else
        {
            value_en = tag_el->FirstChildElement(int_lang.toStdString().c_str())->GetText();
        }

        // Write new tag
        XMLElement *new_tag = output.NewElement("string");
        root->InsertEndChild(new_tag);
        new_tag->SetAttribute("name", value_tag);
        new_tag->SetText(value_en);
        new_tag->NextSiblingElement();
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
    qDebug() << "Open function";

    XMLDocument input;
    XMLDocument input_tr;

    QString int_lang;
    int_lang;

    // Check language

    if(language == "fr"){int_lang = "French";};
    if(language == "ge"){int_lang = "German";};
    if(language == "it"){int_lang = "Italian";};
    if(language == "sp"){int_lang = "Spanish";};
    if(language == "po"){int_lang = "Polish";};
    if(language == "en"){int_lang = "English";};
    qDebug() << "language is " << int_lang;

    // Open files
    QDir dir_temp;
    dir_temp.mkdir("import");

    /* Search the tag trough existing files and replace with the new value if found */
    // List all files
    QDir root(".");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList folders;
    folders = root.entryList(xml_filter, QDir::Files);
    QDir translated("imported/");
    QStringList files_translated;
    files_translated = translated.entryList(xml_filter, QDir::Files);

    qDebug() << "Entering loop";
    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        int s = 0;

        // Open file. Check if it can close the file
        QString current = *it;
        QString current_copy = "import/" + current;
        //QFile::copy(current,current_copy);
        qDebug() << "Comparing " << current;

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
            QString current_new = "imported/" + *tr;
            input_tr.LoadFile(current_new.toStdString().c_str());

            XMLElement* tag_orig = input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
            for(tag_orig;tag_orig != NULL;tag_orig = tag_orig->NextSiblingElement())
            {
                // Compare each occurence with each tag of the translated file
                const char* value_tag = tag_orig->FirstChildElement("Tag")->GetText();
                //qDebug() << "Tag : " << value_tag;
                XMLElement* tag_tr = input_tr.FirstChildElement("resources")->FirstChildElement("string")->ToElement();
                for(tag_tr;tag_tr != NULL;tag_tr = tag_tr->NextSiblingElement())
                {
                    const char* value_tag_tr = tag_tr->Attribute("name");
                    //qDebug() << "Tr. tag : " << value_tag_tr;
                    if (!std::strcmp(value_tag,value_tag_tr))
                    {
                        s++;
                        const char * value_text_tr = tag_tr->GetText();
                        const char* value_text = tag_orig->FirstChildElement(int_lang.toStdString().c_str())->GetText();
                        //qDebug() << value_text << " / " << value_text_tr;
                        if(std::strcmp(value_text,value_text_tr))
                        {
                            qDebug() << current << " - " << value_tag << " : " << value_text << " -> " << value_text_tr;
                            tag_orig->FirstChildElement(int_lang.toStdString().c_str())->SetText(value_text_tr);
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

}

void languages::SortCategories()
{
    QStringList languages_list;
    languages_list << "English" << "French" << "German" << "Italian" << "Spanish" << "Polish";

    // List all root files
    qDebug() << "Parameters";
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
    qDebug() << "Creation of export file";
    xml_export.LoadFile("export/bigfile.xml");
    xml_export.InsertFirstChild(xml_export.NewDeclaration());
    qDebug() << "2nd node";
    XMLNode *root = xml_export.NewElement("Civ4GameText");
    xml_export.InsertEndChild(root);


    qDebug() << "Entering loop";
    for (QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QString current = *it;

        // Begin iteration

        xml_input.LoadFile(current.toStdString().c_str());
        XMLElement* tag_orig = xml_input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
        for(tag_orig;tag_orig != NULL;tag_orig = tag_orig->NextSiblingElement())
        {
            qDebug() << current << " - " << tag_orig->FirstChildElement("Tag")->GetText();

            // Create xml structure

            XMLElement* text_value = xml_export.NewElement("TEXT");
            root->InsertEndChild(text_value);
            XMLElement* tag_export = xml_export.NewElement("Tag");
            text_value->InsertEndChild(tag_export);
            XMLElement* english_export = xml_export.NewElement("English");
            text_value->InsertEndChild(english_export);
            XMLElement* french_export = xml_export.NewElement("French");
            text_value->InsertEndChild(french_export);
            XMLElement* german_export = xml_export.NewElement("German");
            text_value->InsertEndChild(german_export);
            XMLElement* italian_export = xml_export.NewElement("Italian");
            text_value->InsertEndChild(italian_export);
            XMLElement* spanish_export = xml_export.NewElement("Spanish");
            text_value->InsertEndChild(spanish_export);
            XMLElement* polish_export = xml_export.NewElement("Polish");
            text_value->InsertEndChild(polish_export);

            // Read elements and check if NULL

            tags << tag_orig->FirstChildElement("Tag")->GetText();
            tag_export->SetText(tag_orig->FirstChildElement("Tag")->GetText());
            if(tag_orig->FirstChildElement("English") == NULL)
            {
                //english_export->SetText("");
            }
            if(tag_orig->FirstChildElement("English")->GetText() == NULL)
            {
                english_export->SetText("");
            }
            else
            {
                english_export->SetText(tag_orig->FirstChildElement("English")->GetText());
            }

            if(tag_orig->FirstChildElement("French") == NULL)
            {
                //french_export->SetText("");
            }
            if(tag_orig->FirstChildElement("French")->GetText() == NULL)
            {
                french_export->SetText("");
            }
            else
            {
                french_export->SetText(tag_orig->FirstChildElement("French")->GetText());
            }

            if(tag_orig->FirstChildElement("German") == NULL)
            {
                //german_export->SetText("");
            }
            if(tag_orig->FirstChildElement("German")->GetText() == NULL)
            {
                german_export->SetText("");
            }
            else
            {
                german_export->SetText(tag_orig->FirstChildElement("German")->GetText());
            }

            if(tag_orig->FirstChildElement("Italian") == NULL)
            {
                //italian_export->SetText("");
            }
            if(tag_orig->FirstChildElement("Italian")->GetText() == NULL)
            {
                italian_export->SetText("");
            }
            else
            {
                italian_export->SetText(tag_orig->FirstChildElement("Italian")->GetText());
            }

            if(tag_orig->FirstChildElement("Spanish") == NULL)
            {
                //spanish_export->SetText("");
            }
            if(tag_orig->FirstChildElement("Spanish")->GetText() == NULL)
            {
                spanish_export->SetText("");
            }
            else
            {
                spanish_export->SetText(tag_orig->FirstChildElement("Spanish")->GetText());
            }

            if(tag_orig->FirstChildElement("Polish") == NULL)
            {
                //polish_export->SetText("");
            }
            if(tag_orig->FirstChildElement("Polish")->GetText() == NULL)
            {
                polish_export->SetText("");
            }
            else
            {
                polish_export->SetText(tag_orig->FirstChildElement("Polish")->GetText());
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
    XMLDocument xml_sort;
    xml_export.LoadFile("export/bigfile.xml");

    tags.sort();
    QString tag_search;
    QStringList sort_categories;
    QString category;
    sort_categories << "AI_DIPLO" << "INTERFACE_" << "TXT_KEY_BUILDING";

// Loop are inversed
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
                        qDebug() << "Found " << tag_value;
                        XMLElement* text_value = xml_sort.NewElement("TEXT");
                        root->InsertEndChild(text_value);
                        XMLElement* tag_export = xml_sort.NewElement("Tag");
                        text_value->InsertEndChild(tag_export);
                        XMLElement* english_export = xml_sort.NewElement("English");
                        text_value->InsertEndChild(english_export);
                        XMLElement* french_export = xml_sort.NewElement("French");
                        text_value->InsertEndChild(french_export);
                        XMLElement* german_export = xml_sort.NewElement("German");
                        text_value->InsertEndChild(german_export);
                        XMLElement* italian_export = xml_sort.NewElement("Italian");
                        text_value->InsertEndChild(italian_export);
                        XMLElement* spanish_export = xml_sort.NewElement("Spanish");
                        text_value->InsertEndChild(spanish_export);
                        XMLElement* polish_export = xml_sort.NewElement("Polish");
                        text_value->InsertEndChild(polish_export);

                        // Read elements

                        tag_export->SetText(tag_search.toStdString().c_str());
                        if(tag_orig->FirstChildElement("English") != NULL)
                        {
                            english_export->SetText(tag_orig->FirstChildElement("English")->GetText());
                        }
                        if(tag_orig->FirstChildElement("French") != NULL)
                        {
                            french_export->SetText(tag_orig->FirstChildElement("French")->GetText());
                        }
                        //if(german_export->SetText(tag_orig->FirstChildElement("German")->GetText()) != NULL);
                        //if(italian_export->SetText(tag_orig->FirstChildElement("Italian")->GetText()) != NULL);
                        //if(spanish_export->SetText(tag_orig->FirstChildElement("Spanish")->GetText()) != NULL);
                        //if(polish_export->SetText(tag_orig->FirstChildElement("Polish")->GetText()) != NULL);

                    }
                }
            }
        }

        xml_sort.SaveFile(category_file.toStdString().c_str());

    }

    // Export tags to file
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


    // Sort by category
}

void languages::SorterHelper(QString prefix)
{

}
