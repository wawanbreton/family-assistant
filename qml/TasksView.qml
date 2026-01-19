import QtQuick
import QtQuick.Layouts

Item
{
    id: root

    RowLayout
    {
        id: views
        spacing: 0
        anchors.fill: parent

        Repeater
        {
            model: kid_manager.kids

            KidTasksView
            {
                kid: modelData
                Layout.fillWidth: true
                Layout.fillHeight: true

                onDisplayRewards: mainView.displayRewards(kid)
            }
        }
    }
}
