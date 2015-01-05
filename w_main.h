#ifndef W_MAIN_H
#define W_MAIN_H

#include <QMainWindow>

namespace Ui {
class w_main;
}

class w_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit w_main(QWidget *parent = 0);
    ~w_main();
    void exportToAndroidXML(QString langCode);
    void importToCivXML(QString langCode, bool all_files);

private slots:
    void on_bt_baseDirChange_clicked();
    void on_bt_exportDirChange_clicked();
    void on_actionTest_triggered();
    void on_actionExport_to_Android_XML_triggered();
    void debugWindow(int time);

    void on_actionImport_to_game_files_triggered();

    void on_bt_importDirChange_clicked();

    void on_bt_openBaseDir_clicked();

    void on_bt_openTranslDir_clicked();

    void on_bt_openExportDir_clicked();

private:
    Ui::w_main *ui;
};

#endif // W_MAIN_H
