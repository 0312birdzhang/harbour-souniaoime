TEMPLATE = app
TARGET = harbour-souniaoime

CONFIG += sailfishapp


SOURCES += src/harbour-souniaoime.cpp

OTHER_FILES += qml/harbour-souniaoime.qml


target.path = /usr/bin

qml.files = qml
qml.path = /usr/share/harbour-souniaoime

desktop.files = harbour-souniaoime.desktop
desktop.path = /usr/share/applications

icon.files = harbour-souniaoime.png
icon.path = /usr/share/icons/hicolor/86x86/apps

maliit.files = maliit
maliit.path = /usr/share
INSTALLS += target qml desktop icon maliit

DISTFILES += \
    maliit/plugins/com/jolla/SouniaoHandler.qml \
    maliit/plugins/com/jolla/layouts/zh_cn_souniaoime.qml \
    maliit/plugins/com/jolla/layouts/layouts_sz_google_pinyin.conf
