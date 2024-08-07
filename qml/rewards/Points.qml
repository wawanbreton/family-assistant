import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant

RewardsTab
{
    id: root

    ColumnLayout
    {
        anchors.fill: parent

        ThemeItemSelector
        {
            kid: root.kid
            Layout.preferredWidth: root.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            themeCategory: ThemeCategory.Point

            onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { setSelectedItem(item); })
        }

        ThemeItemSelector
        {
            kid: root.kid
            Layout.preferredWidth: root.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            themeCategory: ThemeCategory.PointsStorage

            onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { setSelectedItem(item); })
        }
    }
}
