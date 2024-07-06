import QtQuick 2.15
import QtQuick.Layouts
import "../components"
import ".."

Item
{
    property var kid
    property alias model: carousel.model
    property alias currentIndex: carousel.currentIndex
    property alias currentItem: carousel.currentItem
    default property alias delegate: carousel.delegate

    signal itemSelected(string item, var button)

    id: root
    implicitHeight: column.implicitHeight

    ColumnLayout
    {
        id: column
        anchors.fill: parent
        spacing: 16

        Carousel
        {
            id: carousel
            model: Theme.getAvailablePoints()
            Layout.fillWidth: true
            currentIndex: Theme.getAvailablePoints().indexOf(kid.theme.point)

            Loader
            {
                source: Theme.getItemFilePath("points", modelData)
                opacity: PathView.textOpacity
                scale: PathView.textScale
            }
        }

        ConfirmButton
        {
            id: button
            width: 300
            Layout.alignment: Qt.AlignHCenter
            enabled: Theme.getAvailablePoints()[carousel.currentIndex] !== kid.theme.point && carousel.currentItem.item.cost <= kid.points

            PointsCounter
            {
                kid: root.kid
                points: carousel.currentItem.item.cost
                anchors.centerIn: parent
            }

            onTriggered: root.itemSelected(Theme.getAvailablePoints()[carousel.currentIndex], button)
        }
    }
}
