import QtQuick
import QtQuick.Effects
import QtQuick.Layouts
import "components"

Item
{
    id: root

    Component
    {
        id: componentKidsTasks

        TasksView {}
    }

    Component
    {
        id: componentRewards

        Rewards {}
    }

    Component
    {
        id: componentSettings

        Settings {}
    }

    Connections
    {
        target: hardware

        function onParentUnlock()
        {
            setMainItem(componentSettings);
        }
    }

    UniqueItemContainer
    {
        id: uniqueContainer
        anchors.fill: parent
    }

    Component.onCompleted: setMainItem(componentKidsTasks, {})

    function setMainItem(component: var, arguments)
    {
        var item = component.createObject(null, arguments);
        uniqueContainer.setItem(item);
    }

    function displayRewards(kid: var)
    {
        setMainItem(componentRewards, {kid: kid});
    }

    function setView(view: string)
    {
        if(view === "children")
        {
            setMainItem(componentKidsTasks);
        }
        else if(view === "settings")
        {
            setMainItem(componentSettings);
        }
    }
}
