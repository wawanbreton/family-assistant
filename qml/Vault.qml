import QtQuick
import QtQuick.Effects
import FamilyAssistant
import "components"

Item
{
    property var kid
    property int delayHide: 5000
    property bool pointsAlwaysVisible: false

    signal displayRewards()

    id: root
    width: buttonVault.width
    height: buttonVault.height

    Item
    {
        id: totalPointsDisplay
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        height: rectanglePointsDisplay.height + 30
        clip: true

        ButtonShadow
        {
            id: pointsDisplayShadow
            source: rectanglePointsDisplay
        }

        Rectangle
        {
            id: rectanglePointsDisplay
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            radius: height / 2
            width: pointsCounter.x + pointsCounter.width + 38
            height: 100

            gradient: StyledGradient {}

            PointsCounter
            {
                id: pointsCounter
                kid: root.kid
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 170
            }
        }

        state: pointsAlwaysVisible ? "displayed" : "hidden"
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

        MouseArea
        {
            anchors.fill: parent
            onPressed: root.displayRewards()
        }

        onStateChanged:
        {
            timerHideTotalPoints.stop();

            if(state === "displayed" && !root.pointsAlwaysVisible)
            {
                timerHideTotalPoints.start();
            }
        }
    }

    CornerButton
    {
        id: buttonVault
        x: 0
        y: 0
        imageComponent: Theme.getItemFilePath(ThemeCategory.PointsStorage, kid.theme.points_storage)
        imageScale: 0.65

        onPressed:
        {
            if(!root.pointsAlwaysVisible)
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

    TextOutline
    {
        target: textPointsEarned
        borderWidth: 4
        blurFactor: 0.5
        color: "#7d5132"
    }

    LargeText
    {
        property int pointsEarned: 0
        property int verticalOffset: 0

        id: textPointsEarned
        text: (pointsEarned > 0 ? "+" : "") + pointsEarned
        x: buttonVault.visualCenterX - width / 2
        y: buttonVault.visualCenterY - height / 2 + verticalOffset
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

        Behavior on verticalOffset
        {
            NumberAnimation
            {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }

        onVerticalOffsetChanged: if(verticalOffset === 0) { textPointsEarned.pointsEarned = 0 }

        states:
        [
            State
            {
                name: "hidden"
                PropertyChanges
                {
                    target: textPointsEarned
                    opacity: 0.0
                    verticalOffset: 0
                }
            },
            State
            {
                name: "displayed"
                PropertyChanges
                {
                    target: textPointsEarned
                    opacity: 1.0
                    verticalOffset: -120
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
}
