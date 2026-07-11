TEMPLATE = lib
TARGET = QmlWubi

VERSION = 1.0.0
QT += qml quick core
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)

uri = xyz.birdzhang.wubi

SOURCES += \
    src/wubi.cpp

HEADERS += \
    src/wubi.h

RESOURCES += src/wubi.qrc

DISTFILES += qmldir

data.files += data/dict.txt data/phrases.txt \
    data/dict98.txt data/phrases98.txt \
    data/dictxsj.txt data/phrasesxsj.txt
data.path = /usr/share/harbour-souniaoime/data

qmldir.files = qmldir
qmldir.path = $$[QT_INSTALL_QML]/$$replace(uri, \., /)
target.path = $$[QT_INSTALL_QML]/$$replace(uri, \., /)

INSTALLS += target qmldir data

CONFIG *= thread
