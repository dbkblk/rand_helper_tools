#include <QtCore>
#include <lib/tinyxml2.h>
#include <QDebug>

using namespace tinyxml2;

class langage
{
public:
    void ParseDocument(QString input_file, QString langage);
    void ParseAllFiles(QString langage);

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
    xml->ParseAllFiles("en");
    xml->ParseAllFiles("fr");
    xml->ParseAllFiles("ge");
    xml->ParseAllFiles("it");
    xml->ParseAllFiles("sp");
    xml->ParseAllFiles("po");


    return a.exec();
}

void langage::ParseAllFiles(QString langage)
{
    QDir root(".");
    QStringList folders;
    folders = root.entryList(QDir::Files);

    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        ParseDocument(*it,langage);
    }
}

void langage::ParseDocument(QString input_file, QString langage)
{
    XMLDocument input;
    XMLDocument output;

    // Getting filenames
    QString output_file = langage + "_" + input_file;
    qDebug() << output_file;
    bool save = false;

    QString int_lang;
    int_lang = "English";

    // Check langage
    if(langage == "fr"){int_lang = "French";};
    if(langage == "ge"){int_lang = "German";};
    if(langage == "it"){int_lang = "Italian";};
    if(langage == "es"){int_lang = "Spanish";};
    if(langage == "po"){int_lang = "Polish";};

    QFile::remove("lang/" + output_file);
    input.LoadFile(input_file.toStdString().c_str());
    output.LoadFile(output_file.toStdString().c_str());

    XMLDeclaration *declaration = output.NewDeclaration("xml version=\"1.0\" encoding=\"ISO-8859-1\"");
    output.InsertFirstChild(declaration);
    XMLNode *root = output.NewElement("resources");
    output.InsertEndChild(root);

    // Loop process
    XMLElement* tag_el = input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
    const char* value_en;

    for(tag_el;tag_el != NULL;tag_el = tag_el->NextSiblingElement())
    {
        // Read tags

        const char* value_tag = tag_el->FirstChildElement("Tag")->GetText();
        qDebug() << int_lang << " : " << value_tag;

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
        XMLElement *new_tag = output.NewElement("tag");
        root->InsertEndChild(new_tag);
        new_tag->SetAttribute("name", value_tag);
        new_tag->SetText(value_en);
        new_tag->NextSiblingElement();
    }

    output.SaveFile(output_file.toStdString().c_str());
    QDir dir;
    dir.mkdir("lang");
    QFile::copy(output_file,"lang/" + output_file);
    QFile::remove(output_file);

    // Save input if modified
    if(save)
    {
        dir.mkdir("modif");
        input_file = "modif/" + input_file;
        input.SaveFile(input_file.toStdString().c_str());
    }
}
