#include <iostream>
#include <QtCore>
#include <lib/tinyxml2.h>
#include <QDebug>

using namespace tinyxml2;

class langage
{
public:
    void ParseDocument(QString input_file, QString langage);
    void ParseAllFiles(QString langage);
    void ImportDocument(QString langage);

private:
    XMLDocument input;
    XMLDocument output;
    QString langage;
    const char* lang_file;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    langage *xml = new langage();

    // Clean directory
    QDir dir("lang/");
    dir.removeRecursively();
    QDir dir2("modif/");
    dir2.removeRecursively();

    int ch;
    int z = 0;
    do {
    qDebug() << "\nWhat do you want to do ?\n1 - Export all langage\n2 - Export a specific langage\n3 - Import a langage\n4 - Exit program\n\n";
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
            std::cout << "Please specify which langage (en, fr, ge, it, sp, po) ?";
            std::cin >> lang;
            if(lang == "en" && lang == "fr" && lang == "ge" && lang == "it" && lang == "sp" && lang == "po")
            {
                qDebug() << "A valid langage code is required";
                break;
            }
            xml->ParseAllFiles(QString::fromStdString(lang));
            break;

        case 3 :
            qDebug() << "This is not yet implemented";
            xml->ImportDocument("fr");
            break;

        case 4 :
            return 0;
            break;

        default :
            break;

    }
    } while (z=1);

    return a.exec();
}

void langage::ParseAllFiles(QString langage)
{
    // List all files

    QDir root(".");
    QStringList folders;
    folders = root.entryList(QDir::Files);

    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        ParseDocument(*it,langage);
    }

    qDebug() << "Export finished. All translations are into subfolders 'lang/langage/'";
}

void langage::ParseDocument(QString input_file, QString langage)
{
    XMLDocument input;
    XMLDocument output;

    QString int_lang;
    int_lang = "English";

    // Check langage
    if(langage == "fr"){int_lang = "French";};
    if(langage == "ge"){int_lang = "German";};
    if(langage == "it"){int_lang = "Italian";};
    if(langage == "sp"){int_lang = "Spanish";};
    if(langage == "po"){int_lang = "Polish";};

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

void langage::ImportDocument(QString langage)
{
    qDebug() << "Open function";

    XMLDocument input;
    XMLDocument input_tr;
    XMLDocument output;

    QString int_lang;
    int_lang;

    // Check langage

    if(langage == "fr"){int_lang = "French";};
    if(langage == "ge"){int_lang = "German";};
    if(langage == "it"){int_lang = "Italian";};
    if(langage == "sp"){int_lang = "Spanish";};
    if(langage == "po"){int_lang = "Polish";};
    if(langage == "en"){int_lang = "English";};
    qDebug() << "Langage is " << int_lang;

    // Open files
    qDebug() << "Create folder";
    QDir dir_temp;
    dir_temp.mkdir("temp");
    qDebug() << "Open first file";
    //input_tr.LoadFile("test/A_New_Dawn_Concept_Civ4GameText.xml");

    /* Search the tag trough existing files and replace with the new value if found */
    // List all files
    qDebug() << "Look for all files in the folder";
    QDir root(".");
    QStringList folders;
    folders = root.entryList(QDir::Files);
    QDir translated("test/");
    QStringList files_translated;
    files_translated = translated.entryList(QDir::Files);

    qDebug() << "Entering loop";
    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        int s = 0;

        // Open file. Check if it can close the file
        QString current = *it;
        QString current_copy = "temp/" + current;
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

        // Look into each root file



        // Comparing to each new file
        for(QStringList::Iterator tr = files_translated.begin(); tr != files_translated.end(); tr++)
        {
            QString current_new = "test/" + *tr;
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
