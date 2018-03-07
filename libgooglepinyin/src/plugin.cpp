#include "plugin.h"
#include "pinyindecoderservice.h"

#include <qqml.h>

void QmlPinyinPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<PinyinDecoderService> (uri, 1, 0, "QmlPinyin");
}
