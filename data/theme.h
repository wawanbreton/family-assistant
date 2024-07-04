#pragma once

#include <QObject>

class Theme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString point READ getPoint WRITE setPoint NOTIFY pointChanged)

public:
    explicit Theme(QObject* parent = nullptr);

    void load(const QJsonObject& json_object);

    const QString& getPoint() const;

    void setPoint(const QString& point);

    Q_INVOKABLE static QStringList getAvailablePoints();

    Q_INVOKABLE static QString getItemFilePath(const QString& category, const QString& base_name);

signals:
    void pointChanged();

private:
    QString point_{ "StarCoin" };
};
