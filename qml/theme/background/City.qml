import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Shapes 1.10
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
            height: root.height * 0.55 * (Math.random() * 0.5 + 1.0)

            Shape
            {
                property real spikeSide: [0, 0.5, 1.0][Math.floor(Math.random() * 3)]
                id: roof
                layer.enabled: true
                layer.samples: 4
                anchors.bottom: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: roof.width * (0.25 + Math.random() * 0.25)
                visible: Math.random() > 0.3

                ShapePath
                {
                    fillColor: Qt.hsva(0.7, 0.023, 0.392 + Math.random() * (0.792 - 0.392), 1.0)
                    strokeWidth: -1.0
                    startX: 0; startY: roof.height
                    PathLine { x: roof.width; y: roof.height }
                    PathLine { x: roof.spikeSide * roof.width; y: 0 }
                }

            }

            Rectangle
            {
                id: antenna
                visible: roof.visible && roof.spikeSide == 0.5 && Math.random() > 0.5
                anchors.horizontalCenter: roof.horizontalCenter
                anchors.bottom: roof.top
                anchors.bottomMargin: -width
                width: 3 + Math.random() * 5
                height: 25 + Math.random() * 50
                radius: width / 4
                color: "#5a7ca8"
            }

            GridLayout
            {
                property real windowRadiusFactor: Math.random()
                id: gridWindows
                rows: building.height * 0.015 * (Math.random() * 0.5 + 0.75)
                columns: building.width * 0.025 * (Math.random() * 0.5 + 0.75)
                columnSpacing: 10 + 5 * Math.random()
                rowSpacing: 10 + 5 * Math.random()
                anchors.fill: parent
                anchors.margins: (columnSpacing + rowSpacing) * (Math.random() / 2.0 + 0.5)

                Repeater
                {
                    model: gridWindows.rows * gridWindows.columns

                    Rectangle
                    {
                        color: "#5a7ca8"
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        radius: (width / 4) * parent.windowRadiusFactor
                    }
                }
            }
        }
    }

    Repeater
    {
        model: Math.floor((Math.random() + 0.5) * root.width * 0.012)

        Image
        {
            property int sourceId: Math.floor(Math.random() * 7)
            source: DataStorage.findResource(["tree1", "tree2", "tree3", "tree4", "street-lamp1",
                                              "street-lamp1", "traffic-light"][sourceId],
                                             ResourceType.Icon)
            anchors.bottom: root.bottom
            z: 2.0
            x: -width / 2 + root.width * Math.random()
            height: 100 + (sourceId <= 3 ? 200 : 50) * Math.random()
            width: height
        }
    }
}
