import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Effects
import QtQuick.Shapes

Item
{
    default property list<Item> tabs
    readonly property int tabsWidth: 300
    readonly property int tabsHeight: 100
    property int currentTab: 0

    id: root

    Repeater
    {
        model: tabs

        Item
        {
            anchors.fill: parent
            z: index == root.currentTab ? 1.0 : 0.0

            Shape
            {
                id: shape
                anchors.fill: parent
                preferredRendererType: Shape.CurveRenderer

                ShapePath
                {
                    fillGradient: StyledGradient
                    {
                        color: colors[index % colors.length]
                        y1: 0.0
                        y2: height;
                    }
                    strokeWidth: -1
                    pathHints: ShapePath.PathFillOnRight | ShapePath.PathSolid | ShapePath.PathNonIntersecting

                    startX: root.tabsWidth; startY: (index + 1) * root.tabsHeight;

                    readonly property list<QtObject> tabPath:
                    [
                        PathLine { x: root.tabsHeight / 2.0; y: (index + 1) * root.tabsHeight; },
                        PathArc
                        {
                            x: root.tabsHeight / 2.0
                            y: index * root.tabsHeight
                            radiusX: root.tabsHeight / 2.0
                            radiusY: radiusX
                        },
                        PathLine { x: root.tabsWidth; y: index * root.tabsHeight; }
                    ]

                    readonly property list<QtObject> widgetPath:
                    [
                        PathLine { x: root.tabsWidth; y: 0; },
                        PathLine { x: width; y: 0; },
                        PathLine { x: width; y: height; },
                        PathLine { x: root.tabsWidth; y: height; }
                    ]

                    pathElements: root.currentTab == index ? tabPath.concat(widgetPath) : tabPath

                }
            }

            MultiEffect
            {
                source: shape
                anchors.fill: parent
                shadowBlur: 1.0
                shadowEnabled: root.currentTab == index
                shadowColor: "black"
                shadowOpacity: 0.7
            }

            Item
            {
                id: tabArea
                anchors.left: parent.left
                width: root.tabsWidth
                y: index * root.tabsHeight
                height: root.tabsHeight

                NormalText
                {
                    text: modelData.title
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                }

                MouseArea
                {
                    anchors.fill: parent
                    onPressed: root.currentTab = index
                }
            }

            Item
            {
                id: contentArea
                anchors.left: tabArea.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                visible: index == root.currentTab

                Component.onCompleted:
                {
                    children = modelData.children;
                    children[0].anchors.fill = contentArea;
                }
            }
        }
    }
}
