import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "components"
import "rewards"

Item
{
    property var kid

    signal done()

    id: root

    Rectangle
    {
        anchors.fill: parent
        color: "white"
    }

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: spacing
        spacing: 16

        Rectangle
        {
            height: 100
            width: row.implicitWidth + radius * 2
            Layout.alignment: Qt.AlignHCenter
            radius: height / 2
            gradient: StyledGradient {}

            RowLayout
            {
                id: row
                anchors.fill: parent
                anchors.leftMargin: parent.radius * 0.6
                anchors.rightMargin: anchors.leftMargin
                spacing: 0

                LargeText
                {
                    text: kid.name
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                }

                Item { Layout.preferredWidth: 10 }

                Point
                {
                    kid: root.kid
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: Layout.preferredWidth
                }

                LargeText
                {
                    text: kid.points
                    Layout.fillHeight: true
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        VerticalTabView
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Tab
            {
                title: "Texte"

                Text
                {
                    text: "My Item"
                    anchors.centerIn: parent
                }
            }

            Tab
            {
                title: "Fonds"

                Text
                {
                    text: "My Other item"
                }
            }

            Tab
            {
                title: "Points"
                Points {}
            }

            Tab
            {
                title: "Temps d'écran"

                Text
                {
                    text: "My Other item"
                }
            }

            Tab
            {
                title: "Spécial"

                Text
                {
                    text: "My Other item"
                }
            }
        }
    }

    CornerButton
    {
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        imageSource: DataStorage.findResource("arrow", DataStorage.Icon)
        imageScale: 0.75

        onPressed: root.done()
    }
}
