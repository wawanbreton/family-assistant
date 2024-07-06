import QtQuick
import QtQuick.Shapes

LinearGradient
{
    property real color: blue
    property alias positionTop: gradientStopTop.position
    property alias positionBottom: gradientStopBottom.position
    property bool invertedColor: color === blue && enabled

    readonly property real blue: 0.536
    readonly property real green: 0.383
    readonly property real yellow: 0.140
    readonly property real red: 0.0
    readonly property real orange: 0.064
    readonly property real pink: 0.828
    readonly property var colors: [blue, green, yellow, orange, red, pink]

    id: root

    GradientStop
    {
        id: gradientStopTop
        position: 1.0
        color: invertedColor ? darkerColor() : lighterColor()

        Behavior on color { ColorAnimation { duration: 300; } }

    }
    GradientStop
    {
        id: gradientStopBottom
        position: 0.0
        color: invertedColor ? lighterColor() : darkerColor()

        Behavior on color { ColorAnimation { duration: 300; } }
    }

    function lighterColor()
    {
        return Qt.hsva(root.color, enabled ? 0.694 : 0.0, enabled ? 0.898 : 0.6, 1.0)
    }

    function darkerColor()
    {
        return Qt.hsva(root.color + 0.042, enabled ? 0.663 : 0.0, enabled ? 0.886 : 0.8, 1.0)
    }
}
