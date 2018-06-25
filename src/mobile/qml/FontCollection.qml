pragma Singleton
import QtQuick 2.0

Item {

    property string mainFontName : logoFont.name
    property string connectionFontName : linuxLibertine.name

    FontLoader{
        id : linuxLibertine
        source: "/res/font/linux_libertine/LinLibertine_RZ.ttf"
    }

    FontLoader{
        id : logoFont
        source: "/res/font/rock-it/Rockit.ttf"
    }

}
