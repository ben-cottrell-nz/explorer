cmake_minimum_required(VERSION 3.16)

project(Explorer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Core Svg REQUIRED)

set(SOURCES
    main.cc
    abstract_file_entry_model.cc
    place_dir_pairs_abstract_model.h
    place_dir_pairs_abstract_model.cc
    ui_data.cc
    )

if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    set(SOURCES
        ${SOURCES}
        )
elseif ((${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows"))
    set(SOURCES
        ${SOURCES}
        )
endif()

qt_add_resources(SOURCES resources.qrc)

qt_add_executable(appExplorer
    ${SOURCES}
)
if (ANDROID)
    set_target_properties(appExplorer PROPERTIES QT_ANDROID_PACKAGE_SOURCE_DIR ${CMAKE_SOURCE_DIR}/android)
endif()

qt_add_qml_module(appExplorer
    URI Explorer
    VERSION 1.0
    QML_FILES main.qml Expander.qml
)

target_compile_definitions(appExplorer
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appExplorer
    PRIVATE Qt6::Quick Qt6::Svg)
if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(appExplorer PRIVATE -static-libstdc++ -static-libgcc)
endif()
