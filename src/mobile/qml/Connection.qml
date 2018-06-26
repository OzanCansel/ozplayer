import QtQuick 2.0
import mobile 1.0
import "../qml"

Item {

    property ServerFinder finder : ({})
    property PlayerProxy proxy : ({})

    id: searchServer

    function connectToServer(host , port){
        playerProxy.open(host , port)
    }

    Column{
        width:parent.width
        height:parent.height
        y : Responsive.v(100)
        spacing: Responsive.v(10)

        Item{
            anchors.horizontalCenter: parent.horizontalCenter
            height: Responsive.v(330)
            width: parent.width
            Text {
                id: name
                text: qsTr("OzPlayer")
                anchors.centerIn: parent
                font.family: FontCollection.mainFontName
                font.pixelSize: Responsive.v(80)
                color : "white"
            }
        }

        ListView {
            id : serverList
            width: parent.width
            height : Responsive.v(1600)
            model: finder.servers
            delegate : Item {
                id : delegateItem
                width:parent.width
                height: Responsive.v(150)
                property var port : finder.servers[index].port
                property var serverIp : finder.servers[index].ip
                Text {
                    anchors.fill:parent
                    text : finder.servers[index].pcName
                    width: parent.width
                    font.pixelSize: Responsive.v(85)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: "white"
                    font.family: FontCollection.connectionFontName
                }

                MouseArea{
                    id : area
                    anchors.fill: parent
                    onClicked: {
                        if(serverList.currentIndex != index)
                            serverList.currentIndex = index
                    }
                    onPressAndHold: connectToServer(delegateItem.serverIp , delegateItem.port)
                    onDoubleClicked: connectToServer(delegateItem.serverIp , delegateItem.port)
                }
            }
            highlight: Rectangle{
                color : "#2286fb"
            }
        }
    }

    TextButton{
        id : connectButton
        width: Responsive.h(500)
        height: Responsive.v(200)
        text: "Baglan"
        font.pixelSize: Responsive.v(70)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom:parent.bottom
        anchors.bottomMargin: Responsive.v(110)
        onClicked: {
            connectToServer(serverList.currentItem.serverIp , serverList.currentItem.port)
        }
    }
}
