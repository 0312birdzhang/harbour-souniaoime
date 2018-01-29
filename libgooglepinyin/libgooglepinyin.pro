TEMPLATE = lib

TARGET = libgooglepinyin

CONFIG += staticlib

QT       -= gui

SOURCES += \
    src\dictbuilder.cpp \
    src\dictlist.cpp \
    src\dicttrie.cpp \
    src\lpicache.cpp \
    src\matrixsearch.cpp \
    src\mystdlib.cpp \
    src\ngram.cpp \
    src\pinyinime.cpp \
    src\searchutility.cpp \
    src\spellingtable.cpp \
    src\spellingtrie.cpp \
    src\splparser.cpp \
    src\sync.cpp \
    src\userdict.cpp \
    src\utf16char.cpp \
    src\utf16reader.cpp
HEADERS += \
    src\atomdictbase.h \
    src\dictbuilder.h \
    src\dictdef.h \
    src\dictlist.h \
    src\dicttrie.h \
    src\lpicache.h \
    src\matrixsearch.h \
    src\mystdlib.h \
    src\ngram.h \
    src\pinyinime.h \
    src\searchutility.h \
    src\spellingtable.h \
    src\spellingtrie.h \
    src\splparser.h \
    src\sync.h \
    src\userdict.h \
    src\utf16char.h \
    src\utf16reader.h
