import QtQuick
import QtQuick.Layouts
import "components"

Item
{
    property var kid

    id: root

    Rectangle
    {
        anchors.fill: parent
        color: "white"
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: spacing
        spacing: 26

        LargeText
        {
            text: kid.name
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            color: "black"
        }

        KidTasksList
        {
            kid: root.kid
            Layout.fillWidth: true
            Layout.fillHeight: true
            onRewardEarned: (amount, item) => triggerEarnPoints(amount, item)
        }
    }

    Vault
    {
        id: vault
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 140
        height: width
        kid: root.kid
    }

    Component
    {
        id: componentPoint
        Point {}
    }

    function triggerEarnPoints(amount: int, item: var)
    {
        var item_rect = root.mapFromItem(item, 0, 0, item.width, item.height)
        for (var i=0 ; i<amount ; ++i)
        {
            var point_item = componentPoint.createObject(root)
            point_item.kid = root.kid

            var x_center = item_rect.x + Math.random() * item.width
            var y_center = item_rect.y + Math.random() * item.height

            point_item.x = x_center - point_item.width / 2
            point_item.y = y_center - point_item.height / 2

            point_item.moveToVault(vault.x + vault.width / 2, vault.y + vault.height / 2)
        }
    }
}
