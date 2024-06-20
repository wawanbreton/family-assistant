import QtQuick
import QtQuick.Layouts

RowLayout
{
    spacing: 0

    Repeater
    {
        model: kid_manager.kids

        KidTasksView
        {
            kid: modelData
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
