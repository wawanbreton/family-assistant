import QtQuick 2.15
import "../../components"

SkyWithClouds
{
    readonly property int cost: 100
    maximumCloudsY: height / 2

    Rectangle
    {
        id: field
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height / 2
        z: 1.0

        gradient: Gradient
        {
             GradientStop { position: 0.0; color: "#9ccc65" }
             GradientStop { position: 1.0; color: "#83bd46" }
        }

        Image
        {
            source: DataStorage.findResource("barn", DataStorage.Icon)
            x: 10
            y: -180
            width: 240
            height: width
        }

        Repeater
        {
            model: (Math.random() + 0.5) * field.width * field.height * 0.000035

            Image
            {
                id: cow
                source: DataStorage.findResource("cow", DataStorage.Icon)
                x: Math.random() * (field.width - width)
                y: Math.random() * (field.height - height)

                RandomMirror {}

                ParallelAnimation
                {
                    id: animationMove

                    PropertyAnimation
                    {
                        id: moveX
                        target: cow
                        property: "x"
                    }

                    PropertyAnimation
                    {
                        id: moveY
                        target: cow
                        property: "y"
                    }
                }

                Timer
                {
                    id: timerMove
                    onTriggered: move()
                }

                Component.onCompleted: setNextTimer()

                function move()
                {
                    moveX.to = Math.random() * (field.width - width);
                    moveY.to = Math.random() * (field.height - height);
                    var distance = Math.sqrt(Math.pow(moveX.to - x, 2) + Math.pow(moveY.to - y, 2));
                    moveX.duration = distance / ((Math.random() + 0.5) * 0.07);
                    moveY.duration = moveX.duration;
                    animationMove.start();
                    setNextTimer();
                }

                function setNextTimer()
                {
                    timerMove.interval = Math.random() * 20000;
                    timerMove.start();
                }
            }
        }
    }
}
