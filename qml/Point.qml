import QtQuick
import FamilyAssistant
import "components"

ChangeableImage
{
    property var kid
    property alias size: root.width

    id: root
    width: 60
    height: width
    source: Theme.getItemFilePath(ThemeCategory.Point, kid.theme.point)
}
