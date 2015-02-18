#include "f_lang.h"
#include "f_files.h"
#include <QtCore>
#include <QtXml>

f_lang::f_lang(QObject *parent) :
    QObject(parent)
{
}

QString f_lang::getIntlName(QString langCode){
    // Input international code and output game code (fr > French)
    // Open the settings
    QString value = "Unsupported";
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("supported").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(langCode == lang.attribute("code")){
            value = lang.firstChild().nodeValue();
        }
        lang = lang.nextSiblingElement();
    }
    return value;
}

QString f_lang::getIntlCode(QString langName){
    // Input international code and output game code (fr > French)
    // Open the settings
    QString value = "Unsupported";
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("supported").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(langName ==  lang.firstChild().nodeValue()){
            value = lang.attribute("code");
        }
        lang = lang.nextSiblingElement();
    }
    return value;
}

QStringList f_lang::getEnabledCodes(){
    // Output list of supported languages
    QStringList list;

    // Open the settings
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("supported").firstChildElement("language").toElement();
    while(!lang.isNull()){
        if(lang.attribute("enabled").toInt() == 1){
            list << lang.attribute("code");
        }
        lang = lang.nextSiblingElement();
    }
     return list;
}

QStringList f_lang::getSupportedCodes(){
    // Output list of supported languages
    QStringList list;

    // Open the settings
    QFile settings("xml_parser.config");
    settings.open(QIODevice::ReadOnly);
    QDomDocument xml;
    xml.setContent(&settings);
    settings.close();
    QDomElement lang = xml.firstChildElement("main").firstChildElement("supported").firstChildElement("language").toElement();
    while(!lang.isNull()){
        list << lang.attribute("code");
        lang = lang.nextSiblingElement();
    }
     return list;
}

bool f_lang::isSupportedLang(QString langCode){
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

bool f_lang::isEnabledLang(QString langCode){
    // Ouput true or false
    QStringList list = getEnabledCodes();
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

bool f_lang::convertCivToUTF(QString file)
{
    /* Read all the file
     * Save the content to another file
     * Replace the other file with the first */
    qDebug() << file;
    // Check for charset specific rules (ex: Russian)
    int charset = 0;
    QFile settings("xml_parser.config");
    QStringList value_languages;
    if(settings.exists()){
        QDomDocument xml;
        settings.open(QIODevice::ReadOnly);
        xml.setContent(&settings);
        settings.close();
        QDomElement xml_main = xml.firstChildElement("main").firstChildElement("charsets").firstChildElement();

        for (xml_main;!xml_main.isNull();xml_main = xml_main.nextSiblingElement()){
            value_languages <<xml_main.tagName();
            //qDebug() << xml_main.tagName() << "charset rules found";
        }
        charset = 1;
    }

    // First, pure read to remove faulty characters
    QFile amp_read(file);
    QFile amp_write(file + "_amptemp_");
    amp_read.open(QIODevice::ReadOnly);
    amp_write.open(QIODevice::WriteOnly);
    QTextStream amp_in(&amp_read);
    QTextStream amp_out(&amp_write);
    while(!amp_in.atEnd())
    {
        QString line = amp_in.readLine();
        line.replace("&#xd;","").replace(QLatin1Char('&'), "&amp;").replace("&amp;amp;","&amp;").replace("&amp;#","&#").replace("&amp;lt;","&lt;").replace("&amp;gt;","&gt;");
        line.simplified();
        amp_out << line;
    }
    amp_read.close();
    amp_write.close();


    // Open the input file
    QDomDocument read;
    QString filename = file + "_amptemp_";
    QFile file_in(filename);
    f_files f;
    f_lang l;
    if(!f.checkXMLConformity(filename)){
        qDebug() << "Non conformal XML" << filename;
        return false;
    }
    file_in.open(QIODevice::ReadOnly);
    read.setContent(&file_in);
    QDomElement read_text = read.firstChildElement("Civ4GameText").firstChildElement("TEXT");

    // Create the output file
    QDomDocument write;
    QFile file_out("rewrite.xml");
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDomNode declaration = write.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    write.insertBefore(declaration,write.firstChild());
    QDomNode write_root = write.createElement("Civ4GameText");
    write.appendChild(write_root);

   // Loop the input file
    for (read_text;!read_text.isNull();read_text = read_text.nextSiblingElement())
    {
        //qDebug() << read_text.firstChildElement().tagName();
        QDomElement read_element = read_text.firstChildElement();
        QDomElement write_node = write.createElement("TEXT");
        write_root.appendChild(write_node);

        for (read_element;!read_element.isNull();read_element = read_element.nextSiblingElement())
        {
            //qDebug() << read_element.tagName();
            if(!read_element.firstChildElement("Text").isNull())
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QDomElement write_element_text = write.createElement("Text");
                QDomElement write_element_gender = write.createElement("Gender");
                QDomElement write_element_plural = write.createElement("Plural");
                write_element.appendChild(write_element_text);
                write_element.appendChild(write_element_gender);
                write_element.appendChild(write_element_plural);

                QString write_text_value = read_element.firstChildElement("Text").firstChild().nodeValue();
                QString write_gender_value = read_element.firstChildElement("Gender").firstChild().nodeValue();
                QString write_plural_value = read_element.firstChildElement("Plural").firstChild().nodeValue();

                // Check charsets
                if (charset == 1){
                    foreach (QString temp, value_languages){
                        if(read_element.tagName() == temp)
                        {
                            //qDebug() << "Converting" << write_text_value;
                            //write_text_value = l.convertLatinToCharset(temp, write_text_value);
                        }
                    }
                }

                QDomText write_text = write.createTextNode(write_text_value);
                QDomText write_gender = write.createTextNode(write_gender_value);
                QDomText write_plural = write.createTextNode(write_plural_value);
                write_element_text.appendChild(write_text);
                write_element_gender.appendChild(write_gender);
                write_element_plural.appendChild(write_plural);
            }
            else
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QString write_text_value = read_element.firstChild().nodeValue();

                // Check charsets
                if (charset == 1){
                    foreach (QString temp, value_languages){
                        if(read_element.tagName() == temp)
                        {
                            //qDebug() << "Converting" << write_text_value;
                            //write_text_value = l.convertLatinToCharset(temp, write_text_value);
                        }
                    }
                }

                QDomText write_text = write.createTextNode(write_text_value);
                write_element.appendChild(write_text);
            }
        }
    }
    QTextStream ts(&file_out);
    write.save(ts, 3);
    file_out.close();
    file_in.close();
    QFile::remove(file + "_amptemp_");
    QFile::remove(file);
    QFile::rename("rewrite.xml",file);
}

