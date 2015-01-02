#include "f_files.h"
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

