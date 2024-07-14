import QtQuick 2.15

Item
{
    property var kid

    signal pointsSpent(amount: int, targetItem: var, callback: var)
}
