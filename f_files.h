#ifndef F_FILES_H
#define F_FILES_H
#include <QtCore>

class f_files
{
public:
    f_files();
    bool compareMd5(QFile file1, QFile file2);
    bool generateXMLAndroid(QFile file);
    bool checkXMLConformity(QFile file);
    QString readParam(QString param);
    bool setParam(QString param, QString newValue);
};

#endif // F_FILES_H
