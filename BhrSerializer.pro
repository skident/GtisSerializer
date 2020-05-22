QT -= gui
QT += sql

TARGET = BhrSerializer
#TEMPLATE = lib
TEMPLATE = subdirs

CONFIG += staticlib
CONFIG += c++17

QMAKE_CXXFLAGS += -Wno-gnu-string-literal-operator-template
QMAKE_CXXFLAGS += -Wno-padded


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "third/magic_get/include"
INCLUDEPATH += "third/"
INCLUDEPATH += "include/"

#SOURCES += \
#        src/main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/types/Serializable.h \
    \
    include/serializer/NlohmannJSON.h \
    include/serializer/QtSqlInsertSerializer.h \
    include/serializer/QtSqlSelectSerializer.h \
    include/serializer/StdOstream.h \
    include/serializer/StdString.h \
    include/serializer/TSerializer.h \
    \
    src/Example_1.h \
    src/Example_2.h \
    third/magic_get/doc/pfr.qbk \
    third/magic_get/include/boost/pfr.hpp \
    third/magic_get/include/boost/pfr/detail/cast_to_layout_compatible.hpp \
    third/magic_get/include/boost/pfr/detail/config.hpp \
    third/magic_get/include/boost/pfr/detail/core14.hpp \
    third/magic_get/include/boost/pfr/detail/core14_classic.hpp \
    third/magic_get/include/boost/pfr/detail/core14_loophole.hpp \
    third/magic_get/include/boost/pfr/detail/core17.hpp \
    third/magic_get/include/boost/pfr/detail/core17_generated.hpp \
    third/magic_get/include/boost/pfr/detail/detectors.hpp \
    third/magic_get/include/boost/pfr/detail/fields_count.hpp \
    third/magic_get/include/boost/pfr/detail/flat_tie_from_structure_tuple.hpp \
    third/magic_get/include/boost/pfr/detail/for_each_field_impl.hpp \
    third/magic_get/include/boost/pfr/detail/functional.hpp \
    third/magic_get/include/boost/pfr/detail/io.hpp \
    third/magic_get/include/boost/pfr/detail/make_flat_tuple_of_references.hpp \
    third/magic_get/include/boost/pfr/detail/make_integer_sequence.hpp \
    third/magic_get/include/boost/pfr/detail/offset_based_getter.hpp \
    third/magic_get/include/boost/pfr/detail/rvalue_t.hpp \
    third/magic_get/include/boost/pfr/detail/sequence_tuple.hpp \
    third/magic_get/include/boost/pfr/detail/size_array.hpp \
    third/magic_get/include/boost/pfr/detail/stdtuple.hpp \
    third/magic_get/include/boost/pfr/detail/tie_from_structure_tuple.hpp \
    third/magic_get/include/boost/pfr/flat.hpp \
    third/magic_get/include/boost/pfr/flat/core.hpp \
    third/magic_get/include/boost/pfr/flat/functions_for.hpp \
    third/magic_get/include/boost/pfr/flat/functors.hpp \
    third/magic_get/include/boost/pfr/flat/global_ops.hpp \
    third/magic_get/include/boost/pfr/flat/io.hpp \
    third/magic_get/include/boost/pfr/flat/ops.hpp \
    third/magic_get/include/boost/pfr/flat/tuple_size.hpp \
    third/magic_get/include/boost/pfr/precise.hpp \
    third/magic_get/include/boost/pfr/precise/core.hpp \
    third/magic_get/include/boost/pfr/precise/functions_for.hpp \
    third/magic_get/include/boost/pfr/precise/functors.hpp \
    third/magic_get/include/boost/pfr/precise/global_ops.hpp \
    third/magic_get/include/boost/pfr/precise/io.hpp \
    third/magic_get/include/boost/pfr/precise/ops.hpp \
    third/magic_get/include/boost/pfr/precise/tuple_size.hpp \
    third/nlohmann/json.hpp \
    utils/CompareUtils.h \
    utils/Utils.h

DISTFILES += \
    third/magic_get/README.md \
    third/magic_get/doc/Jamfile.v2 \
    third/magic_get/misc/generate_cpp17.py \
    third/magic_get/test/Jamfile.v2 \
    third/magic_get/test/appveyor.yml
