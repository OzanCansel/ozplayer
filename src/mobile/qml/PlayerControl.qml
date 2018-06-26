import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import mobile 1.0

Item {

    property PlayerProxy proxy : ({})
    id : mainItem
    property color headerColor : "#1D3262"

    property var discovered : []

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

    Connections{
        target:proxy
        onCurrentDirectoryChanged : {
            if(discovered.indexOf(proxy.currentDirectory) >= 0)
                return;

            discovered.push(proxy.currentDirectory)
        }
    }

    function up(){
        if(proxy.entries[0].isUp){
            proxy.retrieveFiles(proxy.entries[0].path)
            return true
        }

        return false
    }

    ListView {
        z : 2
        id : serverList
        width: parent.width
        height: parent.height
        model: proxy.entries
        populate: Transition {
            id: _popuTrans
            enabled: discovered.indexOf(proxy.currentDirectory) < 0
            SequentialAnimation {
                PropertyAction { property: "opacity"; value: 0.0 }
                PauseAnimation { duration: 80*_popuTrans.ViewTransition.index }
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 150; easing.type: Easing.InOutQuad }
            }
        }
        delegate : Item {
            id : delegateItem
            width:parent.width
            height: Responsive.v(150)
            property var fileName : proxy.entries[index].fileName
            property var isFolder : proxy.entries[index].isFolder
            property var path : proxy.entries[index].path
            property var isCurrent : delegateItem.path == proxy.currentTrack
            property var containsCurrent : proxy.currentTrack.indexOf(delegateItem.path) >= 0

            Row {
                z : 2
                anchors.fill : parent
                anchors.leftMargin: Responsive.h(35)
                anchors.rightMargin:  Responsive.h(35)
                spacing : Responsive.h(30)

                Image {
                    source: {
                        if(delegateItem.isFolder) return "/res/img/folder.png";
                        if(delegateItem.isCurrent)
                                return "/res/img/note-playing.png"

                        return "/res/img/note.png"
                    }

                    width : Responsive.v(40)
                    height: Responsive.h(40)
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    text : proxy.entries[index].fileName
                    width: delegateItem.width
                    font.pixelSize: delegateItem.isCurrent ? Responsive.v(46) : Responsive.v(43)
                    anchors.verticalCenter: parent.verticalCenter
                    color: delegateItem.isCurrent || (delegateItem.isFolder && delegateItem.containsCurrent && delegateItem.fileName != "Yukarı") ? "green" : "white"
                    font.bold : delegateItem.isCurrent
                    font.family: FontCollection.connectionFontName
                    z:2
                }

            }

            Row {
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                z : 3

                Button{
                    id : playButton
                    width : Responsive.v(130)
                    height: Responsive.h(150)
                    background: Image{
                        source: "/res/img/play.png"
                        width:Responsive.v(50)
                        height:Responsive.v(50)
                        fillMode: Image.PreserveAspectFit
                        visible:serverList.currentIndex == index && !delegateItem.isFolder
                        anchors.verticalCenter: playButton.verticalCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                    visible: !delegateItem.isCurrent
                    onClicked:  proxy.play(delegateItem.path)
                }

                Button{
                    id : resumePauseButton
                    width : Responsive.v(130)
                    height: Responsive.h(150)
                    background: Image{
                        source: proxy.trackStatus == 1 ? "/res/img/play.png" : "/res/img/pause.png"
                        width: Responsive.v(50)
                        height:Responsive.v(50)
                        fillMode: Image.PreserveAspectFit
                        visible:serverList.currentIndex == index && !delegateItem.isFolder
                        anchors.verticalCenter: resumePauseButton.verticalCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                    visible: delegateItem.isCurrent
                    onClicked: {
                        if(proxy.trackStatus == 1)
                            proxy.resume()
                        else
                            proxy.pause()
                    }
                }
            }

            MouseArea {
                id : area
                anchors.fill: parent
                onClicked:  serverList.currentIndex = index
                onDoubleClicked: {
                    if(delegateItem.isFolder)
                        proxy.retrieveFiles(delegateItem.path)
                    else {
                        proxy.play(delegateItem.path)
                    }
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
