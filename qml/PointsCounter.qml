import QtQuick
import QtQuick.Layouts
import "components"

RowLayout
{
    property var kid
    property alias points: textNbPoints.text

    id: root
    spacing: 16

    Point
    {
        id: pointImage
        Layout.alignment: Qt.AlignVCenter
        Layout.preferredWidth: 60
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
