import QtQuick 2.15
import QtQuick.Layouts
import "../components"
import ".."

Item
{
    ColumnLayout
    {
        anchors.fill: parent
        anchors.leftMargin: 16
        anchors.rightMargin: 16

        NormalText
        {
            text: "Ajouter une tache ponctuelle"
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }

        Carousel
        {
            id: carousel
            model: tasks_scheduler.casual_tasks
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
