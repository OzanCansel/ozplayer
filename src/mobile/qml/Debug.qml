import QtQuick 2.0
import QtQuick.Controls 2.1
import "../"

Rectangle {

    readonly property real marginRatio      :   0.02

    property bool   clearFlag               :   false

    id          :   consoleTextContainer
    color       :   "transparent"

    function    checkFlags(){
        if(clearFlag){
            consoleText.clear()
            clearFlag   =   false
        }
    }

    function clearOutput(){
        consoleText.clear()
    }

    function    infoOutput(text){
        checkFlags();
        var     texts = text.split("\n")
        for(var i = 0; i < texts.length; i++){
            consoleText.append("<span style=\"color : "  + "#00ff00" + " \">" + texts[i] + "</span>")
        }
    }

    function    commandOutput(text){
        checkFlags();
        var     texts = text.split("\n")
        for(var i = 0; i < texts.length; i++){
            consoleText.append("<span style=\"color : "  + "#00ff00" + " \">" + texts[i] + "</span>")
        }
    }

    function    standartOutput(text){
        checkFlags()
        var     texts = text.split("\n")
        for(var i = 0; i < texts.length; i++){
            consoleText.append("<span style=\"color : " + "#00ff00" +  "\">" + texts[i] + "</span>");
        }
    }

    function    errorOutput(text){
        checkFlags()
        var     texts = text.split("\n")
        for(var i = 0; i < texts.length; i++){

            consoleText.append("<span style=\"color : " + "#ff0000" + ";\">"  + texts[i] + "</span>")
        }
    }

    Flickable   {

        id                  :   consoleFlickable
        anchors.fill        :   parent
        anchors.margins     :   10

        TextArea.flickable  :   TextArea    {
            id              :   consoleText
            font.bold       :   true
            selectByKeyboard:   true
            selectByMouse   :   true
            font.pointSize  :   9
            wrapMode        :   TextArea.Wrap
            textFormat      :   TextArea.RichText

            background      :   Rectangle{
                width       :   consoleFlickable.width
                height      :   consoleFlickable.height
                radius      :   2
                color       :   "black"
            }
            onTextChanged   :   {
                flickableScroll.increase()
            }

            readOnly        :   true
            text            :   ""
            overwriteMode   :   true

        }

        rebound     :   Transition {
            NumberAnimation {
                properties  : "x,y"
                duration    : 1000
                easing.type : Easing.OutBounce
            }
        }

        ScrollBar.vertical  :   ScrollBar   {  id   :   flickableScroll }
    }

}
