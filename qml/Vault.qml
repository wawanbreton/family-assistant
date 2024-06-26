import QtQuick
import QtQuick.Effects
import FamilyAssistant

Item
{
    property var kid
    property int delayHide: 5000

    id: root

    Item
    {
        id: totalPointsDisplay
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: rectanglePointsDisplay.height + 30
        clip: true

        MultiEffect
        {
            id: pointsDisplayShadow
            source: rectanglePointsDisplay
            anchors.fill: rectanglePointsDisplay
            shadowBlur: shadowEffect.shadowBlur
            shadowEnabled: shadowEffect.shadowEnabled
            shadowColor: shadowEffect.shadowColor
            shadowOpacity: shadowEffect.shadowOpacity
        }

        Rectangle
        {
            id: rectanglePointsDisplay
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            radius: height / 2
            color: "#008fff"
            width: textNbPoints.x + textNbPoints.width + 38
            height: 100

            Point
            {
                id: pointImage
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 170
            }

            Text
            {
                id: textNbPoints
                text: kid.points
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: pointImage.right
                anchors.leftMargin: 16
                font.pointSize: 50
                color: "white"
            }
        }

        state: "hidden"
        states:
        [
            State
            {
                name: "hidden"
                PropertyChanges
                {
                    target: totalPointsDisplay
                    width: 120
                }
            },
            State
            {
                name: "displayed"
                PropertyChanges
                {
                    target: totalPointsDisplay
                    width: pointsDisplayShadow.itemRect.right
                }
            }
        ]

        Behavior on width
        {
            NumberAnimation
            {
                duration: 600
                easing.type: Easing.InOutQuad
            }
        }

        Timer
        {
            id: timerHideTotalPoints
            interval: root.delayHide
            onTriggered: totalPointsDisplay.state = "hidden"
        }

        onStateChanged:
        {
            timerHideTotalPoints.stop();

            if(state === "displayed")
            {
                timerHideTotalPoints.start();
            }
        }
    }

    Item
    {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: parent.width * 2
        height: parent.height * 2
        clip: true

        MultiEffect
        {
            id: shadowEffect
            source: backgroundRectangle
            anchors.fill: backgroundRectangle
            shadowBlur: 1.0
            shadowEnabled: true
            shadowColor: "black"
            shadowOpacity: 0.7
            shadowScale: 1.03
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
        }
    }

    Image
    {
        id: image
        anchors.centerIn: parent
        anchors.horizontalCenterOffset: -8
        anchors.verticalCenterOffset: 6
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
        id: timerHideEarnedPoints
        interval: root.delayHide
        onTriggered: textPointsEarned.state = "hidden"
    }

    Connections
    {
        target: kid
        function onPointsChanged(points, delta)
        {
            textPointsEarned.state = "displayed"
            textPointsEarned.pointsEarned += delta;
            timerHideEarnedPoints.restart()
        }
    }

    MouseArea
    {
        anchors.fill: parent
        onPressed:
        {
            if(totalPointsDisplay.state === "displayed")
            {
                totalPointsDisplay.state = "hidden"
            }
            else
            {
                totalPointsDisplay.state = "displayed"
            }
        }
    }
}
