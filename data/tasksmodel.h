#pragma once

#include <QAbstractListModel>
#include <QObject>

class DueTask;

class TasksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TasksModel(QObject* parent = nullptr);

    void load(const QJsonArray& json_array);

    void save(QJsonArray& json_array) const;

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void append(DueTask* task);

    void remove(DueTask* task);

    void clear();

    QDate getDueDate() const;

signals:
    void changed();

    void taskAccomplished(DueTask* task);

private:
    QList<DueTask*> tasks_;
};
