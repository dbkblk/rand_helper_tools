#ifndef F_LANG_H
#define F_LANG_H
#include <QtCore>

class f_lang
{
public:
    f_lang();
    QString getIntlName(QString langCode);
    QString getIntlCode(QString langName);
    QStringList getSupportedCodes();
    bool isSupportedLang(QString langCode);
    QStringList getEnabledCodes();
    bool isEnabledLang(QString langCode);
    bool convertCivToUTF(QString file);
    bool convertUTFToCiv(QString file);
    QString convertStringToCiv(QString string);
    QString convertLatinToCharset(QString langName, QString string);
    QString convertCharsetToLatin(QString langName, QString string);
    QStringList listLangInDir(QString dir);
};

#endif // F_LANG_H
