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
        target: access

        function onAdminLoggedIn()
        {
            setMainItem(componentSettings);
        }

        function onKidLoggedIn(kid)
        {
            setMainItem(componentRewards, {kid: kid});
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
        access.tryLogKidIn(kid);
    }

    function setView(view: string)
    {
        if(view === "children")
        {
            setMainItem(componentKidsTasks);
        }
        else if(view === "settings")
        {
            access.tryLogAdminIn();
        }
    }
}
