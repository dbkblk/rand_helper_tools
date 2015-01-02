#ifndef F_LANG_H
#define F_LANG_H
#include <QtCore>

class f_lang
{
public:
    f_lang();
    QString getIntlCode(QString langCode);
    QStringList getSupportedLang();
    bool isSupportedLang(QString langCode);
    void convertCivToUTF(QString file);
    void convertUTFToCiv(QString file);
    QString convertStringToCiv(QString string);
    QString convertLatinToCharset(QString language, QString string);
    QString convertCharsetToLatin(QString language, QString string);
    QStringList listLangInDir(QString dir);
};

#endif // F_LANG_H
