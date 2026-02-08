import QtQuick 2.15
import QtQuick.Shapes
import "../../components"

Rainbow
{
    readonly property int cost: 200

    id: root

    PropertyAnimation
    {
        target: root.gradient
        property: "delta"
        from: 0.0
        to: 1.0
        duration: 5000
        running: true
        loops: Animation.Infinite
    }
}
