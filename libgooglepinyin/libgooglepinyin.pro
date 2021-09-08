TEMPLATE = lib
TARGET = QmlPinyin

VERSION = 1.0.0
QT += qml quick
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)

uri = xyz.birdzhang.ime

	
CONFIG *= thread	

HEADERS += \
    src/plugin.h \
    src/dictbuilder.h \
    src/dictdef.h \
    src/dictlist.h \
    src/dicttrie.h \
    src/lpicache.h \
    src/matrixsearch.h \
    src/mystdlib.h \
    src/ngram.h \
    src/pinyinime.h \
    src/searchutility.h \
    src/spellingtable.h \
    src/spellingtrie.h \
    src/splparser.h \
    src/sync.h \
    src/userdict.h \
    src/utf16char.h \
    src/utf16reader.h \
    src/atomdictbase.h \
    src/pinyindecoderservice.h

SOURCES += \
    src/plugin.cpp \
    src/dictbuilder.cpp \
    src/dictlist.cpp \
    src/dicttrie.cpp \
    src/lpicache.cpp \
    src/matrixsearch.cpp \
    src/mystdlib.cpp \
    src/ngram.cpp \
    src/pinyinime.cpp \
    src/searchutility.cpp \
    src/spellingtable.cpp \
    src/spellingtrie.cpp \
    src/splparser.cpp \
    src/sync.cpp \
    src/userdict.cpp \
    src/utf16char.cpp \
    src/utf16reader.cpp \
    src/pinyindecoderservice.cpp \
    src/pinyinime_dictbuilder.cpp

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

data.files += data/dict_pinyin.dat \
                data/dict_pinyin_64.dat

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    data.path = /usr/share/harbour-souniaoime/data
    INSTALLS += target qmldir data
}
