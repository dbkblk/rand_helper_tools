#include <QtCore>

QString getMd5(QString file_path){
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
    // Opening file
    QFile file("checksums");
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        // Read ini file to get the dir to scan
        QSettings settings("checksum.ini", QSettings::IniFormat);
        QString dir = settings.value("checksum/dir").toString();

        QDirIterator it(dir, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            QString path = it.filePath();
            QString md5 = getMd5(path);
            path.remove(dir);
            stream << QString(path + " | " + md5) << endl;
        }

        return 0;
    }

    return 1;
}
