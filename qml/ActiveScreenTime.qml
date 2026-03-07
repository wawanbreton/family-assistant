import QtQuick
import FamilyAssistant
import "components"

ConfirmButton
{
    property var kid

    id: root
    width: 260
    clickConfirmWidth: width - height / 2

    gradient: StyledGradient
    {
        color:
        {
            switch(root.kid.screen_time_state)
            {
                case ScreenTimeState.Plenty:
                    return blue;
                case ScreenTimeState.NotMuch:
                    return orange;
                case ScreenTimeState.Out:
                    return red;
            }
        }
    }

    Image
    {
        id: icon
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 12
        width: height
        mipmap: true
        source: DataStorage.findResource("screen-time", ResourceType.Icon)
    }

    NormalText
    {
        id: textActive
        anchors.top: icon.top
        anchors.left: icon.right
        anchors.leftMargin: 20
        text: kid.active_screen_time_duration_str
    }

    SmallText
    {
        id: textRemaining
        anchors.bottom: icon.bottom
        anchors.horizontalCenter: textActive.horizontalCenter
        text: kid.screen_time_str
    }

    onTriggered:
    {
        kid.stopScreenTime();
    }
}
