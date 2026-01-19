import QtQuick
import "components"

Item
{
    property alias imageSource: image.source
    signal clicked()

    id: root
    width: parent.width
    height: width

    Rectangle
    {
        id: rectangle
        color: "white"
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
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
