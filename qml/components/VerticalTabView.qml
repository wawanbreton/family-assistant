import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Effects

Item
{
    default property list<Item> tabs

    ColumnLayout
    {
        id: buttons
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 400
        spacing: 0

        Repeater
        {
            model: tabs

            Item
            {
                height: 100
                Layout.fillWidth: true
                z: stack.currentIndex == index ? 1.0 : 0.5

                Rectangle
                {
                    id: buttonRectangle
                    anchors.fill: parent
                    radius: height / 2
                    gradient: StyledGradient
                    {
                        color: colors[index % colors.length]
                        positionTop: ((buttonRectangle.y + buttonRectangle.height) / buttons.height) * buttonRectangle.height
                        positionBottom: (buttonRectangle.y / buttons.height) * buttonRectangle.height
                    }

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
                        onPressed: stack.currentIndex = index
                    }
                }

                MultiEffect
                {
                    source: buttonRectangle
                    anchors.fill: parent
                    shadowBlur: 1.0
                    shadowEnabled: stack.currentIndex == index
                    shadowColor: "black"
                    shadowOpacity: 0.7
                }
            }
        }

        Item
        {
            Layout.fillHeight: true
        }
    }

    StackLayout
    {
        id: stack
        anchors.left: buttons.right
        anchors.leftMargin: -50
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        Repeater
        {
            model: tabs

            Item
            {
                Rectangle
                {
                    id: contentRect
                    anchors.fill: parent

                    gradient: StyledGradient
                    {
                        color: colors[index % colors.length]
                    }

                    Component.onCompleted:
                    {
                        children = [modelData];
                        modelData.anchors.fill = contentRect;
                    }
                }

                MultiEffect
                {
                    source: contentRect
                    anchors.fill: parent
                    shadowBlur: 1.0
                    shadowEnabled: true
                    shadowColor: "black"
                    shadowOpacity: 0.7
                }

            }
        }
    }
}
