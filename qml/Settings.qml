import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import FamilyAssistant
import "components"
import "rewards"
import "settings"

Item
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
        anchors.bottomMargin: 16

        Tab
        {
            title: "Contrôle"

            TaskControl { }
        }

        Tab
        {
            title: "Tâches"

            TaskList { }
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
}
