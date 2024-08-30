#pragma once

#include <QAbstractListModel>
#include <QObject>

class DueTask;

class TasksModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TasksModel(QObject* parent = nullptr);

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    void append(DueTask* task);

    void remove(DueTask* task);

private:
    QList<DueTask*> tasks_;
};
