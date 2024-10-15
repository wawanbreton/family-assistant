import QtQuick 2.15
import QtQuick.Effects

MultiEffect
{
    property int animatiomDuration: 1500

    id: glow
    blurEnabled: true
    z: 1.0
    visible: false

    SequentialAnimation
    {
        id: animationShine

        ParallelAnimation
        {
            PropertyAnimation
            {
                target: glow
                easing.type: Easing.InOutQuad
                properties: "brightness,blur"
                to: 0.7
                duration: glow.animatiomDuration
            }

            PropertyAnimation
            {
                target: glow
                property: "scale"
                easing.type: Easing.InOutQuad
                to: 1.5
                duration: glow.animatiomDuration
            }
        }

        ParallelAnimation
        {
            PropertyAnimation
            {
                target: glow
                easing.type: Easing.InOutQuad
                properties: "brightness,blur,scale"
                to: 0.0
                duration: glow.animatiomDuration
            }

            PropertyAnimation
            {
                target: glow
                property: "scale"
                easing.type: Easing.InOutQuad
                to: 1.0
                duration: glow.animatiomDuration
            }
        }

        onFinished: { glow.visible = false; glow.source.visible = true; }
    }

    function startGlowing()
    {
        glow.source.visible = false
        glow.visible = true
        animationShine.start();
    }
}
