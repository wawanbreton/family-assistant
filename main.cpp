#include <QApplication>
#include <QCommandLineParser>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "data/kid.h"
#include "data/kidmanager.h"
#include "data/metamanager.h"
#include "data/taskscheduler.h"
#include "data/taskstate.h"
#include "data/theme.h"
#include "data/themecategory.h"
#include "easyqt/datastorage.h"
#include "easyqt/resourcetype.h"
#include "hardware/hardware.h"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

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

        MetaManager::init(&app);

        if (commands_line_parser.isSet(option_data_file))
        {
            MetaManager::access()->load(commands_line_parser.value(option_data_file));
        }
        else
        {
            MetaManager::access()->load();
        }

        bool all_due_tasks_empty = true;
        bool obsolete_tasks = false;
        const auto current_date = QDateTime::currentDateTime().date();
        for (const Kid* kid : KidManager::access()->getKids())
        {
            if (kid->hasTasks())
            {
                all_due_tasks_empty = false;
                if (kid->getTasks()->getDueDate() != current_date)
                {
                    obsolete_tasks = true;
                }
            }
        }

        const bool reset_tasks
            = obsolete_tasks || all_due_tasks_empty || commands_line_parser.isSet(option_reset_tasks);
        TaskScheduler::access()->start(reset_tasks);

        Theme global_theme;

        QPalette palette = qApp->palette();
        palette.setColor(QPalette::Text, Qt::white);
        qApp->setPalette(palette);

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("kid_manager", KidManager::access());
        engine.rootContext()->setContextProperty("tasks_scheduler", TaskScheduler::access());
        engine.rootContext()->setContextProperty("hardware", Hardware::access());
        engine.rootContext()->setContextProperty("DataStorage", easyqt::DataStorage::access());
        engine.rootContext()->setContextProperty("Theme", &global_theme);
        engine.rootContext()->setContextProperty("fullscreen", commands_line_parser.isSet("fullscreen"));
        engine.load("qml/Main.qml");

        return app.exec();
    }
}
