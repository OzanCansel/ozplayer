#!bin/bash

RESOURCE_NAME=kodlio-kodlioide

SCRIPT_PATH=$( cd $(dirname $0) ; pwd )
cd "${SCRIPT_PATH}"

# Default mode is to install.
UNINSTALL=false

XDG_DESKTOP_DIR="${HOME}/Desktop"
if [ -f "${XDG_CONFIG_HOME:-${HOME}/.config}/user-dirs.dirs" ]; then
  . "${XDG_CONFIG_HOME:-${HOME}/.config}/user-dirs.dirs"
fi

xdg_install_f(){

  # Create a temp dir accessible by all users
  TMP_DIR=`mktemp --directory`

  # Create *.desktop file using the existing template file
  sed -e "s,<BINARY_LOCATION>,${SCRIPT_PATH}/kodlio,g" \
      -e "s,<ICON_NAME>,${RESOURCE_NAME},g" "${SCRIPT_PATH}/lib/desktop.template" > "${TMP_DIR}/${RESOURCE_NAME}.desktop"

  # Install the icon files using name and resolutions
  xdg-icon-resource install --context apps --size 16 "${SCRIPT_PATH}/lib/icons/16x16/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 24 "${SCRIPT_PATH}/lib/icons/24x24/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 32 "${SCRIPT_PATH}/lib/icons/32x32/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 48 "${SCRIPT_PATH}/lib/icons/48x48/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 64 "${SCRIPT_PATH}/lib/icons/64x64/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 72 "${SCRIPT_PATH}/lib/icons/72x72/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 96 "${SCRIPT_PATH}/lib/icons/96x96/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 128 "${SCRIPT_PATH}/lib/icons/128x128/kodlio.png" $RESOURCE_NAME
  xdg-icon-resource install --context apps --size 256 "${SCRIPT_PATH}/lib/icons/256x256/kodlio.png" $RESOURCE_NAME

  # Install the created *.desktop file
  xdg-desktop-menu install "${TMP_DIR}/${RESOURCE_NAME}.desktop"

  # Create icon on the desktop
  xdg-desktop-icon install "${TMP_DIR}/${RESOURCE_NAME}.desktop"

  # Install Kodlio mime type
  xdg-mime install "${SCRIPT_PATH}/lib/${RESOURCE_NAME}.xml"

  # Install icons for mime type
  xdg-icon-resource install --context mimetypes --size 16 "${SCRIPT_PATH}/lib/icons/16x16/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 24 "${SCRIPT_PATH}/lib/icons/24x24/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 32 "${SCRIPT_PATH}/lib/icons/32x32/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 48 "${SCRIPT_PATH}/lib/icons/48x48/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 64 "${SCRIPT_PATH}/lib/icons/64x64/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 72 "${SCRIPT_PATH}/lib/icons/72x72/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 96 "${SCRIPT_PATH}/lib/icons/96x96/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 128 "${SCRIPT_PATH}/lib/icons/128x128/kodlio.png" text-x-kodlio
  xdg-icon-resource install --context mimetypes --size 256 "${SCRIPT_PATH}/lib/icons/256x256/kodlio.png" text-x-kodlio

  # Make Kodlio IDE the default application for *.cpp
  xdg-mime default ${RESOURCE_NAME}.desktop text/x-kodlio

  # Clean up
  rm "${TMP_DIR}/${RESOURCE_NAME}.desktop"
  rmdir "$TMP_DIR"

}

# Check availability of xdg-utils
xdg_exists_f() {

  if ! command -v xdg-icon-resource > /dev/null; then return 1; fi
  if ! command -v xdg-desktop-menu > /dev/null; then return 1; fi
  if ! command -v xdg-desktop-icon > /dev/null; then return 1; fi
  if ! command -v xdg-mime > /dev/null; then return 1; fi
  return 0

}

# Install by simply copying desktop file (fallback)
simple_install_f() {

  # Create a temp dir accessible by all users
  TMP_DIR=`mktemp --directory`

  # Create *.desktop file using the existing template file
  sed -e "s,<BINARY_LOCATION>,${SCRIPT_PATH}/kodlio,g" \
      -e "s,<ICON_NAME>,${SCRIPT_PATH}/lib/kodlio.png,g" "${SCRIPT_PATH}/lib/desktop.template" > "${TMP_DIR}/${RESOURCE_NAME}.desktop"

  mkdir -p "${HOME}/.local/share/applications"
  cp "${TMP_DIR}/${RESOURCE_NAME}.desktop" "${HOME}/.local/share/applications/"

  # Copy desktop icon if desktop dir exists (was found)
  if [ -d "${XDG_DESKTOP_DIR}" ]; then
   cp "${TMP_DIR}/${RESOURCE_NAME}.desktop" "${XDG_DESKTOP_DIR}/"
   # Altering file permissions to avoid "Untrusted Application Launcher" error on Ubuntu
   chmod u+x "${XDG_DESKTOP_DIR}/${RESOURCE_NAME}.desktop"
  fi

  # Clean up temp dir
  rm "${TMP_DIR}/${RESOURCE_NAME}.desktop"
  rmdir "${TMP_DIR}"

}

