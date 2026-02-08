import QtQuick 2.15
import FamilyAssistant

Item
{
    readonly property int cost: 100
    readonly property string font: DataStorage.findResource(font_name, ResourceType.Font)
    property string font_name
}
