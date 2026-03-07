import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
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
            id: button
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
    }
}
