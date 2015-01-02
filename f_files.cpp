#include "f_files.h"
#include <QtCore>

f_files::f_files()
{
}

bool f_files::compareMd5(QFile file1, QFile file2){
    // Input two files and compare md5
}

bool f_files::generateXMLAndroid(QFile file){
    // Generate a basic Android XML file ready to be used
}

bool f_files::checkXMLConformity(QFile file){
    // Check the structure of a civ4 xml file
}

QString f_files::readParam(QString param)
{
    QSettings settings("xmlparser_config.ini", QSettings::IniFormat);
    if(!settings.contains(param)) {
        return "error";
    }
    QString value = settings.value(param).toString();
    return value;
}

bool f_files::setParam(QString param, QString newValue)
{
    if(!QFile::exists("xmlparser_config.ini")) {
        QFile ch_conf;
        ch_conf.open(QIODevice::WriteOnly);
    }
    QSettings settings("xmlparser_config.ini", QSettings::IniFormat);
    settings.setValue(param, newValue);
    return 0;
}
