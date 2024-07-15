import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import ".."

RewardsTab
{
    id: root

    ColumnLayout
    {
        anchors.fill: parent

        ThemeItemSelector
        {
            kid: root.kid
            Layout.preferredWidth: root.width - 32
            Layout.alignment: Qt.AlignHCenter
            themeCategory: ThemeCategory.Font

            onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { setSelectedItem(item); })

            delegate: KidName
            {
                property var modelData
                property var cost: fontItem.cost

                kid: root.kid
                fontName: modelData
            }
        }

        ThemeItemSelector
        {
            kid: root.kid
            Layout.preferredWidth: root.width - 32
            Layout.alignment: Qt.AlignHCenter
            themeCategory: ThemeCategory.TextStyle

            onItemSelected: (item, button) => root.pointsSpent(currentItem.cost, button, () => { setSelectedItem(item); })

            delegate: KidName
            {
                property var modelData
                property var cost: styleItem.cost

                kid: root.kid
                textStyle: modelData
            }
        }
    }
}
