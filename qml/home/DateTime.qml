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

        BaseText
        {
            id: textTime
            text: time.time
            font.pointSize: 88
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
