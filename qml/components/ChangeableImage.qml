import QtQuick
import FamilyAssistant

Item
{
    property url source

    id: root
    implicitWidth: loader.item ? loader.item.implicitWidth : -1
    implicitHeight: loader.item ? loader.item.implicitHeight : -1

    Item
    {
        id: container
        anchors.fill: parent

        Loader
        {
            id: loader
            anchors.fill: parent
        }

        PropertyAnimation
        {
            id: animationDisappear
            target: container
            property: "scale"
            easing.type: Easing.InBack
            easing.overshoot: 3
            duration: 500
            from: 1.0
            to: 0.0

            onFinished:
            {
                loader.source = root.source;
                animationAppear.start();
            }
        }

        PropertyAnimation
        {
            id: animationAppear
            target: container
            property: "scale"
            easing.type: Easing.OutQuad
            duration: 3000
            from: 0.0
            to: 1.0
        }
    }

    onSourceChanged:
    {
        if(loader.source.toString() === "")
        {
            loader.source = root.source;
        }
        else
        {
            animationDisappear.start();
        }
    }
}
