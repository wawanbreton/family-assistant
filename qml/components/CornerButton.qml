import QtQuick
import QtQuick.Effects
import FamilyAssistant

Item
{
    property alias imageSource: image.source
    property alias imageScale: image.scale
    property alias backgroundScale: buttonShadow.scale
    property int visualCenterX: image.x + image.width / 2
    property int visualCenterY: image.y + image.height / 2
    property int corner: Qt.BottomLeftCorner

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
        transform:
        [
            Scale
            {
                xScale: corner === Qt.BottomRightCorner || corner === Qt.TopRightCorner ? -1.0 : 1.0
            },
            Translate
            {
                x: corner === Qt.BottomRightCorner || corner === Qt.TopRightCorner ? width : 0.0
            }
        ]

        ButtonShadow
        {
            id: buttonShadow;
            source: backgroundRectangle;

            MouseArea
            {
                anchors.fill: parent
                onPressed: root.pressed()
            }
        }

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
            visible: false

            gradient: StyledGradient {}
        }
    }

    Image
    {
        id: image
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: corner === Qt.BottomRightCorner || corner === Qt.TopRightCorner ? 8 : -8
        anchors.verticalCenterOffset: 6
    }
}
