import QtQuick 2.15
import QtQuick.Layouts
import "../components"
import ".."

Item
{
    ColumnLayout
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 16
        spacing: 32

        NormalText
        {
            text: "Ajouter une tache ponctuelle"
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }

        Carousel
        {
            id: carousel
            model: tasks_scheduler.tasks
            Layout.fillWidth: true

            Item
            {
                property var modelData
                id: taskItemParent

                Task
                {
                    id: task
                    task: taskItemParent.modelData
                    width: 400
                    anchors.centerIn: parent
                    clickConfirmDuration: 500

                    onTaskActivated:
                    {
                        rollbackProgress();
                        glow.startGlowing();
                        tasks_scheduler.appendCasualTask(task.task);
                    }
                }

                Glow
                {
                    id: glow
                    anchors.fill: task
                    source: task
                    animatiomDuration: 600
                }
            }
        }
    }
}
