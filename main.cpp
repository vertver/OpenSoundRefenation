#include "OSRMainWindowr.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OSR w;
    w.show();

    return a.exec();
}
