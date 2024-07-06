import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../components"

RewardsTab
{
    property var kid

    id: root

    ColumnLayout
    {
        anchors.fill: parent

        ThemeItemSelector
        {
            kid: root.kid
            model: Theme.getAvailablePoints()
            Layout.preferredWidth: root.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            currentIndex: Theme.getAvailablePoints().indexOf(kid.theme.point)

            Loader
            {
                source: Theme.getItemFilePath("points", modelData)
                opacity: PathView.textOpacity
                scale: PathView.textScale
            }

            onItemSelected: (item, button) => root.pointsSpent(currentItem.item.cost, button, () => { kid.theme.point = item; })
        }

        ThemeItemSelector
        {
            kid: root.kid
            model: Theme.getAvailablePoints()
            Layout.preferredWidth: root.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            currentIndex: Theme.getAvailablePoints().indexOf(kid.theme.point)

            Loader
            {
                source: Theme.getItemFilePath("points", modelData)
                opacity: PathView.textOpacity
                scale: PathView.textScale
            }
        }
    }
}
