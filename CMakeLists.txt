cmake_minimum_required(VERSION 3.16)

project(family-assistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick ShaderTools)
find_package(easyqt REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

set(SHADER_FILES
    resources/shaders/text_outline.frag
)

qt_add_executable(family-assistant
    # Non-sources files
    credits.md
    .clang-format
    test/config-2kids.json

    # CPP source files
    main.cpp
    data/kid.h data/kid.cpp
    data/kidmanager.h data/kidmanager.cpp
    data/preferenceentry.h
    data/preferences.h data/preferences.cpp
    data/task.h data/task.cpp
    data/taskscheduler.h data/taskscheduler.cpp
    data/tasksmodel.h data/tasksmodel.cpp
    data/taskstate.h
    data/theme.h data/theme.cpp
    data/themecategory.h
    utils/json.h utils/json.cpp

    # Shaders
    ${SHADER_FILES}

    # qml files
    qml/Main.qml
    qml/TasksView.qml
    qml/KidTasksView.qml
    qml/KidTasksList.qml
    qml/Task.qml
    qml/Vault.qml
    qml/Point.qml
    qml/Rewards.qml
    qml/components/NormalText.qml
    qml/components/BaseText.qml
    qml/components/LargeText.qml
    qml/components/StyledGradient.qml
    qml/components/VerticalTabView.qml
    qml/components/Tab.qml
    qml/components/CornerButton.qml
    qml/components/TextOutline.qml
    qml/components/ButtonShadow.qml
    qml/components/Carousel.qml
    qml/components/ConfirmButton.qml
    qml/theme/points/DollarCoin.qml
    qml/theme/points/StarCoin.qml
    qml/theme/points/Burger.qml
    qml/theme/points/Cash.qml
    qml/theme/points/ChristmasGiftbox.qml
    qml/theme/points/Euro.qml
    qml/theme/points/Flower.qml
    qml/theme/points/GemRed.qml
    qml/theme/points/GemRedSparkling.qml
    qml/theme/points/GemBlue.qml
    qml/theme/points/GemBlueSparkling.qml
    qml/theme/points/GemGreen.qml
    qml/theme/points/GemGreenSparkling.qml
    qml/theme/points/GemHeart.qml
    qml/theme/points/GemHeartSparkling.qml
    qml/theme/points/GiftBox.qml
    qml/theme/points/Medal.qml
    qml/theme/points/MoneyBag.qml
    qml/theme/points/Poop.qml
    qml/theme/points/Rainbow.qml
    qml/theme/points/Ruby.qml
    qml/theme/points/RubySparkling.qml
    qml/theme/points/Trophy.qml
    qml/theme/points/Unicorn.qml
    qml/theme/points/Zirconia.qml
    qml/theme/points/ZirconiaSparkling.qml
    qml/theme/points_storage/Bank.qml
    qml/theme/points_storage/Bank2.qml
    qml/theme/points_storage/Diamonds.qml
    qml/theme/points_storage/GoldPile.qml
    qml/theme/points_storage/PiggyBank.qml
    qml/theme/points_storage/Refrigerator.qml
    qml/theme/points_storage/SafeBox.qml
    qml/theme/points_storage/StrongBox.qml
    qml/theme/points_storage/Treasure.qml
    qml/theme/points_storage/Treasure2.qml
    qml/theme/points_storage/Wallet.qml
    qml/theme/points_storage/Wealth.qml
    qml/rewards/Points.qml
    qml/rewards/ThemeItemSelector.qml
    qml/PointsCounter.qml
    qml/KidName.qml
    qml/PointsTransfer.qml
    qml/rewards/RewardsTab.qml
    qml/components/ChangeableImage.qml
)

qt6_add_resources(family-assistant "family-assistant"
    PREFIX
        "/"
)

qt6_add_shaders(family-assistant "family-assistant-shaders"
    PREFIX
        "/"
    FILES
        ${SHADER_FILES}
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

install(DIRECTORY qml DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_BINDIR})
