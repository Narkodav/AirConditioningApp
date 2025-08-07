#include "controllerwidget.h"


#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerWidget widget;
    widget.show();

    return a.exec();
}
