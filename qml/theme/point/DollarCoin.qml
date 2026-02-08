import QtQuick 2.15
import FamilyAssistant

Image
{
    readonly property int cost: 100

    source: DataStorage.findResource("dollar_coin", ResourceType.Icon)
}
