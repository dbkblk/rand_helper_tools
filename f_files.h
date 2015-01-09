#ifndef F_FILES_H
#define F_FILES_H
#include <QtCore>

void saveDir(QString dir, QString value);
QString readDir(QString dir);

class f_files
{
public:
    f_files();
    QString checkMd5(QString file_path);
    bool generateXMLAndroid(QFile file);
    bool checkXMLConformity(QString fileName);
    bool convertXMLCivToAndroid(QString file, QString langCode);
    bool convertXMLAndroidToCiv(QString file, QStringList files_to_check, QString langCode);
    QStringList getBaseFilesList();
    QStringList getTranslationFilesList(QString langCode);
    QStringList checkImportLanguages();
};

#endif // F_FILES_H
