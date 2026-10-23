import QtQuick 2.15
import QtQuick.Layouts
import "../components"


Item
{
    Layout.fillWidth: true
    Layout.fillHeight: true

    ButtonShadow
    {
        source: rectangle
        shadowScale: 1.0
        shadowHorizontalOffset: 4
        shadowVerticalOffset: shadowHorizontalOffset
    }

    Rectangle
    {
        id: rectangle
        anchors.fill: parent
        radius: 50
        clip: true
        gradient: StyledGradient {}
    }
}