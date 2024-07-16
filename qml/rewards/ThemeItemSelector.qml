import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Effects
import "../components"
import ".."

Item
{
    default property Component delegate
    property var kid
    property alias model: carousel.model
    property alias currentIndex: carousel.currentIndex
    property var currentItem: carousel.currentItem.item
    property int themeCategory
    property string currentKidItem
    property int itemImplicitWidth: -1
    property int itemImplicitHeight: -1
    property bool clipGlowEffect: false

    signal itemSelected(string item, var button)

    id: root
    implicitHeight: column.implicitHeight

    ColumnLayout
    {
        id: column
        anchors.fill: parent
        spacing: 16

        Carousel
        {
            id: carousel
            model: Theme.getAvailableItems(root.themeCategory)
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: Theme.getAvailableItems(root.themeCategory).indexOf(root.currentKidItem)

            Item
            {
                property var modelData
                property alias item: loader.item

                Loader
                {
                    id: loader
                    anchors.centerIn: parent

                    MultiEffect
                    {
                        id: glow
                        anchors.fill: parent
                        source: loader.item
                        blurEnabled: true
                        z: 1.0
                        visible: false
                        clip: root.clipGlowEffect

                        SequentialAnimation
                        {
                            id: animationShine

                            ParallelAnimation
                            {
                                PropertyAnimation
                                {
                                    target: glow
                                    easing.type: Easing.InOutQuad
                                    properties: "brightness,blur"
                                    to: 0.7
                                    duration: 1500
                                }

                                PropertyAnimation
                                {
                                    target: glow
                                    property: "scale"
                                    easing.type: Easing.InOutQuad
                                    to: 1.5
                                    duration: 1500
                                }
                            }

                            ParallelAnimation
                            {
                                PropertyAnimation
                                {
                                    target: glow
                                    easing.type: Easing.InOutQuad
                                    properties: "brightness,blur,scale"
                                    to: 0.0
                                    duration: 1500
                                }

                                PropertyAnimation
                                {
                                    target: glow
                                    property: "scale"
                                    easing.type: Easing.InOutQuad
                                    to: 1.0
                                    duration: 1500
                                }
                            }

                            onFinished: { glow.visible = false; loader.item.visible = true; }
                        }
                    }

                    onLoaded:
                    {
                        if(root.itemImplicitWidth >= 0 && root.itemImplicitHeight >= 0)
                        {
                            item.implicitWidth = Qt.binding(function() { return root.itemImplicitWidth });
                            item.implicitHeight = Qt.binding(function() { return root.itemImplicitHeight });
                        }

                        if(root.delegate)
                        {
                            item.modelData = modelData;
                        }
                    }
                }

                Component.onCompleted:
                {
                    if(root.delegate)
                    {
                        loader.sourceComponent = root.delegate;
                    }
                    else
                    {
                        loader.source = Theme.getItemFilePath(root.themeCategory, modelData);
                    }
                }

                function shine()
                {
                    loader.item.visible = false
                    glow.visible = true
                    animationShine.start();
                }
            }
        }

        ConfirmButton
        {
            id: button
            width: 300
            Layout.alignment: Qt.AlignHCenter
            enabled: Theme.getAvailableItems(root.themeCategory)[carousel.currentIndex] !== currentKidItem && root.currentItem.cost <= kid.points

            PointsCounter
            {
                kid: root.kid
                points: root.currentItem.cost
                anchors.centerIn: parent
            }

            onTriggered: root.itemSelected(Theme.getAvailableItems(root.themeCategory)[carousel.currentIndex], button)
        }
    }

    Component.onCompleted: currentKidItem = kid.theme.getItem(themeCategory)

    Connections
    {
        target: kid.theme

        function onItemChanged(category)
        {
            if(category === themeCategory)
            {
                currentKidItem = kid.theme.getItem(themeCategory);
            }
        }
    }

    function setSelectedItem(item: string)
    {
        carousel.currentItem.shine()
        kid.theme.setItem(themeCategory, item);
    }
}
