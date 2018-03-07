#ifndef PLUGIN_H
#define PLUGIN_H

#include <QQmlExtensionPlugin>

class QmlPinyinPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri = "xyz.birdzhang.ime");
};


#endif // PLUGIN_H
