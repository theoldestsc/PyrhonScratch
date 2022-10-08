
#include "openGLWidget.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize( 24 );
    format.setMajorVersion(4);
    format.setMinorVersion(5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    format.setSwapInterval(1);
    format.setDefaultFormat(format);
    MyGLWidget w;
    w.show();

    return a.exec();
}
