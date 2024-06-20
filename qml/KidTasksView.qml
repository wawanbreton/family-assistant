import QtQuick
import QtQuick.Layouts

Item
{
    property var kid

    id: root

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: spacing
        spacing: 26

        Text
        {
            text: kid.name
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 40
        }

        KidTasksList
        {
            kid: root.kid
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

}
