import QtQuick
import QtQuick.Effects
import QtQuick.Layouts

Item
{
    id: root

    RowLayout
    {
        id: views
        spacing: 0
        anchors.fill: parent

        Repeater
        {
            model: kid_manager.kids

            KidTasksView
            {
                kid: modelData
                Layout.fillWidth: true
                Layout.fillHeight: true

                onDisplayRewards: root.displayRewards(kid)
            }
        }
    }

    Component
    {
        id: componentRewards

        Rewards
        {
            id: rewards
            anchors.fill: parent
        }
    }

    Component
    {
        id: componentSettings

        Settings
        {
            id: settings
            anchors.fill: parent
        }
    }

    Connections
    {
        target: hardware

        function onParentUnlock()
        {
            componentSettings.createObject(root);
        }
    }

    Component.onCompleted: componentSettings.createObject(root);

    function displayRewards(kid: var)
    {
        componentRewards.createObject(root, {kid: kid});
    }
}
