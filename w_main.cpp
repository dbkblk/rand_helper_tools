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
    f_files f;
    ui->line_baseDir->setText(f.readParam("Main/base_dir"));
    ui->line_exportDir->setText(f.readParam("Main/export_dir"));
}

w_main::~w_main()
{
    delete ui;
}

void w_main::on_bt_baseDirChange_clicked()
{
    f_files f;
    QString filesdir = QFileDialog::getExistingDirectory(0, tr("Select base directory"), QString(), QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    f.setParam("Main/base_dir",filesdir);
    ui->line_baseDir->setText(filesdir);
}

void w_main::on_bt_exportDirChange_clicked()
{
    f_files f;
    QString filesdir = QFileDialog::getExistingDirectory(0, tr("Select export directory"), QString(), QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    f.setParam("Main/export_dir",filesdir);
    ui->line_exportDir->setText(filesdir);
}
