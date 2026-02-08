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
                    clickConfirmDuration: 0
                }
            }
        }

        Carousel
        {
            id: carouselKids
            model: user_manager.kids.concat([null])
            Layout.fillWidth: true
            currentIndex:
            {
                var current_task = tasks_scheduler.tasks[carouselTasks.currentIndex];
                if(current_task.affected_kid !== null)
                {
                    return user_manager.findIndex(current_task.affected_kid);
                }
                else
                {
                    return user_manager.kids.length;
                }
            }

            Item
            {
                property var modelData
                id: kidItemParent

                KidName
                {
                    id: kidName
                    visible: parent.modelData !== null
                    kid: kidItemParent.modelData
                    anchors.centerIn: parent
                }

                LargeText
                {
                    visible: !kidName.visible
                    text: "Tous"
                    color: "black"
                    anchors.centerIn: parent
                }
            }

            onCurrentIndexChanged:
            {
                var current_task = tasks_scheduler.tasks[carouselTasks.currentIndex];
                var current_kid;
                if(carouselKids.currentIndex < user_manager.kids.length)
                {
                    current_kid = user_manager.kids[carouselKids.currentIndex];
                }
                else
                {
                    current_kid = null;
                }

                current_task.affected_kid = current_kid;
            }
        }
    }
}
