cmake_minimum_required(VERSION 3.24)

project(ImproveYourAIBot VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets WebEngineWidgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets WebEngineWidgets LinguistTools)

option(DEVELPOMENT_MODE "Enable development mode" OFF)

add_subdirectory(QtVideoTranscriber)
#add_subdirectory(whisper.cpp)

if(DEVELPOMENT_MODE)
    add_definitions(-DSERVER_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/assets")
endif()

set(TS_FILES ImproveYourAIBot_it_IT.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        sidebar.h sidebar.cpp
        ${TS_FILES}
        webserver.h
        webserver.cpp
        httplib.h
)


    qt_add_executable(ImproveYourAIBot
        ${PROJECT_SOURCES}
        resources.qrc
        webviewmanager.h webviewmanager.cpp
        optionsmanager.h
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::WebEngineWidgets VideoTranscriber)

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.ImproveYourAIBot)
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
       $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/assets
      $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
    #COMMAND ${Qt${QT_VERSION_MAJOR}_INSTALL_DIR}/bin/windeployqt --dir $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_FILE:${PROJECT_NAME}>
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
