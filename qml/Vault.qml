import QtQuick
import QtQuick.Effects
import FamilyAssistant

Item
{
    property var kid

    width: 140
    height: width

    Item
    {
        anchors.fill: parent
        clip: true

        Rectangle
        {
            anchors.right: parent.right
            anchors.top: parent.top
            width: 180
            height: width
            color: "#008fff"
            radius: width / 2
        }
    }

    Image
    {
        id: image
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -10
        anchors.verticalCenterOffset: 10
        source: DataStorage.findResource("treasure", DataStorage.Icon)
        scale: 0.65
    }

    TextOutline
    {
        target: textPointsEarned
        borderWidth: 4
        blurFactor: 0.5
        color: "#7d5132"
    }

    Text
    {
        property int pointsEarned: 0

        id: textPointsEarned
        text: "+" + pointsEarned
        anchors.centerIn: image
        font.pointSize: 50
        opacity: 0.0
        color: "#f2da2c"

        Behavior on opacity
        {
            NumberAnimation
            {
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on anchors.verticalCenterOffset
        {
            NumberAnimation
            {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }

        anchors.onVerticalCenterOffsetChanged: if(textPointsEarned.anchors.verticalCenterOffset === 0) { textPointsEarned.pointsEarned = 0 }

        states:
        [
            State
            {
                name: "hidden"
                PropertyChanges
                {
                    target: textPointsEarned
                    opacity: 0.0
                    anchors.verticalCenterOffset: 0
                }
            },
            State
            {
                name: "displayed"
                PropertyChanges
                {
                    target: textPointsEarned
                    opacity: 1.0
                    anchors.verticalCenterOffset: -120
                }
            }
        ]
    }

    Timer
    {
        id: timerHide
        interval: 5000
        onTriggered: textPointsEarned.state = "hidden"
    }

    Connections
    {
        target: kid
        function onPointsChanged(points, delta)
        {
            textPointsEarned.state = "displayed"
            textPointsEarned.pointsEarned += delta;
            timerHide.restart()
        }
    }
}
