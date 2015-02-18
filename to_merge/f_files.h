#ifndef F_FILES_H
#define F_FILES_H
#include <QtCore>

void saveDir(QString dir, QString value);
QString readDir(QString dir);
void saveOption(QString option, QString value);
QString readOption(QString option);

class f_files : public QObject
{
    Q_OBJECT

public:
    f_files();
    QString checkMd5(QString file_path);
    //bool generateXMLAndroid(QFile file);
    bool checkXMLConformity(QString fileName);
    bool convertXMLCivToAndroid(QString file, QString langCode);
    bool convertXMLAndroidToCiv(QString file, QStringList files_to_check, QString langCode);
    QStringList getBaseFilesList();
    QStringList getTranslationFilesList(QString langCode);
    QStringList checkImportLanguages();
    void sortCategories();

signals:
    void message(QString message);
};

#endif // F_FILES_H
