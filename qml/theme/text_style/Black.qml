import QtQuick 2.15
import "../../components"

Item
{
    readonly property int cost: 10
    property var target
    property alias outline: textOutline
    property string color: "black"

    id: root

    Component.onCompleted: if(color !== "") { target.color = color }

    TextOutline
    {
        id: textOutline
        target: root.target
        parent: root.target.parent
        borderWidth: 4
        blurFactor: 1.0
        color: "white"
        z: -1.0
    }
}
