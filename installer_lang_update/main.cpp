#include <QtCore>
#include <QtXml>

QString getIntlName(QString langCode){
    // Input international code and output game code (fr > French)
    // Open the settings
    QString value = "Unsupported";
    QFile settings("languageMap.xml");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(langCode == lang.attribute("code")){
            value = lang.firstChild().nodeValue();
        }
        lang = lang.nextSiblingElement();
    }
    return value;
}

QString getLanguageCodeFromName(QString langName){
    QDomDocument xml;
    QFile file("languageMap.xml");
    if(!file.open(QIODevice::ReadOnly))
     {
         qDebug() << "Error opening languageMap.xml";
         return 0;
     }
    xml.setContent(&file);
    file.close();
    QString value = "en"; // Fallback to English

    QDomElement lang = xml.firstChildElement("main").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(langName == lang.firstChild().nodeValue()){
            value = lang.attribute("code");
        }
        lang = lang.nextSiblingElement();
    }
    return value;
}

int getLocalVersion()
{
    //** Get version number from "Assets/Python/Contrib/CvModName.py"
    // Open python file
    QFile file("../Rise of Mankind - A New Dawn/Assets/Python/Contrib/CvModName.py");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in_enc(&file);
    while(!in_enc.atEnd())
    {
        // Go to the mod version
        QString line = in_enc.readLine();
        QString version;
        if(line.contains("modVersion = "))
        {
            version = line.right(5).left(3);
            return version.toInt();
        }
    }
    file.close();

    return 0;
}

QStringList getTagList()
{
    // Output list of supported languages
    QStringList list;

    // Open the settings
    QFile settings("translations/installer_en.xml");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement tag = xml.firstChildElement("resources").firstChildElement("string").toElement();
    while(!tag.isNull()){
        list << tag.attribute("name");
        tag = tag.nextSiblingElement();
    }
     return list;
}

QString getTranslation(QString langCode, QString tag)
{
    // Open the english file
    QFile settings("translations/installer_en.xml");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString result = "";
    QDomElement tagEl = xml.firstChildElement("resources").firstChildElement("string").toElement();
    while(!tagEl.isNull()){
        if(tagEl.attribute("name") == ("INSTALLER_" + tag)){
            result = tagEl.firstChild().nodeValue();
        }
        tagEl = tagEl.nextSiblingElement();
    }

    // Open the translated file
    QFile transl("translations/installer_" + langCode + ".xml");
    if(!transl.open(QIODevice::ReadOnly))
    {
        return result;
    }
    QDomDocument xmltransl;
    xmltransl.setContent(&transl);
    transl.close();
    QDomElement tagTransl = xmltransl.firstChildElement("resources").firstChildElement("string").toElement();
    while(!tagTransl.isNull()){
        if(tagTransl.attribute("name") == ("INSTALLER_" + tag)){
            if(!QString(tagTransl.firstChild().nodeValue()).isNull()){
                result = tagTransl.firstChild().nodeValue();
                result.replace("\"","'");
                result.replace("\\'","'");
                return result;
            }
        }
        tagTransl = tagTransl.nextSiblingElement();
    }
     return result;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Open the installer file
    QString name_in = "full_installer_script.nsi";
    QString name_out = name_in + "temp";
    QFile file_in(name_in);
    QFile file_out(name_out);
    file_in.open(QIODevice::ReadOnly | QIODevice::Text);
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream in_enc(&file_in);
    QTextStream out_enc(&file_out);
    out_enc.setCodec("UTF-8");
    while(!in_enc.atEnd())
    {
        QString line = in_enc.readLine();
        // Update to current revision
        if(line.contains("  !define REVISION "))
        {
            QStringList temp = line.split("\"");
            temp[1] = QString::number(getLocalVersion());
            line = temp.join("\"");
        }

        // Update translations lines (including commented ones)
        QString langString = "LangString ";
        if(line.startsWith("; LangString")){
            langString = "; LangString ";
        }

        if(line.startsWith(langString)){
            QString backup = line;

            // Get language infos
            QStringList temp_lang = line.replace("{","|").replace("}","|").split("|");
            QStringList temp_lang_name = temp_lang[1].split("_");
            QString langName = temp_lang_name[1];
            QString langCode = getLanguageCodeFromName(langName);

            // Get tag
            QStringList temp_tag = temp_lang[0].split(" ");
            QString tag = temp_tag[1];
            if(backup.startsWith(";"))
            { // Pick the second char as the first is a comment
                tag = temp_tag[2];
            }
            QString translation = getTranslation(langCode, tag);
            if (!translation.isEmpty())
            {
                line = langString + tag + " ${LANG_" + langName + "} " + "\"" + translation + "\"";
            }
            else
            {
                line = backup;
            }
        }

        out_enc << line << "\n";
    }
    file_in.close();
    file_out.close();
    QFile::remove(name_in);
    QFile::rename(name_out,name_in);
    return 0;
}
