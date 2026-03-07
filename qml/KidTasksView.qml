import QtQuick
import QtQuick.Layouts
import FamilyAssistant
import "components"

Item
{
    property var kid

    signal displayRewards()

    id: root

    Loader
    {
        anchors.fill: parent
        source: Theme.getItemFilePath(ThemeCategory.Background, kid.theme.background)
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: spacing
        spacing: 26

        KidName
        {
            kid: root.kid
            Layout.alignment: Qt.AlignHCenter
        }

        KidTasksList
        {
            kid: root.kid
            Layout.fillWidth: true
            Layout.fillHeight: true
            onRewardEarned: (amount, item) =>
            {
                var item_rect = root.mapFromItem(item, 0, 0, item.width, item.height);
                var transfert = componentPoint.createObject(root, { amount: amount, rectStart: item_rect });
            }
        }
    }

    Vault
    {
        id: vault
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        kid: root.kid

        onDisplayRewards: root.displayRewards()
    }

    ActiveScreenTime
    {
        id: activeScreenTime
        anchors.right: parent.right
        anchors.rightMargin: -height / 2

        kid: root.kid
        state: root.kid.screen_time_active ? "displayed" : "hidden"

        states:
        [
            State
            {
                name: "displayed"

                AnchorChanges
                {
                    target: activeScreenTime
                    anchors.bottom: parent.bottom
                    anchors.top: undefined
                }
            },

            State
            {
                name: "hidden"

                AnchorChanges {
                    target: activeScreenTime
                    anchors.bottom: undefined
                    anchors.top: parent.bottom
                }
            }
        ]

        transitions:
        [
             Transition
            {
                to: "*"

                 AnchorAnimation
                 {
                     id: animationExpand
                     duration: 300
                     easing.type: Easing.OutQuad
                 }
             }
         ]
    }

    Component
    {
        id: componentPoint

        PointsTransfer
        {
            kid: root.kid;
            parentObject: root;
            targetPosition: Qt.point(vault.x + vault.width / 2, vault.y + vault.height / 2)
        }
    }
}
