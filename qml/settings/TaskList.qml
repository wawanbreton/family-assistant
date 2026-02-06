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
            text: "Affectation des taches ponctuelles"
            Layout.fillWidth: true
            horizontalAlignment: Qt.AlignHCenter
        }

        Carousel
        {
            id: carouselTasks
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
                    clickConfirmDuration: 0
                }
            }
        }

        Carousel
        {
            id: carouselKids
            model: user_manager.kids
            Layout.fillWidth: true
            currentIndex:
            {
                var current_task = tasks_scheduler.casual_tasks[carouselTasks.currentIndex];
                if(current_task.affected_kid !== null)
                {
                    return user_manager.findIndex(current_task.affected_kid);
                }

                return 0
            }

            Item
            {
                property var modelData
                id: kidItemParent

                KidName
                {
                    id: kidName
                    kid: kidItemParent.modelData
                    anchors.centerIn: parent
                }
            }

            onCurrentIndexChanged:
            {
                var current_task = tasks_scheduler.casual_tasks[carouselTasks.currentIndex];
                var current_kid = user_manager.kids[carouselKids.currentIndex];
                current_task.affected_kid = current_kid;
            }
        }
    }
}
