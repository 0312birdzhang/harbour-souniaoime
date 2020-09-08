#ifndef OPENCCQMLPLUGIN_PLUGIN_H
#define OPENCCQMLPLUGIN_PLUGIN_H

#pragma once

#include <QQmlExtensionPlugin>

class Opencc_Qml_PluginPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri);
};

#endif // OPENCCQMLPLUGIN_PLUGIN_H
