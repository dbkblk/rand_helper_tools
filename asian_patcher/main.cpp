#include <QtCore>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Civ. IV Asian language patcher v1.0; \"A New Dawn Resurection, dbkblk\"";

    // Read the executable location in checker_config.ini
    QSettings settings("checker/checker_config.ini", QSettings::IniFormat);
    if(!settings.contains("Main/ExecutablePath")) {
        qDebug() << "Please set your executable path in the launcher before to launch the patch.";
        return a.exec();
    }
    QString executable = settings.value("Main/ExecutablePath").toString();
    QFile exe(executable);
    if(!exe.exists())
    {
        qDebug() << "The executable set in the launcher doesn't exists.";
        return a.exec();
    }

    qDebug() << "Executable found at" << executable;

    // Get tool paths
    const QString TOOL_XZ = "checker/xz.exe -d ";
    const QString TOOL_TAR = "checker/tar.exe -xpf ";

    // Extract the asian executable next to the english one
    qDebug() << "Extracting package";
    QStringList path = executable.split("/");
    path.removeLast();
    QString dirPath = path.join("/");
    QFile::copy("checker/Civ4BeyondSword_Asian.tar.xz","checker/temp.tar.xz");
    QProcess unzip;
    unzip.execute(TOOL_XZ + "checker/temp.tar.xz");
    unzip.execute(TOOL_TAR + "checker/temp.tar");
    QFile::remove("checker/temp.tar");
    QFile::remove(QString(dirPath + "/Civ4BeyondSword_Asian.exe"));
    QFile::rename("Civ4BeyondSword_Asian.exe",QString(dirPath + "/Civ4BeyondSword_Asian.exe"));

    // Generate symlinks
    qDebug() << "Generating symlinks";
    path.removeLast();
    QString civ4Path = path.join("/");
    QString myGames = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/My Games/";
    QString link1 = QString("mklink /d \"%1\" \"%2\"").arg(civ4Path + "/Beyond the Sword(J)").arg(civ4Path + "/Beyond the Sword");
    QString link2 = QString("mklink /d \"%1\" \"%2\"").arg(civ4Path + "/Warlords(J)").arg(civ4Path + "/Warlords");
    QString link3 = QString("mklink /d \"%1\" \"%2\"").arg(myGames + "/Beyond the Sword(J)").arg(myGames + "/Beyond the Sword");
    if(!QFile::exists(QString(civ4Path + "/Beyond the Sword(J)")))
    {
        system(link1.toStdString().c_str());
    }
    else
    {
        qDebug() << "BTS symlink already created";
    }
    if(!QFile::exists(QString(civ4Path + "/Warlords(J)")))
    {
        system(link2.toStdString().c_str());
    }
    else
    {
        qDebug() << "Warlords symlink already created";
    }
    if(!QFile::exists(QString(myGames + "/Beyond the Sword(J)")))
    {
        system(link3.toStdString().c_str());
    }
    else
    {
        // Check if this is a symlink or a real folder
        if(QFile::symLinkTarget(myGames + "/Beyond the Sword(J)").isEmpty() && !QFile::exists(QString(myGames + "/Beyond the Sword")))
        { // Rename the folder to European style and make a symlink
            QDir dir;
            dir.rename(QString(myGames + "/Beyond the Sword(J)"),QString(myGames + "/Beyond the Sword"));
            system(link3.toStdString().c_str());
        }
        else
        {
            qDebug() << "\"My Games\" symlink already created";
        }
    }

    // Auto-close process
    qDebug() << "This window will auto-close in 5s";
    system("SLEEP 5");

    return 0;
}
