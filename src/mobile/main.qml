import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import mobile 1.0
import "qml"

ApplicationWindow {
    id: app
    visible: true
    width: visibility == ApplicationWindow.Windowed ? Screen.height * 0.4 : Screen.width
    height: visibility == ApplicationWindow.Windowed ? Screen.height * 0.8 : Screen.height
    visibility: osInfo.isAndroid() ? ApplicationWindow.FullScreen :  (fullscreenButton.checked ? ApplicationWindow.FullScreen : ApplicationWindow.Windowed)
    title: qsTr("OzPlayer")

    header: Item {
        height : Responsive.v(140)
        width: app.width
        visible: stack.depth

        Button{
            id : backButton
            height: Responsive.v(140)
            width:Responsive.h(140)
            x : Responsive.h(30)
            anchors.verticalCenter: parent.verticalCenter
            z : 2
            visible: stack.depth > 1
            background: Item {
                Image
                {
                    id: name
                    source: "/res/img/back-arrow.png"
                    fillMode: Image.PreserveAspectFit
                    height: Responsive.v(60)
                    width: Responsive.h(60)
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            onClicked: {
                if(stack.currentItem == playerControl){
                    if(!playerControl.up())
                        stack.pop()
                }
                else {
                    stack.pop()
                }
            }
        }


        Text {
            z : 2
            text: stack.currentItem.headerText ? stack.currentItem.headerText : qsTr("OzPlayer")
            font.pixelSize: Responsive.v(30)
            font.family: FontCollection.mainFontName
            color : "white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            visible: stack.depth > 1
        }

        Rectangle{
            anchors.fill:parent
            color: stack.currentItem.headerColor ? stack.currentItem.headerColor : "black"
        }

        Item {
            z : 5
            anchors.right: parent.right
            y:Responsive.v(10)
            width: Responsive.h(350)
            height: Responsive.v(120)

            Item {
                anchors.right: shutdownButtonItem.left
                width: Responsive.h(120)
                height: Responsive.v(100)
                visible: osInfo.isWindows() || osInfo.isUnix()
                enabled: osInfo.isWindows() || osInfo.isUnix()

                Item {

                    property bool checked : false
                    id : fullscreenButton
                    anchors.right: parent.right
                    anchors.rightMargin: Responsive.h(16)
                    anchors.top: parent.top
                    anchors.topMargin: Responsive.v(10)
                    width: Responsive.v(100)
                    height: Responsive.h(100)
                    z : 2

                    Image{
                        source : fullscreenButton.checked ? "/res/img/windowed.png" : "res/img/fullscreen.png"
                        height: Responsive.v(70)
                        width: Responsive.h(70)
                        fillMode: Image.PreserveAspectFit
                        y : Responsive.v(10)
                    }

                    opacity: shutdownButtonArea.containsMouse || fullScreenButtonArea.containsMouse ? 1 : 0
                    Behavior on opacity{
                        SequentialAnimation {

                            PauseAnimation {
                                duration: fullscreenButton.opacity == 1 ? 1500 : 0
                            }
                            NumberAnimation{    }
                        }
                    }

                    MouseArea{
                        id : fullScreenButtonArea
                        anchors.fill:parent
                        hoverEnabled: true
                        onClicked: fullscreenButton.checked = !fullscreenButton.checked
                    }
                }
            }

            Item {
                id : shutdownButtonItem
                z : 5
                anchors.right: parent.right
                width: Responsive.h(120)
                height: Responsive.v(100)

                Item {

                    id : shutdownButton
                    anchors.right: parent.right
                    anchors.rightMargin: Responsive.h(5)
                    anchors.top: parent.top
                    anchors.topMargin: Responsive.v(10)
                    width: Responsive.v(100)
                    height: Responsive.h(100)
                    z : 2

                    Image {
                        source : "res/img/logout.png"
                        height: Responsive.v(70)
                        width: Responsive.h(70)
                        fillMode: Image.PreserveAspectFit
                        y : Responsive.v(10)
                    }
                    opacity: (!osInfo.isWindows() && !osInfo.isUnix()) || fullScreenButtonArea.containsMouse || shutdownButtonArea.containsMouse ? 1 : 0

                    Behavior on opacity {
                        SequentialAnimation {

                            PauseAnimation {
                                duration: shutdownButton.opacity == 1 ? 1500 : 0
                            }
                            NumberAnimation {    }
                        }
                    }

                    MouseArea{
                        id : shutdownButtonArea
                        anchors.fill:parent
                        hoverEnabled: true
                        onClicked: Qt.quit()
                    }
                }
            }
        }
    }

    ServerFinder{
        id : serverFinder
    }

    PlayerProxy{
        id : playerProxy
        onConnectedChanged: {
            if(connected)
                stack.push(playerControl)
            else if(stack.depth > 1){
                while(stack.depth == 1)
                    stack.pop()
            }
        }
    }

    StackView{
        id:stack
        z : 3
        anchors.fill:parent

        initialItem : searchServer

        Connection {
            id : searchServer
            finder: serverFinder
            proxy: playerProxy
        }

        PlayerControl{
            id : playerControl
            proxy: playerProxy
        }

        Keys.onVolumeDownPressed: {
            playerProxy.volumeDown()
            event.accepted = true
        }

        Keys.onVolumeUpPressed: {
            playerProxy.volumeUp()
            event.accepted = true
        }

        Keys.onUpPressed: {
            playerProxy.volumeUp()
            event.accepted = true
        }

        Keys.onDownPressed: {
            playerProxy.volumeDown()
            event.accepted = true
        }

        Keys.onRightPressed: {
            playerProxy.volumeUp()
            event.accepted = true
        }

        Keys.onLeftPressed: {
            playerProxy.volumeDown()
            event.accepted = true
        }
    }

    Image{
        source: "/res/img/main-bg.jpg"
        width: parent.width
        height: parent.height
        mipmap: true
        smooth: true
    }

    Rectangle{
        anchors.fill:parent
        color : "black"
        opacity: 0.3
        z : 2
    }


    Component.onCompleted: {
        serverFinder.search()
        Responsive.appWidth = app.width
        Responsive.appHeight = app.height
    }

}
