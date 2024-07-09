#include "theme.h"

#include <QDir>

#include "utils/json.h"


Theme::Theme(QObject* parent)
    : QObject{ parent }
{
    for (ThemeCategory::Enum category : ThemeCategory::getAllValues())
    {
        QStringList items;

        for (const QFileInfo& file_info : QDir(getCategoryFolder(category)).entryInfoList({ "*.qml" }, QDir::Files))
        {
            items << file_info.baseName();
        }

        available_items_[category] = items;
    }
}

void Theme::load(const QJsonObject& json_object)
{
    Json::mapPropertyToObject(json_object, this);
}

const QString& Theme::getPoint() const
{
    return point_;
}

void Theme::setPoint(const QString& point)
{
    if (point != point_)
    {
        point_ = point;
        emit pointChanged();
    }
}

const QString& Theme::getPointsStorage() const
{
    return points_storage_;
}

void Theme::setPointsStorage(const QString& points_storage)
{
    if (points_storage != points_storage_)
    {
        points_storage_ = points_storage;
        emit pointsStorageChanged();
    }
}

QStringList Theme::getAvailableItems(ThemeCategory::Enum category)
{
    return available_items_.value(category);
}

QString Theme::getItemFilePath(ThemeCategory::Enum category, const QString& base_name)
{
    return QDir().absoluteFilePath(QString("%1/%2.qml").arg(getCategoryFolder(category), base_name));
}

QString Theme::getCategoryFolder(ThemeCategory::Enum category)
{
    QString folder = "qml/theme/%1";

    switch (category)
    {
        case ThemeCategory::Points:
            return folder.arg("points");
        case ThemeCategory::PointsStorage:
            return folder.arg("points_storage");
    }

    return {};
}
