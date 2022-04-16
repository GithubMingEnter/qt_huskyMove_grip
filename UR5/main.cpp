#include "ur5.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UR5 w;
    w.show();

    return a.exec();
}