bool f_lang::convertUTFToCiv(QString file)
{
    /* Read all the file
     * Save the content to another file
     * Replace the other file with the first */

    // Check for charset specific rules (ex: Russian)
    int charset = 0;
    QFile settings("xml_parser.config");
    QStringList value_languages;
    if(settings.exists()){
        QDomDocument xml;
        settings.open(QIODevice::ReadOnly);
        xml.setContent(&settings);
        settings.close();
        QDomElement xml_main = xml.firstChildElement("main").firstChildElement("charsets").firstChildElement();

        for (xml_main;!xml_main.isNull();xml_main = xml_main.nextSiblingElement()){
            value_languages <<xml_main.tagName();
            //qDebug() << xml_main.tagName() << "charset rules found";
        }
        charset = 1;
    }

    // Open the input file
   QDomDocument read;
   QFile file_in(file);
   f_files f;
   f_lang l;
   if(!f.checkXMLConformity(file)){
       qDebug() << "Non conformal XML" << file;
       return false;
   }
   file_in.open(QIODevice::ReadOnly);
   read.setContent(&file_in);
   QDomNode read_text = read.firstChildElement("Civ4GameText").firstChildElement();

    // Create the output file
    QDomDocument write;
    QFile file_out(file + "_tempconvert_");
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QDomNode declaration = write.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"ISO-8859-1\""));
    write.insertBefore(declaration,write.firstChild());
    QDomNode write_root = write.createElement("Civ4GameText");
    write.appendChild(write_root);
    //qDebug() << write.firstChildElement().tagName();

   // Loop the input file
    for (read_text;!read_text.isNull();read_text = read_text.nextSibling())
    {
        //qDebug() << read_text.firstChildElement().tagName();
        QDomElement read_element = read_text.firstChildElement();
        QDomNode write_node = write.createElement("TEXT");
        write_root.appendChild(write_node);

        for (read_element;!read_element.isNull();read_element = read_element.nextSiblingElement())
        {
            //qDebug() << read_element.tagName();
            if(!read_element.firstChildElement("Text").isNull())
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);
                QDomElement write_element_text = write.createElement("Text");
                QDomElement write_element_gender = write.createElement("Gender");
                QDomElement write_element_plural = write.createElement("Plural");
                write_element.appendChild(write_element_text);
                write_element.appendChild(write_element_gender);
                write_element.appendChild(write_element_plural);

                QString write_text_value = read_element.firstChildElement("Text").firstChild().nodeValue();
                QString write_gender_value = read_element.firstChildElement("Gender").firstChild().nodeValue();
                QString write_plural_value = read_element.firstChildElement("Plural").firstChild().nodeValue();

                // Check charsets
                if (charset == 1){
                    foreach (QString temp, value_languages){
                        if(read_element.tagName() == temp)
                        {
                            write_text_value = l.convertCharsetToLatin(temp, write_text_value);
                        }
                    }
                }

                write_text_value = l.convertStringToCiv(write_text_value);
                write_gender_value = l.convertStringToCiv(write_gender_value);
                write_plural_value = l.convertStringToCiv(write_plural_value);

                QDomText write_text = write.createTextNode(write_text_value);
                QDomText write_gender = write.createTextNode(write_gender_value);
                QDomText write_plural = write.createTextNode(write_plural_value);

                write_element_text.appendChild(write_text);
                write_element_gender.appendChild(write_gender);
                write_element_plural.appendChild(write_plural);
            }
            else
            {
                QDomElement write_element = write.createElement(read_element.tagName());
                write_node.appendChild(write_element);

                QString write_text_value = read_element.firstChild().nodeValue();

                // Check charsets
                if (charset == 1){
                    foreach (QString temp, value_languages){
                        if(read_element.tagName() == temp)
                        {
                            write_text_value = l.convertCharsetToLatin(temp, write_text_value);
                        }
                    }
                }

                write_text_value = l.convertStringToCiv(write_text_value);

                QDomText write_text = write.createTextNode(write_text_value);
                write_element.appendChild(write_text);
            }
        }
    }

    QTextStream ts(&file_out);
    QString docString = write.toString().replace("&amp;", "&").replace(QLatin1Char('&'), "&amp;").replace("&amp;#","&#").replace("&amp;lt;","&lt;").replace("&amp;gt;","&gt;").replace("&amp;amp;","&amp;");
    ts << docString;
    //write.save(ts, 3);
    file_out.close();
    file_in.close();

    QFile::remove(file);
    QFile::rename(file + "_tempconvert_",file);
}

