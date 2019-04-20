QT += qml quick quickcontrols2 widgets gui sql
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    backend/fontdatabase.cpp \
    backend/clipboard.cpp \
    backend/utils.cpp \
    backend/sqlhandler.cpp

RESOURCES += qml.qrc

linux {
    CONFIG += link_pkgconfig
    PKGCONFIG += fontconfig
}

win32 {
    INCLUDEPATH += "M:/path/to/your/fontconfig/include"
    LIBS += "M:/path/to/your/fontconfig.lib" \
            "M:/path/to/your/libfontconfig.lib"
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    backend/fontdatabase.hpp \
    backend/clipboard.hpp \
    backend/utils.hpp \
    backend/sqlhandler.hpp
