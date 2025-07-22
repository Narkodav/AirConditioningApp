#include "controllerwidget.h"
#include "mockcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ControllerWidget widget;
    widget.show();

    MockController controller(&widget);
    return a.exec();
}
