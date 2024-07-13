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
            opacity: 0.0

            Component.onCompleted: opacity = 1.0
            onDone: opacity = 0.0
            onOpacityChanged:
            {
                if(opacity === 0.0)
                {
                    destroy()
                }
            }

            Behavior on opacity
            {
                NumberAnimation { duration: 300; easing.type: Easing.InOutQuad; }
            }

        }
    }

    function displayRewards(kid: var)
    {
        componentRewards.createObject(root, {kid: kid});
    }

    //Component.onCompleted: displayRewards(kid_manager.kids[0])
}
