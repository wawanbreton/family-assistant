import QtQuick
import QtQuick.Effects
import FamilyAssistant
import "components"

Item
{
    property var task
    property var due_task

    signal taskActivated()

    property int clickConfirmDuration: 800

    id: root
    height: 100
    opacity: due_task ? (due_task.state === TaskState.Early ? 0.5 : 1.0) : 1.0

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
            gradient: StyledGradient
            {
                color:
                {
                    if(due_task)
                    {
                        switch(due_task.state)
                        {
                            case TaskState.Early:
                            case TaskState.Soon:
                                return blue;
                            case TaskState.InProgress:
                                return green;
                            case TaskState.CloseToEnd:
                                return orange;
                            case TaskState.Late:
                                return red;
                        }
                    }
                    else
                    {
                        return blue;
                    }
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
                    onFinished:
                    {
                        mouseArea.enabled = false;
                        root.taskActivated();
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

            NormalText
            {
                id: textDesc
                anchors.top: parent.top
                anchors.bottom: due_task ? parent.verticalCenter : parent.bottom
                anchors.left: iconBubble.right
                anchors.leftMargin: 10
                anchors.topMargin: 6
                anchors.bottomMargin: due_task ? 0 : anchors.topMargin
                text: task.desc
                verticalAlignment: Text.AlignVCenter
            }

            NormalText
            {
                id: textTime
                anchors.left: iconBubble.right
                anchors.top: parent.verticalCenter
                anchors.leftMargin: textDesc.anchors.leftMargin
                anchors.topMargin: textDesc.anchors.bottomMargin
                text: due_task ? due_task.due_time_str : ""
            }

            SequentialAnimation
            {
                running: due_task ? (due_task.state === TaskState.CloseToEnd || task.state === TaskState.Late) : false
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
        if(due_task)
        {
            switch(due_task.state)
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
        else
        {
            return "full"
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
        onFinished: due_task.setAccomplished()
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
        onReleased: rollbackProgress()
    }

    function accomplish()
    {
        animationAccomplish.start();
    }

    function rollbackProgress()
    {
        animationClickProgress.stop();

        animationClickCancel.from = clickProgress.width;
        animationClickCancel.duration = (clickConfirmDuration * (clickProgress.width / root.width) / 3);
        animationClickCancel.start();
    }
}
