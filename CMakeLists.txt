cmake_minimum_required(VERSION 3.20)
project(FontColor LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# version
set(FC_NAME           FontColor)
set(FC_VERSION_MAJOR  1)
set(FC_VERSION_MINOR  0)
set(FC_VERSION_STATUS Dev)
configure_file(config.h.in config.h @ONLY)

if (UNIX)
    add_compile_options(-Wall -Werror -Wextra)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-Weffc++)
    endif ()

    add_link_options(-Wl,--unresolved-symbols=report-all)
else () # Windows
    if (MINGW)
        add_compile_options(-Wall -Werror -Wextra)

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(-Weffc++)
        endif ()

        add_link_options(-Wl,--unresolved-symbols=report-all)
    else () # msvc or icc
        add_compile_options(/W4 /utf-8)
    endif (MINGW)
endif (UNIX) # end if (UNIX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.9 COMPONENTS Widgets Quick QuickControls2 REQUIRED)
find_package(Fontconfig REQUIRED)
find_package(SQLite3 REQUIRED)
set(QT_QML_GENERATE_QMLLS_INI on)

qt_add_executable(FC_NAME
    main.cpp
)

qt_add_qml_module(FC_NAME
    URI FC
    QML_FILES
        main.qml
        components/TitleText.qml
        components/ToolTipButton.qml

    SOURCES
        backend/clipboard.cpp
        backend/clipboard.hpp
        backend/fontdatabase.cpp
        backend/fontdatabase.hpp
        backend/sqlhandler.cpp
        backend/sqlhandler.hpp
        backend/utils.cpp
        backend/utils.hpp

    RESOURCES
        fontColor.ico
        RESOURCES config.h.in
)

target_link_libraries(FC_NAME
    PRIVATE

    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    SQLite::SQLite3
    Fontconfig::Fontconfig
)

if (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
    target_link_options(FC_NAME PRIVATE $<$<CONFIG:Debug>:/INCREMENTAL>)
    target_compile_options(FC_NAME PRIVATE $<$<CONFIG:Debug>:/ZI>)
endif (MSVC AND WIN32 AND NOT MSVC_VERSION VERSION_LESS 142)
