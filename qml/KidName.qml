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
    readonly property int extraHeight: 30 // Add extra height for some fonts that miss the accents in their metrics

    id: root
    implicitHeight: text.implicitHeight
    implicitWidth: text.implicitWidth

    Item
    {
        id: textContainer
        width: text.implicitWidth
        height: text.implicitHeight + root.extraHeight
        y: -root.extraHeight

        LargeText
        {
            id: text
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: implicitHeight
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
    }

    Loader
    {
        id: styleLoader
    }

    onTextStyleChanged: styleLoader.setSource(Theme.getItemFilePath(ThemeCategory.TextStyle, textStyle), { "target": text, "parent": textContainer })
}
