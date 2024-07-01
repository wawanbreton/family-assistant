#include <QCommandLineParser>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPalette>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "data/kidmanager.h"
#include "data/preferences.h"
#include "data/taskscheduler.h"
#include "data/taskstate.h"
#include "easyqt/datastorage.h"
#include "easyqt/file.h"
#include "easyqt/logger.h"
#include "easyqt/resourcetype.h"


int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QCommandLineParser commands_line_parser;
    QCommandLineOption option_help = commands_line_parser.addHelpOption();

    QCommandLineOption option_data_file("data", "Path of the JSON data file to load", "file_path");
    commands_line_parser.addOption(option_data_file);

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
        qmlRegisterType<easyqt::ResourceType>("FamilyAssistant", 1, 0, "ResourceType");

        easyqt::DataStorage::init(&app);
        easyqt::Logger::init(&app);
        Preferences::init(&app);
        TaskScheduler::init(&app);

        KidManager kid_manager;

        if (commands_line_parser.isSet(option_data_file))
        {
            QString data_file_path = commands_line_parser.value(option_data_file);
            QJsonParseError error;
            auto doc = QJsonDocument::fromJson(easyqt::File::readFile(data_file_path), &error);
            if (error.error == QJsonParseError::NoError)
            {
                QJsonObject json_object = doc.object();
                kid_manager.load(json_object);
            }
            else
            {
                qWarning() << "Error when parsing JSON data file" << data_file_path << ":" << error.errorString();
            }
        }

        QPalette palette = qApp->palette();
        palette.setColor(QPalette::Text, Qt::white);
        qApp->setPalette(palette);

        QQmlApplicationEngine engine;
        engine.rootContext()->setContextProperty("kid_manager", &kid_manager);
        engine.rootContext()->setContextProperty("DataStorage", easyqt::DataStorage::access());
        engine.rootContext()->setContextProperty("fullscreen", commands_line_parser.isSet("fullscreen"));
        engine.load("qml/Main.qml");

        return app.exec();
    }
}
