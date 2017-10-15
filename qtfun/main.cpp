#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    Dialog w;
    w.show();
    w.initCamera(argc, argv);
    return a.exec();
}
