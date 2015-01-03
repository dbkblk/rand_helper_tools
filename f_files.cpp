#include "f_files.h"
#include "f_lang.h"
#include <QtCore>
#include <QtXml>

// Out of class functions (for code convenience)
void saveDir(QString dir, QString value){
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    xml.firstChildElement("main").firstChildElement("directories").setAttribute(dir,value);
    settings.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts(&settings);
    xml.save(ts, 4);
    settings.close();
}

QString readDir(QString dir){
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement value = xml.firstChildElement("main").firstChildElement("directories").toElement();
    return value.attribute(dir);
}

// f_files definition

f_files::f_files()
{
}

bool f_files::compareMd5(QFile file1, QFile file2){
    // Input two files and compare md5
}

bool f_files::generateXMLAndroid(QFile file){
    // Generate a basic Android XML file ready to be used
}

bool f_files::checkXMLConformity(QString fileName){
    // Check the structure of a civ4 xml file
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDomDocument read;
    read.setContent(&file);
    QDomElement read_text = read.firstChildElement("Civ4GameText").firstChildElement("TEXT");
    if(!read_text.isNull()){
        return true;
    }
    else{
        return false;
    }
}

bool f_files::convertXMLCivToAndroid(QString file, QString langCode)
{
    QDomDocument input;
    QDomDocument output;

    // Get language name
    f_lang l;
    QString langName = l.getIntlName(langCode);

    // Getting filenames
    QString exportdir = readDir("export");
    //qDebug() << "Export dir is" << exportdir;
    file.replace(".XML", ".xml", Qt::CaseSensitive);
    QString output_dir = exportdir + "/" + langCode + "/";

    // Get filename from path
    QStringList filelist = file.split("/");
    QString filename = "";
    foreach(QString entry, filelist){
        filename = entry;
    }

    QString output_file = output_dir + filename;
    qDebug() << "Generating" << output_file;
    QDir dir;
    dir.mkpath(output_dir);
    bool save = false;

    // Begin process
    QFile file_input(file);
    QFile file_output(output_file);
    file_input.open(QIODevice::ReadOnly);
    file_output.open(QIODevice::Truncate | QIODevice::WriteOnly);
    input.setContent(&file_input);
    file_input.close();

    QDomNode declaration = output.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    output.insertBefore(declaration,output.firstChild());

    // Check file integrity
    f_files f;
    if (!f.checkXMLConformity(file))
    {
        qDebug() << file << " is not properly formatted";
        file_input.remove();
        return false;
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
        if(tag_el.firstChildElement(langName).isNull())
        {
            QDomText write_empty_node = output.createTextNode("");
            new_tag.appendChild(write_empty_node);
            new_tag.setAttribute("name", value_tag);
        }
        else if (!tag_el.firstChildElement(langName).firstChildElement("Text").firstChild().isNull())
        {
            QString node_text_value = tag_el.firstChildElement(langName).firstChildElement("Text").firstChild().nodeValue();
            node_text_value.replace("\"","\\\"");

            new_tag.appendChild(output.createTextNode(node_text_value));
            new_tag.setAttribute("name", value_tag);
            new_tag.setAttribute("gender", tag_el.firstChildElement(langName).firstChildElement("Gender").firstChild().nodeValue());
            new_tag.setAttribute("plural", tag_el.firstChildElement(langName).firstChildElement("Plural").firstChild().nodeValue());


        }
        else
        {
            QString node_text_value = tag_el.firstChildElement(langName).firstChild().nodeValue();
            node_text_value.replace("\"","\\\"");

            QDomText node_value = output.createTextNode(node_text_value);
            new_tag.appendChild(node_value);
            new_tag.setAttribute("name", value_tag);
        }
    }

    file_output.write(output.toByteArray());
    file_input.close();
    file_output.close();
}

