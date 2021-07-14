TEMPLATE = app
TARGET = dictbuilder

CONFIG += sailfishapp


HEADERS += \
    ../libgooglepinyin/src/plugin.h \
    ../libgooglepinyin/src/dictbuilder.h \
    ../libgooglepinyin/src/dictdef.h \
    ../libgooglepinyin/src/dictlist.h \
    ../libgooglepinyin/src/dicttrie.h \
    ../libgooglepinyin/src/lpicache.h \
    ../libgooglepinyin/src/matrixsearch.h \
    ../libgooglepinyin/src/mystdlib.h \
    ../libgooglepinyin/src/ngram.h \
    ../libgooglepinyin/src/pinyinime.h \
    ../libgooglepinyin/src/searchutility.h \
    ../libgooglepinyin/src/spellingtable.h \
    ../libgooglepinyin/src/spellingtrie.h \
    ../libgooglepinyin/src/splparser.h \
    ../libgooglepinyin/src/sync.h \
    ../libgooglepinyin/src/userdict.h \
    ../libgooglepinyin/src/utf16char.h \
    ../libgooglepinyin/src/utf16reader.h \
    ../libgooglepinyin/src/atomdictbase.h \
    ../libgooglepinyin/src/pinyindecoderservice.h

SOURCES += \
    ../libgooglepinyin/src/plugin.cpp \
    ../libgooglepinyin/src/dictbuilder.cpp \
    ../libgooglepinyin/src/dictlist.cpp \
    ../libgooglepinyin/src/dicttrie.cpp \
    ../libgooglepinyin/src/lpicache.cpp \
    ../libgooglepinyin/src/matrixsearch.cpp \
    ../libgooglepinyin/src/mystdlib.cpp \
    ../libgooglepinyin/src/ngram.cpp \
    ../libgooglepinyin/src/pinyinime.cpp \
    ../libgooglepinyin/src/searchutility.cpp \
    ../libgooglepinyin/src/spellingtable.cpp \
    ../libgooglepinyin/src/spellingtrie.cpp \
    ../libgooglepinyin/src/splparser.cpp \
    ../libgooglepinyin/src/sync.cpp \
    ../libgooglepinyin/src/userdict.cpp \
    ../libgooglepinyin/src/utf16char.cpp \
    ../libgooglepinyin/src/utf16reader.cpp \
    ../libgooglepinyin/src/pinyindecoderservice.cpp \
    ../libgooglepinyin/src/pinyinime_dictbuilder.cpp

target.path = /usr/bin
INSTALLS += target