import QtQuick
import "access"

Window
{
    id: main
    width: 1280
    height: 800
    visible: true

    MainView
    {
        id: mainView
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: menuBar.left
    }

    MenuBar
    {
        id: menuBar
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        onButtonClicked: (id) =>
        {
            mainView.setView(id);
        }
    }

    ScanFingerprint
    {
        anchors.fill: parent
    }

    Component.onCompleted:
    {
        if (fullscreen)
        {
            showFullScreen()
        }
    }
}
