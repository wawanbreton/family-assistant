import QtQuick
import FamilyAssistant
import "components"

Item
{
    property var kid
    property string fontName: kid.theme.font
    property alias fontItem: itemLoader.item
    property string textStyle: kid.theme.text_style
    property alias styleItem: styleLoader.item

    id: root
    implicitHeight: text.implicitHeight
    implicitWidth: text.implicitWidth

    LargeText
    {
        id: text
        anchors.fill: parent
        text: kid.name
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

    Loader
    {
        id: styleLoader
    }

    onTextStyleChanged: styleLoader.setSource(Theme.getItemFilePath(ThemeCategory.TextStyle, textStyle), { "target": text, "parent": this })
}
