TEMPLATE = lib
TARGET = OpenCC

VERSION = 1.0.0
QT += qml quick
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)

uri = xyz.birdzhang.opencc

CONFIG *= thread


DISTFILES = qmldir



!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}


data.files += data/config

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    data.path = /usr/share/harbour-souniaoime/data/config
    INSTALLS += target qmldir data
}

INCLUDEPATH += $$PWD/deps/gtest-1.7.0/include \
               $$PWD/deps/rapidjson-0.11 \
               $$PWD/deps/tclap-1.2.1 \
               $$PWD/deps/darts-clone

HEADERS += \
    src/BinaryDict.hpp \
    src/CmdLineOutput.hpp \
    src/Common.hpp \
    src/Config.hpp \
    src/Conversion.hpp \
    src/ConversionChain.hpp \
    src/Converter.hpp \
    src/DartsDict.hpp \
    src/Dict.hpp \
    src/DictConverter.hpp \
    src/DictEntry.hpp \
    src/DictGroup.hpp \
    src/Exception.hpp \
    src/Export.hpp \
    src/Lexicon.hpp \
    src/MaxMatchSegmentation.hpp \
    src/Optional.hpp \
    src/PhraseExtract.hpp \
    src/Segmentation.hpp \
    src/Segments.hpp \
    src/SerializableDict.hpp \
    src/SimpleConverter.hpp \
    src/TextDict.hpp \
    src/UTF8StringSlice.hpp \
    src/UTF8Util.hpp \
    src/opencc-qml-plugin_plugin.h \
    src/opencc.h \
    src/opencc_service.hpp

SOURCES += \
    src/BinaryDict.cpp \
    src/Config.cpp \
    src/Conversion.cpp \
    src/ConversionChain.cpp \
    src/Converter.cpp \
    src/DartsDict.cpp \
    src/Dict.cpp \
    src/DictConverter.cpp \
    src/DictEntry.cpp \
    src/DictGroup.cpp \
    src/MaxMatchSegmentation.cpp \
    src/PhraseExtract.cpp \
    src/Segmentation.cpp \
    src/SimpleConverter.cpp \
    src/TextDict.cpp \
    src/UTF8StringSlice.cpp \
    src/UTF8Util.cpp \
    src/opencc-qml-plugin_plugin.cpp \
    src/opencc_service.cpp

OTHER_FILES += \
    CMakeLists.txt \
    src/CMakeLists.txt
