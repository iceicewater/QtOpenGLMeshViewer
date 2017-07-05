#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setSamples(4);
    format.setOption(QSurfaceFormat::DebugContext);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setVersion(3,3);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;
    w.resize(800,600);
    w.show();

    return a.exec();
}
