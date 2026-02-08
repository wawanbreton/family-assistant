import QtQuick 2.15
import FamilyAssistant

Image
{
    readonly property int cost: 1

    source: DataStorage.findResource("poop", ResourceType.Icon)
}
