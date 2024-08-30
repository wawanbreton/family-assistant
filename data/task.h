#pragma once

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString desc READ getDesc WRITE setDesc NOTIFY dummyChanged)
    Q_PROPERTY(QString icon WRITE setIcon)
    Q_PROPERTY(QString icon_path READ getIconPath NOTIFY dummyChanged)
    Q_PROPERTY(quint32 reward READ getReward WRITE setReward NOTIFY dummyChanged)

public:
    explicit Task(QObject* parent = nullptr);

    virtual void load(const QJsonObject& json_object);

    const QString& getDesc() const;

    void setDesc(const QString& desc);

    const QString& getIconPath() const;

    void setIcon(const QString& icon);

    quint32 getReward() const;

    void setReward(quint32 reward);

signals:
    void dummyChanged();

private:
    QString desc_;
    QString icon_path_;
    quint32 reward_{ 0 };
};
