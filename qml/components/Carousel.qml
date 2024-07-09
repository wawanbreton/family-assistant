import QtQuick 2.15

Item
{
    default property Component delegate
    property alias model: view.model
    property alias currentIndex: view.currentIndex
    property var currentItem: view.currentItem.loadedItem

    id: root
    implicitHeight: buttonLeft.width * (buttonLeft.implicitHeight / buttonLeft.implicitWidth)

    Image
    {
        id: buttonLeft
        source: DataStorage.findResource("arrowhead", DataStorage.Icon)
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        fillMode: Image.PreserveAspectFit
        width: 80
        height: 0 // Just to kill binding loop, it will be ignored anyway
        visible: false
    }

    ButtonShadow
    {
        source: buttonLeft
        scale: -1

        MouseArea
        {
            anchors.fill: parent
            onPressed: view.decrementCurrentIndex()
        }
    }

    PathView
    {
        id: view
        pathItemCount: 3
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: buttonLeft.right
        anchors.right: buttonRight.left

        path: Path
        {
            startX: 0
            startY: height / 2
            PathAttribute { name: "itemOpacity"; value: 0.0 }
            PathAttribute { name: "itemScale"; value: 0.2 }
            PathLine { x: view.width/2; y: height / 2; }
            PathAttribute { name: "itemOpacity"; value: 1.0 }
            PathAttribute { name: "itemScale"; value: 1.0 }
            PathLine { x: view.width; y: height / 2; }
        }

        delegate: Item
        {
            opacity: PathView.itemOpacity
            scale: PathView.itemScale
            property var loadedItem

            Component.onCompleted: loadedItem = root.delegate.createObject(this, {"modelData": modelData})
        }
    }

    Image
    {
        id: buttonRight
        source: DataStorage.findResource("arrowhead", DataStorage.Icon)
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        fillMode: Image.PreserveAspectFit
        width: buttonLeft.width
        visible: false

        MouseArea
        {
            anchors.fill: parent
            onPressed: view.incrementCurrentIndex()
        }
    }

    ButtonShadow
    {
        source: buttonRight

        MouseArea
        {
            anchors.fill: parent
            onPressed: view.incrementCurrentIndex()
        }
    }
}
