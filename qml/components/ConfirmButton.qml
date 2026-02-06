import QtQuick
import QtQuick.Effects
import FamilyAssistant

Item
{
    signal triggered()

    property alias gradient: mainRectangle.gradient
    property int clickConfirmDuration: 800

    id: root
    height: 100

    Item
    {
        id: mainContainer
        anchors.fill: parent

        ButtonShadow
        {
            source: backgroundRectangle
            shadowScale: 1.0
            shadowHorizontalOffset: 4
            shadowVerticalOffset: shadowHorizontalOffset
        }

        Rectangle
        {
            id: backgroundRectangle
            anchors.fill: parent
            radius: height / 2
            visible: false
        }

        Rectangle
        {
            id: mainRectangle
            anchors.fill: parent
            radius: height / 2
            clip: true
            gradient: StyledGradient {}

            Item
            {
                id: clickProgress
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: 0
                clip: true

                Rectangle
                {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: root.width
                    color: "white"
                    opacity: 0.5
                    radius: mainRectangle.radius
                }

                PropertyAnimation
                {
                    id: animationClickProgress
                    target: clickProgress
                    property: "width"
                    from: 0
                    to: root.width
                    onFinished:
                    {
                        mouseArea.enabled = false;
                        mouseArea.reset()
                        root.triggered()
                    }
                }

                PropertyAnimation
                {
                    id: animationClickCancel
                    target: clickProgress
                    property: "width"
                    to: 0
                    onFinished: mouseArea.enabled = true
                }
            }
        }
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
        enabled: clickConfirmDuration > 0
        onPressed:
        {
            animationClickCancel.stop();

            animationClickProgress.from = clickProgress.width;
            animationClickProgress.duration = (clickConfirmDuration * (1.0 - clickProgress.width / root.width));
            animationClickProgress.start();
        }
        onReleased:
        {
            if(enabled)
            {
                reset()
            }
        }

        function reset()
        {
            animationClickProgress.stop();

            animationClickCancel.from = clickProgress.width;
            animationClickCancel.duration = (clickConfirmDuration * (clickProgress.width / root.width) / 3);
            animationClickCancel.start();
        }
    }
}
