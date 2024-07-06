import QtQuick 2.15

Image
{
    readonly property int cost: 20

    source: DataStorage.findResource("christmas_giftbox", DataStorage.Icon)
}
