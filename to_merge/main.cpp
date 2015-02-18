#include <iostream>
#include <QtCore>
#include <QtWidgets>
#include <QtXml>
#include <QDebug>
#include "w_main.h"

namespace constants {
const QString VERSION = "2.0.0";
}

class languages
{
public:
    void ParseDocument(QString input_file, QString int_lang);
    void ParseAllFiles(QString language);
    void ImportAllDocuments(bool all_files);
    void ImportDocumentToAll(QString int_lang, QStringList file_list);
    void ImportDocumentToSameName(QString int_lang, QStringList file_list);
    QStringList ListTags(QString dir);
    void CleanFiles();
    void SortCategories();
    void SortCategoriesExperimental();
    QStringList FindUnusedTags();
    QString ConvertLatin1ToCharset(QString language, QString string);
    QString ConvertCharsetToLatin1(QString language, QString string);
    void SorterHelper(QString prefix);
    QString ConvertStringToCiv4(QString string);
    void ConvertCiv4ToUTF8(QString file);
    void ConvertUTF8ToCiv4(QString file);
    QStringList CheckImportLanguages();
    void RemoveLanguage();
    QStringList ListLanguages(QString dir);
    QString AutomaticLanguageDetection(QString dir);
    void RemoveUnusedTags();
    QString test();

private:
    QString language;
    const char* lang_file;
};

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w_main w;
    w.show();

    return a.exec();
}
/*
QStringList languages::ListTags(QString dir)
{
    QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    QDir root(dir);
    files = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QFile file_in(root.path()+"/"+*it);
        //qDebug() << root.path()+"/"+*it;
        file_in.open(QIODevice::ReadOnly);
        QDomDocument xml;
        xml.setContent(&file_in);
        QDomElement xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        if(xml_tag.isNull())
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

QStringList languages::ListLanguages(QString dir)
{
    /*QStringList tags;
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList files;
    QDir root(dir);
    files = root.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = files.begin(); it != files.end(); it++)
    {
        QFile file_in(*it);
        file_in.open(QIODevice::ReadOnly);
        QDomDocument xml;
        xml.setContent(&file_in);
        QDomElement xml_tag = xml.firstChildElement("Civ4GameText").firstChildElement("TEXT").toElement();
        if(xml_tag.isNull())
        {
            qDebug() << *it << " is not properly formatted";
            file_in.close();
        }

        for(xml_tag;!xml_tag.isNull();xml_tag = xml_tag.nextSiblingElement())
        {
            QDomElement xml_language = xml_tag.firstChildElement();
            for(xml_language;!xml_language.isNull();xml_language = xml_language.nextSiblingElement())
            {
                QString tag;
                tag = xml_language.tagName();
                if(tag == "English" || tag == "Tag" || tag == "Text" || tag == "Gender" || tag == "Plural") {}
                else {
                    //qDebug() << xml_language.tagName();
                    tags << tag;
                }
            }

        }
        file_in.close();
    }

    tags.sort();
    tags.removeDuplicates();

    return tags;
}

void languages::CleanFiles()
{
    qDebug() << "Preparing files...";
    QDir dir_import("cleaned/");
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // Search the tag trough existing files and replace with the new value if found
    QStringList xml_filter;
    xml_filter << "*.xml";

    // Root files
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Import files
    QDir import_dir("cleaned/");
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"cleaned/"+*it);
        ConvertCiv4ToUTF8("cleaned/"+*it);
    }
    import_files = import_dir.entryList(xml_filter, QDir::Files);

    // Print list

    QStringList print_list;

    // Loop through all files

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        int s = 0;

        QString current = "cleaned/" + *it;

        qDebug() << "Cleaning " << current;

        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomNode input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");

        // Check file integrity
        if (input.firstChildElement("Civ4GameText").firstChildElement("TEXT").isNull())
        {
            qDebug() << current << " is not properly formatted";
            QFile::remove(current);
        }
        else
        {
            // Check english tag
            while(!input_node.isNull())
            {
                QString english_tag;
                // Check plural form english
                if(!input_node.firstChildElement("English").firstChildElement("Text").isNull())
                {
                    english_tag = input_node.firstChildElement("English").firstChildElement("Text").firstChild().nodeValue();
                    //qDebug() << "Found special node" << english_tag;
                }
                else
                {
                    english_tag = input_node.firstChildElement("English").firstChild().nodeValue();
                }

                // Compare all elements to english
                QDomElement element = input_node.firstChildElement();
                QString list_element;
                QStringList list_removal;
                while(!element.isNull())
                {
                    // Case: Language subtag with value equal to english
                    if(!element.firstChildElement("Text").isNull() && element.tagName() != "English" && english_tag == element.firstChildElement("Text").firstChild().nodeValue())
                    {
                        // Do nothing
                    }
                    // Case: Normal tag with same value as english
                    else if(element.firstChild().nodeValue() == english_tag && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (identical)";
                        print_list << operation;
                        s++;
                        list_removal << element.tagName();
                    }
                    // Case: Language tag is normal and empty
                    else if (element.firstChildElement("Text").isNull() && element.firstChild().nodeValue().isEmpty() && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (empty)";
                        print_list << operation;
                        s++;
                        list_removal << element.tagName();
                    }
                    // Case: Language tag is subtag and empty
                    else if (!element.firstChildElement("Text").isNull() && element.firstChildElement("Text").firstChild().nodeValue().isEmpty() && element.tagName() != "English")
                    {
                        QString operation = "FILE: " + *it + " - TAG: " + input_node.firstChildElement("Tag").firstChild().nodeValue() + " - Removing " + element.tagName() + " (empty subtag)";
                        print_list << operation;
                        s++;
                        list_removal << element.tagName();
                    }
                    element = element.nextSiblingElement();
                }

            foreach (list_element, list_removal)
            {
                QDomNode to_remove = input_node.firstChildElement(list_element);
                //qDebug() << input_node.firstChildElement(list_element).tagName();
                to_remove.parentNode().removeChild(to_remove);
            }

            input_node = input_node.nextSibling();

            }
        }

        // Save to file
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
        file_input.close();

        if(s == 0)
        {
            QFile::remove(current);
        }
        else
        {
            ConvertUTF8ToCiv4(current);
        }
    }

    // Output modified list
    QString print_value;
    QFile print_file("cleaned/_cleaned_values.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, print_list)
            {
                stream << print_value << endl;
            }
        }
        print_file.close();

        qDebug() << "\n\nFiles successfully processed. Modified files are in 'cleaned/'.\nA report of the modified values have been generated in 'cleaned/_cleaned_values.txt'";
}

void languages::RemoveLanguage()
{
    QString lang_remove = AutomaticLanguageDetection(".");
    if (lang_remove == "error"){return;}

    // Prepare folder
    qDebug() << "Preparing files...";
    QDir dir_import("backup/");
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // List root files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Copy and convert files to backup dir
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"backup/"+*it);
        ConvertCiv4ToUTF8("backup/"+*it);
    }
    import_files = dir_import.entryList(xml_filter, QDir::Files);

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        QString current = "backup/" + *it;
        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomNode input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");

        int counter = 0;
        while(!input_node.isNull())
        {
            if(!input_node.firstChildElement(lang_remove).isNull())
            {
                input_node.removeChild(input_node.firstChildElement(lang_remove));
                counter++;
            }

            input_node = input_node.nextSibling();
        }
        file_input.open(QIODevice::Truncate | QIODevice::WriteOnly);
        file_input.write(input.toByteArray());
        file_input.close();

        if(counter == 0)
        {
            QFile::remove(current);
        }
        else
        {
            ConvertUTF8ToCiv4(current);
        }
    }

    qDebug() << "All modified files are in the \"backup\" folder";
}



QStringList languages::FindUnusedTags()
{
    // Check the directory
    QDir root(".");
    QStringList empty;
    if(!root.exists("../../../Assets/"))
    {
        qDebug() << "The parser need to be in 'Assets/XML/Text/' folder for this function to work. Aborting...";
        return empty;
    }

    // Getting game path and source path
    QFile categories("_xml_parser.config");
    if(!categories.exists())
    {
       qDebug() << "The parser need a '_xml_parser.config' file to continue. Aborting...";
       return empty;
    }
    QDomDocument xml_categories;
    categories.open(QIODevice::ReadOnly);
    xml_categories.setContent(&categories);
    categories.close();
    QString gamepath = xml_categories.firstChildElement().firstChildElement("gamepath").firstChild().nodeValue();
    if (gamepath.isEmpty())
    {
        qDebug() << "You need to add the base game path to the '_xml_parser.config' file";
        return empty;
    }
    QString srcpath = xml_categories.firstChildElement().firstChildElement("sourcepath").firstChild().nodeValue();
    if (srcpath.isEmpty())
    {
        qDebug() << "You need to add the dll source path to the '_xml_parser.config' file";
        return empty;
    }
    gamepath.replace("\\","/");
    srcpath.replace("\\","/");
	
    // List tags
    qDebug() << "Listing tags...";
    QStringList list_tags = ListTags(QDir::currentPath());
    QString tag;
	QString base_tag;
    list_tags.removeDuplicates();
	
	// List base game tags
	QStringList list_base_tags = ListTags(gamepath+"Assets/XML/Text/");
	QStringList list_ext_tags = ListTags(gamepath+"Beyond the Sword/Assets/XML/Text/");

    // Remove exceptions from the tag list

    foreach(tag, list_tags)
    {
        //qDebug() << tag;
        QDomElement exceptions = xml_categories.firstChildElement().firstChildElement("exceptions").firstChildElement();
        for(exceptions;!exceptions.isNull();exceptions = exceptions.nextSiblingElement())
        {
            QString value = exceptions.firstChild().nodeValue();
            if(tag.startsWith(value))
            {
                list_tags.removeAll(tag);
            }
        }
    }
	
	// Remove base game tags from the tag list
	foreach(tag,list_tags)
	{
		foreach(base_tag,list_base_tags)
		{
			if (tag == base_tag)
			{
				list_tags.removeAll(tag);
			}
		}
		foreach(base_tag,list_ext_tags)
		{
			if (tag == base_tag)
			{
				list_tags.removeAll(tag);
			}
		}
	}		

    int tags_total_counter = list_tags.count();

    qDebug() << "Collecting files...";
    QStringList list_ext;
    list_ext << "*.xml" << "*.py" << "*.CivBeyondSwordWBSave" << "*.Civ4WorldBuilderSave" << "*.CivWarlordsWBSave";
    QStringList list_cpp;
    list_cpp << "*.cpp" << "*.h";


    // List files in mod folder
    QStringList exclusion_list;
    exclusion_list << "Assets/XML/Text/";
    root.setCurrent("../../../");
    QDir game_folder(gamepath);
    QDir source_folder(srcpath);

    // Collect all files in a bigfile
    QDirIterator mod_iterator(root.absolutePath(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QDirIterator game_iterator(game_folder.path(), list_ext, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QDirIterator source_iterator(source_folder.path(), list_cpp, QDir::Files | QDir::NoDotAndDotDot,  QDirIterator::Subdirectories);
    QFile output("TEMPFILE");
    output.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&output);
    int file_counter = 0;
    while (mod_iterator.hasNext())
    {
        // Check if it isn't in the text folder
        if(mod_iterator.filePath().contains("Assets/XML/Text") || mod_iterator.filePath() == "")
        {
            mod_iterator.next();
        }

        else {
            file_counter++;
            QFile file(mod_iterator.filePath());

            file.open(QIODevice::ReadOnly);
            QTextStream temp(&file);
            while(!temp.atEnd())
            {
                QString line = temp.readLine();
                stream << line;
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
        else {
            file_counter++;
            QFile file(game_iterator.filePath());

            file.open(QIODevice::ReadOnly);
            QTextStream temp(&file);
            while(!temp.atEnd())
            {
                QString line = temp.readLine();
                stream << line;
            }
            file.close();
            //qDebug() << game_iterator.filePath();
            game_iterator.next();
        }
    }
    while (source_iterator.hasNext())
    {
        if(source_iterator.filePath() == "")
        {
            source_iterator.next();
        }
        else {
            file_counter++;
            QFile file(source_iterator.filePath());

            file.open(QIODevice::ReadOnly);
            QTextStream temp(&file);
            while(!temp.atEnd())
            {
                QString line = temp.readLine();
                stream << line;
            }
            file.close();
            //qDebug() << source_iterator.filePath();
            source_iterator.next();
        }
    }
    output.close();

    // Looking for tags
    int found = 0;
    qDebug() << "Looking for tags...";
    output.open(QIODevice::ReadOnly);
    QTextStream read_stream(&output);
    while (!read_stream.atEnd())
    {
        QString line = read_stream.readLine();
        foreach(tag,list_tags)
        {
            if (line.contains(tag))
            {
                //qDebug() << "Found" << tag;
                list_tags.removeAll(tag);
                found++;
                tags_total_counter--;
                if(tags_total_counter%100==0)
                {
                    qDebug() << "Still" << tags_total_counter << "tags to process";
                }
            }
        }

    }
    output.close();
    output.remove();

    qDebug() << tags_total_counter << "tags have not been found. Printing list: _tags_not_found.txt in text folder";

    root.setCurrent("Assets/XML/Text/");

    // Print not found list
    QString print_value;
    QFile print_file("_tags_not_found.txt");

    if ( print_file.open(QIODevice::ReadWrite) )
        {
            QTextStream stream( &print_file );

            foreach(print_value, list_tags)
            {
                stream << print_value << endl;
            }
        }
    print_file.close();

    return list_tags;
}

void languages::RemoveUnusedTags()
{
    // Test code
    QFile test("_tags_not_found.txt");
    QStringList list_tags;
    test.open(QIODevice::ReadOnly);
    QTextStream read_stream(&test);
    while (!read_stream.atEnd())
    {
        list_tags << read_stream.readLine();
    }
    test.close();

    // Prepare folder
    qDebug() << "Preparing files...";
    QDir dir_import("unused/");
    dir_import.removeRecursively();
    dir_import.mkdir(".");

    // List root files
    QStringList xml_filter;
    xml_filter << "*.xml";
    QDir root(".");
    QStringList root_files;
    root_files = root.entryList(xml_filter, QDir::Files);

    // Copy and convert files to backup dir
    QStringList import_files;
    for(QStringList::Iterator it = root_files.begin(); it != root_files.end(); it++)
    {
        QFile::copy(*it,"unused/"+*it);
        ConvertCiv4ToUTF8("unused/"+*it);
    }
    import_files = dir_import.entryList(xml_filter, QDir::Files);

    QString tags;
    int counter = 0;
    int file_number = 1;
    QStringList operation_list;

    for(QStringList::Iterator it = import_files.begin(); it != import_files.end(); it++)
    {
        qDebug() << "Processing file" << file_number << "of" << import_files.count();
        QString current = "unused/" + *it;
        QDomDocument input;
        QFile file_input(current);
        file_input.open(QIODevice::ReadOnly);
        input.setContent(&file_input);
        file_input.close();

        QDomNode input_node = input.firstChildElement("Civ4GameText").firstChildElement("TEXT");
        int remove_tag = 0;
        int remove_file = 0;
        while(!input_node.isNull())
        {
            QString tag_value = input_node.firstChildElement("Tag").firstChild().nodeValue();
            //qDebug() << tag_value;
            remove_tag = 0;
            foreach(tags,list_tags)
            {
                if(tag_value == tags)
                {
                    list_tags.removeAll(tags);
                    QString operation = "FILE: " + *it + " | REMOVED TAG: " + tags;
                    operation_list << operation;
                    counter++;
                    remove_file++;
                    remove_tag++;
                }
            }
            input_node = input_node.nextSibling();
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
        file_input.write(input.toByteArray());
        file_input.close();

        if(remove_file == 0)
        {
            QFile::remove(current);
        }
        else
        {
            ConvertUTF8ToCiv4(current);
        }
        file_number++;
    }

    // Output modified list
    QString print_value;
    QString print_file_name = "unused/_unused_tags_report.txt";
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

    qDebug() << "All modified files are in the \"unused\" folder. Removed" << counter << "tags. A detailled report has been print in _unused_tags_report.txt";
    return;
}
*/
