import QtQuick
import QtQuick.Layouts
import "components"

RowLayout
{
    property var kid
    property alias points: textNbPoints.text
    property int iconSize: 60
    property alias textSize: textNbPoints.fontSize

    id: root
    spacing: 16

    Point
    {
        id: pointImage
        Layout.alignment: Qt.AlignVCenter
        Layout.preferredWidth: root.iconSize
        Layout.preferredHeight: Layout.preferredWidth
        kid: root.kid
    }

    LargeText
    {
        id: textNbPoints
        text: kid.points
        Layout.alignment: Qt.AlignVCenter
    }
}
