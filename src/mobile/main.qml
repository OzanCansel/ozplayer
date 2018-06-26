import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0
import mobile 1.0
import "qml"

ApplicationWindow {
    id: app
    visible: true
    width: 562
    height: 1000
//    visibility: ApplicationWindow.FullScreen
    title: qsTr("Oz Player")

    header: Item {
        height : Responsive.v(140)
        width: app.width
        visible: stack.depth > 1

        Button{
            id : backButton
            height: Responsive.v(110)
            width:Responsive.h(110)
            x : Responsive.h(33)
            anchors.verticalCenter: parent.verticalCenter
            z : 2
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
            onClicked: stack.pop()
        }


        Text {
            z : 2
            text: qsTr("OzPlayer")
            font.pixelSize: Responsive.v(30)
            font.family: FontCollection.mainFontName
            color : "white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Rectangle{
            anchors.fill:parent
            color: stack.currentItem.headerColor ? stack.currentItem.headerColor : "black"
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
