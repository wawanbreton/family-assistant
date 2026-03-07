import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../components"
import ".."

RewardsTab
{
    id: root

    readonly property int buyCost: 100

    ColumnLayout
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 16
        spacing: 32

        NormalText
        {
            text: "Temps d'écran restant"
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }

        LargeText
        {
            text: root.kid.screen_time_str
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }

        ConfirmButton
        {
            id: buttonStart
            width: textStart.x + textStart.implicitWidth + 12
            Layout.alignment: Qt.AlignHCenter
            enabled: !kid.screen_time_active && kid.screen_time > 0

            Image
            {
                id: iconStart
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 12
                width: height
                mipmap: true
                source: DataStorage.findResource("screen-time", ResourceType.Icon)
            }

            NormalText
            {
                id: textStart
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: iconStart.right
                anchors.margins: 12
                text: "Démarrer le temps d'écran"
                verticalAlignment: Text.AlignVCenter
            }

            onTriggered:
            {
                kid.startScreenTime();
                main.goToHomeView();
            }
        }

        ConfirmButton
        {
            id: buttonBuy
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 32
            enabled: root.buyCost <= kid.points
            width: row.implicitWidth + row.anchors.leftMargin + row.anchors.rightMargin

            RowLayout
            {
                id: row
                spacing: 12
                anchors.fill: parent
                anchors.leftMargin: 16
                anchors.rightMargin: anchors.leftMargin

                NormalText
                {
                    id: textBuy
                    text: "Ajouter 30 minutes"
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillHeight: true
                }

                PointsCounter
                {
                    id: buyCost
                    kid: root.kid
                    points: root.buyCost.toString()
                    Layout.fillHeight: true
                }
            }

            onTriggered:
            {
                root.pointsSpent(root.buyCost, buttonBuy, () =>
                                 {
                                     animationAddTime.from = kid.screen_time;
                                     animationAddTime.to = kid.screen_time + 30 * 60;
                                     animationAddTime.start();
                                 });
            }
        }

        NumberAnimation
        {
            id: animationAddTime
            target: kid
            property: "screen_time"
            duration: 1000
            easing.type: Easing.OutQuad
        }
    }
}
