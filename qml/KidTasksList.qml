import QtQuick

Flow
{
    property var kid

    spacing: 12
    add: Transition
    {
        NumberAnimation
        {
            properties: "x,y"
            easing.type: Easing.OutBounce
            duration: 1000
        }
    }

    Repeater
    {
        model: kid.tasks

        Task
        {
            task: modelData
        }
    }
}
