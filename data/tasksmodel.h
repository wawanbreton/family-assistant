#pragma once

#include <QAbstractListModel>
#include <QObject>

class Task;

class TasksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TasksModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void append(Task* task);

    void remove(Task* task);

private:
    QList<Task*> tasks_;
};
