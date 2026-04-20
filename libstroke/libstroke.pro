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

DISTFILES += qmldir

qmldir.files = qmldir
qmldir.path = $$[QT_INSTALL_QML]/$$replace(uri, \., /)

INSTALLS += qmldir

CONFIG *= thread