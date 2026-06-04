import QtQuick 2.15
import QtQuick.Shapes 1.10
import "../../components"

Rectangle
{
    readonly property int cost: 250

    id: root
    color: "#eeeeee"

    ButtonShadow
    {
        source: consoleFrame
        shadowScale: 1.02
        shadowHorizontalOffset: 0
        shadowVerticalOffset: shadowHorizontalOffset
    }

    Rectangle
    {
        id: consoleFrame
        anchors.fill: parent
        anchors.margins: 15
        color: ["#c1bdba", "#1293c5", "#4a5493"][Math.floor(Math.random() * 3)]
        radius: 15

        Rectangle
        {
            id: frameAroundScreen
            width: parent.width * 0.8
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: x
            color:  "#444444"

            SpaceshipGame
            {
                width: parent.width * 0.8
                height: parent.height * 0.8
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Item
        {
            anchors.left: frameAroundScreen.left
            anchors.right: frameAroundScreen.right
            anchors.top: frameAroundScreen.bottom
            anchors.bottom: parent.bottom

            Item
            {
                id: buttons
                height: root.height * 0.2
                width: parent.width
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                Item
                {
                    id: cross
                    height: parent.height
                    width: height
                    anchors.left: parent.left

                    Component
                    {
                        id: buttonTriangle

                        Item
                        {
                            clip: false

                            Shape
                            {
                                id: buttonTriangleShape
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.rightMargin: parent.height * 0.2
                                width: parent.height * 0.6
                                height: parent.height * 0.6
                                clip: false

                                ShapePath
                                {

                                    id: pathButtonTriangle
                                    fillColor: "#888888"
                                    strokeWidth: 5.0
                                    strokeColor: fillColor
                                    capStyle: ShapePath.RoundCap
                                    joinStyle: ShapePath.RoundJoin
                                    startX: pathButtonTriangle.strokeWidth / 2; startY: pathButtonTriangle.strokeWidth / 2
                                    PathLine { x: pathButtonTriangle.strokeWidth / 2; y: buttonTriangleShape.height - pathButtonTriangle.strokeWidth / 2 }
                                    PathLine { x: buttonTriangleShape.width - pathButtonTriangle.strokeWidth / 2; y: buttonTriangleShape.height / 2 }
                                    PathLine { x: pathButtonTriangle.strokeWidth / 2; y: pathButtonTriangle.strokeWidth / 2 }

                                }
                            }
                        }
                    }

                    Component
                    {
                        id: buttonBar

                        Item
                        {
                            ButtonShadow
                            {
                                source: buttonBarSingle
                                shadowScale: 1.1
                                shadowHorizontalOffset: 0
                                shadowVerticalOffset: shadowHorizontalOffset
                            }

                            Rectangle
                            {
                                id: buttonBarSingle
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                height: buttons.height / 3
                                radius: 5
                                color: "#000000"
                                clip: false

                                Loader
                                {
                                    anchors.fill: parent
                                    sourceComponent: buttonTriangle
                                    clip: false
                                }

                                Loader
                                {
                                    anchors.fill: parent
                                    sourceComponent: buttonTriangle
                                    rotation: 180
                                }
                            }
                        }
                    }

                    Loader
                    {
                        anchors.fill: parent
                        sourceComponent: buttonBar
                    }

                    Loader
                    {
                        anchors.fill: parent
                        sourceComponent: buttonBar
                        rotation: 90
                    }
                }

                Item
                {
                    id: roundButtons
                    property var color: ["#7c0c43", "#474033", "#b0b6b7"][Math.floor(Math.random() * 3)]

                    anchors.right: parent.right
                    height: parent.height * 0.8
                    width: parent.height * 1.2
                    anchors.verticalCenter: parent.verticalCenter

                    Component
                    {
                        id: roundButton

                        Item
                        {
                            width: buttons.height / 2
                            height:width

                            ButtonShadow
                            {
                                source: button
                                shadowScale: 1.1
                                shadowHorizontalOffset: 0
                                shadowVerticalOffset: shadowHorizontalOffset
                            }

                            Rectangle
                            {
                                id: button
                                anchors.fill: parent
                                radius: width / 2
                                color: roundButtons.color
                            }
                        }
                    }

                    Loader
                    {
                        anchors.right: parent.right
                        anchors.top: parent.top
                        sourceComponent: roundButton
                    }

                    Loader
                    {
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        sourceComponent: roundButton
                    }
                }
            }
        }
    }
}
