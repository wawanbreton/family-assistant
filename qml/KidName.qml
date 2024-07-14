import QtQuick
import FamilyAssistant
import "components"

LargeText
{
    property var kid
    property string fontName: kid.theme.font
    property alias item: itemLoader.item

    text: kid.name
    color: "black"
    font.family: fontLoader.font.family

    Loader
    {
        id: itemLoader
        source: fontName !== "" ? Theme.getItemFilePath(ThemeCategory.Font, fontName) : ""
    }

    FontLoader
    {
        id: fontLoader
        source: itemLoader.item.font
    }
}
