import QtQuick
import QtQuick.Controls
import "components"

Item
{
    property var kid

    VerticalTabView
    {
        anchors.fill: parent

        Tab
        {
            title: "Coucou"

            Text
            {
                text: "My Item"
                anchors.centerIn: parent
            }
        }

        Tab
        {
            title: "Recoucou"

            Text
            {
                text: "My Other item"
            }
        }
    }
}
