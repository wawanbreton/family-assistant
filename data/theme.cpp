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
        emit itemChanged(ThemeCategory::Point);
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
        emit itemChanged(ThemeCategory::PointsStorage);
    }
}

const QString& Theme::getBackground() const
{
    return background_;
}

void Theme::setBackground(const QString& background)
{
    if (background != background_)
    {
        background_ = background;
        emit backgroundChanged();
        emit itemChanged(ThemeCategory::Background);
    }
}

QString Theme::getItem(ThemeCategory::Enum category)
{
    switch (category)
    {
        case ThemeCategory::Point:
            return getPoint();
        case ThemeCategory::PointsStorage:
            return getPointsStorage();
        case ThemeCategory::Background:
            return getBackground();
    }

    qCritical() << "Invalid category" << category;
    return {};
}

void Theme::setItem(ThemeCategory::Enum category, const QString& item)
{
    switch (category)
    {
        case ThemeCategory::Point:
            return setPoint(item);
        case ThemeCategory::PointsStorage:
            return setPointsStorage(item);
        case ThemeCategory::Background:
            return setBackground(item);
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
        case ThemeCategory::Point:
            return folder.arg("point");
        case ThemeCategory::PointsStorage:
            return folder.arg("points_storage");
        case ThemeCategory::Background:
            return folder.arg("background");
    }

    return {};
}
