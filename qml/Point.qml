import QtQuick
import FamilyAssistant

Image
{
    property var kid

    id: root
    source: DataStorage.findResource("point", DataStorage.Icon)
    width: 60
    height: width

    ParallelAnimation
    {
        id: animationAppear

        PropertyAnimation
        {
            target: root
            property: "scale"
            duration: 300
            easing.type: Easing.OutBack
            from: 0.0
            to: 1.0
        }

        PropertyAnimation
        {
            id: animationX
            target: root
            property: "x"
            easing.type: Easing.InCubic
        }

        PropertyAnimation
        {
            id: animationY
            target: root
            property: "y"
            easing: animationX.easing
            duration: animationX.duration
        }

        onFinished:
        {
            kid.points += 1;
            root.destroy();
        }
    }

    function moveToVault(x_vault, y_vault)
    {
        animationX.to = x_vault - width / 2;
        animationX.duration = 400 + Math.random() * 1000

        animationY.to = y_vault - height / 2;

        animationAppear.start();
    }
}
