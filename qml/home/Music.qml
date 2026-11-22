import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls
import FamilyAssistant
import "../components"
import ".."

HomeWidget
{
    ColumnLayout
    {
        anchors.fill: parent
        spacing: 16
        anchors.margins: 16

        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout
            {
                anchors.fill: parent

                Rectangle
                {
                    color: "white"
                    radius: 16
                    Layout.fillHeight: true
                    Layout.preferredWidth: height
                    visible: music.playing

                    Image
                    {
                        anchors.fill: parent
                        anchors.margins: 16
                        source: music.iconPath.length == 0 ? DataStorage.findResource("music", ResourceType.Icon) : music.iconPath
                        fillMode: Image.PreserveAspectFit
                    }
                }

                ColumnLayout
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    NormalText
                    {
                        text: music.title
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignBottom
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    NormalText
                    {
                        text: music.artist
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignTop
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        font.bold: true
                    }
                }
            }
        }

        RowLayout
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Item { Layout.fillWidth: true }

            Item
            {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                MusicButton
                {
                    anchors.centerIn: parent
                    width: parent.width * 0.8
                    height: width
                    enabled: music.playing && music.hasPrevious
                    imageSource: DataStorage.findResource("previous", ResourceType.Icon)
                    onClicked: music.previousButtonClicked()
                }
            }

            MusicButton
            {
                Layout.fillHeight: true
                Layout.preferredWidth: height
                imageSource: DataStorage.findResource(music.playing ? "stop" : "play", ResourceType.Icon)
                onClicked: music.mainButtonClicked()
            }

            Item
            {
                Layout.fillHeight: true
                Layout.preferredWidth: height

                MusicButton
                {
                    anchors.centerIn: parent
                    width: parent.width * 0.8
                    height: width
                    enabled: music.playing
                    imageSource: DataStorage.findResource("next", ResourceType.Icon)
                    onClicked: music.nextButtonClicked()
                }
            }

            Item { Layout.fillWidth: true }
        }

        Slider
        {
            id: slider
            Layout.fillWidth: true
            Layout.preferredHeight: 64
            value: music.volume

            background: Item
            {
                height: 16
                width: parent.availableWidth
                y: parent.topPadding + parent.availableHeight / 2 - height / 2

                Rectangle
                {
                    id: sliderFilled
                    width: handleDisc.x + handleDisc.width / 2
                    height: parent.height
                    color: "#e4cc46"
                    radius: height / 2
                }

                Rectangle
                {
                    height: parent.height
                    anchors.left: sliderFilled.right
                    anchors.right: parent.right
                    radius: height / 2
                    color: "#ffffff"
                }

            }

            handle: Rectangle
            {
                id: handleDisc
                x: parent.leftPadding + parent.visualPosition * (parent.availableWidth - width)
                y: parent.topPadding + parent.availableHeight / 2 - height / 2
                height: parent.height
                width: parent.height
                radius: width / 2
                color: "#ffffff"
            }

            ButtonShadow
            {
                source: handleDisc
                shadowScale: parent.pressed ? 0.96 : 1.0
                shadowHorizontalOffset: parent.pressed ? 0 : 4
                shadowVerticalOffset: shadowHorizontalOffset
                visible: !parent.pressed
            }

            onValueChanged: music.volume = value
        }
    }
}
