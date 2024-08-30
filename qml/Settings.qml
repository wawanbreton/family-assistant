import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FamilyAssistant
import "components"
import "rewards"

PopupDialog
{
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
            title: "Contrôle"

            NameTexts
            {
                kid: root.kid
                onPointsSpent: (amount, button, callback) => root.transferPoints(amount, button, callback)
            }
        }

        Tab
        {
            title: "Tâches"

            Backgrounds
            {
                kid: root.kid
                onPointsSpent: (amount, button, callback) => root.transferPoints(amount, button, callback)
            }
        }

        Tab
        {
            title: "Cadre photo"

            Points
            {
                kid: root.kid
                onPointsSpent: (amount, button, callback) => root.transferPoints(amount, button, callback)
            }
        }

        Tab
        {
            title: "Météo"

            Text
            {
                text: "My Other item"
            }
        }
    }

    CornerButton
    {
        id: buttonBack
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        imageSource: DataStorage.findResource("arrow", ResourceType.Icon)
        imageScale: 0.75
        corner: Qt.BottomRightCorner

        onPressed: root.done()
    }
}
