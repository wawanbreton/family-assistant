import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FamilyAssistant
import "components"
import "rewards"

Item
{
    property var kid

    signal done()

    id: root

    opacity: 0.0
    Component.onCompleted: opacity = 1.0
    onDone: opacity = 0.0
    onOpacityChanged:
    {
        if(opacity === 0.0)
        {
            destroy()
        }
    }

    Behavior on opacity
    {
        NumberAnimation { duration: 300; easing.type: Easing.InOutQuad; }
    }
}
