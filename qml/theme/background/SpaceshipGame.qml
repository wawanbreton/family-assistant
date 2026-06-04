import QtQuick 2.15
import FamilyAssistant
import "../../components"
import "generic"

Rectangle
{
    readonly property int cost: 200

    id: root
    clip: true

    gradient: Gradient
    {
         GradientStop { position: 0.0; color: "#00006d" }
         GradientStop { position: 1.0; color: "#00003e" }
    }

    SlidingBackground
    {
        id: slidingBackground
        anchors.fill: parent
        speedMin: 100
        speedMax: 100
        direction: SlidingBackground.RightToLeft
        generateIntervalMin: 100
        generateIntervalMax: 200

        Rectangle
        {
            width: (Math.random() * 2) + 1
            height: width
            radius: width / 2
            color: "#ffffff"
        }
    }

    SlidingBackground
    {
        id: aliens
        anchors.fill: parent
        speedMin: 30
        speedMax: 50
        direction: SlidingBackground.RightToLeft
        generateIntervalMin: 500
        generateIntervalMax: 1000

        Image
        {
            width: spaceship.width * 0.5
            height:width
            source: DataStorage.findResource(["alien1", "alien2", "alien3", "alien4", "alien5"][Math.floor(Math.random() * 5)], ResourceType.Icon)
        }
    }

    Component
    {
        id: explosionComponent

        Image
        {
            id: explosion
            source: DataStorage.findResource("explosion", ResourceType.Icon)
            width: spaceship.width / 2
            height: width

            SequentialAnimation
            {
                id: animationExplosion

                PropertyAnimation
                {
                    target: explosion
                    property: "scale"
                    from: 0.0
                    to: 1.0
                    duration: 200
                }

                PropertyAnimation
                {
                    target: explosion
                    property: "scale"
                    from: 1.0
                    to: 0.0
                    duration: 200
                }

                onFinished: explosion.destroy()
            }

            Component.onCompleted: animationExplosion.start()
        }
    }

    Image
    {
        property int laserSide: -1

        id: spaceship
        source: DataStorage.findResource("spaceship", ResourceType.Icon)
        width: parent.width * 0.2
        height: width
        y: 0

        Item
        {
            id: flameFrame
            width: parent.width * (42 / 182)
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            Image
            {
                id: flame
                source: DataStorage.findResource("flame", ResourceType.Icon)
                height: width
                width: flameFrame.width / 2
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                PropertyAnimation
                {
                    target: flame;
                    property: "width";
                    easing.type: Easing.Linear;
                    duration: 150;
                    from: flameFrame.width;
                    to: flameFrame.width * 0.5
                    loops: -1
                    Component.onCompleted: start()
                }
            }
        }

        SequentialAnimation
        {
            id: spaceshipAnimation

            PropertyAnimation
            {
                id: move
                target: spaceship;
                property: "y";
                easing.type: Easing.Linear;
            }

            PauseAnimation { id: pause }

            function restart()
            {
                pause.duration = Math.random() * 1000;
                move.from = spaceship.y;
                move.to = Math.random() * (root.height - spaceship.height);
                move.duration = Math.abs(move.to - move.from) / 0.3;
                spaceshipAnimation.start();
            }

            Component.onCompleted: restart()
            onFinished: restart()
        }

        Component
        {
            id: componentLaser

            Rectangle
            {
                id: laser
                width: spaceship.width * 0.2
                height: spaceship.height * 0.05
                color: "#2dcb1c"
                radius: height / 2
                x: (spaceship.width - width) / 2

                PropertyAnimation
                {
                    target: laser;
                    property: "x";
                    easing.type: Easing.Linear;
                    from: laser.x
                    to: root.width
                    duration: 500
                    onFinished: laser.destroy()
                    Component.onCompleted: start()
                }

                onXChanged:
                {
                    var laserRect = Qt.rect(laser.x, laser.y, laser.width, laser.height);
                    for(let alien of aliens.elements)
                    {
                        if(alien.visible)
                        {
                            var alienRect = Qt.rect(alien.x, alien.y, alien.width, alien.height);
                            if (!(laserRect.x > alienRect.x + alienRect.width ||
                                  laserRect.x + laserRect.width < alienRect.x ||
                                  laserRect.y > alienRect.y + alienRect.height ||
                                  laserRect.y + laserRect.height < alienRect.y))
                            {
                                laser.destroy();
                                alien.visible = false;

                                var explosion = explosionComponent.createObject(root);
                                explosion.x = laser.x + laser.width - explosion.width * 0.3;
                                explosion.y = laser.y - explosion.height * 0.6;
                            }
                        }
                    }
                }
            }
        }

        PauseAnimation
        {
            Component.onCompleted: start();
            onFinished:
            {
                var laserObj = componentLaser.createObject(root);
                laserObj.y = spaceship.y + spaceship.height * ( 0.5 + spaceship.laserSide * 0.26 ) - laserObj.height / 2
                spaceship.laserSide = -spaceship.laserSide;
                duration = Math.random() * 500;
                start();
            }
        }
    }
}
