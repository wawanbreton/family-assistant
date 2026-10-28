import QtQuick
import QtQuick.Layouts
import "home"
import "components"

Item
{
    id: root

    Rectangle
    {
        color: "white"
        anchors.fill: parent
    }

    GridLayout
    {
        anchors.fill: parent
        anchors.margins: 32
        columns: 2
        columnSpacing: 32
        rowSpacing: columnSpacing

        DateTime
        {
            id: dateTime
            Layout.preferredHeight: 2
            Layout.preferredWidth: 2
        }

        WeatherForecast
        {
            Layout.rowSpan: 2
            Layout.verticalStretchFactor: 7
            Layout.preferredWidth: 1
        }

        HomeWidget
        {
            id: music
            Layout.preferredHeight: 1
        }
    }
}
