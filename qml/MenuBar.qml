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
            model: ["home", "children", "settings"]

            MenuBarButton
            {
                imageSource: DataStorage.findResource(modelData, ResourceType.Icon)
                onClicked: root.buttonClicked(modelData)
            }
        }
    }

    Connections
    {
        target: hardware.power

        function onBacklightStateChanged()
        {
            root.autoChangeScreen();
        }
    }

    Instantiator
    {
        model: user_manager.kids

        delegate: Connections
        {
            required property QtObject modelData

            target: modelData.tasks

            function onChanged()
            {
                root.autoChangeScreen();
            }
        }
    }

    function autoChangeScreen()
    {
        if(hardware.power.backlightState === BacklightState.Asleep)
        {
            var has_active_task = false;
            for(let kid of user_manager.kids)
            {
                if(kid.tasks.hasActiveTask())
                {
                    has_active_task = true;
                }
            }

            root.buttonClicked(has_active_task ? "children" : "home");
        }
        else
        {
            root.buttonClicked("children");
        }
    }
}
