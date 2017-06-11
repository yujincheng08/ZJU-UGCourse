TEMPLATE = subdirs

include(MiniSQL.pri)

SUBDIRS += \
    src/interpreter \
    src/bufferManager \
    src/indexManager \
    src/catalogManager \
    src/recordManager \
    src/API \
    src/main.pro

CONFIG += order
