#pragma once

#include <QObject>

class Task : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString desc READ getDesc CONSTANT)
    Q_PROPERTY(QString icon READ getIcon WRITE setIcon CONSTANT)
    Q_PROPERTY(QString icon_path READ getIconPath CONSTANT)
    Q_PROPERTY(quint32 reward READ getReward CONSTANT)

public:
    explicit Task(QObject* parent = nullptr);

    Task(const Task& other);

    // virtual ~Task() = default;

    virtual void copyFrom(const Task* other);

    virtual void load(const QJsonObject& json_object);

    virtual void save(QJsonObject& json_object) const;

    const QString& getDesc() const;

    const QString& getIconPath() const;

    QString getIcon() const;

    void setIcon(const QString& icon);

    quint32 getReward() const;

private:
    QString desc_;
    QString icon_path_;
    quint32 reward_{ 0 };
};

// Q_DECLARE_METATYPE(Task)
