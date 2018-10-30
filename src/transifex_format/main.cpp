#include <QCoreApplication>
#include <QtCore>
#include <QtXml/QtXml>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // List all json files in the folder
    QStringList filter;
    QStringList list;
    filter << "*.json";
    QDir root(".");
    foreach(QFileInfo entry, root.entryInfoList(filter, QDir::Files)){
        list << entry.fileName();
    }

    // Open output xml file
    QFile file("../release_files/Rise of Mankind - A New Dawn/checker/languagesDefine.xml");
    file.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&file);
    file.close();

    // For each file, extract language and progress and add it to the languageDefines
    foreach(QString langFile, list){
        // Read language name from file
        QString langCode = langFile;
        langCode.replace(".json","");
        qDebug() << langFile;

        // Parse file
        //qDebug() << langFile;
        QFile json_file(langFile);
        json_file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString val = "";
        QTextStream ts(&json_file);
        int i = 0;
        QString line;
        while(!ts.atEnd()){
            line = ts.readLine();
            if(i >= 11){
                val.append(line + "\n");
            }
            i++;
        }
        //qDebug() << val;
        json_file.close();
        QJsonDocument json = QJsonDocument::fromJson(val.toUtf8());
        QJsonObject object = json.object();

        // Compute progress
        int progress = object["translated_segments"].toDouble() / object["total_segments"].toDouble() * 100;
        qDebug() << progress;

        // Inject into the xml
        QDomElement element_define = xml.firstChildElement("Civ4Defines").firstChildElement("Define").toElement();
        for(;!element_define.isNull();element_define = element_define.nextSiblingElement()){
            if(element_define.firstChildElement("Code").firstChild().nodeValue() == langCode){
                element_define.firstChildElement("Progress").firstChild().setNodeValue(QString::number(progress));
            }
        }
    }

    // Save the release file
    if (!file.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        return 1;
    }
    QTextStream ts1(&file);
    xml.save(ts1, 4);
    file.close();

    return 0;
}
