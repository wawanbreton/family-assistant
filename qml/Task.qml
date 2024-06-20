import QtQuick

Rectangle
{
    property var task

    radius: height / 2
    color: "#008fff"
    width: textDesc.x + textDesc.width + 24
    height: 100

    Rectangle
    {
        id: iconBubble
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 12
        width: height
        color: "#ffffff"
        radius: height / 2

        Image
        {
            anchors.fill: parent
            anchors.margins: 10
            id: name
            source: task.icon_path
        }
    }

    Text
    {
        id: textDesc
        anchors.left: iconBubble.right
        anchors.bottom: parent.verticalCenter
        anchors.leftMargin: 8
        anchors.topMargin: 6
        text: task.desc
        font.pointSize: 26
        color: "#ffffff"
    }

    Text
    {
        anchors.left: iconBubble.right
        anchors.top: parent.verticalCenter
        anchors.leftMargin: textDesc.anchors.leftMargin
        anchors.topMargin: textDesc.anchors.bottomMargin
        text: task.due_time_str
        font.pointSize: textDesc.font.pointSize
        color: "#ffffff"
    }
}
