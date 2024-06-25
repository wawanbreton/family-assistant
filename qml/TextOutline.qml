import QtQuick
import QtQuick.Effects

Item
{
    property alias target: effectSource.sourceItem
    property alias borderWidth: effect.border_width
    property alias blurFactor: effect.blur_factor
    property alias color: effect.color

    anchors.centerIn: target
    width: target.width + borderWidth * 2
    height: target.height + borderWidth * 2
    opacity: target.opacity

    ShaderEffectSource
    {
        id: effectSource
    }

    ShaderEffect
    {
        id: effect
        anchors.fill: parent
        property variant target_size: Qt.size(target.width, target.height)
        property variant source: effectSource
        property int border_width: 4
        property real blur_factor: 1.0
        property color color: "black"
        fragmentShader: "qrc:/resources/shaders/text_outline.frag.qsb"
        blending: true
    }
}
