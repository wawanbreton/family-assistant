import QtQuick
import QtQuick.Layouts

Flow
{
    property var kid

    spacing: 12

    Repeater
    {
        model: kid.tasks

        Task
        {
            task: modelData
        }
    }
}
