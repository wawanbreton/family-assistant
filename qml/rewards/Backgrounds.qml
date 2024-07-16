import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant

RewardsTab
{
    id: root

    ThemeItemSelector
    {
        kid: root.kid
        anchors.fill: parent
        anchors.margins: 16
        themeCategory: ThemeCategory.Background
        itemImplicitWidth: itemImplicitHeight * 0.8
        itemImplicitHeight: height * 0.8
        clipGlowEffect: true

        onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { setSelectedItem(item); })
    }
}
