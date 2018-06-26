import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import mobile 1.0

Item {

    property PlayerProxy proxy : ({})
    id : mainItem
    property color headerColor : "#1D3262"

    LinearGradient{
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(0, mainItem.height)
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#1D3262" }
            GradientStop { position: 0.15; color: "#1D3262" }
            GradientStop { position: 1.0; color: "black" }
        }
    }

//    Connections{
//        target:proxy
//        onEntriesChanged : {
//            serverList.currentIndex = -1;
//            serverList.model = proxy.entries
//        }
//    }

    ListView {
        z : 2
        id : serverList
        width: parent.width
        height: parent.height
        model: proxy.entries
        delegate : Item {
            id : delegateItem
            width:parent.width
            height: Responsive.v(150)
            property var fileName : proxy.entries[index].fileName
            property var isFolder : proxy.entries[index].isFolder
            property var folder : proxy.entries[index].path
            Row {
                z : 2
                anchors.fill : parent
                anchors.leftMargin: Responsive.h(35)
                anchors.rightMargin:  Responsive.h(35)
                spacing : Responsive.h(30)

                Image {
                    source: delegateItem.isFolder ? "/res/img/folder.png" : "/res/img/note.png"
                    width : Responsive.v(40)
                    height: Responsive.h(40)
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text : proxy.entries[index].fileName
                    width: delegateItem.width
                    font.pixelSize: Responsive.v(43)
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    font.family: FontCollection.connectionFontName
                    z:2
                }

            }

            Button{
                id : playButton
                width : Responsive.v(50)
                height: Responsive.h(50)
                background: Image{
                    source: "/res/img/play.png"
                    width:playButton.width
                    height:playButton.height
                    fillMode: Image.PreserveAspectFit
                    visible:serverList.currentIndex == index && !delegateItem.isFolder
                }
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: Responsive.h(40)
                z : 3
            }

            MouseArea {
                id : area
                anchors.fill: parent
                onClicked:  serverList.currentIndex = index
                onDoubleClicked: {
                    if(delegateItem.isFolder)
                        proxy.retrieveFiles(delegateItem.folder)
                }
                hoverEnabled: true
            }

            Rectangle{
                anchors.fill:parent
                color: serverList.currentIndex == index ?  "#0F1833" :  "transparent"
                opacity: 1
                z : 1
            }
        }
    }
}
