#pragma once

#include <QObject>

#include "data/themecategory.h"

class Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString point READ getPoint WRITE setPoint NOTIFY pointChanged)
    Q_PROPERTY(QString points_storage READ getPointsStorage WRITE setPointsStorage NOTIFY pointsStorageChanged)

public:
    explicit Theme(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getPoint() const;

    void setPoint(const QString& point);

    const QString& getPointsStorage() const;

    void setPointsStorage(const QString& points_storage);

    Q_INVOKABLE QString getItem(ThemeCategory::Enum category);

    Q_INVOKABLE void setItem(ThemeCategory::Enum category, const QString& item);

    Q_INVOKABLE QStringList getAvailableItems(ThemeCategory::Enum category);

    Q_INVOKABLE QString getItemFilePath(ThemeCategory::Enum category, const QString& base_name);

signals:
    void itemChanged(ThemeCategory::Enum category);

    void pointChanged();

    void pointsStorageChanged();

private:
    static QString getCategoryFolder(ThemeCategory::Enum category);

private:
    QMap<ThemeCategory::Enum, QStringList> available_items_;
    QString point_{ "StarCoin" };
    QString points_storage_{ "Treasure" };
};
