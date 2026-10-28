import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Shapes
import "../components"


Item
{
    id: root

    property int radius: 50
    property bool topLeftCornerRounded: true
    property bool topRightCornerRounded: true
    property bool bottomLeftCornerRounded: true
    property bool bottomRightCornerRounded: true

    readonly property real cornerRadius: Math.max(0, Math.min(radius, Math.min(width, height) / 2))

    Layout.fillWidth: true
    Layout.fillHeight: true

    ButtonShadow
    {
        source: shape
        shadowScale: 1.0
        shadowHorizontalOffset: 4
        shadowVerticalOffset: shadowHorizontalOffset
    }

    Shape
    {
        id: shape
        anchors.fill: parent
        preferredRendererType: Shape.CurveRenderer

        ShapePath
        {
            fillGradient: StyledGradient { y2: height }
            strokeWidth: -1
            pathHints: ShapePath.PathFillOnRight | ShapePath.PathSolid | ShapePath.PathNonIntersecting

            startX: root.topLeftCornerRounded ? root.cornerRadius : 0
            startY: 0

            PathLine
            {
                x: width - (root.topRightCornerRounded ? root.cornerRadius : 0)
                y: 0
            }

            PathArc
            {
                x: width
                y: root.topRightCornerRounded ? root.cornerRadius : 0
                radiusX: root.topRightCornerRounded ? root.cornerRadius : 0
                radiusY: radiusX
            }

            PathLine
            {
                x: width
                y: height - (root.bottomRightCornerRounded ? root.cornerRadius : 0)
            }

            PathArc
            {
                x: width - (root.bottomRightCornerRounded ? root.cornerRadius : 0)
                y: height
                radiusX: root.bottomRightCornerRounded ? root.cornerRadius : 0
                radiusY: radiusX
            }

            PathLine
            {
                x: root.bottomLeftCornerRounded ? root.cornerRadius : 0
                y: height
            }

            PathArc
            {
                x: 0
                y: height - (root.bottomLeftCornerRounded ? root.cornerRadius : 0)
                radiusX: root.bottomLeftCornerRounded ? root.cornerRadius : 0
                radiusY: radiusX
            }

            PathLine
            {
                x: 0
                y: root.topLeftCornerRounded ? root.cornerRadius : 0
            }

            PathArc
            {
                x: root.topLeftCornerRounded ? root.cornerRadius : 0
                y: 0
                radiusX: root.topLeftCornerRounded ? root.cornerRadius : 0
                radiusY: radiusX
            }
        }
    }
}
