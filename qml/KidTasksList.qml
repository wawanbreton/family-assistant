import QtQuick

Flow
{
    property var kid

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
            task: modelData
        }
    }
}
