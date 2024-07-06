import QtQuick

Item
{
    property var kid
    property int amount
    property var parentObject
    property var rectStart
    property var targetPosition
    property int pointDelta: 1
    property int pointsDestroyed: 0

    signal finished()

    id: root

    Component
    {
        id: componentPoint

        Point
        {
            id: point
            kid: root.kid

            ParallelAnimation
            {
                id: animationAppear

                PropertyAnimation
                {
                    target: point
                    property: "scale"
                    duration: 300
                    easing.type: Easing.OutBack
                    from: 0.0
                    to: 1.0
                }

                PropertyAnimation
                {
                    id: animationX
                    target: point
                    property: "x"
                    easing.type: Easing.InCubic
                }

                PropertyAnimation
                {
                    id: animationY
                    target: point
                    property: "y"
                    easing: animationX.easing
                    duration: animationX.duration
                }

                onStarted:
                {
                    if(root.pointDelta < 0)
                    {
                        kid.points += root.pointDelta;
                    }
                }

                onFinished:
                {
                    if(root.pointDelta > 0)
                    {
                        kid.points += root.pointDelta;
                    }
                    point.destroy();
                    onPointDestroyed();
                }
            }

            function moveToVault()
            {
                animationX.to = root.targetPosition.x - width / 2;
                animationX.duration = 400 + Math.random() * 1000

                animationY.to = root.targetPosition.y - height / 2;

                animationAppear.start();
            }
        }
    }

    Component.onCompleted:
    {
        for (var i=0 ; i<amount ; ++i)
        {
            var point_item = componentPoint.createObject(parentObject)

            var x_center_start = rectStart.x + Math.random() * rectStart.width
            var y_center_start = rectStart.y + Math.random() * rectStart.height

            point_item.x = x_center_start - point_item.width / 2
            point_item.y = y_center_start - point_item.height / 2

            point_item.moveToVault()
        }
    }

    function onPointDestroyed()
    {
        root.pointsDestroyed++;
        if(root.pointsDestroyed == root.amount)
        {
            root.finished()
            root.destroy()
        }
    }
}
