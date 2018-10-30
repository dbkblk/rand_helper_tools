#include <QCoreApplication>
#include <QtCore>
#include <QtXml>

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
    // Prepare the XML file
    QFile file("list.xml");
    file.remove();
    QDomDocument xml;
    QDomNode declaration = xml.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
    xml.insertBefore(declaration,xml.firstChild());
    QDomComment comment = xml.createComment("Do not edit this file, it is regenerated automatically!");
    xml.appendChild(comment);
    QDomNode root = xml.createElement("files");
    xml.appendChild(root);

    // Liste tous les fichiers
    QStringList liste;
    QDirIterator it(".", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        if(QString(it.hasNext()).startsWith("./Assets/Modules/Custom_Civilizations_MCP/"))
        {}
        else{
            // Add the file to the list
            QDomElement entity = xml.createElement("file");
            entity.appendChild(xml.createTextNode(it.next()));
            entity.setAttribute("md5", checkMd5(it.next()));
            root.appendChild(entity);
        }
    }

    // Save the file
    file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    QTextStream ts(&file);
    xml.save(ts, 4);
    file.close();

    return a.exec();
}
