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

void saveOption(QString option, QString value){
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    xml.firstChildElement("main").firstChildElement("option").firstChildElement(option).firstChild().setNodeValue(value);
    settings.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts(&settings);
    xml.save(ts, 4);
    settings.close();
}

QString readOption(QString option){
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomNode value = xml.firstChildElement("main").firstChildElement("option").firstChildElement(option).firstChild();
    return value.nodeValue();
}

// f_files definition

f_files::f_files()
{
}

QString f_files::checkMd5(QString file_path){
    // Generate hash of original file
    QFile file(file_path);
    QCryptographicHash crypto(QCryptographicHash::Md5);
    file.open(QFile::ReadOnly);
    while(!file.atEnd()){
        crypto.addData(file.read(8192));
    }
    QByteArray hash = crypto.result().toHex();
    QString hash_string = hash;
    //qDebug() << "Generated hash " << orig_hash_string << " of " << *it;
    file.close();
    return hash_string;
}

/*bool f_files::generateXMLAndroid(QFile file){
    // Generate a basic Android XML file ready to be used
}*/

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
    QFile log_file(readDir("export") + "/report_" + langCode + ".xml");
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
                    //qDebug() << value_tag;
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

void f_files::sortCategories()
{
    /* Sorting logic */
    /* Check parse file presence
     * Prepare original files by converting them to UTF8
     * List all tags in a QStringList
     * Initialize all XML files by category plus MISC. Add a _PEDIA and _REMOVED_ for all categories
     * For each tag (until the end of the list):
        * Check category
        * Open category file
        * Check all files looking for the tag
        * Check if the tag has already been found
        * Add the tag to the category file
        * Save category file
    * Convert the files back to Civ4
    * Print the duplicate list */
/*
    // Check requirements
    QFile categories("xml_parser.config");
    if(!categories.exists())
    {
       qDebug() << "The parser need the config file to continue. Aborting...";
       return;
    }

    // Listing root files
    qDebug() << "Listing files...";
    QString exportdir_string = readDir("export") + "/";
    QDir exportdir(exportdir_string);
    if(!export_dir.exists()){
        emit message(QString("Export directory not set, aborting."));
        return;
    }
    exportdir.mkpath("new/");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(readDir("base"));
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Import files
    qDebug() << "Copying files...";
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"sorted/"+*it);
        ConvertCiv4ToUTF8("sorted/"+*it);
    }

    // Collect all tags in a bigfile
    qDebug() << "Collecting all tags to a file";
    QDomDocument xml_collect;
    QFile xml_collect_file("sorted/TEMP");
    xml_collect_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    // Create the output file
    QDomNode declaration = xml_collect.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    xml_collect.insertBefore(declaration,xml_collect.firstChild());
    QDomNode collect_root = xml_collect.createElement("Civ4GameText");
    xml_collect.appendChild(collect_root);

   // Loop the input file
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QString current = "sorted/" + *it;
        QDomDocument read;
        QFile file_in(current);
        file_in.open(QIODevice::ReadOnly);
        read.setContent(&file_in);
        QDomNode read_text = read.firstChildElement("Civ4GameText").firstChildElement();

        for (read_text;!read_text.isNull();read_text = read_text.nextSiblingElement())
        {
            QDomElement read_element = read_text.firstChildElement();
            QDomElement collect_node = xml_collect.createElement("TEXT");
            collect_root.appendChild(collect_node);

            for (read_element;!read_element.isNull();read_element = read_element.nextSiblingElement())
            {
                //qDebug() << read_element.tagName();
                if(!read_element.firstChildElement("Text").isNull())
                {
                    QDomElement collect_element = xml_collect.createElement(read_element.tagName());
                    collect_node.appendChild(collect_element);
                    QDomElement collect_element_text = xml_collect.createElement("Text");
                    QDomElement collect_element_gender = xml_collect.createElement("Gender");
                    QDomElement collect_element_plural = xml_collect.createElement("Plural");
                    collect_element.appendChild(collect_element_text);
                    collect_element.appendChild(collect_element_gender);
                    collect_element.appendChild(collect_element_plural);

                    QString collect_text_value = read_element.firstChildElement("Text").firstChild().nodeValue();
                    QString collect_gender_value = read_element.firstChildElement("Gender").firstChild().nodeValue();
                    QString collect_plural_value = read_element.firstChildElement("Plural").firstChild().nodeValue();

                    QDomText collect_text = xml_collect.createTextNode(collect_text_value);
                    QDomText collect_gender = xml_collect.createTextNode(collect_gender_value);
                    QDomText collect_plural = xml_collect.createTextNode(collect_plural_value);
                    collect_element_text.appendChild(collect_text);
                    collect_element_gender.appendChild(collect_gender);
                    collect_element_plural.appendChild(collect_plural);
                }
                else
                {
                    QDomElement collect_element = xml_collect.createElement(read_element.tagName());
                    collect_node.appendChild(collect_element);

                    QString collect_text_value = read_element.firstChild().nodeValue();

                    QDomText collect_text = xml_collect.createTextNode(collect_text_value);
                    collect_element.appendChild(collect_text);
                }
            }
        }
        file_in.close();
    }
    QTextStream ts(&xml_collect_file);
    xml_collect.save(ts, 3);
    xml_collect_file.close();

    // List all tags
    qDebug() << "Listing tags...";
    QStringList duplicate_tags;
    QStringList tags = ListTags(".");
    tags.removeDuplicates();
    int tags_total_counter = tags.count();
    qDebug() << "Detected" << tags_total_counter << "tags";

    // Check categories
    qDebug() << "Checking categories...";
    QStringList categories_list;
    QDomDocument xml_categories;
    categories.open(QIODevice::ReadOnly);
    xml_categories.setContent(&categories);
    categories.close();

    // Initialize all XML classics, pedia and removed
    QDomElement read_categories = xml_categories.firstChildElement().firstChildElement("category");
    for(read_categories;!read_categories.isNull(); read_categories = read_categories.nextSiblingElement())
    {
        QDomDocument xml_temp;
        xml_temp.insertBefore(xml_temp.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_temp.firstChild());
        xml_temp.appendChild(xml_temp.createElement("Civ4GameText"));

        QString filename = read_categories.attribute("file");
        QString filename_pedia = "sorted/new/" + filename + "_PEDIA.xml";
        QString filename_removed = "sorted/new/_REMOVED_TAGS_" + filename + ".xml";
        QString filename_removed_pedia = "sorted/new/_REMOVED_TAGS_" + filename + "_PEDIA.xml";
        categories_list << filename << filename + "_PEDIA";
        filename = "sorted/new/" + filename + ".xml";
        QFile temp(filename);
        temp.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_temp(&temp);
        xml_temp.save(ts_temp,4);
        temp.close();
        QFile temp_pedia(filename_pedia);
        temp_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_pedia(&temp_pedia);
        xml_temp.save(ts_pedia,4);
        temp_pedia.close();
        QFile temp_removed(filename_removed);
        temp_removed.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_removed(&temp_removed);
        xml_temp.save(ts_removed,4);
        temp_removed.close();
        QFile temp_removed_pedia(filename_removed_pedia);
        temp_removed_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_removed_pedia(&temp_removed_pedia);
        xml_temp.save(ts_removed_pedia,4);
        temp_removed_pedia.close();

    }
    // Create MISC classic, pedia, and removed
    QDomDocument xml_misc;
    xml_misc.insertBefore(xml_misc.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_misc.firstChild());
    xml_misc.appendChild(xml_misc.createElement("Civ4GameText"));
    QFile misc("sorted/new/MISC.xml");
    misc.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp(&misc);
    xml_misc.save(ts_temp,4);
    misc.close();
    QFile misc_pedia("sorted/new/MISC_PEDIA.xml");
    misc_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_pedia(&misc_pedia);
    xml_misc.save(ts_temp_pedia,4);
    misc_pedia.close();
    QFile misc_removed("sorted/new/_REMOVED_TAGS_MISC.xml");
    misc_removed.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed(&misc_removed);
    xml_misc.save(ts_temp_removed,4);
    misc_removed.close();
    QFile misc_removed_pedia("sorted/new/_REMOVED_TAGS_MISC_PEDIA.xml");
    misc_removed_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed_pedia(&misc_removed_pedia);
    xml_misc.save(ts_temp_removed_pedia,4);
    misc_removed_pedia.close();
    categories_list << "MISC" << "MISC_PEDIA";
    categories_list.sort();

    // Sorting

    QString tag;
    QString category;
    int tags_counter = 1;
    int category_total_counter = categories_list.count();
    int category_counter = 1;
    QDir import_dir("sorted/");
    QStringList import_files;
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Open input file
    xml_collect_file.open(QIODevice::ReadOnly);
    QDomDocument input;
    input.setContent(&xml_collect_file);

    // For each category, make a list of the concerned tags
    foreach(category,categories_list)
    {
        qDebug() << "Checking category" << category_counter << "of" << category_total_counter << " : " << category;
        QStringList category_tags_list;

        // For each tag in the list, check the category. If it's the same, the add the tag to the list
        foreach (tag,tags)
        {
            QDomElement xml_category = xml_categories.firstChildElement().firstChildElement();
            QString category_value = "MISC";
            for(xml_category;!xml_category.isNull();xml_category = xml_category.nextSiblingElement())
            {
                QDomElement xml_category_tag = xml_category.firstChildElement();
                for(xml_category_tag;!xml_category_tag.isNull();xml_category_tag = xml_category_tag.nextSiblingElement())
                {
                    if(tag.startsWith(xml_category_tag.firstChild().nodeValue()))
                    {
                        category_value = xml_category.attribute("file");
                    }
                }
            }
            if(tag.contains("_PEDIA"))
            {
                category_value = category_value + "_PEDIA";
            }

            if(category == category_value)
            {
                category_tags_list << tag;
            }
        }
        category_tags_list.removeDuplicates();

        // Open the category files
        QFile file_category("sorted/new/" + category + ".xml");
        QFile file_removed("sorted/new/_REMOVED_TAGS_" + category + ".xml");
        QDomDocument xml_detected;
        QDomDocument xml_removed;
        file_category.open(QIODevice::ReadOnly);
        file_removed.open(QIODevice::ReadOnly);
        xml_detected.setContent(&file_category);
        xml_removed.setContent(&file_removed);
        file_category.close();
        file_removed.close();

        // For each tag in the category, populate the xml.
        foreach(tag,category_tags_list)
        {
            int tag_occurence = 0;

            // Check the counter !!!!
            //qDebug() << "Sorting tag" << tags_counter << "of" << tags_total_counter << ":" << tag;

            // Looking for tag in the file
                QDomElement input_text_node = input.firstChildElement().firstChildElement();
                for(input_text_node;!input_text_node.isNull();input_text_node = input_text_node.nextSiblingElement())
                {
                    if(tag == input_text_node.firstChildElement("Tag").firstChild().nodeValue())
                    {
                        // If tag has already been checked, move the tag to removed file
                        if(tag_occurence > 0)
                        {
                            qDebug() << "Duplicate detected : " << tag;
                            QString operation = "TAG:" + tag + " | FILE:" + category + ".xml";
                            duplicate_tags << operation;

                            // Write the node to the correct category
                            QDomElement input_element_node = input_text_node.firstChildElement();
                            QDomElement xml_removed_node = xml_removed.firstChildElement();
                            QDomNode xml_text_node = xml_removed.createElement("Text");
                            xml_removed_node.appendChild(xml_text_node);
                            for(input_element_node;!input_element_node.isNull();input_element_node = input_element_node.nextSiblingElement())
                            {
                                // Check if the node is a plural form
                                if (!input_element_node.firstChildElement("Text").isNull())
                                {
                                    //qDebug() << "Multiple tag detected";
                                    QString element_name = input_element_node.tagName();
                                    QString element_text_name = input_element_node.firstChildElement("Text").tagName();
                                    QString element_gender_name = input_element_node.firstChildElement("Gender").tagName();
                                    QString element_plural_name = input_element_node.firstChildElement("Plural").tagName();

                                    QDomNode node_element_name = xml_removed.createElement(element_name);
                                    QDomElement node_element_text_name = xml_removed.createElement(element_text_name);
                                    QDomElement node_element_gender_name = xml_removed.createElement(element_gender_name);
                                    QDomElement node_element_plural_name = xml_removed.createElement(element_plural_name);

                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(node_element_text_name);
                                    node_element_name.appendChild(node_element_gender_name);
                                    node_element_name.appendChild(node_element_plural_name);

                                    QString element_text_value = input_element_node.firstChildElement("Text").firstChild().nodeValue();
                                    QString element_gender_value = input_element_node.firstChildElement("Gender").firstChild().nodeValue();
                                    QString element_plural_value = input_element_node.firstChildElement("Plural").firstChild().nodeValue();

                                    node_element_text_name.appendChild(xml_removed.createTextNode(element_text_value));
                                    node_element_gender_name.appendChild(xml_removed.createTextNode(element_gender_value));
                                    node_element_plural_name.appendChild(xml_removed.createTextNode(element_plural_value));
                                }
                                else
                                {
                                    QString element_name = input_element_node.tagName();
                                    QString element_value = input_element_node.firstChild().nodeValue();
                                    QDomElement node_element_name = xml_removed.createElement(element_name);
                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(xml_removed.createTextNode(element_value));
                                }
                            }
                        }
                        else
                        {
                            // Write the node to the correct category
                            QDomElement input_element_node = input_text_node.firstChildElement();
                            QDomElement xml_detected_node = xml_detected.firstChildElement();
                            QDomNode xml_text_node = xml_detected.createElement("Text");
                            xml_detected_node.appendChild(xml_text_node);
                            for(input_element_node;!input_element_node.isNull();input_element_node = input_element_node.nextSiblingElement())
                            {
                                // Check if the node is a plural form
                                if (!input_element_node.firstChildElement("Text").isNull())
                                {
                                    //qDebug() << "Multiple tag detected";
                                    QString element_name = input_element_node.tagName();
                                    QString element_text_name = input_element_node.firstChildElement("Text").tagName();
                                    QString element_gender_name = input_element_node.firstChildElement("Gender").tagName();
                                    QString element_plural_name = input_element_node.firstChildElement("Plural").tagName();

                                    QDomNode node_element_name = xml_detected.createElement(element_name);
                                    QDomElement node_element_text_name = xml_detected.createElement(element_text_name);
                                    QDomElement node_element_gender_name = xml_detected.createElement(element_gender_name);
                                    QDomElement node_element_plural_name = xml_detected.createElement(element_plural_name);

                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(node_element_text_name);
                                    node_element_name.appendChild(node_element_gender_name);
                                    node_element_name.appendChild(node_element_plural_name);

                                    QString element_text_value = input_element_node.firstChildElement("Text").firstChild().nodeValue();
                                    QString element_gender_value = input_element_node.firstChildElement("Gender").firstChild().nodeValue();
                                    QString element_plural_value = input_element_node.firstChildElement("Plural").firstChild().nodeValue();

                                    node_element_text_name.appendChild(xml_detected.createTextNode(element_text_value));
                                    node_element_gender_name.appendChild(xml_detected.createTextNode(element_gender_value));
                                    node_element_plural_name.appendChild(xml_detected.createTextNode(element_plural_value));
                                }
                                else
                                {
                                    QString element_name = input_element_node.tagName();
                                    QString element_value = input_element_node.firstChild().nodeValue();
                                    QDomElement node_element_name = xml_detected.createElement(element_name);
                                    xml_text_node.appendChild(node_element_name);
                                    node_element_name.appendChild(xml_detected.createTextNode(element_value));
                                }
                            }
                        }
                        tag_occurence++;
                    }
                }
            tags_counter++;
        }

        // Save both xml
        QFile file_detected_write("sorted/new/_TEMPWRITE_.xml");
        QFile file_removed_write("sorted/new/_TEMPREMOVED_.xml");
        QFile::remove("sorted/new/_TEMPWRITE_.xml");
        QFile::remove("sorted/new/_TEMPREMOVED_.xml");
        file_detected_write.open(QIODevice::WriteOnly);
        file_removed_write.open(QIODevice::WriteOnly);
        QTextStream ts_detected(&file_detected_write);
        QTextStream ts_removed(&file_removed_write);
        xml_detected.save(ts_detected,4);
        xml_removed.save(ts_removed,4);
        file_detected_write.close();
        file_removed_write.close();
        file_category.remove();
        file_removed.remove();
        file_detected_write.rename("sorted/new/" + category + ".xml");
        file_removed_write.rename("sorted/new/_REMOVED_TAGS_" + category + ".xml");
        category_counter++;
    }

    // Clean up folder and encoding back
    qDebug() << "Clean up and preparing files";
    import_files = import_dir.entryList(xml_filter, QDir::Files);
    xml_collect_file.close();
    QFile::remove("sorted/TEMP");
    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        QString file_remove = "sorted/" + *it;
        QFile::remove(file_remove);
    }
    QDir dir_sorted("sorted/new/");
    QStringList dir_sorted_list;
    dir_sorted_list = dir_sorted.entryList(xml_filter, QDir::Files);
    for(QStringList::Iterator it = dir_sorted_list.begin(); it != dir_sorted_list.end(); it++)
    {
        QString old_file = "sorted/new/" + *it;
        QString new_file = "sorted/" + *it;
        QFile::rename(old_file,new_file);
        QDomDocument doc_tester;
        QFile file_tester(new_file);
        file_tester.open(QIODevice::ReadOnly);
        doc_tester.setContent(&file_tester);
        file_tester.close();
        if (doc_tester.firstChildElement("Civ4GameText").firstChildElement().isNull())
        {
            QFile::remove(new_file);
        }
        else
        {
            ConvertUTF8ToCiv4(new_file);
        }
    }
    dir_sorted.setPath("sorted/");
    dir_sorted.rmdir("new/");

    // Print duplicate list
    qDebug() << "Creating duplicates list";
    QString print_value;
    QFile print_file("sorted/_duplicates_list.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, duplicate_tags)
            {
                stream << print_value << endl;
            }
        }
    print_file.close();

    QStringList tags_output = ListTags("sorted/");
    tags_output.removeDuplicates();
    int tags_output_counter = tags_output.count();

    qDebug() << "Original tag number:" << tags_total_counter << "| New tag number:" << tags_output_counter;*/
}

