#include "opencc-qml-plugin_plugin.h"
#include "opencc_service.hpp"

#include <qqml.h>

void Opencc_Qml_PluginPlugin::registerTypes(const char *uri)
{
    // @uri xyz.birdzhang.opencc
    qmlRegisterType<OpenCCService>(uri, 1, 0, "OpenCC");
}
