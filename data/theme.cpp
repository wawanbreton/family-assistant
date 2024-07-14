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

    connect(this, &Theme::pointChanged, this, [this] { emit itemChanged(ThemeCategory::Point); });
    connect(this, &Theme::pointsStorageChanged, this, [this] { emit itemChanged(ThemeCategory::PointsStorage); });
    connect(this, &Theme::backgroundChanged, this, [this] { emit itemChanged(ThemeCategory::Background); });
    connect(this, &Theme::fontChanged, this, [this] { emit itemChanged(ThemeCategory::Font); });
    connect(this, &Theme::textStyleChanged, this, [this] { emit itemChanged(ThemeCategory::TextStyle); });
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
    }
}

const QString& Theme::getFont() const
{
    return font_;
}

void Theme::setFont(const QString& font)
{
    if (font != font_)
    {
        font_ = font;
        emit fontChanged();
    }
}

const QString& Theme::getTextStyle() const
{
    return text_style_;
}

void Theme::setTextStyle(const QString& text_style)
{
    if (text_style != text_style_)
    {
        text_style_ = text_style;
        emit textStyleChanged();
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
        case ThemeCategory::Font:
            return getFont();
        case ThemeCategory::TextStyle:
            return getTextStyle();
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
        case ThemeCategory::Font:
            return setFont(item);
        case ThemeCategory::TextStyle:
            return setTextStyle(item);
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
        case ThemeCategory::Font:
            return folder.arg("font");
        case ThemeCategory::TextStyle:
            return folder.arg("text_style");
    }

    return {};
}
