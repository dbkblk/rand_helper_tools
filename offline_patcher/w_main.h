#ifndef W_MAIN_H
#define W_MAIN_H

#include <QMainWindow>
#include <QSettings>

namespace Ui {
class w_main;
}

namespace tools {
// Define OS tools
const QString TOOL_SVN = "tools\\svn.exe";
const QString TOOL_DIFF = "tools\\diff.exe";
const QString TOOL_CONSOLE = "cmd ";
}

class w_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_main(QWidget *parent = 0);
    ~w_main();

private slots:
    void on_bt_download_clicked();

    bool on_bt_apply_clicked();

private:
    Ui::w_main *ui;
};

bool clearCache();
void setConfig(QString option, QString value);
QString getConfig(QString option);
int getLocalVersion(QString modPath);

#endif // W_MAIN_H
