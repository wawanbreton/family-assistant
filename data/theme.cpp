#include "theme.h"

#include <QDir>

#include "utils/json.h"


Theme::Theme(QObject* parent)
    : QObject{ parent }
{
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

QStringList Theme::getAvailablePoints()
{
#warning cache this
    QStringList result;

    for (const QFileInfo& file_info : QDir("qml/theme/points").entryInfoList({ "*.qml" }, QDir::Files))
    {
        result << file_info.baseName();
    }

    return result;
}

QString Theme::getItemFilePath(const QString& category, const QString& base_name)
{
    return QDir().absoluteFilePath(QString("qml/theme/%2/%3.qml").arg(category, base_name));
}
