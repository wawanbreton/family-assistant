import QtQuick 2.15


ConfirmButton
{
    id: root

    clickConfirmDuration: 0
    property real progress: 0.0


    Rectangle
    {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 12
        radius: height / 2
        width: root.progress * (root.width - 2 * (anchors.margins))
    }
}
