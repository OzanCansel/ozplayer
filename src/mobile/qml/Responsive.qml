pragma Singleton
import QtQuick 2.0
import QtQuick.Window 2.2

Item    {

    property real viewHeight : 1920
    property real viewWidth : 1080
    property real appHeight : Screen.height < Screen.width ? Screen.width : Screen.height
    property real appWidth : Screen.width > Screen.height ? Screen.height : Screen.width
    readonly property real vFactor : appHeight / viewHeight
    readonly property real hFactor : appWidth / viewWidth

    function v(y){
        return vFactor * y
    }

    function h(x){
        return hFactor * x
    }
}
