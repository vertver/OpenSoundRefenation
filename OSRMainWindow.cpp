#include "OSRMainWindowr.h"
#include "ui_OSRMainWindow.h"

OSR::OSR(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OSR)
{
    ui->setupUi(this);
}

OSR::~OSR()
{
    delete ui;
}
