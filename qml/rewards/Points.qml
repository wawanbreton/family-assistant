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
            Layout.preferredWidth: root.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            currentKidItem: kid.theme.point
            themeCategory: ThemeCategory.Points

            onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { kid.theme.point = item; })
        }

        ThemeItemSelector
        {
            kid: root.kid
            Layout.preferredWidth: root.width * 0.6
            Layout.alignment: Qt.AlignHCenter
            currentKidItem: kid.theme.points_storage
            themeCategory: ThemeCategory.PointsStorage

            onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { kid.theme.points_storage = item; })
        }
    }
}
