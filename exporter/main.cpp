#include <QCoreApplication>
#include <QtCore>
#include <QtXml>

bool checkXMLConformity(QString fileName){
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

QString getIntlName(QString langCode){
    // Input international code and output game code (fr > French)
    // Open the settings
    QString value = "Unsupported";
    QFile settings("exporter.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("languages").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(langCode == lang.attribute("code")){
            value = lang.firstChild().nodeValue();
        }
        lang = lang.nextSiblingElement();
    }
    return value;
}

QStringList getEnabledCodes(){
    // Output list of supported languages
    QStringList list;

    // Open the settings
    QFile settings("exporter.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("languages").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(lang.attribute("enabled").toInt() == 1){
            list << lang.attribute("code");
        }
        lang = lang.nextSiblingElement();
    }
     return list;
}


bool convertXMLCivToAndroid(QString exportDir, QString file, QString langCode)
{
    QDomDocument input;
    QDomDocument output;

    QString langName = getIntlName(langCode);

    file.replace(".XML", ".xml", Qt::CaseSensitive);
    exportDir = exportDir + "/" + langCode + "/";

    // Get filename from path
    QStringList fileList = file.split("/");
    QString fileName = "";
    foreach(QString entry, fileList){
        fileName = entry;
    }

    QString outputFile = exportDir + fileName;
    qDebug() << "Generating" << outputFile;
    QDir dir;
    dir.mkpath(exportDir);

    // Begin process
    QFile file_input(file);
    QFile file_output(outputFile);
    file_input.open(QIODevice::ReadOnly);
    file_output.open(QIODevice::Truncate | QIODevice::WriteOnly);
    input.setContent(&file_input);
    file_input.close();

    QDomNode declaration = output.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    output.insertBefore(declaration,output.firstChild());

    // Check file integrity
    if (!checkXMLConformity(file))
    {
        emit(QString(file + " is not properly formatted"));
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

    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Announcement
    qDebug() << "Civ. IV XML exporter v1.0; \"A New Dawn Resurection, dbkblk\"";

    // First extract informations from the config file
    QFile settings("exporter.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString dirFrom = xml.firstChildElement("main").firstChildElement("directories").attribute("from");
    QString dirTo = xml.firstChildElement("main").firstChildElement("directories").attribute("to");
    if(dirFrom.isEmpty() || dirTo.isEmpty())
    {
        qDebug() << "One of the config directory is empty, cannot continue.";
        return 1;
    }
    else{
        qDebug() << "Exporting files";
    }

    // List all files
    QDir root(dirFrom);
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList folders;
    folders = root.entryList(xml_filter, QDir::Files);

    // Get enabled languages
    QStringList languages;
    if(xml.firstChildElement("main").firstChildElement("options").firstChildElement("englishOnly").firstChild().nodeValue() == "1"){
        languages << "en";
    }
    else{
        languages = getEnabledCodes();
    }
    foreach(QString entry, languages){
        QString langCode = entry;
        // Remove and create the output directory to reflect text files changes in the main mod
        QDir recreate(QString(dirTo + "/" + langCode + "/"));
        recreate.removeRecursively();
        recreate.mkpath(".");

        // Convert all the files in the language
        for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
        {
            // Prepare conversion
            QString fileName = dirFrom + "/" + *it;
            QString exportName = dirTo + "/" + langCode + "/" + *it;
            if(QFile::exists(exportName)){QFile::remove(exportName);}

            // Convert each file to Android XML
            if(!convertXMLCivToAndroid(dirTo, fileName,langCode))
            {
                qDebug() << "A problem has occured on" << fileName << ". Please review the file before to continue.";
                return 1;
            }
        }

        // Check output files
        QStringList output_files;
        QDir output(dirTo + "/" + langCode);
        output_files = output.entryList(xml_filter, QDir::Files);
        for(QStringList::Iterator it = output_files.begin(); it != output_files.end(); it++)
        {
            QString fileName = dirTo + "/" + langCode + "/" + *it;
            int not_empty_counter = 0;
            QDomDocument check;
            QFile file_check(fileName);
            file_check.open(QIODevice::ReadOnly);
            check.setContent(&file_check);
            file_check.close();
            QDomElement node_check = check.firstChildElement("resources").firstChildElement("string");
            for(node_check;!node_check.isNull();node_check = node_check.nextSiblingElement())
            {
                if(!node_check.firstChild().nodeValue().isEmpty())
                {
                    not_empty_counter++;
                }
            }

            if(not_empty_counter == 0)
            {
                file_check.remove();
            }
            check.clear();
        }
    }

    qDebug() << "Exportation is done.";
    return 0;
}
