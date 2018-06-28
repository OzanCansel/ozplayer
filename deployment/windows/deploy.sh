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

#temp klasoru temizleniyor 
rm -rf temp
#temp klasoru olusturuluyor
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
	#Run make_all calistiriliyor
	$MAKE_COMMAND qmake_all
	#Compile proj
	$MAKE_COMMAND -f Makefile.Release
	cd ..
echo "ozlib compiled."

#server compiling
echo "server is compiling..."
	mkdir server
	cd server
	#qmake kullanilarak makefile uretiliyor
	qmake "$SRC_DIR/server/server.pro" -spec win32-g++
	#qmake make_all calistiriliyor
	$MAKE_COMMAND qmake_all
	#Proje derleniyor
	$MAKE_COMMAND -f Makefile.Release
	cd ..
echo "server is compiled."

echo "Deploying server binary..."
#deploy klasorune giriliyor
cd "$DEPLOY_DIR"
#Uygulama deploy islemi baslatiliyor
cp "$BUILD_DIR/server/release/server.exe" "$DEPLOY_DIR/server.exe"
cp "$BUILD_DIR/ozlib/release/ozlib.dll" "$DEPLOY_DIR/ozlib.dll"
windeployqt.exe "$DEPLOY_DIR\\server.exe" --qmldir="$SRC_DIR" --release
windeployqt.exe "$DEPLOY_DIR\\ozlib.dll" --release
echo "Deployed server as server.exe."

echo "Copying files..."
cp -r "$SCRIPTDIR/installerTemplate/"* "$INSTALLER_DIR"
cp -r "$DEPLOY_DIR/"* "$INSTALLER_DIR/packages/ide/data"

echo "Generating installer"
cd "$INSTALLER_DIR"
binarycreator -c config\\config.xml -p packages OzPlayerInstaller.exe

mkdir -p "$INSTALLER_DIST_DIR"
mv "$INSTALLER_DIR/OzPlayerInstaller.exe" "$INSTALLER_DIST_DIR/OzPlayerInstaller.exe"

echo "Installer has been copied to "
echo "$INSTALLER_DIST_DIR/OzPlayerInstaller.exe"

cd "$SCRIPTDIR"
#derleme dosyasi siliniyor
echo "Cleaning files..."
rm -rf "$TEMP_DIR"

echo "Deploy succeeded!"

exit