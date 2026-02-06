import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../components"

Rectangle
{
    id: root
    color: "white"
    visible: false
    opacity: 0.0

    readonly property real targetOpacity: 0.9
    property alias progressVisible: progressBar.visible
    property alias progress: progressBar.progress
    property alias progressText: textProgress.text

    ColumnLayout
    {
        anchors.centerIn: parent
        spacing: 32

        Image
        {
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: 256
            Layout.preferredWidth: Layout.preferredHeight
            mipmap: true
            source: DataStorage.findResource("fingerprint-scan", ResourceType.Icon)
        }

        ProgressBar
        {
            id: progressBar
            Layout.alignment: Qt.AlignCenter
            Layout.preferredWidth: root.width * 0.6
            Layout.preferredHeight: 80
            progress: 0.5

            NormalText
            {
                id: textProgress
                anchors.centerIn: parent
                color: "black"
                font.weight: 600
            }
        }

        LargeText
        {
            id: textMessage
            Layout.alignment: Qt.AlignCenter
        }
    }

    Behavior on opacity
    {
        NumberAnimation
        {
            duration: 300
            onRunningChanged:
            {
                if(!running && root.opacity < 0.1)
                {
                    root.visible = false;
                }
            }
        }
    }

    Connections
    {
        target: access
        function onScanFingerprintStart(display_progress: bool)
        {
            textMessage.text = "Veuillez scanner votre empreinte"
            textMessage.color = "black"
            root.visible = true;
            root.opacity = root.targetOpacity
            progressBar.visible = display_progress;
        }

        function onScanFingerprintProgress(step, total_steps)
        {
            textProgress.text = step + "/" + total_steps;
            progressBar.progress = step / total_steps;
        }

        function onScanFingerprintDone()
        {
            textMessage.text = "Empreinte enregistrée !"
            textMessage.color = "green"
        }

        function onScanFingerprintError()
        {
            textMessage.text = "Erreur, veuillez recommencer"
            textMessage.color = "red"
        }
    }

    MouseArea
    {
        anchors.fill: parent
        onClicked: { root.opacity = 0.0 }
    }
}
