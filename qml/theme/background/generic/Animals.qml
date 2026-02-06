import QtQuick 2.15
import FamilyAssistant
import "../../../components"
import ".."

SkyWithClouds
{
    id: root

    readonly property int cost: 100
    property string building
    property var animal
    property alias groundColorTop: groundColor0.color
    property alias groundColorBottom: groundColor1.color
    property alias buildingY: imageBuilding.y
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
             GradientStop { id: groundColor0; position: 0.0; color: "#9ccc65" }
             GradientStop { id: groundColor1; position: 1.0; color: "#83bd46" }
        }

        Image
        {
            id: imageBuilding
            source: DataStorage.findResource(root.building, ResourceType.Icon)
            x: 10 + Math.random() * (parent.width - 20 - width)
            y: -180
            width: 240
            height: width
        }

        Repeater
        {
            model: (Math.random() + 0.5) * field.width * field.height * 0.000035

            Image
            {
                id: imageAnimal
                source:
                {
                    var actualAnimal;
                    if (Array.isArray(root.animal))
                    {
                        actualAnimal = root.animal[Math.floor(Math.random() * root.animal.length)];
                    }
                    else
                    {
                        actualAnimal = root.animal;
                    }

                    return DataStorage.findResource(actualAnimal, ResourceType.Icon);
                }

                x: Math.random() * (field.width - width)
                y: Math.random() * (field.height - height)

                RandomMirror {}

                ParallelAnimation
                {
                    id: animationMove

                    PropertyAnimation
                    {
                        id: moveX
                        target: imageAnimal
                        property: "x"
                    }

                    PropertyAnimation
                    {
                        id: moveY
                        target: imageAnimal
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
