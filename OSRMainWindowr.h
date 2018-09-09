#ifndef OSRMAINWINDOWR_H
#define OSRMAINWINDOWR_H

#include <QMainWindow>

namespace Ui {
class OSR;
}

class OSR : public QMainWindow
{
    Q_OBJECT

public:
    explicit OSR(QWidget *parent = 0);
    ~OSR();

private:
    Ui::OSR *ui;
};

#endif // OSRMAINWINDOWR_H
