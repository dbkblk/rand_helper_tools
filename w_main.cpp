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
    ui->line_importDir->setText(readDir("translations"));
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

void w_main::on_bt_importDirChange_clicked()
{
    QString filesdir = QFileDialog::getExistingDirectory(0, tr("Select import  directory"), QString(), QFileDialog::ShowDirsOnly |QFileDialog::DontResolveSymlinks);
    saveDir("translations", filesdir);
    ui->line_importDir->setText(filesdir);
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

void w_main::importToCivXML(QString langCode, bool all_files){



}

void w_main::on_actionTest_triggered()
{
    QString langCode = "hu";
    // Create / open existing log list
    QFile log_file(readDir("export") + "/report_" + langCode );
    QDomDocument log;
    if(log_file.exists()){
        log_file.open(QIODevice::ReadOnly);
        log.setContent(&log_file);
        log_file.close();
    }
    else{
        QDomNode declaration = log.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\""));
        log.insertBefore(declaration,log.firstChild());
        log.appendChild(log.createElement("root"));
    }
    QDomElement log_root = log.firstChildElement();
    qDebug() << log_root.tagName();

    // log entry
    QDomElement log_entry = log.createElement("entry");
    log_entry.setAttribute("file","file");
    log_entry.setAttribute("tag","value_tag");
    log_entry.setAttribute("old_value","value_text");
    log_entry.setAttribute("new_value","value_text_tr");
    log_root.appendChild(log_entry);

    log_file.open(QIODevice::Truncate | QIODevice::WriteOnly);
    QTextStream rep(&log_file);
    log.save(rep, 4);
    log_file.close();
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

void w_main::on_actionImport_to_game_files_triggered()
{
    ui->console->clear();
    ui->console->append("Preparing files (conversion to UTF8)...");
    // Check options
    bool opt_same_name = true;
    // Read export dir
    f_files f;
    f_lang l;
    QStringList translations = f.checkImportLanguages();
    translations.removeAll("en"); // Remove english from the list

    // Create counter for the progress bar
    int counter = 0;
    int range = 0;
    foreach(QString tr_code, translations){
        QStringList tr_fileslist = f.getTranslationFilesList(tr_code);
        range = range + tr_fileslist.count();
    }

    // Get the directories
    QString export_dir_string = readDir("export") + "/";
    QDir export_dir(export_dir_string);
    if(!export_dir.exists()){
        ui->console->append("Export directory not set, aborting.");
        return;
    }

    // Get base files, copy and convert them to export
    QStringList list = f.getBaseFilesList();
    range = range + list.count() + list.count(); // x2 because of the process at the end.
    ui->progressBar->setRange(0,range);
    ui->progressBar->setValue(0);
    QStringList exported_list;
    QStringList base_list;
    foreach(QString entry, list){
        QStringList filelist = entry.split("/");
        QString filename = "";
        foreach(QString entry, filelist){
            filename = entry;
        }
        base_list << filename;
        QFile output(export_dir_string + filename);
        if(output.exists()){
            output.remove();
        }
        QFile::copy(entry,export_dir_string + filename);
        l.convertCivToUTF(export_dir_string + filename);
        exported_list << export_dir_string + filename;
        counter++;
        ui->progressBar->setValue(counter);
    }



    // Read each translation file for importation
    foreach(QString tr_code, translations){
        QStringList tr_fileslist = f.getTranslationFilesList(tr_code);
        ui->console->append("Processing " + l.getIntlName(tr_code));
        foreach(QString tr_file, tr_fileslist){
            QStringList same_name;
            if(opt_same_name){
                // Extract translated files names if option is checked;
                QStringList tr_temp = tr_file.split("/");
                QString tr_filename = "";
                foreach(QString entry, tr_temp){
                    tr_filename = entry;
                }
                foreach(QString entry, base_list){
                    if(entry == tr_filename){
                        same_name << QString(export_dir_string + entry);
                    }
                }
            }
            else{
                same_name = exported_list;
            }

            //qDebug() << "Importing" << tr_file << "in" << tr_code;
            f.convertXMLAndroidToCiv(tr_file, same_name,tr_code);
            counter++;
            ui->progressBar->setValue(counter);
        }
    }

    // Convert the files back to ISO-8859-1
    ui->console->append("Saving files (conversion to ISO-8859-1)...");
    foreach(QString entry, exported_list){
        l.convertUTFToCiv(entry);
        counter++;
        ui->progressBar->setValue(counter);
    }

    ui->console->append("Import complete !");
}



void w_main::on_bt_openBaseDir_clicked()
{
    if(readDir("base").isEmpty()){
        QMessageBox::critical(0, "Error", QObject::tr("You must set the directory first"));
    }
    else{
        QDesktopServices::openUrl(QUrl::fromLocalFile(readDir("base")));
    }
}

void w_main::on_bt_openTranslDir_clicked()
{
    if(readDir("translations").isEmpty()){
        QMessageBox::critical(0, "Error", QObject::tr("You must set the directory first"));
    }
    else{
        QDesktopServices::openUrl(QUrl::fromLocalFile(readDir("translations")));
    }
}

void w_main::on_bt_openExportDir_clicked()
{
    if(readDir("export").isEmpty()){
        QMessageBox::critical(0, "Error", QObject::tr("You must set the directory first"));
    }
    else{
        QDesktopServices::openUrl(QUrl::fromLocalFile(readDir("export")));
    }
}
