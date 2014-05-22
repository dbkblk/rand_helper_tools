#include <QtCore>
#include <lib/tinyxml2.h>
#include <QDebug>

using namespace tinyxml2;

bool traitementExport();
class langage
{
public:
    void ParseDocument(QString input_file, QString langage);

private:
    XMLDocument input;
    XMLDocument output;
    QString langage;
    const char* lang_file;
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    langage *english = new langage();
    langage *french = new langage();
    langage *german = new langage();
    langage *italian = new langage();
    langage *spanish = new langage();
    langage *polish = new langage();

    QString xml_file = "A_New_Dawn_Concept_Civ4GameText.xml";
    english->ParseDocument(xml_file, "en");
    french->ParseDocument(xml_file, "fr");
    german->ParseDocument(xml_file, "ge");
    italian->ParseDocument(xml_file, "it");
    spanish->ParseDocument(xml_file, "sp");
    polish->ParseDocument(xml_file, "po");

    return a.exec();
}

void langage::ParseDocument(QString input_file, QString langage)
{
    XMLDocument input;
    XMLDocument output;

    // Getting filenames
    QString output_file = langage + "_" + input_file;
    qDebug() << output_file;

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

    XMLDeclaration *declaration = output.NewDeclaration();
    output.InsertFirstChild(declaration);
    XMLNode *root = output.NewElement("values");
    output.InsertEndChild(root);

    // Loop process
    XMLElement* tag_el = input.FirstChildElement("Civ4GameText")->FirstChildElement("TEXT")->ToElement();
    int i = 0;

    for(;i<36;tag_el = tag_el->NextSiblingElement())
    {
        qDebug() << i++;
        // Read tags

        const char* value_tag = tag_el->FirstChildElement("Tag")->GetText();
        /*const char* value_tag;
        if(tag_el->FirstChildElement("Tag")->GetText() == NULL)
        {
            value_tag = "";
        };*/

        const char* value_en = tag_el->FirstChildElement(int_lang.toStdString().c_str())->GetText();

        // Write english tag
        XMLElement *new_tag = output.NewElement("tag");
        root->InsertEndChild(new_tag);
        new_tag->SetAttribute("name", value_tag);
        new_tag->SetText(value_en);
        new_tag->NextSiblingElement();
    }

    output.SaveFile(output_file.toStdString().c_str());
    QDir lang;
    lang.mkdir("lang");
    QFile::copy(output_file,"lang/" + output_file);
    QFile::remove(output_file);
}
