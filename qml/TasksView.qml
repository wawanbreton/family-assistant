import QtQuick
import QtQuick.Effects
import QtQuick.Layouts

Item
{
    property var popup

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

    MultiEffect
    {
        id: blurEffect
        source: views
        anchors.fill: views
        blur: 1.0
        blurEnabled: true

        Behavior on blur
        {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad; }
        }
    }

    MouseArea
    {
        id: mouseEventsCatcher
        anchors.fill: parent
        onPressed:
        {
            root.state = "normal";
            root.popup.opacity = 0.0
        }
    }

    Component
    {
        id: componentRewards

        MouseArea
        {
            property alias kid: rewards.kid

            id: mouseEventsBlocker
            width: parent.width * 0.8
            height: parent.height * 0.8
            anchors.centerIn: parent

            Rewards
            {
                id: rewards
                anchors.fill: parent
                opacity: 0.0
                Component.onCompleted: opacity = 1.0
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
    }

    state: "normal"
    states:
    [
        State
        {
            name: "normal"

            PropertyChanges
            {
                target: blurEffect
                blur: 0.0
            }

            PropertyChanges
            {
                target: mouseEventsCatcher
                enabled: false
            }
        },
        State
        {
            name: "popup"

            PropertyChanges
            {
                target: blurEffect
                blur: 1.0
            }

            PropertyChanges
            {
                target: mouseEventsCatcher
                enabled: true
            }
        }
    ]

    function displayRewards(kid: var)
    {
        root.popup = componentRewards.createObject(root, {kid: kid});
        root.state = "popup";
    }

    Component.onCompleted: displayRewards(kid_manager.kids[0])
}
