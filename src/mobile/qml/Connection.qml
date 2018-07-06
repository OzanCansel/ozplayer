import QtQuick 2.0
import mobile 1.0
import "../qml"

Item {

    property ServerFinder finder : ({})
    property PlayerProxy proxy : ({})
    property color headerColor : "black"

    id: searchServer

    function connectToServer(host , port){
        proxy.open(host , port)
    }

    function connectToFileServer(host , port){
        proxy.openFileService(host , port);
    }

    Column{
        width:parent.width
        height:parent.height
        y : Responsive.v(100)
        spacing: Responsive.v(50)

        Item{
            anchors.horizontalCenter: parent.horizontalCenter
            height: Responsive.v(200)
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

        Item{
            width:parent.width
            height: Responsive.v(900)
            visible:serverList.count == 0

            Text{
                text : "Cihaz bulunamadı."
                font.pixelSize: Responsive.v(110)
                color : "white"
                font.family: FontCollection.connectionFontName
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.centerIn: parent
            }

            MouseArea{
                anchors.fill:parent
                onClicked: Qt.openUrlExternally("https://www.google.com.tr")
            }
        }

        Item{
            width: parent.width
            height: Responsive.v(40)
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
                property var fileServerPort : finder.servers[index].fileServerPort

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
                    onPressAndHold: {
                        connectToServer(delegateItem.serverIp , delegateItem.port)
                        connectToFileServer(delegateItem.serverIp , delegateItem.fileServerPort)
                    }
                    onDoubleClicked: {
                        connectToServer(delegateItem.serverIp , delegateItem.port)
                        connectToFileServer(delegateItem.serverIp , delegateItem.fileServerPort)
                    }
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
        text: "Bağlan"
        font.pixelSize: Responsive.v(70)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom:parent.bottom
        anchors.bottomMargin: Responsive.v(120)
        visible: serverList.count > 0
        onClicked: {
            connectToServer(serverList.currentItem.serverIp , serverList.currentItem.port)
            connectToFileServer(serverList.currentItem.serverIp , serverList.currentItem.fileServerPort)

        }
    }

    TextButton{
        id : refreshButton
        width: Responsive.h(500)
        height: Responsive.v(200)
        text: "Tekrar Dene"
        font.pixelSize: Responsive.v(70)
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: Responsive.v(120)
        anchors.bottom:parent.bottom
        visible: serverList.count == 0
        onClicked: {
            finder.search()
        }
    }
}
