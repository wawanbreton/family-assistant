import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import ".."
import "../components"

RewardsTab
{
    id: root

    ColumnLayout
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 16
        spacing: 32

        Item
        {
            Layout.preferredHeight: 160
            Layout.fillWidth: true

            Image
            {
                id: iconAdult
                anchors.left: parent.left
                anchors.leftMargin: 64
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                source: DataStorage.findResource("family", ResourceType.Icon)
                fillMode: Image.PreserveAspectFit
            }

            SmallText
            {
                text: "Avant d'acheter une récompense spéciale, assure-toi qu'un adulte est présent pour la valider !"
                anchors.left: iconAdult.right
                anchors.leftMargin: 32
                anchors.right: parent.right
                anchors.rightMargin: iconAdult.anchors.leftMargin
                anchors.verticalCenter: parent.verticalCenter
                wrapMode: Text.WordWrap
            }
        }

        Carousel
        {
            id: carousel
            model: special_rewards.special_rewards
            Layout.fillWidth: true

            Item
            {
                property var modelData
                id: taskItemParent

                ConfirmButton
                {
                    id: buttonBuy
                    width: textName.x + textName.width + 24
                    anchors.centerIn: parent
                    clickConfirmDuration: 2000
                    enabled: root.kid.points >= modelData.cost

                    Rectangle
                    {
                        id: iconBubble
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.margins: 12
                        width: height
                        color: "white"
                        radius: height / 2

                        Image
                        {
                            anchors.fill: parent
                            anchors.margins: 10
                            id: name
                            source: modelData.icon
                        }
                    }

                    NormalText
                    {
                        id: textName
                        anchors.top: iconBubble.top
                        anchors.left: iconBubble.right
                        anchors.leftMargin: 10
                        text: modelData.name
                        verticalAlignment: Text.AlignTop
                    }

                    PointsCounter
                    {
                        kid: root.kid
                        points: modelData.cost
                        iconSize: 30
                        textSize: 26
                        anchors.left: textName.left
                        anchors.bottom: iconBubble.bottom
                    }

                    onTriggered:
                    {
                        glow.startGlowing();
                        root.pointsSpent(modelData.cost, buttonBuy, () =>
                                         {
                                             var text = root.kid.pronounced_name + " a acheté " + modelData.pronouncedName;
                                             announcement.addAnnouncement(text);
                                         });
                    }
                }

                Glow
                {
                    id: glow
                    anchors.fill: buttonBuy
                    source: buttonBuy
                    animatiomDuration: 600
                }
            }
        }
    }
}
