import QtQuick
import QtQuick.Effects
import FamilyAssistant

Item
{
    property alias imageSource: image.source
    property alias imageScale: image.scale
    property int visualCenterX: image.x + image.width / 2
    property int visualCenterY: image.y + image.height / 2

    signal pressed()

    id: root
    width: 140
    height: width

    Item
    {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width * 2
        height: parent.height * 2
        clip: true

        ButtonShadow { source: backgroundRectangle }

        Rectangle
        {
            id: backgroundRectangle
            anchors.left: parent.left
            anchors.leftMargin: -40
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -40
            width: 180
            height: width
            color: "#008fff"
            radius: width / 2

            gradient: StyledGradient {}

            MouseArea
            {
                anchors.fill: parent
                onPressed: root.pressed()
            }
        }
    }

    Image
    {
        id: image
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -8
        anchors.verticalCenterOffset: 6
    }
}
