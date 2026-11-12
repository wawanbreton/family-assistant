import QtQuick 2.15
import "../components"

Item
{
    property alias imageSource: image.source
    signal clicked()

    id: root

    Rectangle
    {
        id: rectangle
        color: "#ccffffff"
        radius: width / 2
        anchors.fill: parent

        Image
        {
            id: image
            anchors.fill: parent
            anchors.margins: 10
        }
    }

    ButtonShadow
    {
        source: rectangle
        shadowScale: mouseArea.pressed ? 0.96 : 1.0
        shadowHorizontalOffset: mouseArea.pressed ? 0 : 4
        shadowVerticalOffset: shadowHorizontalOffset
        visible: root.enabled
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
