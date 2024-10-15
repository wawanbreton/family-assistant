import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FamilyAssistant
import "components"
import "rewards"
import "settings"

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

            TaskControl { }
        }

        Tab
        {
            title: "Tâches"

            Text
            {
                text: "Rien à voir ici"
            }
        }

        Tab
        {
            title: "Cadre photo"

            Text
            {
                text: "Rien à voir ici"
            }
        }

        Tab
        {
            title: "Météo"

            Text
            {
                text: "Rien à voir ici"
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
