#include "w_main.h"
#include "ui_w_main.h"
#include "QDebug"
#include "QMessageBox"
#include "QtCore"
#include "QFileDialog"
#include <QSettings>

w_main::w_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_main)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("AND offline patcher"));

    // Set version number
    ui->lb_version->setText("v0.9.2");
}

w_main::~w_main()
{
    delete ui;
}

void w_main::on_bt_download_clicked()
{
    // Convert the entry into a int
    QString str_value = ui->text_rev->toPlainText();
    int int_value = str_value.toInt();
    if (int_value == 0){
        QMessageBox::critical(this, "Error", QObject::tr("You must enter a valid number'"));
    }
    else{
        // Invoke svn diff based on the revision
        QString patch_tool = tools::TOOL_SVN + QString(" diff -r%1 --force --diff-cmd ").arg(int_value) + tools::TOOL_DIFF + QString(" -x -au \"http://svn.code.sf.net/p/anewdawn/code/Trunk/Rise of Mankind - A New Dawn/\" > and_%1_update.patch\"").arg(int_value);
        //qDebug() << patch_tool;
        system(patch_tool.toStdString().c_str());

        // Warn the user
        QMessageBox::information(this, "Done", QObject::tr("The patch has been downloaded"));
    }
}

bool w_main::on_bt_apply_clicked()
{
    // Get patch and output paths
    QString patchPath = QFileDialog::getOpenFileName(this, "Select the patch file", QString("."), "*.patch");
    qDebug() << "Patch location:" << patchPath;
    if(patchPath.isEmpty()){
        qDebug() << "Aborted.";
        return false;
    }

    QString gdir = getConfig("GameDir");
    QString dir;
    if(gdir != "error"){
        dir = QFileDialog::getExistingDirectory(this, tr("Select the mod directory"),gdir,QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks) + "/";
    }
    else{
        dir = QFileDialog::getExistingDirectory(this, tr("Select the mod directory"),QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks) + "/";
        setConfig("GameDir", dir);
    }
    if(dir == "/"){
        qDebug() << "Aborted.";
        return false;
    }

    qDebug() << "Installation directory : " << dir;

    // Get filename
    QStringList patchList = patchPath.split("/");
    QString patchName;
    foreach(QString entry, patchList){
        patchName = entry;
    }
    QString patchOutput = dir + patchName;

    // Get patch revision number
    QStringList revList = patchName.split("_");
    QString revision = revList[1];
    qDebug() << patchName;

    // Compare with game revision to apply
    QString local_revision = QString::number(getLocalVersion(dir));
    qDebug() << "Comparing" << revision << "with" << local_revision;
    if(revision != local_revision){
        QMessageBox::critical(this, "Error", QString("Patch version is " + revision + " while local installation is " + local_revision + ". Cannot apply patch."));
        return false;
    }

    // Copy the patchs files to the directory to patch
    QFile::copy(patchPath,patchOutput);
    QFile::copy(QString(QCoreApplication::applicationDirPath() + "/tools/apply.exe"),QString(dir + "apply.exe"));
    QFile::copy(QString(QCoreApplication::applicationDirPath() + "/tools/cygwin1.dll"),QString(dir + "cygwin1.dll"));
    QDir::setCurrent(dir);

    // Invoke patch
    QString patchCommand =  "apply.exe -p0 -E --binary --input=" + patchName;
    //qDebug() << patchCommand;
    QProcess process;
    process.setStandardOutputFile("patch.log");
    process.start(patchCommand);
    process.waitForFinished(-1);

    // Remove unneeded files
    QFile::remove(patchOutput);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QFile::remove("patch.log");
    QFile::rename(QString(dir + "patch.log"),"patch.log");
    QFile::remove(QString(dir + "apply.exe"));
    QFile::remove(QString(dir + "cygwin1.dll"));

    // Rewrite patch log with CRLF
    QFile file_in("patch.log");
    QFile file_out("temp");
    file_in.open(QIODevice::ReadOnly | QIODevice::Text);
    file_out.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream in_enc(&file_in);
    QTextStream out_enc(&file_out);
    while(!in_enc.atEnd())
    {
        QString line = in_enc.readLine();
        out_enc << line << "\r\n";
    }
    file_in.close();
    file_out.close();
    QFile::remove("patch.log");
    QFile::rename("temp","patch.log");

    // Clear the cache
    clearCache();

    // Warn the user
    QMessageBox::information(this, "Done", QObject::tr("Patch applied. Check 'patch.log' for eventual errors."));
    return true;
}

bool clearCache()
{
    // Getting the cache path
    QString cacheDir;
    QString finalDir = "\\My Games\\Beyond the Sword\\cache\\";
    char* Appdata = getenv("LOCALAPPDATA");
    cacheDir = Appdata + finalDir;
    qDebug() << "Cache found at " << cacheDir;
    QDir cache(cacheDir);
    cache.removeRecursively();

    return 0;
}

QString getConfig(QString option)
{
    QSettings settings("config.ini", QSettings::IniFormat);
    if(!settings.contains(option)) {
        return "error";
    }
    QString value = settings.value(option).toString();
    return value;
}

void setConfig(QString option, QString value)
{
    if(!QFile::exists("config.ini")) {
        QFile conf;
        conf.open(QIODevice::WriteOnly);
    }
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.setValue(option, value);
}

int getLocalVersion(QString modPath)
{
    //** Get version number from "Assets/Python/Contrib/CvModName.py"
    // Open python file
    QString file_path = modPath + "Assets/Python/Contrib/CvModName.py";
    QFile file(file_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in_enc(&file);
    while(!in_enc.atEnd())
    {
        // Go to the mod version
        QString line = in_enc.readLine();
        QString version;
        if(line.contains("modVersion = "))
        {
            version = line.right(5).left(3);
            return version.toInt();
        }
    }
    file.close();

    return 0;
}
