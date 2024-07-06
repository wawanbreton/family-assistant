import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../components"

Item
{
    property var kid

    id: root

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 24
        uniformCellSizes: true

        Carousel
        {
            model: Theme.getAvailablePoints()
            Layout.preferredWidth: root.width * 0.7
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignHCenter
            currentIndex: Theme.getAvailablePoints().indexOf(kid.theme.point)

            Loader
            {
                source: Theme.getItemFilePath("points", modelData)
                opacity: PathView.textOpacity
                scale: PathView.textScale
            }
        }

        Carousel
        {
            model: 10
            Layout.fillWidth: true
            Layout.fillHeight: true

            Text { text: index; font.pointSize: 200; opacity: PathView.textOpacity; scale: PathView.textScale; }
        }
    }
}
