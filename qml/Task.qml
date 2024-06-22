import QtQuick
import QtQuick.Effects
import FamilyAssistant 1.0

Item
{
    property var task

    readonly property int clickConfirmDuration: 800

    id: root
    height: 100
    opacity: task.state === TaskState.Early ? 0.5 : 1.0

    Item
    {
        id: mainContainer
        anchors.fill: parent

        MultiEffect
        {
            id: shadowEffect
            source: backgroundRectangle
            anchors.fill: parent
            shadowBlur: 1.0
            shadowEnabled: true
            shadowColor: "black"
            shadowOpacity: 0.7
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
            color:
            {
                switch(task.state)
                {
                    case TaskState.Early:
                    case TaskState.Soon:
                        return "#3498db";
                    case TaskState.InProgress:
                        return "#2ecc71";
                    case TaskState.CloseToEnd:
                        return "#e67e22";
                    case TaskState.Late:
                        return "#e74c3c";
                }
            }

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
                    duration: 1000
                    onFinished: { animationAccomplish.start(); mouseArea.enabled = false; }
                }

                PropertyAnimation
                {
                    id: animationClickCancel
                    target: clickProgress
                    property: "width"
                    to: 0
                }
            }

            Rectangle
            {
                id: iconBubble
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 12
                width: height
                color: "white"
                radius: height / 2

                Image
                {
                    anchors.fill: parent
                    anchors.margins: 10
                    id: name
                    source: task.icon_path
                }
            }

            Text
            {
                id: textDesc
                anchors.left: iconBubble.right
                anchors.bottom: parent.verticalCenter
                anchors.leftMargin: 10
                anchors.topMargin: 6
                text: task.desc
                font.pointSize: 26
                color: "white"
            }

            Text
            {
                id: textTime
                anchors.left: iconBubble.right
                anchors.top: parent.verticalCenter
                anchors.leftMargin: textDesc.anchors.leftMargin
                anchors.topMargin: textDesc.anchors.bottomMargin
                text: task.due_time_str
                font.pointSize: textDesc.font.pointSize
                color: "white"
            }

            SequentialAnimation
            {
                running: task.state === TaskState.CloseToEnd || task.state === TaskState.Late
                loops: Animation.Infinite

                RotationAnimation
                {
                    id: rotationFirst
                    target: mainContainer
                    duration: 120
                    from: 0
                    to: 5
                    easing.type: Easing.InOutQuad
                }

                SequentialAnimation
                {
                    loops: 5

                    RotationAnimation
                    {
                        target: mainContainer
                        duration: rotationFirst.duration
                        from: rotationFirst.to
                        to: -rotationFirst.to
                        easing: rotationFirst.easing
                    }

                    RotationAnimation
                    {
                        target: mainContainer
                        duration: rotationFirst.duration
                        from: -rotationFirst.to
                        to: rotationFirst.to
                        easing: rotationFirst.easing
                    }
                }

                RotationAnimation
                {
                    target: mainContainer
                    duration: rotationFirst.duration
                    from: rotationFirst.to
                    to: 0
                    easing: rotationFirst.easing
                }

                PauseAnimation
                {
                    duration: 1000
                }
            }
        }
    }

    states:
    [
        State
        {
            name: "preview"
            PropertyChanges { target: root; width: root.height }
        },
        State
        {
            name: "full"
            PropertyChanges { target: root; width: textDesc.x + textDesc.width + 24 }
        }
    ]

    state:
    {
        switch(task.state)
        {
            case TaskState.Early:
            case TaskState.Soon:
                return "preview";
            case TaskState.InProgress:
            case TaskState.CloseToEnd:
            case TaskState.Late:
                return "full";
        }
    }

    transitions:
    [
        Transition
        {
            to: "*"
            PropertyAnimation { target: root; property: "width"; easing.type: Easing.InOutQuad; duration: 300}
        }
    ]

    PropertyAnimation
    {
        id: animationAccomplish
        target: root
        property: "scale"
        easing.type: Easing.InQuad
        duration: 300
        from: 1.0
        to: 0.0
        onFinished: task.setAccomplished()
    }

    MouseArea
    {
        id: mouseArea
        anchors.fill: parent
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
                animationClickProgress.stop();

                animationClickCancel.from = clickProgress.width;
                animationClickCancel.duration = (clickConfirmDuration * (clickProgress.width / root.width) / 3);
                animationClickCancel.start();
            }
        }
    }
}
