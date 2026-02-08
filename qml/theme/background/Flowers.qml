import QtQuick 2.15
import FamilyAssistant

Rectangle
{
    readonly property int cost: 300
    readonly property int average_lifetime: 10000
    readonly property real density: 0.00025
    property bool animated: true
    property var flowers: []

    id: root
    color: "#6dc173"
    clip: true

    Component
    {
        id: flowersMaker

        Item
        {
            id: flower
            width: 0
            height: 0

            PropertyAnimation
            {
                id: animationFadeOut
                target: flowerImage
                property: "width"
                to: 0
                easing.type: Easing.InOutQuad

                onFinished:
                {
                    root.flowers.splice(root.flowers.indexOf(flower), 1);
                    flower.destroy();
                    root.makeNewFlower();
                }
            }

            PropertyAnimation
            {
                id: animationGrow
                target: flowerImage
                property: "width"
                from: 0
                easing.type: Easing.InOutQuad

                onFinished: root.makeFlowerFadeOut()
            }

            Image
            {
                id: flowerImage
                anchors.centerIn: parent
                height: width
                opacity: 0.9
                mipmap: true
            }

            Component.onCompleted:
            {
                root.flowers.push(this);

                flowerImage.source = DataStorage.findResource("flower" + ((Math.random() * 13.99) + 0.51).toFixed(0), ResourceType.Icon);
                flowerImage.width = (flowerImage.implicitWidth * 0.8) * (Math.random() + 0.5);
                flower.x = -flowerImage.width / 2 + Math.random() * (root.width + flowerImage.width);
                flower.y = -flowerImage.height / 2 + Math.random() * (root.height + flowerImage.height);

                animationGrow.duration = (Math.random() * root.average_lifetime) + 1;
                animationGrow.to = flowerImage.width;

                animationFadeOut.duration = (Math.random() * root.average_lifetime) + 1;
                animationFadeOut.from = flowerImage.width;
            }

            function startFadeOut()
            {
                animationFadeOut.start();
            }

            function startGrow()
            {
                animationGrow.start();
            }
        }
    }

    onWidthChanged: makeNewField()
    onHeightChanged: makeNewField()

    function makeNewField()
    {
        for(var i = 0 ; i < flowers.length ; ++i)
        {
            flowers[i].destroy();
        }
        flowers = [];

        var nb_flowers = root.width * root.height * root.density;
        for(i = 0 ; i < nb_flowers ; ++i)
        {
            flowersMaker.createObject(root);
        }

        if(root.animated)
        {
            makeFlowerFadeOut();
        }
    }

    function makeFlowerFadeOut()
    {
        if(root.flowers.length > 0)
        {
            var flower = root.flowers[Math.floor(Math.random()*root.flowers.length)];
            flower.startFadeOut();
        }
    }

    function makeNewFlower()
    {
        var flower = flowersMaker.createObject(root);
        flower.startGrow();
    }
}