# Uninstall using xdg-utils
xdg_uninstall_f() {

  # Remove *.desktop file
  xdg-desktop-menu uninstall ${RESOURCE_NAME}.desktop

  # Remove icon from desktop
  xdg-desktop-icon uninstall ${RESOURCE_NAME}.desktop

  # Remove icons
  xdg-icon-resource uninstall --size 16 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 24 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 32 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 48 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 64 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 72 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 96 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 128 ${RESOURCE_NAME}
  xdg-icon-resource uninstall --size 256 ${RESOURCE_NAME}

  # Remove MIME type icons
  xdg-icon-resource uninstall --size 16  text-x-kodlio
  xdg-icon-resource uninstall --size 24  text-x-kodlio
  xdg-icon-resource uninstall --size 32  text-x-kodlio
  xdg-icon-resource uninstall --size 48  text-x-kodlio
  xdg-icon-resource uninstall --size 64  text-x-kodlio
  xdg-icon-resource uninstall --size 72  text-x-kodlio
  xdg-icon-resource uninstall --size 96  text-x-kodlio
  xdg-icon-resource uninstall --size 128  text-x-kodlio
  xdg-icon-resource uninstall --size 256  text-x-kodlio

  # Remove Kodlio MIME type
  xdg-mime uninstall "${SCRIPT_PATH}/lib/${RESOURCE_NAME}.xml"

}

# Uninstall by simply removing desktop files (fallback), incl. old one
simple_uninstall_f() {

  if [ -f "${HOME}/.local/share/applications/kodlio.desktop" ]; then
    rm "${HOME}/.local/share/applications/kodlio.desktop"
  fi

  if [ -f "${HOME}/.local/share/applications/${RESOURCE_NAME}.desktop" ]; then
    rm "${HOME}/.local/share/applications/${RESOURCE_NAME}.desktop"
  fi

  if [ -f "${XDG_DESKTOP_DIR}/kodlio.desktop" ]; then
    rm "${XDG_DESKTOP_DIR}/kodlio.desktop"
  fi

  if [ -f "${XDG_DESKTOP_DIR}/${RESOURCE_NAME}.desktop" ]; then
    rm "${XDG_DESKTOP_DIR}/${RESOURCE_NAME}.desktop"
  fi

}

# Update desktop file and mime databases (if possible)
updatedbs_f() {

  if [ -d "${HOME}/.local/share/applications" ]; then
    if command -v update-desktop-database > /dev/null; then
      update-desktop-database "${HOME}/.local/share/applications"
    fi
  fi

  if [ -d "${HOME}/.local/share/mime" ]; then
    if command -v update-mime-database > /dev/null; then
      update-mime-database "${HOME}/.local/share/mime"
    fi
  fi

}

# Check for provided arguments
while [ $# -gt 0 ] ; do
  ARG="${1}"
  case $ARG in
      -u|--uninstall)
        UNINSTALL=true
        shift
      ;;
      -h|--help)
        display_help_f
        exit 0
      ;;
      *)
        printf "\nInvalid option -- '${ARG}'\n"
        display_help_f
        exit 1
      ;;
  esac
done


# If possible, use xdg-utils, if not, use a more basic approach
if xdg_exists_f; then
  if [ ${UNINSTALL} = true ]; then
    echo "Kodlio kısayolu siliniyor"
    xdg_uninstall_f
    simple_uninstall_f
  else
    echo "Kodlio kısayolları ekleniyor"
    xdg_uninstall_f
    simple_uninstall_f
    xdg_install_f
  fi
else
  if [ ${UNINSTALL} = true ]; then
    echo "Kodlio kısayolları siliniyor. S"
    simple_uninstall_f
  else
    echo "Kodlio kısayolları ekleniyor. S"
    simple_uninstall_f
    simple_install_f
  fi
fi
updatedbs_f
printf " done!\n"


exit
