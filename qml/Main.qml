import QtQuick

Window
{
    width: 1280
    height: 800
    visible: true

    TasksView
    {
        id: tasksView
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
