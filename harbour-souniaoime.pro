TEMPLATE = subdirs

SUBDIRS += \
    dictbuilder \
    libgooglepinyin \
    miaomiaomiao

libgooglepinyin.depends = \
    dictbuilder

OTHER_FILES += rpm/harbour-souniaoime.spec \
                rpm/harbour-souniaoime.changes
