import QtQuick 2.15
import "../../../components"
import FamilyAssistant

Item
{
    default property Component item
    property var elements: []
    property int maximumElementsY: height
    property string image: ""
    property real speedMin // In pixels / s
    property real speedMax // In pixels / s
    property int generateIntervalMin: 4000 // in ms
    property int generateIntervalMax: 6000 // in ms

    enum Direction { LeftToRight = 0, RightToLeft = 1, Both = 2 }
    property int direction: SlidingBackground.Both

    id: root
    clip: true

    Component
    {
        id: elementMaker

        Item
        {
            id: element
            width: loadedItem ? loadedItem.width : 0
            height: loadedItem ? loadedItem.height : 0

            property var loadedItem

            PropertyAnimation
            {
                id: animationTravel
                target: element
                property: "x"

                onFinished:
                {
                    root.elements.splice(root.elements.indexOf(element), 1);
                    element.destroy()
                }
            }

            Component.onCompleted:
            {
                loadedItem = root.item.createObject(element);
                loadedItem.anchors.fill = element;

                root.elements.push(this);

                var x_left = -element.width;
                var x_right = root.width;
                y = Math.random() * (root.maximumElementsY - element.height);

                if(root.direction === SlidingBackground.LeftToRight || (root.direction === SlidingBackground.Both && Math.random() > 0.5))
                {
                    animationTravel.from = x_left;
                    animationTravel.to = x_right;
                }
                else
                {
                    animationTravel.from = x_right;
                    animationTravel.to = x_left;
                }

                var speed = (speedMin + Math.random() * (speedMax - speedMin)) / 1000.0;
                animationTravel.duration = root.width / speed;
                animationTravel.start();
            }
        }
    }

    Timer
    {
        id: timerMakeNextElement

        onTriggered: root.makeElement()
    }

    onWidthChanged: clearElements()
    onHeightChanged: clearElements()

    function clearElements()
    {
        for(var i = 0 ; i < elements.length ; ++i)
        {
            elements[i].destroy();
        }
        elements = [];

        makeElement();
    }

    function makeElement()
    {
        if(root.width > 0 && root.height > 0)
        {
            elementMaker.createObject(root)
            timerMakeNextElement.interval = (root.generateIntervalMin + Math.random() * (root.generateIntervalMax - root.generateIntervalMin)) / (maximumElementsY / height)
            timerMakeNextElement.start()
        }
    }
}
