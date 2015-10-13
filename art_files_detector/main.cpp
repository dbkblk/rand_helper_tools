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

int getUnusedFilesList(QStringList fileList)
{
    // First extract informations from the config file
    QFile settings("art_files_detector.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString dirArt = xml.firstChildElement("main").firstChildElement("directories").attribute("art");
    QString dirBTS = xml.firstChildElement("main").firstChildElement("directories").attribute("BTS");
    QString dirSources = xml.firstChildElement("main").firstChildElement("directories").attribute("sources");
    QString dirMod = xml.firstChildElement("main").firstChildElement("directories").attribute("mod");
    if(dirBTS.isEmpty() || dirSources.isEmpty() || dirMod.isEmpty())
    {
        qDebug() << "One of the config directory is empty, cannot continue.";
        return 1;
    }
    else{
        // Make sure the path is normalized
        if(!dirArt.endsWith("/")){
            dirArt.append("/");
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
        dirArt.replace("\\","/");
        dirBTS.replace("\\","/");
        dirSources.replace("\\","/");
        dirMod.replace("\\","/");
        qDebug() << "Importing files";
    }

    // List all the files to check
    qDebug() << "List lenght is" << fileList.count();
    QStringList list_ext;
    list_ext << "*.xml" << "*.py" << "*.CivBeyondSwordWBSave" << "*.Civ4WorldBuilderSave" << "*.CivWarlordsWBSave";
    QStringList list_cpp;
    list_cpp << "*.cpp" << "*.h";
    QStringList list_art;
    list_art << "*.nif";

    // Iterate into all the files into the game and source folder
    QDirIterator art_iterator(QDir(dirArt).path(), list_art, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator mod_iterator(QDir(dirMod).path(), list_ext, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator game_iterator(QDir(dirBTS).path(), list_ext, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QDirIterator source_iterator(QDir(dirSources).path(), list_cpp, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    int file_counter = 0;
    while (art_iterator.hasNext())
    {
        qDebug() << file_counter;
      // Check if it isn't in the text folder
      if(art_iterator.filePath().contains("Assets/XML/Text") || art_iterator.filePath() == "")
      {
        art_iterator.next();
      }
      else
      {
        file_counter++;
        QFile file(art_iterator.filePath());
        QString temp_string;
        QDataStream dataStreamReader(&file);
        file.open(QIODevice::ReadOnly);
        dataStreamReader >> temp_string;
        // Check all the tag list and remove the ones who are found
        foreach (QString tag, fileList)
        {
            if(temp_string.contains(tag, Qt::CaseInsensitive))
            {
                fileList.removeAll(tag);
                qDebug() << "Remaining files" << fileList.count();
            }
        }
        file.close();
        art_iterator.next();
      }
    }
    while (mod_iterator.hasNext())
    {
        qDebug() << file_counter;
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
          foreach (QString tag, fileList)
          {
            if(line.contains(tag, Qt::CaseInsensitive))
            {
                fileList.removeAll(tag);
                qDebug() << "Remaining files" << fileList.count();
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
          foreach (QString tag, fileList)
          {
            if(line.contains(tag, Qt::CaseInsensitive))
            {
              fileList.removeAll(tag);
              qDebug() << "Remaining files" << fileList.count();
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
          foreach (QString tag, fileList)
          {
            if(line.contains(tag, Qt::CaseInsensitive))
            {
              fileList.removeAll(tag);
              qDebug() << "Remaining files" << fileList.count();
            }
          }
        }
        file.close();
        source_iterator.next();
      }
    }

    // Print not found list
    QString print_value;
    QFile print_file("art_filelist.txt");
    if ( print_file.open(QIODevice::ReadWrite) )
    {
      QTextStream stream( &print_file );
      foreach(print_value, fileList)
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
    QFile settings("art_files_detector.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QString dirArt = xml.firstChildElement("main").firstChildElement("directories").attribute("art");
    QString dirBTS = xml.firstChildElement("main").firstChildElement("directories").attribute("BTS");
    QString dirSources = xml.firstChildElement("main").firstChildElement("directories").attribute("sources");
    QString dirMod = xml.firstChildElement("main").firstChildElement("directories").attribute("mod");
    if(dirBTS.isEmpty() || dirSources.isEmpty() || dirMod.isEmpty())
    {
        qDebug() << "One of the config directory is empty, cannot continue.";
        return 1;
    }
    else{
        // Make sure the path is normalized
        if(!dirArt.endsWith("/")){
            dirArt.append("/");
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
        dirArt.replace("\\","/");
        dirBTS.replace("\\","/");
        dirSources.replace("\\","/");
        dirMod.replace("\\","/");
        qDebug() << "Importing files";
    }
    // Prepare the XML file
    QFile file("art_files_list.xml");
    file.remove();
    QDomDocument xml_out;
    QDomNode declaration = xml_out.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    xml_out.insertBefore(declaration,xml_out.firstChild());
    QDomNode root = xml_out.createElement("files");
    xml_out.appendChild(root);

    // Liste tous les fichiers
    QStringList list;
    QDirIterator it(dirArt, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        // Add the file to the list
        QString artPath = QString(it.next()).replace(dirArt, "");
        if (artPath.endsWith("dds") || artPath.endsWith("nif") || artPath.endsWith("tga")){
            list.append(artPath);
        }
//        qDebug() << artPath;
    }

    getUnusedFilesList(list);


    // List tags in the tags list
//    QStringList list_tags;
//    if(tagsList.isEmpty())
//    {
//       qDebug() << "No tag list found. The cleaner will look for unused tags now. It can take a while to process.";
//       getUnusedTagsList("_tags_not_found.txt");
//    }
//    else
//    {
//        // List tags in the file
//        QFile test(tagsList);

//        test.open(QIODevice::ReadOnly);
//        QTextStream read_stream(&test);
//        while (!read_stream.atEnd())
//        {
//            list_tags << read_stream.readLine();
//        }
//        test.close();

//        removeUnusedTags(dirText, dirTo, list_tags);
//    }



    return a.exec();
}
