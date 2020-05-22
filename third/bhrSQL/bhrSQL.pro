#-------------------------------------------------
#
# Project created by QtCreator 2020-01-31T23:08:23
#
#-------------------------------------------------

QT -= gui

CONFIG += c++17

QMAKE_CXXFLAGS += -Wno-padded
QMAKE_CXXFLAGS += -Wno-gnu-string-literal-operator-template

TARGET = bhrSQL
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += "include/"
INCLUDEPATH += "include/bhrSQL/"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    include/bhrSQL/SqlAlterTable.h \
    include/bhrSQL/SqlCondition.h \
    include/bhrSQL/SqlCreateTable.h \
    include/bhrSQL/SqlDelete.h \
    include/bhrSQL/SqlDropTable.h \
    include/bhrSQL/SqlFrom.h \
    include/bhrSQL/SqlGroupBy.h \
    include/bhrSQL/SqlHelper.h \
    include/bhrSQL/SqlInsert.h \
    include/bhrSQL/SqlLimit.h \
    include/bhrSQL/SqlOrderBy.h \
    include/bhrSQL/SqlSelect.h \
    include/bhrSQL/SqlUpdate.h \
    include/bhrSQL/SqlWhere.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DESTDIR = ../build

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc

SOURCES += \
    src/test.cpp
