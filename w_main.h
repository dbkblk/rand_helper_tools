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

private:
    Ui::w_main *ui;
};

#endif // W_MAIN_H
