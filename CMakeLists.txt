cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE "F:/github/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(okna VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets )

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mainForms/checkwindow.cpp
        mainForms/checkwindow.h
        mainForms/checkwindow.ui
        WinEdit/WinEditor.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(okna
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        Database.h Database.cpp
        formsInput/clientsedit.h formsInput/clientsedit.cpp formsInput/clientsedit.ui
        mainForms/orderview.h mainForms/orderview.cpp mainForms/orderview.ui
        util/Editform.h
        util/QT.h
        formsInput/componentsform.h formsInput/componentsform.cpp formsInput/componentsform.ui
        util/QtViewer.h
        mainForms/windoweditor.h mainForms/windoweditor.cpp mainForms/windoweditor.ui
        formsInput/newproduct.h formsInput/newproduct.cpp formsInput/newproduct.ui



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TRGET okna APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(okna SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(okna
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(okna PRIVATE Qt${QT_VERSION_MAJOR}::Widgets )

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.okna)
endif()
set_target_properties(okna PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS okna
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(okna)
endif()

find_package(SqliteOrm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE sqlite_orm::sqlite_orm)
find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE unofficial::sqlite3::sqlite3)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5.1 REQUIRED graphics audio window system)

# target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
# target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
# add_compile_options(-fsanitize=address)
# add_link_options(-fsanitize=address)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-window sfml-system)
#target_compile_options(okna PUBLIC "/Zc:__cplusplus")

