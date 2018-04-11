#ifndef PLUGIN_H
#define PLUGIN_H

#include <QQmlExtensionPlugin>

class RecorderPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri = "xyz.birdzhang.ime.stt");
};


#endif // PLUGIN_H
