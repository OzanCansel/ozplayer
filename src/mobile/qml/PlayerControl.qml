import QtQuick 2.0
import QtQuick.Controls 2.2
import QtGraphicalEffects 1.0
import mobile 1.0

Item {

    property PlayerProxy proxy : ({})
    id : mainItem
    property color headerColor : "#1D3262"

    property var discovered : []
    property var lastPositions : []

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
        onFileDownloaded : {
            serverList.model = ({});
            serverList.model = proxy.entries
        }
    }

    function up(){
        if(proxy.entries.length < 2)
            return false
        if(proxy.entries[0].isUp){
            proxy.retrieveFiles(proxy.entries[0].path)
            lastPositions[proxy.currentDirectory] = serverList.currentIndex
            return true
        }

        proxy.close();

        return false
    }

    ListView {
        z : 2
        id : serverList
        width: parent.width
        height: parent.height - Responsive.v(250)
        model: proxy.entries
        clip:true

        populate: Transition {
            id: _popuTrans
            enabled: discovered.indexOf(proxy.currentDirectory) < 0
            SequentialAnimation {
                PropertyAction { property: "opacity"; value: 0.0 }
                PauseAnimation { duration: 80*_popuTrans.ViewTransition.index }
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 150; easing.type: Easing.InOutQuad }
            }
        }

        onCountChanged: {
            if(lastPositions[proxy.currentDirectory])
                serverList.currentIndex = lastPositions[proxy.currentDirectory]
        }

        delegate : Item {
            id : delegateItem
            width:parent.width
            height: Responsive.v(150)
            property var fileName : proxy.entries[index].fileName
            property var isFolder : proxy.entries[index].isFolder
            property string path : proxy.entries[index].path
            property bool isCurrent : delegateItem.path == proxy.currentTrack
            property bool containsCurrent : proxy.currentTrack.indexOf(delegateItem.path) >= 0
            property bool isDownloaded : proxy.fileExists(delegateItem.path) && !delegateItem.isFolder

            Row {
                z : 2
//                anchors.fill : parent
                anchors.top : parent.top
                anchors.bottom : parent.bottom
                anchors.left : parent.left
                anchors.right : controlButtonsRow.left
                anchors.leftMargin: Responsive.h(35)
                anchors.rightMargin:  Responsive.h(15)
                spacing : Responsive.h(30)
                clip : true

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
                    color: delegateItem.isCurrent || (delegateItem.isFolder && delegateItem.containsCurrent && delegateItem.fileName != "Up") ? "green" : "white"
                    font.bold : delegateItem.isCurrent
                    font.family: FontCollection.connectionFontName
                    z:2
                    font.underline : !osInfo.isAndroid() && isDownloaded
                }
            }

            Row {
                id : controlButtonsRow
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                z : 3
                enabled : !delegateItem.isFolder


                Button {
                    id : downloadButton
                    width : Responsive.v(100)
                    height: Responsive.h(150)
                    visible: !osInfo.isAndroid() && !proxy.fileExists(delegateItem.path) && !delegateItem.isFolder && delegateItem.path.indexOf("downloads") !== 0 &&serverList.currentIndex == index
                    background: Image {
                        source: "/res/img/download.png"
                        width: Responsive.v(50)
                        height:Responsive.v(50)
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: downloadButton.verticalCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: serverList.currentIndex == index
                    onClicked: {
                        if(proxy.fileExists(delegateItem.path))
                            return

                        proxy.downloadFile(delegateItem.path)
                    }
                }

                Button{
                    id : playButton
                    width : !visible ? Responsive.h(0) : Responsive.v(100)
                    height: Responsive.h(150)
                    visible: !delegateItem.isCurrent && !delegateItem.isFolder && serverList.currentIndex == index
                    background: Image{
                        source: "/res/img/play.png"
                        width:Responsive.v(50)
                        height:Responsive.v(50)
                        fillMode: Image.PreserveAspectFit
                        visible:serverList.currentIndex == index && !delegateItem.isFolder
                        anchors.verticalCenter: playButton.verticalCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                    onClicked:  proxy.play(delegateItem.path)

                    Behavior on width{
                        NumberAnimation{ }
                    }
                }

                Button {
                    id : resumePauseButton
                    width : !visible ? Responsive.h(0) : Responsive.v(100)
                    height: Responsive.h(150)
                    visible: delegateItem.isCurrent && !delegateItem.isFolder
                    background: Image{
                        source: proxy.trackStatus == 1 ? "/res/img/play.png" : "/res/img/pause.png"
                        width: Responsive.v(50)
                        height:Responsive.v(50)
                        fillMode: Image.PreserveAspectFit
                        anchors.verticalCenter: resumePauseButton.verticalCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                    enabled: serverList.currentIndex == index
                    onClicked: {
                        if(proxy.trackStatus == 1)
                            proxy.resume()
                        else
                            proxy.pause()
                    }

                    Behavior on width{
                        NumberAnimation{ }
                    }
                }
            }

            MouseArea {
                id : area
                anchors.fill: parent
                onClicked:  serverList.currentIndex = index
                onDoubleClicked: {
                    if(delegateItem.isFolder) {
                        proxy.retrieveFiles(delegateItem.path)
                        lastPositions[proxy.currentDirectory] = serverList.currentIndex
                    }
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

    Item {
        id : currentTrackContainer
        height : Responsive.v(220)
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.bottom: parent.bottom
        readonly property bool noTrack : proxy.trackStatus == 2 || proxy.isFolder(proxy.currentTrack)

        Text {
            text: currentTrackContainer.noTrack ? "No Track" : proxy.fileName(proxy.currentTrack)
            color : "white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: controlButtonsContainer.left
            anchors.rightMargin: Responsive.h(40)
            anchors.left: parent.left
            anchors.leftMargin: Responsive.h(45)
            font.family: FontCollection.connectionFontName
            font.pixelSize: Responsive.v(46)
            horizontalAlignment: Text.AlignLeft
            z : 2
            clip: true

            MouseArea{
                anchors.fill : parent
                onDoubleClicked: {
                    var dir = proxy.getDirectoryPath(proxy.currentTrack)
                    if(dir.indexOf("..") >= 0)
                        return
                    if(proxy.currentDirectory === dir)
                        return

                    proxy.retrieveFiles(dir)
                    lastPositions[proxy.currentDirectory] = serverList.currentIndex
                }
            }
        }

        Button {
            id : controlButtonsContainer
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            width: Responsive.h(200)
            height:parent.height
            enabled : !currentTrackContainer.noTrack
            background : Image{
                source: proxy.trackStatus == 1 ? "/res/img/play-circle.png" : "/res/img/pause-circle.png"
                width: Responsive.v(100)
                height: Responsive.h(100)
                anchors.verticalCenter: parent.verticalCenter
                fillMode: Image.PreserveAspectFit
            }
            onClicked : {
                if(proxy.trackStatus == 1)
                    proxy.resume()
                else
                    proxy.pause()
            }

            z: 2
        }

        ProgressBar{
            id : progress
            value : proxy.trackPercentage
            width: parent.width
            height:Responsive.v(2)
            from : 0
            to : 1

            background: Rectangle {
                implicitHeight: progress.height
                implicitWidth:  progress.width
                color : "white"
            }

            contentItem: Item{
                implicitHeight: progress.height
                implicitWidth: progress.width
                Rectangle{
                    Behavior on width {
                        NumberAnimation { duration: 500 }
                    }
                    width: progress.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "green"
                }
            }
        }
    }

    Item {
        id : volumeView
        width : Responsive.h(800)
        height: Responsive.v(120)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Responsive.v(200)
        anchors.horizontalCenter: parent.horizontalCenter
        visible: false
        z : 2

        Slider {
            id : volumeSlider
            from : 0
            to : 100
            value: proxy.volume
            width: parent.width * 0.8
            anchors.horizontalCenter: parent.horizontalCenter
            height: parent.height
            enabled: false
            z : 2

            background: Rectangle {
                x: volumeSlider.leftPadding
                y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                implicitWidth: volumeSlider.width
                implicitHeight: Responsive.v(10)
                width: volumeSlider.availableWidth
                height: implicitHeight
                radius: Responsive.v(5)
                color : "#555555"
            }

            handle: Rectangle {
                x: volumeSlider.leftPadding + volumeSlider.visualPosition * (volumeSlider.availableWidth - width)
                y: volumeSlider.topPadding + volumeSlider.availableHeight / 2 - height / 2
                implicitWidth: Responsive.v(50)
                implicitHeight: Responsive.v(50)
                color : "#ffffff"
                radius: Responsive.v(25)
            }
        }

        Rectangle{
            color : "#2a2a2a"
            anchors.fill : parent
            radius: height * 0.15
        }
    }

    Connections{
        target:proxy
        onVolumeChanged : {
            volumeFadeOutAnimation.stop()
            volumeView.visible = true
            volumeView.opacity = 1
            volumeFadeOutAnimation.restart()
        }
    }

    SequentialAnimation{
        id : volumeFadeOutAnimation


        PauseAnimation {
            duration: 2000
        }

        NumberAnimation {
            target: volumeView
            property: "opacity"
            from: 1
            to:0
            duration: 500
        }
    }
}
