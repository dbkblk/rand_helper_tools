#include <QtCore>
#include <QtXml>

QStringList ListTags(QString dir)
{
    QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    QDir root(dir);
    files = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QFile file_in(dir + *it);
        file_in.open(QIODevice::ReadOnly);
        QDomDocument xml;
        xml.setContent(&file_in);
        // Be case unsensitive
        QDomElement xml_tag;
        if (xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").isNull())
        {
            xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("Text").toElement();
        }
        else
        {
            xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        }
        if (xml_tag.isNull())
        {
            qDebug() << *it << " is not properly formatted";
            file_in.close();
        }

        for(xml_tag;!xml_tag.isNull();xml_tag = xml_tag.nextSiblingElement())
        {
            tags << xml_tag.firstChildElement("Tag").text();
        }
        file_in.close();
    }

    // Sort unsensitive
    QMap<QString, QString> strMap;
    foreach ( QString str, tags ) {
        strMap.insert( str.toLower(), str );
    }
    tags = strMap.values();

    return tags;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // Announcement
    qDebug() << "Civ. IV XML merger v1.1; \"A New Dawn Resurection, dbkblk\"";

    // First extract informations from the config file
    QFile settings("merger.config");
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
        // Make sure the path is normalized
        if(!dirFrom.endsWith("/")){
            dirFrom.append("/");
        }
        if(!dirTo.endsWith("/")){
            dirTo.append("/");
        }
        dirFrom.replace("\\","/");
        dirTo.replace("\\","/");
        qDebug() << "Merging files";
    }

    /* Sorting logic */
    /* Check parse file presence
     * List all tags in a QStringList
     * Initialize all XML files by category plus MISC. Add a _PEDIA and _REMOVED_ for all categories
     * For each tag (until the end of the list):
        * Check category
        * Open category file
        * Check all files looking for the tag
        * Check if the tag has already been found
        * Add the tag to the category file
        * Save category file
    * Print the duplicate list */

    // Clean the work folder before to begin
    QDir to(dirTo);
    to.removeRecursively();
    to.mkdir(".");
    to.mkpath("./temp/");

    // Listing root files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(dirFrom);
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Exclude MegaCivPack files
    foreach(QString entry, root_files)
    {
        if(entry.startsWith("MCP_"))
        {
            root_files.removeAll(entry);
        }
    }

    // Collect all tags in a bigfile
    qDebug() << "Collecting all tags to a file";
    QDomDocument xml_collect;
    QFile xml_collect_file(dirTo + "TEMPFILE");
    xml_collect_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    // Create the output file
    QDomNode declaration = xml_collect.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    xml_collect.insertBefore(declaration,xml_collect.firstChild());
    QDomNode collect_root = xml_collect.createElement("Civ4GameText");
    xml_collect.appendChild(collect_root);

   // Loop the input file
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QString current = dirFrom + *it;
        QDomDocument read;
        QFile file_in(current);
        file_in.open(QIODevice::ReadOnly);
        read.setContent(&file_in);
        file_in.close();
        QDomNode read_text = read.firstChildElement("Civ4GameText").firstChildElement();

        for (;!read_text.isNull();read_text = read_text.nextSiblingElement())
        {
            QDomElement read_element = read_text.firstChildElement();
            QDomElement collect_node = xml_collect.createElement("TEXT");
            collect_root.appendChild(collect_node);

            for (;!read_element.isNull();read_element = read_element.nextSiblingElement())
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
    }
    QTextStream ts(&xml_collect_file);
    xml_collect.save(ts, 3);
    xml_collect_file.close();

    // List all tags
    qDebug() << "Listing tags...";
    QStringList duplicate_tags;
    QStringList tags = ListTags(dirFrom);
    tags.removeDuplicates();
    int tags_total_counter = tags.count();
    qDebug() << "Detected" << tags_total_counter << "different tags in input files";

    // Check categories
    qDebug() << "Checking categories...";
    QStringList categories_list;

    // Initialize all XML classics, pedia and removed
    QDomElement read_categories = xml.firstChildElement("main").firstChildElement("sorting").firstChildElement("category");
    for(;!read_categories.isNull(); read_categories = read_categories.nextSiblingElement())
    {
        QDomDocument xml_temp;
        xml_temp.insertBefore(xml_temp.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_temp.firstChild());
        xml_temp.appendChild(xml_temp.createElement("Civ4GameText"));

        QString filename = read_categories.attribute("file");
        QString filename_pedia = dirTo + "temp/" + filename + "_PEDIA.xml";
        QString filename_strategy = dirTo + "temp/" + filename + "_STRATEGY.xml";
        QString filename_removed = dirTo + "temp/_REMOVED_TAGS_" + filename + ".xml";
        QString filename_removed_pedia = dirTo + "temp/_REMOVED_TAGS_" + filename + "_PEDIA.xml";
        QString filename_removed_strategy = dirTo + "temp/_REMOVED_TAGS_" + filename + "_STRATEGY.xml";
        categories_list << filename << filename + "_PEDIA" << filename + "_STRATEGY";
        filename = dirTo + "temp/" + filename + ".xml";
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
        QFile temp_strategy(filename_strategy);
        temp_strategy.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_strategy(&temp_strategy);
        xml_temp.save(ts_strategy,4);
        temp_strategy.close();
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
        QFile temp_removed_strategy(filename_removed_strategy);
        temp_removed_strategy.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream ts_removed_strategy(&temp_removed_strategy);
        xml_temp.save(ts_removed_strategy,4);
        temp_removed_strategy.close();

    }
    // Create MISC classic, pedia, and removed
    QDomDocument xml_misc;
    xml_misc.insertBefore(xml_misc.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"utf-8\"")),xml_misc.firstChild());
    xml_misc.appendChild(xml_misc.createElement("Civ4GameText"));
    QFile misc(dirTo + "temp/MISC.xml");
    misc.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp(&misc);
    xml_misc.save(ts_temp,4);
    misc.close();
    QFile misc_pedia(dirTo + "temp/MISC_PEDIA.xml");
    misc_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_pedia(&misc_pedia);
    xml_misc.save(ts_temp_pedia,4);
    misc_pedia.close();
    QFile misc_strategy(dirTo + "temp/MISC_STRATEGY.xml");
    misc_strategy.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_strategy(&misc_strategy);
    xml_misc.save(ts_temp_strategy,4);
    misc_strategy.close();
    QFile misc_removed(dirTo + "temp/_REMOVED_TAGS_MISC.xml");
    misc_removed.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed(&misc_removed);
    xml_misc.save(ts_temp_removed,4);
    misc_removed.close();
    QFile misc_removed_pedia(dirTo + "temp/_REMOVED_TAGS_MISC_PEDIA.xml");
    misc_removed_pedia.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed_pedia(&misc_removed_pedia);
    xml_misc.save(ts_temp_removed_pedia,4);
    misc_removed_pedia.close();
    QFile misc_removed_strategy(dirTo + "temp/_REMOVED_TAGS_MISC_STRATEGY.xml");
    misc_removed_strategy.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream ts_temp_removed_strategy(&misc_removed_strategy);
    xml_misc.save(ts_temp_removed_strategy,4);
    misc_removed_strategy.close();
    categories_list << "MISC" << "MISC_PEDIA" << "MISC_STRATEGY";
    categories_list.sort();

    // Sorting

    QString tag;
    QString category;
    int tags_counter = 1;
    int category_total_counter = categories_list.count();
    int category_counter = 1;



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
            QDomElement xml_category = xml.firstChildElement("main").firstChildElement("sorting").firstChildElement("category");
            QString category_value = "MISC";
            for(;!xml_category.isNull();xml_category = xml_category.nextSiblingElement())
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
            else if(tag.endsWith("_STRATEGY") && (category_value.contains("UNITS") || category_value.contains("TECHNOLOGIES") || category_value.contains("BUILDINGS") || category_value.contains("PROJECTS")))
            {
                category_value = category_value + "_STRATEGY";
            }

            if(category == category_value)
            {
                category_tags_list << tag;
            }
        }
        category_tags_list.removeDuplicates();

        // Open the category files
        QFile file_category(dirTo + "temp/" + category + ".xml");
        QFile file_removed(dirTo + "temp/_REMOVED_TAGS_" + category + ".xml");
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
                            QDomNode xml_text_node = xml_removed.createElement("TEXT");
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
                            QDomNode xml_text_node = xml_detected.createElement("TEXT");
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
        QFile file_detected_write(dirTo + "temp/_TEMPWRITE_.xml");
        QFile file_removed_write(dirTo + "temp/_TEMPREMOVED_.xml");
        QFile::remove(dirTo + "temp/_TEMPWRITE_.xml");
        QFile::remove(dirTo + "temp/_TEMPREMOVED_.xml");
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
        file_detected_write.rename(dirTo + "temp/" + category + ".xml");
        file_removed_write.rename(dirTo + "temp/_REMOVED_TAGS_" + category + ".xml");
        category_counter++;
    }

    // Clean up folder and encoding back
    qDebug() << "Clean up and preparing files";
    xml_collect_file.close();
    QFile::remove(dirTo + "TEMPFILE");
    QDir dir_sorted(dirTo + "temp/");
    QStringList dir_sorted_list;
    dir_sorted_list = dir_sorted.entryList(xml_filter, QDir::Files);
    for(QStringList::Iterator it = dir_sorted_list.begin(); it != dir_sorted_list.end(); it++)
    {
        QString old_file = dirTo + "temp/" + *it;
        QString new_file = dirTo + *it;
        QFile::rename(old_file,new_file);
        QDomDocument doc_tester;
        QFile file_tester(new_file);
        file_tester.open(QIODevice::ReadOnly);
        doc_tester.setContent(&file_tester);
        file_tester.close();
        // Remove malformed or empty files
        if (doc_tester.firstChildElement("Civ4GameText").firstChildElement().isNull())
        {
            QFile::remove(new_file);
        }
    }
    dir_sorted.rmdir(".");

    // Print duplicate list
    qDebug() << "Creating duplicates list";
    QString print_value;
    QFile print_file(dirTo + "_duplicates_list.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, duplicate_tags)
            {
                stream << print_value << endl;
            }
        }
    print_file.close();

    QStringList tags_output = ListTags(dirTo);
    tags_output.removeDuplicates();
    int tags_output_counter = tags_output.count();

    qDebug() << "Original tag number:" << tags_total_counter << "| New tag number:" << tags_output_counter;

    return a.exec();
}
