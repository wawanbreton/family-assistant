import QtQuick 2.15
import FamilyAssistant
import "../../components"
import "generic"

Sky
{
    readonly property int cost: 100
    property real cloudsZ: 0.0
    property alias maximumCloudsY: slidingBackground.maximumElementsY

    id: root

    SlidingBackground
    {
        id: slidingBackground
        anchors.fill: parent
        speedMin: 25
        speedMax: 50

        Image
        {
            id: imageCloud
            source: DataStorage.findResource("cloud", ResourceType.Icon)
            z: root.cloudsZ
            width: (implicitWidth * 0.3) * (Math.random() + 0.5)
            height: (implicitHeight / implicitWidth) * width

            RandomMirror { }
        }
    }
}
