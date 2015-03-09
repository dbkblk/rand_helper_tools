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

int getUnusedTagsList(QString listName)
{
    // First extract informations from the config file
    QFile settings("cleaner.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString dirText = xml.firstChildElement("main").firstChildElement("directories").attribute("text");
    QString dirTo = xml.firstChildElement("main").firstChildElement("directories").attribute("to");
    QString dirBTS = xml.firstChildElement("main").firstChildElement("directories").attribute("BTS");
    QString dirSources = xml.firstChildElement("main").firstChildElement("directories").attribute("sources");
    QString dirMod = xml.firstChildElement("main").firstChildElement("directories").attribute("mod");
    if(dirText.isEmpty() || dirTo.isEmpty() || dirBTS.isEmpty() || dirSources.isEmpty() || dirMod.isEmpty())
    {
        qDebug() << "One of the config directory is empty, cannot continue.";
        return 1;
    }
    else{
        // Make sure the path is normalized
        if(!dirText.endsWith("/")){
            dirText.append("/");
        }
        if(!dirTo.endsWith("/")){
            dirTo.append("/");
        }
        if(!dirBTS.endsWith("/")){
            dirBTS.append("/");
        }
        if(!dirSources.endsWith("/")){
            dirSources.append("/");
        }
        if(!dirMod.endsWith("/")){
            dirMod.append("/");
        }
        dirText.replace("\\","/");
        dirTo.replace("\\","/");
        dirBTS.replace("\\","/");
        dirSources.replace("\\","/");
        dirMod.replace("\\","/");
        qDebug() << "Importing files";
    }
    QDir to(dirTo);
    to.removeRecursively();

    // List tags in the mod text files
    qDebug() << "Listing tags...";
    QStringList tagsList = ListTags(dirText);
    QString tag;
    QString base_tag;
    tagsList.removeDuplicates();

    // List base game tags
    QString dirTextOrig = dirBTS + "Assets/XML/Text/";
    QString dirTextBTS = dirBTS + "Beyond the Sword/Assets/XML/Text/";
    QString dirTextWarlords = dirBTS + "Warlords/Assets/XML/Text/";
    QStringList listTagsOrig = ListTags(dirTextOrig);
    QStringList listTagsWarlords = ListTags(dirTextWarlords);
    QStringList listTagsBTS = ListTags(dirTextBTS);

    // Remove exceptions from the tag list
    foreach(tag, tagsList)
    {
        //qDebug() << tag;
        QDomElement exceptions = xml.firstChildElement().firstChildElement("exceptions").firstChildElement();
        for(exceptions;!exceptions.isNull();exceptions = exceptions.nextSiblingElement())
        {
            QString value = exceptions.firstChild().nodeValue();
            if(tag.startsWith(value))
            {
              tagsList.removeAll(tag);
            }
        }
    }

    // Remove base game tags from the tag list
    foreach(tag,tagsList)
    {
      foreach(base_tag,listTagsOrig)
      {
        if (tag == base_tag)
        {
        tagsList.removeAll(tag);
        }
      }
      foreach(base_tag,listTagsWarlords)
      {
        if (tag == base_tag)
        {
          tagsList.removeAll(tag);
        }
      }
      foreach(base_tag,listTagsBTS)
      {
        if (tag == base_tag)
        {
          tagsList.removeAll(tag);
        }
      }
    }
    qDebug() << "Found" << tagsList.count() << "unique tags";

    // List all the files to check
    qDebug() << "Collecting files...";
    QStringList list_ext;
    list_ext << "*.xml" << "*.py" << "*.CivBeyondSwordWBSave" << "*.Civ4WorldBuilderSave" << "*.CivWarlordsWBSave";
    QStringList list_cpp;
    list_cpp << "*.cpp" << "*.h";

    // List files in mod folder
    QStringList exclusion_list;
    exclusion_list << "Assets/XML/Text/";

    // Iterate into all the files into the game and source folder
    QDirIterator mod_iterator(QDir(dirMod).path(), list_ext, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator game_iterator(QDir(dirBTS).path(), list_ext, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator source_iterator(QDir(dirSources).path(), list_cpp, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    int file_counter = 0;
    while (mod_iterator.hasNext())
    {
      // Check if it isn't in the text folder
      if(mod_iterator.filePath().contains("Assets/XML/Text") || mod_iterator.filePath() == "")
      {
        mod_iterator.next();
      }
      else
      {
        file_counter++;
        QFile file(mod_iterator.filePath());
        file.open(QIODevice::ReadOnly);
        QTextStream temp(&file);
        while(!temp.atEnd())
        {
          QString line = temp.readLine();

          // Check all the tag list and remove the ones who are found
          foreach (QString tag, tagsList)
          {
            if(line.contains(tag))
            {
                tagsList.removeAll(tag);
                qDebug() << "Remaining tags" << tagsList.count();
            }
          }
        }
        file.close();
        mod_iterator.next();
      }
    }
    while (game_iterator.hasNext())
    {
      // Check if it isn't in the text folder
      if(game_iterator.filePath().contains("Assets/XML/Text") || game_iterator.filePath().contains("Mods/") || game_iterator.filePath() == "")
      {
        game_iterator.next();
      }
      else
      {
        file_counter++;
        QFile file(game_iterator.filePath());
        file.open(QIODevice::ReadOnly);
        QTextStream temp(&file);
        while(!temp.atEnd())
        {
          QString line = temp.readLine();

          // Check all the tag list and remove the ones who are found
          foreach (QString tag, tagsList)
          {
            if(line.contains(tag))
            {
              tagsList.removeAll(tag);
              qDebug() << "Remaining tags" << tagsList.count();
            }
          }
        }
        file.close();
        game_iterator.next();
      }
    }
    while (source_iterator.hasNext())
    {
      if(source_iterator.filePath() == "")
      {
        source_iterator.next();
      }
      else
      {
        file_counter++;
        QFile file(source_iterator.filePath());
        file.open(QIODevice::ReadOnly);
        QTextStream temp(&file);
        while(!temp.atEnd())
        {
          QString line = temp.readLine();

          // Check all the tag list and remove the ones who are found
          foreach (QString tag, tagsList)
          {
            if(line.contains(tag))
            {
              tagsList.removeAll(tag);
              qDebug() << "Remaining tags" << tagsList.count();
            }
          }
        }
        file.close();
        source_iterator.next();
      }
    }

    // Print not found list
    QString print_value;
    QFile print_file(listName);
    if ( print_file.open(QIODevice::ReadWrite) )
    {
      QTextStream stream( &print_file );
      foreach(print_value, tagsList)
      {
        stream << print_value << endl;
      }
    }
    print_file.close();

    qDebug() << "The cleaner has finished to look for unused tags.";
    return 0;
}

void removeUnusedTags(QString textFilesDir, QString outputDir, QStringList tagsList)
{
    // Prepare folder
    qDebug() << "Preparing files...";
    QDir dir_import(outputDir);
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // List root files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(textFilesDir);
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Copy and convert files to backup dir
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(textFilesDir + *it, outputDir + *it);
    }
    import_files = dir_import.entryList(xml_filter, QDir::Files);

    QString tags;
    int counter = 0;
    int file_number = 1;
    QStringList operation_list;
    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        qDebug() << "Processing file" << file_number << "of" << import_files.count();
        QString current = outputDir + *it;
        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomElement input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        int remove_file = 0;
        int remove_tag = 0;
        while(!input_node.isNull())
        {
            QString tag_value = input_node.firstChildElement("Tag").firstChild().nodeValue();
            //qDebug() << tag_value;
            remove_tag = 0;
            foreach(tags,tagsList)
            {
                if(tag_value == tags)
                {
                    tagsList.removeAll(tags);
                    QString operation = "FILE: " + *it + " | REMOVED TAG: " + tags;
                    operation_list << operation;
                    counter++;
                    remove_file++;
                    remove_tag++;
                }
            }
            input_node = input_node.nextSiblingElement();
            if(remove_tag == 1)
            {
                input.firstChildElement("Civ4GameText").removeChild(input_node.previousSibling());
            }
        }
        if(remove_tag == 1)
        {
            input.firstChildElement("Civ4GameText").removeChild(input.firstChildElement("Civ4GameText").lastChildElement());
        }

        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        QTextStream ts(&file_input);
        input.save(ts, 4);
        file_input.close();

        if(remove_file == 0)
        {
            QFile::remove(current);
        }
        file_number++;
    }

    // Output modified list
    QString print_value;
    QString print_file_name = outputDir + "report_cleanup.txt";
    QFile print_file(print_file_name);

    if ( print_file.open(QIODevice::Truncate | QIODevice::WriteOnly) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, operation_list)
            {
                stream << print_value << endl;
            }
        }
        print_file.close();

    qDebug() << "All modified files are in the output folder. Removed" << counter << "tags. A detailled report has been print in report_cleanup.txt";
    return;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // First extract informations from the config file
    QFile settings("cleaner.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString dirText = xml.firstChildElement("main").firstChildElement("directories").attribute("text");
    QString dirTo = xml.firstChildElement("main").firstChildElement("directories").attribute("to");
    QString dirBTS = xml.firstChildElement("main").firstChildElement("directories").attribute("BTS");
    QString dirSources = xml.firstChildElement("main").firstChildElement("directories").attribute("sources");
    QString dirMod = xml.firstChildElement("main").firstChildElement("directories").attribute("mod");
    QString tagsList = xml.firstChildElement("main").firstChildElement("list").firstChild().nodeValue();
    if(dirText.isEmpty() || dirTo.isEmpty() || dirBTS.isEmpty() || dirSources.isEmpty() || dirMod.isEmpty())
    {
        qDebug() << "One of the config directory is empty, cannot continue.";
        return 1;
    }
    else{
        // Make sure the path is normalized
        if(!dirText.endsWith("/")){
            dirText.append("/");
        }
        if(!dirTo.endsWith("/")){
            dirTo.append("/");
        }
        if(!dirBTS.endsWith("/")){
            dirBTS.append("/");
        }
        if(!dirSources.endsWith("/")){
            dirSources.append("/");
        }
        if(!dirMod.endsWith("/")){
            dirMod.append("/");
        }
        dirText.replace("\\","/");
        dirTo.replace("\\","/");
        dirBTS.replace("\\","/");
        dirSources.replace("\\","/");
        dirMod.replace("\\","/");
        qDebug() << "Importing files";
    }
    QDir to(dirTo);
    to.removeRecursively();

    // List tags in the tags list
    QStringList list_tags;
    if(tagsList.isEmpty())
    {
       qDebug() << "No tag list found. The cleaner will look for unused tags now. It can take a while to process.";
       list_tags = getUnusedTagsList();
    }
    else
    {
        // List tags in the file
        QFile test(tagsList);

        test.open(QIODevice::ReadOnly);
        QTextStream read_stream(&test);
        while (!read_stream.atEnd())
        {
            list_tags << read_stream.readLine();
        }
        test.close();

        removeUnusedTags(dirText, dirTo, list_tags);
    }



    return a.exec();
}
