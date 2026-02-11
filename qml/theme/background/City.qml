import QtQuick 2.15
import QtQuick.Layouts
import FamilyAssistant
import "../../components"

SkyWithClouds
{
    id: root

    readonly property int cost: 200
    maximumCloudsY: height / 2
    cloudsZ: 0.5

    Repeater
    {
        property real base_building_width: root.width / model
        id: repeater
        model: Math.floor((Math.random() + 0.5) * root.width * 0.008)

        Rectangle
        {
            id: building
            anchors.bottom: parent.bottom

            color: Qt.hsva(0.7, 0.023, 0.392 + Math.random() * (0.792 - 0.392), 1.0)

            x: modelData * repeater.base_building_width - (width - repeater.base_building_width) / 2
            y: 0
            z: Math.random()

            width: repeater.base_building_width * (1.0 + Math.random())
            height: root.height * 0.6 * (Math.random() * 0.5 + 1.0)

            GridLayout
            {
                property real windowRadiusFactor: Math.random()
                id: gridWindows
                rows: building.height * 0.015
                columns: building.width * 0.025
                columnSpacing: 10 + 5 * Math.random()
                rowSpacing: 10 + 5 * Math.random()
                anchors.fill: parent
                anchors.margins: (columnSpacing + rowSpacing) / 2

                Repeater
                {
                    model: gridWindows.rows * gridWindows.columns

                    Rectangle
                    {
                        color: "#283f78"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        radius: (width / 4) * parent.windowRadiusFactor
                    }

                    onModelChanged: console.log(model, gridWindows.rows, gridWindows.columns)
                }
            }
        }
    }
}
