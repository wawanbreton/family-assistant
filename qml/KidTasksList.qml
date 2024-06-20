import QtQuick
import QtQuick.Layouts

Flow
{
    property var kid

    Repeater
    {
        model: kid.tasks

        Task
        {
            task: modelData
        }
    }
}
