#include <QCoreApplication>
#include <QtCore>
#include <QtXml/QtXml>

QString getRootTag(QString fileSuffix)
{
    if(fileSuffix == "Audio2DScripts.xml"){
        return "Script2DSounds";
    }
    else if(fileSuffix == "Audio3DScripts.xml"){
        return "Script3DSounds";
    }
    else if(fileSuffix == "AudioDefines.xml"){
        return "AudioDefines";
    }
    else if(fileSuffix == "AudioSoundscapeScripts.xml"){
        return "ScriptSoundscapes";
    }
    else{
        return "error";
    }
}

QString getStartTag(QString fileSuffix)
{
    if(fileSuffix == "Audio2DScripts.xml"){
        return "Script2DSound";
    }
    else if(fileSuffix == "Audio3DScripts.xml"){
        return "Script3DSound";
    }
    else if(fileSuffix == "AudioDefines.xml"){
        return "SoundData";
    }
    else if(fileSuffix == "AudioSoundscapeScripts.xml"){
        return "ScriptSoundscape";
    }
    else{
        return "error";
    }
}

QString getIDTag(QString fileSuffix)
{
    if(fileSuffix == "Audio2DScripts.xml"){
        return "ScriptID";
    }
    else if(fileSuffix == "Audio3DScripts.xml"){
        return "ScriptID";
    }
    else if(fileSuffix == "AudioDefines.xml"){
        return "SoundID";
    }
    else if(fileSuffix == "AudioSoundscapeScripts.xml"){
        return "ScriptID";
    }
    else{
        return "error";
    }
}

bool injectXml(QString defaultFile, QStringList fileList, QString fileSuffix){
    qDebug() << "Injecting" << fileList.count() << "audio files into" << fileSuffix;
    // Open default xml file and make a list of all tags
    QFile audio_file(defaultFile);
    if(!audio_file.open(QIODevice::ReadOnly)){
        return false;
    }
    QDomDocument audio;
    audio.setContent(&audio_file);
    audio_file.close();

    QString root_tag = getRootTag(fileSuffix);
    QString start_tag = getStartTag(fileSuffix);
    QString id_tag = getIDTag(fileSuffix);

    QStringList audio_tag_list;
    QDomElement audio_tag;
    if(fileSuffix == "AudioDefines.xml"){
        audio_tag = audio.firstChildElement(root_tag).firstChildElement("SoundDatas").firstChildElement(start_tag).toElement();
    }
    else{
        audio_tag = audio.firstChildElement(root_tag).firstChildElement(start_tag).toElement();
    }
    for(;!audio_tag.isNull();audio_tag = audio_tag.nextSiblingElement()){
        // Check if tag exists in the base file
        audio_tag_list << audio_tag.firstChildElement(id_tag).firstChild().nodeValue();
    }

    // List all files with the right suffix
    int total_new = 0;
    foreach(QString fileName, fileList){
        // For each file, for each tag ID, check if exists and inject all dependencies if not.
        if(fileName.endsWith(fileSuffix)){
            QFile file(fileName);
            file.open(QIODevice::ReadOnly);
            QDomDocument xml;
            xml.setContent(&file);
            file.close();
            QDomElement entity;
            if(fileSuffix == "AudioDefines.xml"){
                entity = xml.firstChildElement(root_tag).firstChildElement("SoundDatas").firstChildElement(start_tag).toElement();
            }
            else{
                entity = xml.firstChildElement(root_tag).firstChildElement(start_tag).toElement();
            }
            QList<QDomElement> node_list;
            for(entity;!entity.isNull();entity = entity.nextSiblingElement()){
                // Check if tag exists in the base file
                QString tag = entity.firstChildElement(id_tag).firstChild().nodeValue();
                int j = 0;
                foreach(QString entry, audio_tag_list){
                    if(entry == tag){
                        j++;
                    }
                }
                if(j == 0){
                    total_new++;
                    // Inject all dependencies to the xml file
                    audio_tag_list << tag;
                    node_list << entity;
                }
            }
            foreach(QDomElement element, node_list){
            if(fileSuffix == "AudioDefines.xml"){
                audio.firstChildElement(root_tag).firstChildElement("SoundDatas").appendChild(element);
            }
            else{
                audio.firstChildElement(root_tag).appendChild(element);
            }
            }
        }
    }

    // Save the file
    if(total_new > 0){
        audio_file.remove();
        audio_file.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream ts(&audio_file);
        audio.save(ts, 4);
        audio_file.close();
        return true;
    }
    return false;
}

QString getOfficialFile(QString fileSuffix){
    return fileSuffix.prepend("../XML/Audio/");
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "CivIV - Audio XML injector v1.1 - By dbkblk (A New Dawn), based on the WoC idea.";

    // List all audio files
    QStringList filter;
    filter << "*AudioSoundscapeScripts.xml" << "*AudioDefines.xml" << "*Audio3DScripts.xml" << "*Audio2DScripts.xml";
    QStringList file_list;
    QDir root(".");
    qDebug() << "Listing file";
    QDirIterator it(".", filter, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        //qDebug() << it.next().remove(0,2);
        file_list << it.next().remove(0,2);
    }

    // Inject values
    injectXml("../XML/Audio/Audio2DScripts.xml",file_list,"Audio2DScripts.xml");
    injectXml("../XML/Audio/Audio3DScripts.xml",file_list,"Audio3DScripts.xml");
    injectXml("../XML/Audio/AudioDefines.xml",file_list,"AudioDefines.xml");
    injectXml("../XML/Audio/AudioSoundscapeScripts.xml",file_list,"AudioSoundscapeScripts.xml");

    // Wait for input
    system("PAUSE");
    return 0;
}
