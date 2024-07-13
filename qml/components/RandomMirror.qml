import QtQuick 2.15

Item
{
    Scale { id: scale; xScale: -1; }
    Translate { id: translate; x: parent.width; }

    Component.onCompleted:
    {
        if(Math.random() > 0.5)
        {
            parent.transform = [scale, translate];
        }
    }
}
