#ifndef F_FILES_H
#define F_FILES_H
#include <QtCore>

void saveDir(QString dir, QString value);
QString readDir(QString dir);

class f_files
{
public:
    f_files();
    bool compareMd5(QFile file1, QFile file2);
    bool generateXMLAndroid(QFile file);
    bool checkXMLConformity(QString fileName);
    bool convertXMLCivToAndroid(QString file, QString langCode);
};

#endif // F_FILES_H
