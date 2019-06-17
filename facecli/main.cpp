#include "facecli.h"

#include <QCoreApplication>

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    FaceCli facecli;
    facecli.Run();
    return app.exec();
}
