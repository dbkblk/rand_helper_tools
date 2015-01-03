#include "w_main.h"
#include "ui_w_main.h"
#include "f_files.h"
#include "f_lang.h"
#include <QtCore>
#include <QtWidgets>
#include <QtXml>


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

void w_main::debugWindow(int time)
{
    // GUI: Fix the interface lockup
    QTimer wait_timer;
    QEventLoop wait_install;
    wait_timer.setInterval(time);
    wait_timer.setSingleShot(true);
    connect(&wait_timer,SIGNAL(timeout()),&wait_install,SLOT(quit()));
    wait_timer.start();
    wait_install.exec();
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

void w_main::exportToAndroidXML(QString langCode){

    // List all files
    f_lang l;
    f_files f;
    QDir root(readDir("base"));
    QString exportdir = readDir("export");
    QStringList xml_filter;
    xml_filter << "*.xml";
    QStringList folders;
    folders = root.entryList(xml_filter, QDir::Files);

    // Convert all the files in the language
    for(QStringList::Iterator it = folders.begin(); it != folders.end(); it++)
    {
        QString exportname = exportdir + "/" + *it;
        QFile::copy(*it,exportname);
        l.convertCivToUTF(exportname);
        f.convertXMLCivToAndroid(exportname,langCode);
        QFile::remove(exportname);
    }

    // Check output files
    QStringList output_files;
    QDir output(exportdir + "/" + langCode);
    output_files = output.entryList(xml_filter, QDir::Files);
    for(QStringList::Iterator it = output_files.begin(); it != output_files.end(); it++)
    {
        QString current = exportdir + "/" + langCode + "/" + *it;
        int not_empty_counter = 0;
        QDomDocument check;
        QFile file_check(current);
        file_check.open(QIODevice::ReadOnly);
        check.setContent(&file_check);
        file_check.close();
        QDomElement node_check = check.firstChildElement("resources").firstChildElement("string");
        for(node_check;!node_check.isNull();node_check = node_check.nextSiblingElement())
        {
            if(!node_check.firstChild().nodeValue().isEmpty())
            {
                not_empty_counter++;
            }
        }

        if(not_empty_counter == 0)
        {
            file_check.remove();
        }
        check.clear();

    }
}

void w_main::on_actionTest_triggered()
{

}

void w_main::on_actionExport_to_Android_XML_triggered()
{
    f_lang l;
    f_files f;
    // Get enabled languages
    QStringList languages = l.getEnabledCodes();
    int count = languages.count();
    int counter = 0;
    ui->progressBar->setRange(0,count);
    ui->console->clear();
    ui->console->append(QString("Enabled languages are: " + languages.join(" / ")));
    foreach(QString entry, languages){
        ui->console->append(QString("Processing " + entry));
        debugWindow(1000);
        exportToAndroidXML(entry);
        counter++;
        ui->progressBar->setValue(counter);
    }
    ui->console->append("Export successful");
}
