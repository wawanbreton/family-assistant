import QtQuick 2.15
import QtQuick.Controls

Item
{
    StackView
    {
        id: stack
        anchors.fill: parent

        replaceEnter: Transition
        {
            PropertyAnimation
            {
                id: animationOpacity
                property: "opacity"
                from: 0
                to: 1
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        replaceExit: Transition
        {
            PauseAnimation { duration: animationOpacity.duration }
        }

        popEnter: Transition {}
        popExit: Transition {}
        pushEnter: Transition {}
        pushExit: Transition {}
    }


    function setItem(item: var)
    {
        stack.replaceCurrentItem(item);
    }
}
