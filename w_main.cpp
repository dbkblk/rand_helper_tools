#include "w_main.h"
#include "ui_w_main.h"

w_main::w_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::w_main)
{
    ui->setupUi(this);
}

w_main::~w_main()
{
    delete ui;
}
