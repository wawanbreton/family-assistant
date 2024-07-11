import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../components"

RewardsTab
{
    property var kid

    id: root

    ThemeItemSelector
    {
        kid: root.kid
        anchors.fill: parent
        anchors.margins: 16
        themeCategory: ThemeCategory.Background
        itemImplicitWidth: itemImplicitHeight * 0.8
        itemImplicitHeight: height * 0.8

        onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { setSelectedItem(item); })
    }
}
