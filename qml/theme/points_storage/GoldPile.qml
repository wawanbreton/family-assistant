import QtQuick 2.15
import FamilyAssistant

Image
{
    readonly property int cost: 150

    source: DataStorage.findResource("gold_pile", ResourceType.Icon)
}
