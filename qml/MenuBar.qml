import QtQuick
import QtQuick.Layouts
import FamilyAssistant
import "components"

Rectangle
{
    signal buttonClicked(string id)

    id: root
    width: 120

    gradient: StyledGradient
    {
        color: blue
    }

    Column
    {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width - 16
        spacing: 22

        Repeater
        {
            model: ["children", "settings"]

            MenuBarButton
            {
                imageSource: DataStorage.findResource(modelData, ResourceType.Icon)
                onClicked: root.buttonClicked(modelData)
            }
        }
    }

}
