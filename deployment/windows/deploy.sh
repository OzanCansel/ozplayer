#!/bin/sh

if [ "$#" -ne 4 ]; then
	echo "should be run as \"deploy.sh <Qt kit bin> <tools mingw53 make bin> <Qt Installer Framework bin> <version>.\""
	echo "ornek => " sh deploy.sh /c/Qt/5.11.1/mingw53_32/bin /c/Qt/Tools/mingw530_32/bin /c/Qt/Tools/QtInstallerFramework/2.0/bin v1
	exit
fi

function run {
if [ "$#" -ne 1 ]; then 
	echo "Parameter not specified"
	exit
fi

eval $1

if [ $? -ne 0 ]; then
	echo ------------------------
    echo "$1 Command failed, exiting"
	echo ------------------------
	exit
fi
}

SCRIPTDIR=$(pwd)

#QTPATH=$1
#KITPATH=$2
KIT_FOLDER=$1
MAKE_FOLDER=$2
IFW_FOLDER=$3
VERSION=$4
MAKE_COMMAND=mingw32-make.exe
DEPLOY_PATH=$KITPATH\\bin\\windeployqt.exe
SRC_DIR=$(realpath -sm "$SCRIPTDIR/../../src")
TEMP_DIR="$SCRIPTDIR/temp"
INSTALLER_DIR="$SCRIPTDIR/temp/installer"
DEPLOY_DIR="$SCRIPTDIR/temp/deploy"
BUILD_DIR="$SCRIPTDIR/temp/build"
DIST_DIR=$(realpath -sm "$SCRIPTDIR/../../dist")
INSTALLER_DIST_DIR=$(realpath -sm "$SCRIPTDIR/../../dist/windows/$VERSION")

#export PATH=$KITPATH/bin:$QTPATH/Tools/QtInstallerFramework/2.0/bin:$QTPATH/Tools/mingw530_32/bin:$PATH
export PATH=$KIT_FOLDER:$IFW_FOLDER:$MAKE_FOLDER:$PATH

#clear temp
run "rm -rf temp"
#mkdir temp and deployment folders
run "mkdir -p $TEMP_DIR"
run "mkdir -p $BUILD_DIR"
run "mkdir -p $DEPLOY_DIR"
run "mkdir -p $INSTALLER_DIR"
#temp klasoruna giriliyor	temp
run "cd $BUILD_DIR"

#ozlib compiling
echo "ozlib is compiling..."
	run "mkdir ozlib"
	run "cd ozlib"
	#Generate makefile via qmake
	run "qmake $SRC_DIR/ozlib/ozlib.pro -spec win32-g++"
	#Run make_all
	run "$MAKE_COMMAND qmake_all"
	#Compile project
	run "$MAKE_COMMAND -f Makefile.Release"
	run "cd .."
echo "ozlib compiled."

#qslog compiling
echo "qslog is compiling..."
	run "mkdir third_party"
	run "cd third_party"
	run "mkdir qslog"
	run "cd qslog"
	#Generate makefile via qmake
	run "qmake $SRC_DIR/third_party/qslog/qslog.pro -spec win32-g++"
	#Run make_all
	run "$MAKE_COMMAND qmake_all"
	#Compile project
	run "$MAKE_COMMAND -f Makefile.Release"
	run "cd .."
	run "cd .."
echo "qslig compiled."

#server compiling
echo "server is compiling..."
	run "mkdir server"
	run "cd server"
	#Generate makefile via qmake
	run "qmake $SRC_DIR/server/server.pro -spec win32-g++"
	#run qmake make_all
	run "$MAKE_COMMAND qmake_all"
	#Compile project
	run "$MAKE_COMMAND -f Makefile.Release"
	run "cd .."
echo "server is compiled."

#configurator compiling
echo "configurator is compiling..."
	run "mkdir configurator"
	run "cd configurator"
	#Generator makefile via qmake
	run "qmake $SRC_DIR/configurator/configurator.pro -spec win32-g++"
	#Compile project
	run "$MAKE_COMMAND qmake_all"
	#Compile project
	run "$MAKE_COMMAND -f Makefile.Release"
	run "cd .."
echo "configurator is compiled."

#mobile compiling
echo "mobile is compiling..."
	run "mkdir mobile"
	run "cd mobile"
	#Generator makefile via qmake
	run "qmake $SRC_DIR/mobile/mobile.pro  -spec win32-g++"
	#Compile project
	run "$MAKE_COMMAND qmake_all"
	#Compile project
	run "$MAKE_COMMAND -f Makefile.Release"
	run "cd .."
echo "mobile is compiled."

echo "Deploying executables and libraries..."
#deploy klasorune giriliyor
cd "$DEPLOY_DIR"
#Uygulama deploy islemi baslatiliyor
run "cp $BUILD_DIR/ozlib/release/ozlib.dll $DEPLOY_DIR/ozlib.dll"
run "cp $BUILD_DIR/third_party/qslog/release/qslog.dll $DEPLOY_DIR/qslog.dll"
run "cp $BUILD_DIR/server/release/server.exe $DEPLOY_DIR/server.exe"
run "cp $BUILD_DIR/configurator/release/configurator.exe $DEPLOY_DIR/configurator.exe"
run "cp $BUILD_DIR/mobile/release/mobile.exe $DEPLOY_DIR/ozplayer.exe"
run "windeployqt.exe $DEPLOY_DIR/server.exe --release"
run "echo Deployed server as server.exe."
run "echo Deployed ozlib as ozlib.dll"
run "windeployqt.exe $DEPLOY_DIR/configurator.exe --release"
echo "Deployed configurator as configurator.exe"
run "windeployqt.exe $DEPLOY_DIR/ozplayer.exe --qmldir=$SRC_DIR/mobile --release"
echo "Deployed ozplayer as ozplayer.exe"

echo "Copying files..."
run "cp -r $SCRIPTDIR/installerTemplate/* $INSTALLER_DIR"
run "cp -r $DEPLOY_DIR/* $INSTALLER_DIR/packages/player/data"

echo "Generating installer"
run "cd $INSTALLER_DIR"
run "binarycreator -c config/config.xml -p packages ozplayer-installer.exe"

run "mkdir -p $INSTALLER_DIST_DIR"
run "mv $INSTALLER_DIR/ozplayer-installer.exe $INSTALLER_DIST_DIR/ozplayer-installer.exe"

echo "Installer has been copied to "
echo "$INSTALLER_DIST_DIR/ozplayer-installer.exe"

run "cd $SCRIPTDIR"
#derleme dosyasi siliniyor
echo "Cleaning files..."
run "rm -rf $TEMP_DIR"

echo "Deploy succeeded!"

exit