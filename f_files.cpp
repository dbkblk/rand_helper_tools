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

bool f_files::convertXMLAndroidToCiv(QString file, QStringList files_to_check, QString langCode){
    // Read the translations dir
    f_files f;
    QString translations_dir_string = readDir("translations") + "/" + langCode;
    QDir translations_dir(translations_dir_string);
    if(!translations_dir.exists()){
        qDebug() << "Translations directory not set, aborting.";
        return false;
    }
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files_to_translate;
    files_to_translate = translations_dir.entryList(xml_filter, QDir::Files);

    // Read the language name
    f_lang l;
    QString langName = l.getIntlName(langCode);

    // Read the export dir
    QDir export_dir(readDir("export"));
    if(!export_dir.exists()){
        qDebug() << ("Export directory not set, aborting.");
        return false;
    }

    // Create / open existing log list
    QFile log_file(readDir("export") + "/report_" + langCode );
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
        if (!f.checkXMLConformity(current))
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
                    s++;
                    int skip = 0;
                    QString value_text_tr = tag_tr.firstChild().nodeValue();
                    value_text_tr.replace("\\'","\'").replace("\\\"","\"").replace("\\t","[TAB]");
                    QString value_text;

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

                            // Empty text value, then add subtags
                            tag_orig.removeChild(tag_orig.firstChildElement(langName).firstChild());
                            QDomNode new_node_text = input.createElement("Text");
                            QDomNode new_node_gender = input.createElement("Gender");
                            QDomNode new_node_plural = input.createElement("Plural");
                            tag_orig.firstChildElement(langName).firstChild().appendChild(new_node_text);
                            tag_orig.firstChildElement(langName).firstChild().appendChild(new_node_gender);
                            tag_orig.firstChildElement(langName).firstChild().appendChild(new_node_plural);

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

     qDebug() << file << " imported";
}

QStringList f_files::checkImportLanguages()
{
    qDebug() << "Checking directories in 'import/'...";
    QDir dir_import(readDir("translations"));
    f_lang l;
    QStringList language_list;
    language_list = dir_import.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    QStringList language_found;
    for(QStringList::Iterator it = language_list.begin(); it !=language_list.end();it++)
    {
        if(l.isSupportedLang(*it))
        {
            qDebug() << "Found" << *it << "language in 'Import/'";
            language_found << *it;
        }
        else
        {
            qDebug() << "Dir" << *it << "isn't recognized or is incompatible language";
        }
    }
    return language_found;
}

QStringList f_files::getBaseFilesList(){
    QStringList xml_filter;
    QStringList list;
    xml_filter << "*.xml";
    QDir root(readDir("base"));
    foreach(QFileInfo entry, root.entryInfoList(xml_filter, QDir::Files)){
        list << entry.filePath();
    }
    return list;
}

QStringList f_files::getTranslationFilesList(QString langCode){
    QStringList xml_filter;
    QStringList list;
    xml_filter << "*.xml";
    QDir root(readDir("translations") + "/" + langCode);
    if(!root.exists()){
        qDebug() << "Language folder doesn't exist";
        return list;
    }
    foreach(QFileInfo entry, root.entryInfoList(xml_filter, QDir::Files)){
        list << entry.filePath();
    }
    return list;
}
