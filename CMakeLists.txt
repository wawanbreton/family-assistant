cmake_minimum_required(VERSION 3.16)

project(family-assistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)
find_package(easyqt REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(family-assistant
    main.cpp
    data/kid.h data/kid.cpp
    data/kidmanager.h data/kidmanager.cpp
    data/preferenceentry.h
    data/preferences.h data/preferences.cpp
    data/task.h data/task.cpp
    data/taskscheduler.h data/taskscheduler.cpp
    data/tasksmodel.h data/tasksmodel.cpp
    data/taskstate.h
    utils/json.h utils/json.cpp

    # Other non-sources files
    credits.md
    .clang-format
    test/config-2kids.json

    # qml files
    qml/Main.qml
    qml/TasksView.qml
    qml/KidTasksView.qml
    qml/KidTasksList.qml
    qml/Task.qml
    qml/Vault.qml
    qml/Point.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(family-assistant PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appfamily-assistant
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(family-assistant
    PRIVATE
    Qt6::Quick
    easyqt::easyqt
)

target_include_directories(family-assistant PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

include(GNUInstallDirs)
install(TARGETS family-assistant
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
