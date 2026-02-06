#pragma once

#include <QObject>
#include <QUuid>

class ActiveTask;
class DueTask;

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)

public:
    explicit User(QObject* parent = nullptr);

    virtual void load(const QJsonObject& json_object);

    virtual void save(QJsonObject& object) const;

    const QUuid& getUuid() const;

    const QString& getName() const;

    void setName(const QString& name);

    std::optional<int> getFingerprintId() const;

    void setFingerprintId(int fingerprint_id);

    virtual bool isAdmin() const;

signals:
    void changed();

    void nameChanged(const QString& name);

    void fingerprintIdChanged(int fingerprint_id);

private:
    QUuid uuid_;
    QString name_;
    std::optional<int> fingerprint_id_;
};
