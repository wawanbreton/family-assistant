import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"
import "rewards"

Item
{
    property var kid

    signal done()

    id: root

    Rectangle
    {
        anchors.fill: parent
        color: "white"
    }

    VerticalTabView
    {
        anchors.fill: parent
        anchors.margins: 16
        anchors.bottomMargin: buttonBack.height + 16

        Tab
        {
            title: "Texte"

            Text
            {
                text: "My Item"
                anchors.centerIn: parent
            }
        }

        Tab
        {
            title: "Fonds"

            Backgrounds
            {
                kid: root.kid
                onPointsSpent: (amount, button, callback) => root.transferPoints(amount, button, callback)
            }
        }

        Tab
        {
            title: "Points"

            Points
            {
                kid: root.kid
                onPointsSpent: (amount, button, callback) => root.transferPoints(amount, button, callback)
            }
        }

        Tab
        {
            title: "Temps d'écran"

            Text
            {
                text: "My Other item"
            }
        }

        Tab
        {
            title: "Spécial"

            Text
            {
                text: "My Other item"
            }
        }
    }

    Vault
    {
        id: vault
        kid: root.kid
        pointsAlwaysVisible: true
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }

    KidName
    {
        kid: root.kid
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
    }

    CornerButton
    {
        id: buttonBack
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: DataStorage.findResource("arrow", DataStorage.Icon)
        imageScale: 0.75
        corner: Qt.BottomRightCorner

        onPressed: root.done()
    }

    Component
    {
        id: componentTransfer

        PointsTransfer
        {
            kid: root.kid
            parentObject: root
            rectStart: Qt.rect(vault.x + vault.width / 2 - 30, vault.y + vault.height / 2 - 30, 60, 60)
            pointDelta: -1
        }
    }

    function transferPoints(amount, button, callback)
    {
        var item_rect = root.mapFromItem(button, 0, 0, button.width, button.height);
        var item_center = Qt.point(item_rect.x + item_rect.width / 2, item_rect.y + item_rect.height / 2);
        var transfer = componentTransfer.createObject(root, { amount: amount, targetPosition: item_center });
        transfer.onFinished.connect(callback);
    }
}
