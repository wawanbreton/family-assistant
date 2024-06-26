import QtQuick

Gradient
{
    property real color: blue

    readonly property real blue: 0.536
    readonly property real green: 0.383
    readonly property real yellow: 0.140
    readonly property real red: 0.0
    readonly property real orange: 0.064
    readonly property real pink: 0.828

    id: root

    GradientStop
    {
        position: 1.0
        color: Qt.hsva(root.color, 0.694, 0.898, 1.0)

    }
    GradientStop
    {
        position: 0.0
        color: Qt.hsva(root.color + 0.042, 0.663, 0.886, 1.0)
    }
}
