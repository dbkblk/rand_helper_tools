#include <QtCore>
#include <QtXml>

QString getIntlName(QString langCode){
    // Input international code and output game code (fr > French)
    // Open the settings
    QString value = "Unsupported";
    QFile settings("importer.config");
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
    QFile settings("importer.config");
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

QStringList getSupportedCodes(){
    // Output list of supported languages
    QStringList list;

    // Open the settings
    QFile settings("importer.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("languages").firstChildElement("language").toElement();
    while(!lang.isNull()){
        list << lang.attribute("code");
        lang = lang.nextSiblingElement();
    }
     return list;
}

bool isSupportedLang(QString langCode){
    // Ouput true or false
    QStringList list = getSupportedCodes();
    int counter = 0;
    foreach (QString entry, list){
        if(entry == langCode){
            counter++;
        }
    }
    if(counter>0){
        return true;
    }
    else{
        return false;
    }
}

QStringList checkImportLanguages(QString dirTrans)
{
    qDebug() << "Checking directories to import.";
    QDir dir_import(dirTrans);
    QStringList language_list;
    language_list = dir_import.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList language_found;
    for(QStringList::Iterator it = language_list.begin(); it !=language_list.end();it++)
    {
        if(isSupportedLang(*it))
        {
            qDebug() << "Found" << *it << "language in the translation folder.";
            language_found << *it;
        }
        else
        {
            qDebug() << "Dir" << *it << "isn't recognized or is incompatible language.";
        }
    }
    return language_found;
}

QStringList getTranslationFilesList(QString dirTrans, QString langCode){
    QStringList xml_filter;
    QStringList list;
    xml_filter << "*.xml";
    QDir root(dirTrans + langCode);
    if(!root.exists()){
        qDebug() << "Language folder doesn't exist";
        return list;
    }
    foreach(QFileInfo entry, root.entryInfoList(xml_filter, QDir::Files)){
        list << entry.filePath();
    }
    return list;
}

bool checkXMLConformity(QString fileName){
    // Check the structure of a civ4 xml file
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QDomDocument read;
    read.setContent(&file);
    QDomElement read_text = read.firstChildElement("Civ4GameText").firstChildElement();
    if(!read_text.isNull()){
        return true;
    }
    else{
        return false;
    }
}

bool convertXMLAndroidToCiv(QString dirTrans, QString dirTo, QString file, QStringList files_to_check, QString langCode){
    // Read the translations dir
    QDir translations_dir(dirTrans + langCode);
    if(!translations_dir.exists()){
        qDebug() << "Translations directory not set, aborting.";
        return false;
    }
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files_to_translate;
    files_to_translate = translations_dir.entryList(xml_filter, QDir::Files);

    // Read the language name
    QString langName = getIntlName(langCode);

    // Read the export dir
    QDir export_dir(dirTo);
    if(!export_dir.exists()){
        qDebug() << ("Export directory not set, aborting.");
        return false;
    }

    // Create / open existing log list
    QFile log_file(dirTo + "report_" + langCode + ".xml");
    QDomDocument log;
    if(log_file.exists()){
        log_file.open(QIODevice::ReadOnly);
        log.setContent(&log_file);
        log_file.close();
    }
    else{
        QDomNode declaration = log.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
        log.insertBefore(declaration,log.firstChild());
        log.appendChild(log.createElement("root"));
    }
    QDomElement log_root = log.firstChildElement();

    // Check the file against the list for tags
    for(QStringList::Iterator it = files_to_check.begin(); it != files_to_check.end(); it++)
    {
        int s = 0;
        QString current = *it;

        //qDebug() << "Checking " << current;

        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        // Check file integrity
        if (!checkXMLConformity(current))
        {
            qDebug() << current << " is not properly formatted";
            QFile::remove(current);
            return false;
        }

        // Open the translated file
        QString current_new = file;
        QDomDocument input_tr;
        QFile file_tr(current_new);
        file_tr.open(QIODevice::ReadOnly);
        input_tr.setContent(&file_tr);
        file_tr.close();
        QDomElement root = input_tr.documentElement();


        QDomElement tag_orig = input.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        for(tag_orig;!tag_orig.isNull();tag_orig = tag_orig.nextSiblingElement())
        {
            // Compare each occurence with each tag of the translated file
            QString value_tag = tag_orig.firstChildElement("Tag").firstChild().nodeValue();
            QDomElement tag_tr = root.firstChildElement("string").toElement();
            //qDebug() << tag_tr.attribute("name");

            for(tag_tr;!tag_tr.isNull();tag_tr = tag_tr.nextSiblingElement())
            {
                QString value_tag_tr = tag_tr.attribute("name");
                //qDebug() << value_tag_tr;

                if (value_tag == value_tag_tr)
                {
                    //qDebug() << value_tag;
                    s++;
                    int skip = 0;
                    QString value_text_tr = tag_tr.firstChild().nodeValue();
                    value_text_tr.replace("\\'","\'").replace("\\\"","\"").replace("\\t","[TAB]");
                    QString value_text;

                    // Case: English value equals the value to translate
                    if(value_text_tr == tag_orig.firstChildElement("English").firstChild().nodeValue())
                    {
                        skip++;
                    }

                    // Checking tag presence
                    // Sub language marker
                    if (!tag_orig.firstChildElement(langName).firstChildElement().firstChild().nodeValue().isNull())
                    {
                        value_text = tag_orig.firstChildElement(langName).firstChildElement().firstChild().nodeValue();
                    }
                    // No language marker
                    else if(tag_orig.firstChildElement(langName).isNull())
                    {
                        value_text = "";
                    }

                    // Direct language marker
                    else
                    {
                        value_text = tag_orig.firstChildElement(langName).firstChild().nodeValue();
                    }

                    // Case: English value equals the value to translate but tag already exists (> remove it).
                    if( skip > 0 )
                    {
                        if (!tag_orig.firstChildElement(langName).isNull())
                        {
                            // Remove the element
//                            qDebug() << "Removing value" << value_tag;
                            QDomNode temp = tag_orig.firstChildElement(langName);
                            temp.parentNode().removeChild(temp);

                            // Add a log entry
                            QDomElement log_entry = log.createElement("entry");
                            QStringList temp_list = current.split("/");
                            QString it_name = "";
                            foreach(QString entry, temp_list){
                                it_name = entry;
                            }
                            log_entry.setAttribute("tag",value_tag);
                            log_entry.setAttribute("file",it_name);
                            log_root.appendChild(log_entry);
                            QDomNode log_old_value = log.createElement("old_value");
                            QDomNode log_new_value = log.createElement("new_value");
                            log_entry.appendChild(log_old_value);
                            log_entry.appendChild(log_new_value);
                            QDomText log_old_value_text = log.createTextNode(value_text_tr);
                            QDomText log_new_value_text = log.createTextNode("");
                            log_old_value.appendChild(log_old_value_text);
                            log_new_value.appendChild(log_new_value_text);

                        }
                    }

                    //qDebug() << value_tag << " : " << value_text << " / " << value_text_tr;

                    // Normalize value
                    if(value_text_tr.isEmpty())
                    {
                        value_text_tr = "";
                    }

                    // Compare original and modified strings
                    if(value_text != value_text_tr && skip == 0)
                    {
                        // Log the file
                        QDomElement log_entry = log.createElement("entry");
                        QStringList temp_list = current.split("/");
                        QString it_name = "";
                        foreach(QString entry, temp_list){
                            it_name = entry;
                        }
                        log_entry.setAttribute("tag",value_tag);
                        log_entry.setAttribute("file",it_name);
                        log_root.appendChild(log_entry);
                        QDomNode log_old_value = log.createElement("old_value");
                        QDomNode log_new_value = log.createElement("new_value");
                        log_entry.appendChild(log_old_value);
                        log_entry.appendChild(log_new_value);
                        QDomText log_old_value_text = log.createTextNode(value_text);
                        QDomText log_new_value_text = log.createTextNode(value_text_tr);
                        log_old_value.appendChild(log_old_value_text);
                        log_new_value.appendChild(log_new_value_text);

                        // Case: original direct value, new subtag value
                        if (tag_orig.firstChildElement(langName).firstChildElement("Text").firstChild().isNull() && !tag_tr.attribute("gender").isNull())
                        {
                            QString new_value_text = value_text_tr;
                            QString new_value_gender = tag_tr.attribute("gender");
                            QString new_value_plural = tag_tr.attribute("plural");

                            // If no language value, add it first
                            if(tag_orig.firstChildElement(langName).firstChild().isNull()){
                                QDomNode new_node_language = input.createElement(langName);
                                tag_orig.appendChild(new_node_language);
                            }
                            // Empty text value, then add subtags
                            else {
                                tag_orig.removeChild(tag_orig.firstChildElement(langName).firstChild());
                            }
                            QDomNode new_node_text = input.createElement("Text");
                            QDomNode new_node_gender = input.createElement("Gender");
                            QDomNode new_node_plural = input.createElement("Plural");
                            tag_orig.firstChildElement(langName).appendChild(new_node_text);
                            tag_orig.firstChildElement(langName).appendChild(new_node_gender);
                            tag_orig.firstChildElement(langName).appendChild(new_node_plural);

                            QDomText new_node_text_value = input.createTextNode(new_value_text);
                            QDomText new_node_gender_value = input.createTextNode(new_value_gender);
                            QDomText new_node_plural_value = input.createTextNode(new_value_plural);
                            new_node_text.appendChild(new_node_text_value);
                            new_node_gender.appendChild(new_node_gender_value);
                            new_node_plural.appendChild(new_node_plural_value);

                        }
                        // Case: original, new subtag value
                        else if(!tag_orig.firstChildElement(langName).firstChildElement("Text").firstChild().isNull()){
                            tag_orig.firstChildElement(langName).firstChildElement("Text").firstChild().setNodeValue(value_text_tr);
                            // Set gender and plural
                            if (!tag_tr.attribute("gender").isNull()){
                            QString new_value_gender = tag_tr.attribute("gender");
                            tag_orig.firstChildElement(langName).firstChildElement("Gender").firstChild().setNodeValue(new_value_gender);}
                            if(!tag_tr.attribute("plural").isNull()){
                            QString new_value_plural = tag_tr.attribute("plural");
                            tag_orig.firstChildElement(langName).firstChildElement("Plural").firstChild().setNodeValue(new_value_plural);
                            }
                        }

                        // Case: no original
                        else if(tag_orig.firstChildElement(langName).firstChild().isNull())
                        {
                            // New subtag value
                            if (value_text == "" && !tag_tr.attribute("gender").isNull()){
                                QString new_value_text = value_text_tr;
                                QString new_value_gender = tag_tr.attribute("gender");
                                QString new_value_plural = tag_tr.attribute("plural");

                                // Append nodes
                                QDomNode new_node_language = input.createElement(langName);
                                tag_orig.appendChild(new_node_language);
                                QDomNode new_node_text = input.createElement("Text");
                                QDomNode new_node_gender = input.createElement("Gender");
                                QDomNode new_node_plural = input.createElement("Plural");
                                new_node_language.appendChild(new_node_text);
                                new_node_language.appendChild(new_node_gender);
                                new_node_language.appendChild(new_node_plural);

                                QDomText new_node_text_value = input.createTextNode(new_value_text);
                                QDomText new_node_gender_value = input.createTextNode(new_value_gender);
                                QDomText new_node_plural_value = input.createTextNode(new_value_plural);
                                new_node_text.appendChild(new_node_text_value);
                                new_node_gender.appendChild(new_node_gender_value);
                                new_node_plural.appendChild(new_node_plural_value);
                            }

                            // New direct value
                            else {
                            QDomNode new_tag = input.createElement(langName);
                            tag_orig.appendChild(new_tag);
                            QDomText new_value = input.createTextNode(value_text_tr);
                            new_tag.appendChild(new_value);
                            }
                        }

                        // Case: original, direct value
                        else
                        {
                            //qDebug() << "here";
                            tag_orig.firstChildElement(langName).firstChild().setNodeValue(value_text_tr);
                        }
                    }
                }
                }
            }

        // Save the modified file
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream ts(&file_input);
        input.save(ts, 4);
        file_input.close();
    }

    // Generate the report
    log_file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    QTextStream rep(&log_file);
    log.save(rep, 4);
    log_file.close();

    // Get file name
    QStringList filelist = file.split("/");
    QString filename = "";
    foreach(QString entry, filelist){
        filename = entry;
    }

    qDebug() << langCode << ": Imported" << filename;
    return true;
}

QString checkMd5(QString file_path){
    // Generate hash of original file
    QFile file(file_path);
    QCryptographicHash crypto(QCryptographicHash::Md5);
    file.open(QFile::ReadOnly);
    while(!file.atEnd()){
        crypto.addData(file.read(8192));
    }
    QByteArray hash = crypto.result().toHex();
    QString hash_string = hash;
    file.close();
    return hash_string;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Announcement
    qDebug() << "Civ. IV XML importer v1.2; \"A New Dawn Resurection, dbkblk\"";

    // First extract informations from the config file
    QFile settings("importer.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString dirFrom = xml.firstChildElement("main").firstChildElement("directories").attribute("from");
    QString dirTo = xml.firstChildElement("main").firstChildElement("directories").attribute("to");
    QString dirTrans = xml.firstChildElement("main").firstChildElement("directories").attribute("translations");
    if(dirFrom.isEmpty() || dirTo.isEmpty() || dirTrans.isEmpty())
    {
        qDebug() << "One of the config directory is empty, cannot continue.";
        return 1;
    }
    else{
        // Make sure the path is normalized
        if(!dirFrom.endsWith("/")){
            dirFrom.append("/");
        }
        if(!dirTo.endsWith("/")){
            dirTo.append("/");
        }
        if(!dirTrans.endsWith("/")){
            dirTrans.append("/");
        }
        dirFrom.replace("\\","/");
        dirTo.replace("\\","/");
        dirTrans.replace("\\","/");
        qDebug() << "Importing files";
    }
    QDir to(dirTo);
    to.removeRecursively();

    // Check options
    int opt = xml.firstChildElement("main").firstChildElement("options").firstChildElement("importtoall").firstChild().nodeValue().toInt();
    bool opt_same_name = true;
    if(opt == 1){opt_same_name = false;}

    // Read export dir
    QStringList translations = checkImportLanguages(dirTrans);
    QStringList enabled_translations = getEnabledCodes();
    translations.removeAll("en"); // Remove english from the list
    foreach(QString entry, translations)
    { // Remove disabled translations
        if(!enabled_translations.contains(entry))
        {
            translations.removeAll(entry);
        }
    }

    // Get the directories
    if (!to.mkpath(".")) {
        qDebug() << "Failed to create the export directory.";
        return a.exec();
    }

    // Get base files and copy them to export
    QStringList xml_filter;
    QStringList list;
    xml_filter << "*.xml";
    QDir root(dirFrom);
    foreach(QFileInfo entry, root.entryInfoList(xml_filter, QDir::Files)){
        list << entry.filePath();
    }
    QStringList exported_list;
    QStringList base_list;
    foreach(QString entry, list){
        QStringList filelist = entry.split("/");
        QString filename = "";
        foreach(QString entry, filelist){
            filename = entry;
        }
        base_list << filename;
        QFile::copy(entry,dirTo + filename);
        exported_list << (dirTo + filename);
    }

    // Read each translation file for importation
    foreach(QString tr_code, translations){
        QStringList tr_fileslist = getTranslationFilesList(dirTrans, tr_code);
        qDebug() << "Processing " + getIntlName(tr_code);
        foreach(QString tr_file, tr_fileslist){
            QStringList same_name;
            if(opt_same_name){
                // Extract translated files names if option is checked;
                QStringList tr_temp = tr_file.split("/");
                QString tr_filename = "";
                foreach(QString entry, tr_temp){
                    tr_filename = entry;
                }
                foreach(QString entry, base_list){
                    if(entry == tr_filename){
                        same_name << QString(dirTo + entry);
                    }
                }
            }
            else{
                same_name = exported_list;
            }

            convertXMLAndroidToCiv(dirTrans, dirTo, tr_file, same_name,tr_code);
        }

    }

    // Check md5sum
    foreach(QString entry, list){
        // Check md5 of base file
        QString base_md5 = checkMd5(entry);

        // Check md5 of export file
        QStringList filelist = entry.split("/");
        QString filename = "";
        foreach(QString entry, filelist){
            filename = entry;
        }
        QString export_md5 = checkMd5(dirTo + filename);
        //qDebug() << base_md5 << " / " << export_md5;
        if (base_md5 == export_md5){QFile::remove(dirTo + filename);}
    }

    qDebug() << "Import finished. Only modified files are kept in the output folder. You can quit the program and review the reports.";

    return 0;
}
