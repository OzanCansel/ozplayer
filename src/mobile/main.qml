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
    title: qsTr("Oz Player")

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
        //x : -((width - app.width) / 2)
        //y : -((height - app.height) / 2)
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
