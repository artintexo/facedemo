#include "facegui.h"

#include <QApplication>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    FaceGui facegui;
    facegui.show();
    return app.exec();
}
