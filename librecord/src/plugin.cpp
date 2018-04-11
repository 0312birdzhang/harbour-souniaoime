#include "plugin.h"
#include "recorder.h"

#include <qqml.h>

void RecorderPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<Recorder> (uri, 1, 0, "Recorder");
}
