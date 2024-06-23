import QtQuick
import FamilyAssistant

Item
{
    property var kid

    width: 140
    height: width

    Item
    {
        anchors.right: parent.right
        anchors.top: parent.top
        id: button
        width: 180
        height: width

        Rectangle
        {
            anchors.fill: parent
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

    Text
    {
        property int pointsEarned: 0

        id: textPointsEarned
        text: "+" + pointsEarned
        anchors.centerIn: image
        font.pointSize: 50
        opacity: 0.0

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
                easing.type: Easing.InOutQuad
                duration: 400
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
