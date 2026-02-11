import QtQuick 2.15
import FamilyAssistant
import "../../components"

Sky
{
    readonly property int cost: 100
    property var clouds: []
    property int maximumCloudsY: height
    property real cloudsZ: 0.0

    id: root
    clip: true

    Component
    {
        id: cloudMaker

        Image
        {
            id: cloud
            source: DataStorage.findResource("cloud", ResourceType.Icon)
            z: root.cloudsZ

            RandomMirror { }

            PropertyAnimation
            {
                id: animationTravel
                target: cloud
                property: "x"

                onFinished:
                {
                    root.clouds.splice(root.clouds.indexOf(cloud), 1);
                    cloud.destroy()
                }
            }

            Component.onCompleted:
            {
                root.clouds.push(this)

                cloud.width = (cloud.implicitWidth * 0.3) * (Math.random() + 0.5);
                cloud.height = (cloud.implicitHeight / cloud.implicitWidth) * cloud.width;
                var x_left = -cloud.width;
                var x_right = root.width;
                y = Math.random() * (root.maximumCloudsY - cloud.height);

                if(Math.random() > 0.5)
                {
                    animationTravel.from = x_left;
                    animationTravel.to = x_right;
                }
                else
                {
                    animationTravel.from = x_right;
                    animationTravel.to = x_left;
                }

                animationTravel.duration = (root.width * 50) * (Math.random() + 0.5);
                animationTravel.start();
            }
        }
    }

    Timer
    {
        id: timerMakeNextCloud

        onTriggered: root.makeCloud()
    }

    onWidthChanged: clearClouds()
    onHeightChanged: clearClouds()

    function clearClouds()
    {
        for(var i = 0 ; i < clouds.length ; ++i)
        {
            clouds[i].destroy();
        }
        clouds = [];

        makeCloud();
    }

    function makeCloud()
    {
        if(root.width > 0 && root.height > 0)
        {
            cloudMaker.createObject(root)
            timerMakeNextCloud.interval = (Math.random() + 0.5) * 4000 / (maximumCloudsY / height)
            timerMakeNextCloud.start()
        }
    }
}
