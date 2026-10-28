import QtQuick 2.15
import QtQuick.Layouts
import "../components"

HomeWidget
{
    bottomLeftCornerRounded: false
    bottomRightCornerRounded: false

    ColumnLayout
    {
        anchors.fill: parent
        anchors.topMargin: 26
        spacing: 0
        clip: true

        Image
        {
            Layout.fillWidth: true
            source: weather.icon
            fillMode: Image.PreserveAspectFit
        }

        HugeText
        {
            id: textTemperature
            text: weather.temperature
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        Repeater
        {
            model: [1, 3, 7]

            Rectangle
            {
                color: index % 2 == 0 ? "#33ffffff" : "transparent"
                Layout.fillWidth: true
                Layout.preferredHeight: 100

                RowLayout
                {
                    anchors.fill: parent
                    spacing: 32
                    anchors.leftMargin: spacing
                    anchors.rightMargin: spacing

                    LargeText
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredWidth: 2
                        text: weather.forecast[modelData].hourStr
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignRight
                    }

                    LargeText
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredWidth: 2
                        text: weather.forecast[modelData].temperatureStr
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }

                    Item
                    {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        Layout.preferredWidth: 1

                        Image
                        {
                            height: parent.height * 0.6
                            width: height
                            anchors.verticalCenter: parent.verticalCenter
                            source: weather.forecast[modelData].icon
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                }
            }
        }
    }
}
