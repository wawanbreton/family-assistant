import QtQuick 2.15
import QtQuick.Layouts
import "../components"

HomeWidget
{
    ColumnLayout
    {
        anchors.fill: parent
        anchors.topMargin: 0
        anchors.bottomMargin: 26
        spacing: -10

        HugeText
        {
            id: textTime
            text: time.time
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        LargeText
        {
            id: textDate
            text: time.date
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }
    }
}