QString f_lang::convertStringToCiv(QString string)
{
    QString encode;
    for(int i=0;i<string.size();++i)
    {
        QChar ch = string.at(i);
        if(ch.unicode() > 127)
        {
            encode += QChar(38);
            encode += QString("#%1;").arg((int)ch.unicode());
        }
        else
        {
            encode += ch;
        }
    }
    encode.replace("&amp;", "&").replace(QLatin1Char('&'), "&amp;").replace("&amp;#","&#").replace("&amp;lt;","&lt;").replace("&amp;gt;","&gt;");
    return encode;
}

QString f_lang::convertLatinToCharset(QString langName, QString string)
{
    // Open the charset file and go through the correct language
    QFile charset_file("xml_parser.config");
    QDomDocument charset_xml;
    charset_file.open(QIODevice::ReadOnly);
    charset_xml.setContent(&charset_file);
    charset_file.close();
    QDomElement charset_xml_main = charset_xml.firstChildElement("main").firstChildElement("charsets").firstChildElement(langName).firstChildElement();
    QMap<int, int> table;
    for (charset_xml_main;!charset_xml_main.isNull();charset_xml_main = charset_xml_main.nextSiblingElement()){
        int value_standard = charset_xml_main.firstChild().nodeValue().toInt();
        int value_unicode = charset_xml_main.attribute("unicode").toInt();
        table.insert(value_standard,value_unicode);
    }

    // Open the sentence to translate
    QString encode;
    for(int i=0;i<string.size();++i)
    {
        QChar ch = string.at(i);

        QMap<int,int>::iterator f;
        for (f = table.begin(); f != table.end(); ++f){
            //qDebug() << f.value() << " | " << f.key();
            if (f.key() == ch){
                ch = f.value();
            }
        }

        encode += ch;
    }
    return encode;
}

QString f_lang::convertCharsetToLatin(QString langName, QString string)
{
    // Open the charset file and go through the correct language
    QFile charset_file("xml_parser.config");
    QDomDocument charset_xml;
    charset_file.open(QIODevice::ReadOnly);
    charset_xml.setContent(&charset_file);
    charset_file.close();
    QDomElement charset_xml_main = charset_xml.firstChildElement("main").firstChildElement("charsets").firstChildElement(langName).firstChildElement();
    //qDebug() << charset_xml_main.tagName();
    QMap<int, int> table;
    for (charset_xml_main;!charset_xml_main.isNull();charset_xml_main = charset_xml_main.nextSiblingElement()){
        int value_standard = charset_xml_main.firstChild().nodeValue().toInt();
        int value_unicode = charset_xml_main.attribute("unicode").toInt();
        table.insert(value_unicode,value_standard);
    }

    // Open the sentence to translate
    QString encode;
    for(int i=0;i<string.size();++i)
    {
        QChar ch = string.at(i);

        QMap<int,int>::iterator f;
        for (f = table.begin(); f != table.end(); ++f){
            //qDebug() << f.value() << " | " << f.key();
            if (f.key() == ch){
                ch = f.value();
            }
        }

        encode += ch;
    }
    return encode;
}

QStringList f_lang::listLangInDir(QString dir)
{
    /*// Automatic language detection / menu
    QStringList known_languages = languages::ListLanguages(dir);
    known_languages << "English";
    known_languages.sort();
    QString lang;
    QString lang_check;
    std::string answer;
    int lang_counter = 0;

    qDebug("\nWhich language do you want to process ?\n");

    foreach(lang, known_languages)
    {
        lang_counter++;
        qDebug() << lang_counter << ")" << lang;
    }
    qDebug() << lang_counter+1 << ")  Exit";

    //std::cin >> answer;
    lang_counter = 0;

    if(QString::fromStdString(answer).toInt() > known_languages.count()+1)
    {
        qDebug("Invalid selection");
        return "error";
    }
    else if (QString::fromStdString(answer).toInt() == known_languages.count()+1)
    {
        return "error";
    }

    foreach(lang, known_languages)
    {
        lang_counter++;
        if(lang_counter == QString::fromStdString(answer).toInt())
        {
            qDebug() << "Processing" << lang;
            lang_check = lang;
        }
    }*/
    QStringList empty;
    return empty;
}

