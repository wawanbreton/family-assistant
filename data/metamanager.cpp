#include "data/metamanager.h"

#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <easyqt/datastorage.h>
#include <easyqt/file.h>
#include <easyqt/logger.h>

#include "data/kidmanager.h"
#include "data/preferences.h"
#include "data/taskscheduler.h"
#include "hardware/hardware.h"


SINGLETON_IMPL(MetaManager)
SINGLETON_DESTRUCTOR_IMPL(MetaManager)

MetaManager::MetaManager(QObject* parent)
    : QObject(parent)
    , timer_data_changed_(new QTimer(this))
{
    easyqt::DataStorage::init(this);
    easyqt::Logger::init(this);
    Preferences::init(this);
    KidManager::init(this);
    TaskScheduler::init(this);
    Hardware::init(this);

    data_file_path_ = easyqt::DataStorage::configFile("config.json");

    timer_data_changed_->setSingleShot(true);
    timer_data_changed_->setInterval(1000);
    connect(timer_data_changed_, &QTimer::timeout, this, &MetaManager::save);

    connect(KidManager::access(), &KidManager::changed, timer_data_changed_, qOverload<>(&QTimer::start));
}

void MetaManager::load(const QString& file_path)
{
    const QString actual_file_path = file_path.isEmpty() ? data_file_path_ : file_path;

    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(easyqt::File::readFile(actual_file_path), &error);
    if (error.error == QJsonParseError::NoError)
    {
        QJsonObject json_object = doc.object();
        TaskScheduler::access()->load(json_object);
        KidManager::access()->load(json_object);
    }
    else
    {
        qWarning() << "Error when parsing JSON data file" << file_path << ":" << error.errorString();
    }
}

void MetaManager::save()
{
    QJsonDocument doc;

    QJsonObject object;
    KidManager::get()->save(object);
    TaskScheduler::get()->save(object);
    doc.setObject(object);

    easyqt::File::writeToFile(data_file_path_, doc.toJson(), easyqt::WriteFileMode::Safe);
}
