import QtQuick 2.15

Image
{
    readonly property int cost: 20

    id: root
    source: DataStorage.findResource("ruby", DataStorage.Icon)

    Timer
    {
        id: timer
        onTriggered:
        {
            createSparkle();
            startTimerRandom();
        }
    }

    Component
    {
        id: componentSparkle

        Image
        {
            id: sparkle
            source: DataStorage.findResource("sparkle", DataStorage.Icon)

            SequentialAnimation
            {
                running: true

                PropertyAnimation
                {
                   target: sparkle
                   property: "scale"
                   properties: "scale,opacity"
                   duration: 400
                   easing.type: Easing.InQuad
                   from: 0.0
                   to: 1.0
                }
                PropertyAnimation
                {
                   target: sparkle
                   property: "scale"
                   properties: "scale,opacity"
                   duration: 400
                   easing.type: Easing.OutQuad
                   from: 1.0
                   to: 0.0
                }

                onFinished: sparkle.destroy()
            }
        }
    }

    Component.onCompleted: startTimerRandom()

    function startTimerRandom()
    {
        timer.interval = Math.random() * 1000;
        timer.start();
    }

    function createSparkle()
    {
        var size = (root.width / 2) * (Math.random() + 0.5);
        var x = (root.width / 2) + ((root.width / 1.5) * (Math.random() - 0.5));
        var y = (root.height / 2) + ((root.height / 1.5) * (Math.random() - 0.5));
        componentSparkle.createObject(root, {x: x - size / 2, y: y - size / 2, width: size, height: size});
    }
}
