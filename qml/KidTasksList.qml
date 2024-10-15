import QtQuick

Flow
{
    property var kid

    signal rewardEarned(amount: int, item: var)

    id: root
    spacing: 14
    move: Transition
    {
        NumberAnimation
        {
            properties: "x,y"
            easing.type: Easing.OutBounce
            duration: 500
        }
    }

    Repeater
    {
        model: kid.tasks

        Task
        {
            task: modelData.task
            due_task: modelData
            onTaskActivated: { root.rewardEarned(task.reward, this); accomplish(); }
        }
    }
}
