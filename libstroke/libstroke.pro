TEMPLATE = lib
TARGET = QmlStroke

VERSION = 1.0.0
QT += qml quick core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)

uri = xyz.birdzhang.stroke

SOURCES += \
    src/plugin.cpp \
    src/stroke.cpp

HEADERS += \
    src/plugin.h \
    src/stroke.h

RESOURCES += src/stroke.qrc

DISTFILES += qmldir

data.files += src/data/dict.txt
data.path = /usr/share/harbour-souniaoime/data

qmldir.files = qmldir
qmldir.path = $$[QT_INSTALL_QML]/$$replace(uri, \., /)
target.path = $$[QT_INSTALL_QML]/$$replace(uri, \., /)

INSTALLS += target qmldir data

CONFIG *= thread
