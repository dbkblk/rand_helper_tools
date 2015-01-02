#include "w_main.h"
#include "ui_w_main.h"
#include "f_files.h"
#include "f_lang.h"
#include <QtCore>
#include <QtWidgets>


w_main::w_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_main)
{
    ui->setupUi(this);

    // Initialize line edit
    ui->line_baseDir->setText(readDir("base"));
    ui->line_exportDir->setText(readDir("export"));
}

w_main::~w_main()
{
    delete ui;
}

void w_main::on_bt_baseDirChange_clicked()
{
    QString filesdir = QFileDialog::getExistingDirectory(0, tr("Select base directory"), QString(), QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    saveDir("base", filesdir);
    ui->line_baseDir->setText(filesdir);
}

void w_main::on_bt_exportDirChange_clicked()
{
    QString filesdir = QFileDialog::getExistingDirectory(0, tr("Select export directory"), QString(), QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    saveDir("export", filesdir);
    ui->line_exportDir->setText(filesdir);
}

void w_main::exportToAndroidXML(){

}

void w_main::on_actionTest_triggered()
{
    f_lang l;
    ui->console->clear();
    if(l.isSupportedLang("ze")){
        ui->console->append("true");
    }
    else{
        ui->console->append("false");
    }

}
