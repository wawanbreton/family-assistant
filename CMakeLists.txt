cmake_minimum_required(VERSION 3.16)

project(family-assistant VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick ShaderTools)
find_package(easyqt REQUIRED)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message("####### Simulator mode")
    add_compile_definitions(ENV_SIMULATOR)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Widgets)

    set(EXTRA_SOURCES
        hardware/simulated/simulatedhardware.h hardware/simulated/simulatedhardware.cpp
        hardware/simulated/simulatedhardwarewidget.h hardware/simulated/simulatedhardwarewidget.cpp hardware/simulated/simulatedhardwarewidget.ui
    )
else()
    message("####### Physical mode")
endif()

qt_standard_project_setup(REQUIRES 6.5)

set(SHADER_FILES
    resources/shaders/text_outline.frag
    resources/shaders/text_mask.frag
)

qt_add_executable(family-assistant
    # Non-sources files
    credits.md
    .clang-format
    test/config-2kids.json

    # CPP source files
    main.cpp
    data/activetask.h data/activetask.cpp
    data/dayofweek.h
    data/kid.h data/kid.cpp
    data/kidmanager.h data/kidmanager.cpp
    data/preferenceentry.h
    data/preferences.h data/preferences.cpp
    data/duetask.h data/duetask.cpp
    data/task.h data/task.cpp

    data/taskoccurence.h
    data/taskscheduler.h data/taskscheduler.cpp
    data/tasksmodel.h data/tasksmodel.cpp
    data/taskstate.h
    data/theme.h data/theme.cpp
    data/themecategory.h
    hardware/hardware.h hardware/hardware.cpp

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
    qml/KidName.qml
    qml/PointsCounter.qml
    qml/PointsTransfer.qml
    qml/components/ChangeableImage.qml
    qml/components/NormalText.qml
    qml/components/BaseText.qml
    qml/components/Glow.qml
    qml/components/LargeText.qml
    qml/components/StyledGradient.qml
    qml/components/VerticalTabView.qml
    qml/components/RandomMirror.qml
    qml/components/Tab.qml
    qml/components/CornerButton.qml
    qml/components/TextOutline.qml
    qml/components/ButtonShadow.qml
    qml/components/Carousel.qml
    qml/components/ConfirmButton.qml
    qml/rewards/Backgrounds.qml
    qml/rewards/NameTexts.qml
    qml/rewards/Points.qml
    qml/rewards/RewardsTab.qml
    qml/rewards/ThemeItemSelector.qml
    qml/settings/TaskControl.qml
    qml/theme/point/DollarCoin.qml
    qml/theme/point/StarCoin.qml
    qml/theme/point/Burger.qml
    qml/theme/point/Cash.qml
    qml/theme/point/ChristmasGiftbox.qml
    qml/theme/point/Euro.qml
    qml/theme/point/Flower.qml
    qml/theme/point/GemRed.qml
    qml/theme/point/GemRedSparkling.qml
    qml/theme/point/GemBlue.qml
    qml/theme/point/GemBlueSparkling.qml
    qml/theme/point/GemGreen.qml
    qml/theme/point/GemGreenSparkling.qml
    qml/theme/point/GemHeart.qml
    qml/theme/point/GemHeartSparkling.qml
    qml/theme/point/GiftBox.qml
    qml/theme/point/Medal.qml
    qml/theme/point/MoneyBag.qml
    qml/theme/point/Poop.qml
    qml/theme/point/Rainbow.qml
    qml/theme/point/Ruby.qml
    qml/theme/point/RubySparkling.qml
    qml/theme/point/Trophy.qml
    qml/theme/point/Unicorn.qml
    qml/theme/point/Zirconia.qml
    qml/theme/point/ZirconiaSparkling.qml
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
    qml/theme/background/White.qml
    qml/theme/background/Blue.qml
    qml/theme/background/Green.qml
    qml/theme/background/Orange.qml
    qml/theme/background/Pink.qml
    qml/theme/background/Sky.qml
    qml/theme/background/SkyWithClouds.qml
    qml/theme/background/Farm.qml
    qml/theme/font/Ubuntu.qml
    qml/theme/font/SuperCrafty.qml
    qml/theme/font/SuperPlayful.qml
    qml/theme/font/AmberyGarden.qml
    qml/theme/font/SuperMystery.qml
    qml/theme/font/SupplyCenter.qml
    qml/theme/font/HappyMemories.qml
    qml/theme/font/KogniGear.qml
    qml/theme/font/SuperNormal.qml
    qml/theme/font/SuperScribble.qml
    qml/theme/font/ZtTalk.qml
    qml/theme/font/Agreloy.qml
    qml/theme/font/BudmoJiggler.qml
    qml/theme/font/BurnstownDam.qml
    qml/theme/font/Celtica.qml
    qml/theme/font/DottyShadow.qml
    qml/theme/font/GoodTiming.qml
    qml/theme/font/Heartland.qml
    qml/theme/font/RetroStereoWide.qml
    qml/theme/font/Saddlebag.qml
    qml/theme/font/SnowtopCaps.qml
    qml/theme/font/TopSecret.qml
    qml/theme/font/UrbanSlick.qml
    qml/theme/font/Welbutrin.qml
    qml/theme/text_style/Black.qml
    qml/theme/text_style/Blue.qml
    qml/theme/text_style/Green.qml
    qml/theme/text_style/Orange.qml
    qml/theme/text_style/Pink.qml
    qml/theme/text_style/White.qml
    qml/theme/text_style/Rainbow.qml
    qml/theme/text_style/RainbowAnimated.qml
    qml/Settings.qml
    qml/PopupDialog.qml

    ${EXTRA_SOURCES}
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

target_compile_features(family-assistant PRIVATE cxx_std_23)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(family-assistant
        PRIVATE
        Qt6::Widgets
    )
endif()

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
