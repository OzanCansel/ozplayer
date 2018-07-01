#!/bin/bash

if [ "$#" -ne 3 ]; then
    echo "./deploy.sh <Kit Dizin> <version> seklinde calistirilmalidir."
    echo "ornek => './deploy.sh /home/arnes/Qt /home/arnes/Qt/5.9.1/gcc_64 v2'"
    exit
fi

SCRIPT=$(readlink -f '$0')
SCRIPTDIR=$(pwd)
QTPATH=$1
KITPATH=$2
VERSION=$3

DEPLOY_TOOL_PATH=deploy
SRC_DIR=$(realpath -sm "$SCRIPTDIR/../../src")
TEMP_DIR="$SCRIPTDIR/temp"
DEPLOY_DIR="$SCRIPTDIR/temp/deploy"
BUILD_DIR="$SCRIPTDIR/temp/build"
DIST_DIR=$(realpath -sm "$SCRIPTDIR/../../dist")
INSTALLER_DIST_DIR=$(realpath -sm "$SCRIPTDIR/../../dist/linux/$VERSION")


export PATH=$KITPATH/bin:$QTPATH/Tools/QtInstallerFramework/2.0/bin:$PATH

#grant permission to execute
chmod +x deploy

echo "SRC_DIR $SRC_DIR"

#clear temp
rm -rf temp
#mkdir temp and deployment folders
mkdir -p "$TEMP_DIR"
mkdir -p "$BUILD_DIR" 
mkdir -p "$DEPLOY_DIR"
mkdir -p "$INSTALLER_DIST_DIR"
#temp klasoruna giriliyor	temp
cd "$BUILD_DIR"

echo "Compiling started..."

#ozlib compiling
echo "ozlib is compiling..."
    mkdir ozlib
    cd ozlib
    #Generate makefile via qmake
    qmake CONFIG+=release "$SRC_DIR/ozlib/ozlib.pro" -spec linux-g++
    #Run make all
    make qmake_all
    #Compile project
    make
    cd ..
echo "ozlib is compiled."

#server compiling
echo "server is compiling..."
    mkdir server
    cd server
    #Generate makefile via qmake
    qmake CONFIG+=release "$SRC_DIR/server/server.pro" -spec linux-g++
    #Run make_all
    make qmake_all
    #Compile project
    make
    cd ..
echo "server is compiled."

#configurator compiling
echo "configurator is compiling"
    mkdir configurator
    cd configurator
    #Generate makefile via qmake
    qmake CONFIG+=release "$SRC_DIR/configurator/configurator.pro" -spec linux-g++
    #Compile project
    make
    cd ..
echo "configurator is compiled"

echo "Deploying executables and libraries..."
#deploy klasorune giriliyor
cd "$DEPLOY_DIR"
#Uygulama deploy islemi baslatiliyor
cp "$BUILD_DIR/server/server" "$DEPLOY_DIR/server"
cp "$BUILD_DIR/ozlib/libozlib.so" "$DEPLOY_DIR/libozlib.so"
cp "$BUILD_DIR/configurator/configurator" "$DEPLOY_DIR/configurator"

#Copy extra files
cp "$SRC_DIR/third_party/qtservice/lib/libQtSolutions_Service-head.so.1.0.0" "$SRC_DIR/libQtSolutions_Service-head.so.1"

exit

deploy "$DEPLOY_DIR\\server" --release -appimage
echo "Deployed server as server"
deploy "$DEPLOY_DIR\\ozlib.so" --release -appimage
echo "Deployed ozlib as ozlib.dll"
deploy "$DEPLOY_DIR\\configurator" --release -appimage
echo "Deployed configurator as configurator.exe"

echo "Copying files..."
cp -r "$SCRIPTDIR/installerTemplate/"* "$INSTALLER_DIR"
cp -r "$DEPLOY_DIR/"* "$INSTALLER_DIR/packages/ide/data"

echo "Deploying application"
cd ..
mkdir -p deploy
cd deploy
../../deploy ../build/kodlio -qmldir=../../../../src -appimage
../../deploy ../build/kodlio -qmldir=../../../../src -appimage
mv Application-x86_64.AppImage kodlio 2>/dev/null

echo "Dosyalar kopyalaniyor"
cd ..
mkdir -p installer
cd installer
cp -r ../../installerTemplate/* .
cp -r ../../../../src/extFiles/linux/* packages/ide/data
cp ../deploy/kodlio packages/ide/data/kodlio

echo "Installer uretiliyor..."
binarycreator -c config/config.xml -p packages KodlioKurulum

mkdir -p ../../../../dist/linux/$VERSION
mv KodlioKurulum ../../../../dist/linux/$VERSION/KodlioKurulum

echo "Installer uretildi. Dizin => $(pwd)" 

cd ..
cd ..
rm -rf temp

exit