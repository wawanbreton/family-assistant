#include "tasksmodel.h"

#include <ranges>

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
    size_t insert_index = tasks_.size();
    for (const std::tuple<size_t, const DueTask*> actual_task : tasks_ | std::views::enumerate)
    {
        if (task->getDueTimestamp() < std::get<1>(actual_task)->getDueTimestamp())
        {
            insert_index = std::get<0>(actual_task);
            break;
        }
    }

    beginInsertRows(QModelIndex(), insert_index, insert_index);
    task->setParent(this);
    tasks_.insert(insert_index, task);
    endInsertRows();
}

void TasksModel::remove(DueTask* task)
{
    int index = tasks_.indexOf(task);
    if (index >= 0)
    {
        beginRemoveRows(QModelIndex(), index, index);
        tasks_.at(index)->deleteLater();
        tasks_.remove(index);
        endRemoveRows();
    }
}

void TasksModel::clear()
{
    if (rowCount() > 0)
    {
        beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        for (DueTask* task : tasks_)
        {
            task->deleteLater();
        }
        tasks_.clear();
        endRemoveRows();
    }
}

QDate TasksModel::getDueDate() const
{
    return ! tasks_.empty() ? tasks_.first()->getDueTimestamp().date() : QDate();
}
