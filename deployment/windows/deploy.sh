#!/bin/sh

if [ "$#" -ne 3 ]; then
	echo "deploy.sh <Qt Dizin> <Kit Dizin> <version> seklinde calistirilmalidir."
	echo "ornek => " 'sh deploy.sh /c/Qt /c/Qt/5.9.1/mingw53_32 v1'
	exit
fi

SCRIPTDIR=$(pwd)

QTPATH=$1
KITPATH=$2
VERSION=$3
MAKE_COMMAND=mingw32-make.exe
DEPLOY_PATH=$KITPATH\\bin\\windeployqt.exe
SRC_DIR=$(realpath -sm "$SCRIPTDIR/../../src")
TEMP_DIR="$SCRIPTDIR/temp"
INSTALLER_DIR="$SCRIPTDIR/temp/installer"
DEPLOY_DIR="$SCRIPTDIR/temp/deploy"
BUILD_DIR="$SCRIPTDIR/temp/build"
DIST_DIR=$(realpath -sm "$SCRIPTDIR/../../dist")
INSTALLER_DIST_DIR=$(realpath -sm "$SCRIPTDIR/../../dist/windows/$VERSION")

export PATH=$KITPATH/bin:$QTPATH/Tools/QtInstallerFramework/2.0/bin:$QTPATH/Tools/mingw530_32/bin:$PATH

#clear temp
rm -rf temp
#mkdir temp and deployment folders
mkdir -p "$TEMP_DIR"
mkdir -p "$BUILD_DIR" 
mkdir -p "$DEPLOY_DIR"
mkdir -p "$INSTALLER_DIR"
#temp klasoruna giriliyor	temp
cd "$BUILD_DIR"

#ozlib compiling
echo "ozlib is compiling..."
	mkdir ozlib
	cd ozlib
	#Generate makefile via qmake
	qmake "$SRC_DIR/ozlib/ozlib.pro" -spec win32-g++
	#Run make_all
	$MAKE_COMMAND qmake_all
	#Compile project
	$MAKE_COMMAND -f Makefile.Release
	cd ..
echo "ozlib compiled."

#qslog compiling
echo "qslog is compiling..."
	mkdir third_party
	cd third_party
	mkdir qslog
	cd qslog
	#Generate makefile via qmake
	qmake "$SRC_DIR/third_party/qslog/qslog.pro" -spec win32-g++
	#Run make_all
	$MAKE_COMMAND qmake_all
	#Compile project
	$MAKE_COMMAND -f Makefile.Release
	cd ..
	cd ..
echo "qslig compiled."

#server compiling
echo "server is compiling..."
	mkdir server
	cd server
	#Generate makefile via qmake
	qmake "$SRC_DIR/server/server.pro" -spec win32-g++
	#run qmake make_all
	$MAKE_COMMAND qmake_all
	#Compile project
	$MAKE_COMMAND -f Makefile.Release
	cd ..
echo "server is compiled."

#configurator compiling
echo "configurator is compiling..."
	mkdir configurator
	cd configurator
	#Generator makefile via qmake
	qmake "$SRC_DIR/configurator/configurator.pro" -spec win32-g++
	#Compile project
	$MAKE_COMMAND qmake_all
	#Compile project
	$MAKE_COMMAND -f Makefile.Release
	cd ..
echo "configurator is compiled."

#mobile compiling
echo "mobile is compiling..."
	mkdir mobile
	cd mobile
	#Generator makefile via qmake
	qmake "$SRC_DIR/mobile/mobile.pro"  -spec win32-g++
	#Compile project
	$MAKE_COMMAND qmake_all
	#Compile project
	$MAKE_COMMAND -f Makefile.Release
	cd ..
echo "mobile is compiled."

echo "Deploying executables and libraries..."
#deploy klasorune giriliyor
cd "$DEPLOY_DIR"
#Uygulama deploy islemi baslatiliyor
cp "$BUILD_DIR/ozlib/release/ozlib.dll" "$DEPLOY_DIR/ozlib.dll"
cp "$BUILD_DIR/third_party/qslog/release/qslog.dll" "$DEPLOY_DIR/qslog.dll"
cp "$BUILD_DIR/server/release/server.exe" "$DEPLOY_DIR/server.exe"
cp "$BUILD_DIR/configurator/release/configurator.exe" "$DEPLOY_DIR/configurator.exe"
cp "$BUILD_DIR/mobile/release/mobile.exe" "$DEPLOY_DIR/ozplayer.exe"
windeployqt.exe "$DEPLOY_DIR\\server.exe" --release
echo "Deployed server as server.exe."
echo "Deployed ozlib as ozlib.dll"
windeployqt.exe "$DEPLOY_DIR\\configurator.exe" --release
echo "Deployed configurator as configurator.exe"
windeployqt.exe "$DEPLOY_DIR\\ozplayer.exe" --qmldir="$SRC_DIR\\mobile" --release
echo "Deployed ozplayer as ozplayer.exe"

echo "Copying files..."
cp -r "$SCRIPTDIR/installerTemplate/"* "$INSTALLER_DIR"
cp -r "$DEPLOY_DIR/"* "$INSTALLER_DIR/packages/ide/data"

echo "Generating installer"
cd "$INSTALLER_DIR"
binarycreator -c config\\config.xml -p packages ozplayer-installer.exe

mkdir -p "$INSTALLER_DIST_DIR"
mv "$INSTALLER_DIR/ozplayer-installer.exe" "$INSTALLER_DIST_DIR/ozplayer-installer.exe"

echo "Installer has been copied to "
echo "$INSTALLER_DIST_DIR/ozplayer-installer.exe"

cd "$SCRIPTDIR"
#derleme dosyasi siliniyor
echo "Cleaning files..."
rm -rf "$TEMP_DIR"

echo "Deploy succeeded!"

exit