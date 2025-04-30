; installer.nsi
!define APP_NAME "PackItUP"
!define EXE_NAME "packitup.exe"
!define VERSION  "1.0.0"
!define INSTALL_DIR "$PROGRAMFILES\${APP_NAME}"

; the resulting installer:
OutFile "${APP_NAME}-${VERSION}-Setup.exe"
InstallDir ${INSTALL_DIR}

; show directory page and progress
Page directory
Page instfiles

; uninstaller pages
UninstPage uninstConfirm
UninstPage instfiles

Section "Install"
    ; copy all files from dist/ into the installation folder
    SetOutPath "$INSTDIR"
    File /r "dist\*.*"

    ; create Start Menu shortcut
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${EXE_NAME}"

    ; create Desktop shortcut
    CreateShortcut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${EXE_NAME}"

    ; write uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Uninstall"
    Delete "$INSTDIR\${EXE_NAME}"
    ; remove all file and directories
    RMDir /r "$INSTDIR"

    Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
    RMDir "$SMPROGRAMS\${APP_NAME}"

    Delete "$DESKTOP\${APP_NAME}.lnk"
SectionEnd
