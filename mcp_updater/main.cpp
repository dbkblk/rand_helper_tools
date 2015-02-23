#include <QCoreApplication>
#include <QtCore>
#include <iostream>

bool installation();

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int menu = 0;
    do{
        qDebug() << "Civilization IV - Rise of Mankind, A New Dawn\nMega. Civ. pack manager\n-----------------------\n";
        qDebug() << "1) Download and install Mega Civ. pack\n2) Check for update\n3) Remove the mod\n";
        std::cin >> menu;

        switch(menu){
            case 1:
                qDebug() << installation();
                break;
        }

    }while(menu != 4);

    return a.exec();
}

bool installation(){
    // Check for existing installation
    QDir mcp_dir("Assets/Modules/Custom_Civilizations_MCP/");
    if(mcp_dir.exists()){
        qDebug() << "Mega. Civ. pack is already installed!";
        return true;
    }
    else{
        qDebug() << "\n ----> Downloading graphic files package <----\n";

        // Wait 2s before to check for update
        QEventLoop loop;
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();

        // Downloading base package
        QProcess process;
        process.execute("checker/curl.exe -J -L -C - -# --retry 10 --insecure http://sourceforge.net/projects/and2mirrors/files/AND2_MCP_BASE_V1.0.7z/download -o graphic.7z");

        qDebug() << "\n ----> Downloading base files package <----\n";
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();
        process.execute("checker/curl.exe -J -L -C - -# --retry 10 --insecure https://github.com/dbkblk/civ4_and2_civmegapack/archive/master.zip -o base.zip");

        // Extracting packages to a temp directory
        qDebug() << "\n ----> Extracting packages <----\n";
        QTimer::singleShot(2000, &loop, SLOT(quit()));
        loop.exec();
        process.execute("checker/7za.exe x \"graphic.7z\" -y -o\"temp/\"");
        process.execute("checker/7za.exe x \"base.zip\" -y -o\"temp/\"");
        QDir temp;
        temp.rename("temp/civ4_and2_civmegapack-master/","temp/");
    }
    return false;
}
