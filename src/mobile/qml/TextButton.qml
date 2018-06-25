import QtQuick 2.0
import QtQuick.Controls 2.2
import "../"

Button {

    id  :   button

    readonly property int   normal  :   0
    readonly property int   error   :   1
    property int buttonType         :   normal
    font.family                     :   "Linux Libertine"

    background  :   Rectangle {
        width           :   button.width
        height          :   button.height
        color           :   {
            if(!button.enabled)
                return "#464646"
            if(buttonType === normal){
                return button.pressed ? "#222222" : "#111111"
            } else if (buttonType === error)
                return button.pressed ? "#e43a3a" : "#b91919"
        }
        radius          :   height * 0.1
        border.width    :   Responsive.v(4)
        border.color    :   "white"
    }

    contentItem :   Item{
        Text {
            id                  :   name
            text                :   button.text
            anchors.centerIn    :   parent
            font                :   button.font
            color               :   "white"
        }
    }
}

