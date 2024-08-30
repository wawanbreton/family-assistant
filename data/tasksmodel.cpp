#include "tasksmodel.h"

#include "data/duetask.h"


TasksModel::TasksModel(QObject* parent)
{
}

int TasksModel::rowCount(const QModelIndex& /*parent*/) const
{
    return tasks_.count();
}

QVariant TasksModel::data(const QModelIndex& index, int role) const
{
    int list_index = index.row();
    if (list_index >= 0 && list_index < tasks_.count())
    {
        return QVariant::fromValue(static_cast<QObject*>(tasks_.at(list_index)));
    }

    return QVariant();
}

QHash<int, QByteArray> TasksModel::roleNames() const
{
    return { { 0, "modelData" } };
}

void TasksModel::append(DueTask* task)
{
    beginInsertRows(QModelIndex(), tasks_.count(), tasks_.count());
    tasks_ << task;
    endInsertRows();
}

void TasksModel::remove(DueTask* task)
{
    int index = tasks_.indexOf(task);
    if (index >= 0)
    {
        beginRemoveRows(QModelIndex(), index, index);
        tasks_.remove(index);
        endRemoveRows();
    }
}
