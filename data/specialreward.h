#pragma once

#include <QObject>

class SpecialReward : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(QString pronouncedName READ getPronouncedName CONSTANT)
    Q_PROPERTY(int cost READ getCost CONSTANT)
    Q_PROPERTY(QString icon READ getIcon CONSTANT)

public:
    explicit SpecialReward(QObject* parent = nullptr);

    virtual void load(const QJsonObject& json_object);

    const QString& getName() const;

    const QString& getPronouncedName() const;

    int getCost() const;

    const QString& getIcon() const;

private:
    QString name_;
    QString pronounced_name_;
    int cost_{ 1000 };
    QString icon_;
};
