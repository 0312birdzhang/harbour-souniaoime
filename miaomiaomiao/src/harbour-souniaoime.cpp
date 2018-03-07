#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <sailfishapp.h>

//int main(int argc, char *argv[])
//{

//    QScopedPointer<QGuiApplication> app(SailfishApp::application(argc, argv));
//    QScopedPointer<QQuickView> viewer(SailfishApp::createView());
//    viewer->setSource(SailfishApp::pathTo("qml/harbour-blogofwy.qml"));
//    viewer->show();
//    return app->exec();
//}
int main(int argc, char *argv[])
{

    return SailfishApp::main(argc, argv);
}
