import QtQuick 2.15
import FamilyAssistant

Image
{
    readonly property int cost: 200

    source: DataStorage.findResource("treasure", ResourceType.Icon)
}
