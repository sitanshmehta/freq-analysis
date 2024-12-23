cmake_minimum_required(VERSION 3.16)

project(freq-analysis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Quick Qml Multimedia)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appfreq-analysis
    main.cpp
    audio.cpp
    data_input.cpp
)

target_link_libraries(appfreq-analysis PRIVATE Qt6::Core Qt6::Qml Qt6::Multimedia)

qt_add_qml_module(appfreq-analysis
    URI freq-analysis
    VERSION 1.0
    QML_FILES
        Main.qml
    SOURCES
        data_input.h
        data_input.cpp
        audio.h
        audio.cpp
    RESOURCES
        notes.txt
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appfreq-analysis PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfreq-analysis
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appfreq-analysis
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appfreq-analysis
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
