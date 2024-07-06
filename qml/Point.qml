import QtQuick
import FamilyAssistant

Item
{
    property var kid
    property var itemImage

    id: root
    width: 60
    height: width

    Component
    {
        id: componentImage

        Loader
        {
            id: loader
            anchors.fill: parent
            source: Theme.getItemFilePath("points", kid.theme.point)

            NumberAnimation
            {
                id: animationDisappear
                target: item
                property: "scale"
                easing.type: Easing.InBack
                easing.overshoot: 2
                duration: 400
                from: 1.0
                to: 0.0

                onFinished: destroy()
            }

            NumberAnimation
            {
                id: animationAppear
                target: item
                property: "scale"
                easing.type: Easing.OutQuad
                easing.overshoot: 2
                duration: 2000
                from: 0.0
                to: 1.0
            }

            function startAppear()
            {
                animationAppear.start()
            }

            function startDisappear()
            {
                animationDisappear.start()
            }
        }
    }

    Connections
    {
         target: root.kid.theme

         function onPointChanged()
         {
             setPointImage(true)
         }
    }

    Component.onCompleted: setPointImage(false)

    function setPointImage(animateAppear)
    {
        if(root.itemImage)
        {
            root.itemImage.startDisappear()
        }

        root.itemImage = componentImage.createObject(root, { source: Theme.getItemFilePath("points", kid.theme.point) });

        if(animateAppear)
        {
            root.itemImage.startAppear()
        }
    }
}
