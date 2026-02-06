import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../components"
import ".."

Item
{
    ColumnLayout
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 16
        spacing: 32

        NormalText
        {
            text: "Empreintes digitales"
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }

        Carousel
        {
            id: carouselUsers
            model: user_manager.users
            Layout.fillWidth: true

            Item
            {
                property var modelData
                id: nameItemParent

                LargeText
                {
                    id: text
                    anchors.fill: parent
                    text: nameItemParent.modelData.name
                    anchors.centerIn: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        ConfirmButton
        {
            id: button
            width: textFingerprint.x + textFingerprint.implicitWidth + 12
            Layout.alignment: Qt.AlignHCenter

            Image
            {
                id: iconFingerprint
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 12
                width: height
                mipmap: true
                source: DataStorage.findResource("fingerprint-scan", ResourceType.Icon)
            }

            NormalText
            {
                id: textFingerprint
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: iconFingerprint.right
                anchors.margins: 12
                text: "Scanner l'empreinte digitale"
                verticalAlignment: Text.AlignVCenter
            }

            onTriggered: access.registerFingerprintForUser(carouselUsers.model[carouselUsers.currentIndex])
        }
    }
}
