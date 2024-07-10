import QtQuick
import FamilyAssistant
import "components"

ChangeableImage
{
    property var kid

    id: root
    width: 60
    height: width
    source: Theme.getItemFilePath(ThemeCategory.Point, kid.theme.point)
}
