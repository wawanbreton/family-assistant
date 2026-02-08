import QtQuick 2.15
import QtQuick.Shapes
import "../../components"

Item
{
    readonly property int cost: 100
    property var target
    property alias gradient: rainbowGradient

    id: root

    Shape
    {
        id: gradientRectangle
        parent: root.target
        anchors.fill: parent

        ShapePath
        {
            fillGradient: LinearGradient
            {
                property real delta: 0.0

                id: rainbowGradient
                spread: ShapeGradient.RepeatSpread
                x1: 0; y1: 0
                x2: gradientRectangle.width; y2: 0
                GradientStop { position: -1.0 + rainbowGradient.delta; color: Qt.hsva(0.0, 1.0, 1.0, 1.0); }
                GradientStop { position: -5/6 + rainbowGradient.delta; color: Qt.hsva(1/6, 1.0, 1.0, 1.0); }
                GradientStop { position: -4/6 + rainbowGradient.delta; color: Qt.hsva(2/6, 1.0, 1.0, 1.0); }
                GradientStop { position: -3/6 + rainbowGradient.delta; color: Qt.hsva(3/6, 1.0, 1.0, 1.0); }
                GradientStop { position: -2/6 + rainbowGradient.delta; color: Qt.hsva(4/6, 1.0, 1.0, 1.0); }
                GradientStop { position: -1/6 + rainbowGradient.delta; color: Qt.hsva(5/6, 1.0, 1.0, 1.0); }
                GradientStop { position: 0 + rainbowGradient.delta; color: Qt.hsva(0.0, 1.0, 1.0, 1.0); }
                GradientStop { position: 1/6 + rainbowGradient.delta; color: Qt.hsva(1/6, 1.0, 1.0, 1.0); }
                GradientStop { position: 2/6 + rainbowGradient.delta; color: Qt.hsva(2/6, 1.0, 1.0, 1.0); }
                GradientStop { position: 3/6 + rainbowGradient.delta; color: Qt.hsva(3/6, 1.0, 1.0, 1.0); }
                GradientStop { position: 4/6 + rainbowGradient.delta; color: Qt.hsva(4/6, 1.0, 1.0, 1.0); }
                GradientStop { position: 5/6 + rainbowGradient.delta; color: Qt.hsva(5/6, 1.0, 1.0, 1.0); }
                GradientStop { position: 1 + rainbowGradient.delta; color: Qt.hsva(1.0, 1.0, 1.0, 1.0); }
            }

            startX: 0.0; startY: 0.0;
            PathLine { x: gradientRectangle.width; y: 0; }
            PathLine { x: gradientRectangle.width; y: gradientRectangle.height; }
            PathLine { x: 0; y: gradientRectangle.height; }
        }
    }

    ShaderEffect
    {
        id: gradientEffect
        parent: root.target.parent

        property var source: ShaderEffectSource
        {
            sourceItem: root.target
            hideSource: true
        }

        property var gradient: ShaderEffectSource
        {
            sourceItem: gradientRectangle
            hideSource: true
        }

        fragmentShader: "qrc:/resources/shaders/text_mask.frag.qsb"

        // Only set anchors when parent is set, otherwise QML gets confused
        onParentChanged: anchors.fill = root.target
    }

    TextOutline
    {
        id: textOutline
        target: gradientEffect
        parent: gradientEffect.parent
        borderWidth: 4
        blurFactor: 1.0
        color: "white"
        z: -1.0
    }
}
