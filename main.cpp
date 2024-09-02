#include <QCommandLineParser>
#ifdef QT_WIDGETS_LIB
#include <QApplication>
#else
#include <QGuiApplication>
#endif
#include <QJsonDocument>
#include <QJsonObject>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "data/kid.h"
#include "data/kidmanager.h"
#include "data/preferences.h"
#include "data/taskscheduler.h"
#include "data/taskstate.h"
#include "data/theme.h"
#include "data/themecategory.h"
#include "easyqt/datastorage.h"
#include "easyqt/file.h"
#include "easyqt/logger.h"
#include "easyqt/resourcetype.h"
#include "hardware/hardware.h"


int main(int argc, char* argv[])
{
#ifdef QT_WIDGETS_LIB
    QApplication app(argc, argv);
#else
    QGuiApplication app(argc, argv);
#endif
    QCommandLineParser commands_line_parser;
    QCommandLineOption option_help = commands_line_parser.addHelpOption();

    QCommandLineOption option_data_file("data", "Path of the JSON data file to load", "file_path");
    commands_line_parser.addOption(option_data_file);

    QCommandLineOption option_reset_tasks("reset-tasks", "Ignore loaded due tasks and create a new planning");
    commands_line_parser.addOption(option_reset_tasks);

    QCommandLineOption option_fullscreen("fullscreen", "Display the application in full screen");
    commands_line_parser.addOption(option_fullscreen);

    commands_line_parser.parse(app.arguments());

    if (commands_line_parser.isSet(option_help))
    {
        commands_line_parser.showHelp();
    }
    else
    {
        qmlRegisterType<TaskState>("FamilyAssistant", 1, 0, "TaskState");
        qmlRegisterType<ThemeCategory>("FamilyAssistant", 1, 0, "ThemeCategory");
        qmlRegisterType<easyqt::ResourceType>("FamilyAssistant", 1, 0, "ResourceType");

        easyqt::DataStorage::init(&app);
        easyqt::Logger::init(&app);
        Preferences::init(&app);
        KidManager::init(&app);
        TaskScheduler::init(&app);
        Hardware::init(&app);

        Theme global_theme;

        if (commands_line_parser.isSet(option_data_file))
        {
            QString data_file_path = commands_line_parser.value(option_data_file);
            QJsonParseError error;
            auto doc = QJsonDocument::fromJson(easyqt::File::readFile(data_file_path), &error);
            if (error.error == QJsonParseError::NoError)
            {
                QJsonObject json_object = doc.object();
                KidManager::access()->load(json_object);
                TaskScheduler::access()->load(json_object);
            }
            else
            {
                qWarning() << "Error when parsing JSON data file" << data_file_path << ":" << error.errorString();
            }
        }

        const bool all_due_tasks_empty = std::all_of(
            KidManager::access()->getKids().begin(),
            KidManager::access()->getKids().end(),
            [](const Kid* kid) { return ! kid->hasTasks(); });
        const bool reset_tasks = all_due_tasks_empty || commands_line_parser.isSet(option_reset_tasks);
        TaskScheduler::access()->start(reset_tasks);

        QPalette palette = qApp->palette();
        palette.setColor(QPalette::Text, Qt::white);
        qApp->setPalette(palette);

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("kid_manager", KidManager::access());
        engine.rootContext()->setContextProperty("hardware", Hardware::access());
        engine.rootContext()->setContextProperty("DataStorage", easyqt::DataStorage::access());
        engine.rootContext()->setContextProperty("Theme", &global_theme);
        engine.rootContext()->setContextProperty("fullscreen", commands_line_parser.isSet("fullscreen"));
        engine.load("qml/Main.qml");

        return app.exec();
    }
}
