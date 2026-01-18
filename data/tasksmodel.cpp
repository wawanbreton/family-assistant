#include "tasksmodel.h"

#include <QJsonArray>
#include <QJsonObject>
#include <ranges>

#include "data/duetask.h"


TasksModel::TasksModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

void TasksModel::load(const QJsonArray& json_array)
{
    for (const QJsonValue& task_object : json_array)
    {
        if (! task_object.isObject())
        {
            qWarning() << "Value" << task_object << "is not an object as expected";
            continue;
        }

        auto task = new DueTask(this);
        if (task->load(task_object.toObject()))
        {
            append(task);
        }
        else
        {
            delete task;
        }
    }
}

void TasksModel::save(QJsonArray& json_array) const
{
    for (const DueTask* due_task : tasks_)
    {
        QJsonObject task_object;
        due_task->save(task_object);
        json_array.append(task_object);
    }
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
    connect(task, &DueTask::accomplished, this, [this, task] { remove(task); });
    tasks_.insert(insert_index, task);
    endInsertRows();
    emit changed();
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
        emit changed();
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
        emit changed();
    }
}

QDate TasksModel::getDueDate() const
{
    return ! tasks_.empty() ? tasks_.first()->getDueTimestamp().date() : QDate();
}
