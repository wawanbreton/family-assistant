import QtQuick 2.15

Rectangle
{
    readonly property int cost: 30

    gradient: Gradient
    {
         GradientStop { position: 0.0; color: "#9ed1ee" }
         GradientStop { position: 1.0; color: "#249ada" }
    }
}
