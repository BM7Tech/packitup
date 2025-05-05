; installer.nsi
!define APP_NAME "PackItUP"
!define EXE_NAME "packitup.exe"
!define VERSION  "1.0.0"
!define INSTALL_DIR "$PROGRAMFILES\${APP_NAME}"
!include MUI2.nsh

!define MUI_ICON    "dist\\packitup.ico"
!define MUI_UNICON  "dist\\packitup.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; the resulting installer:
OutFile "${APP_NAME}-${VERSION}-Setup.exe"
InstallDir ${INSTALL_DIR}

; uninstaller pages
UninstPage uninstConfirm
UninstPage instfiles

Section "Install"
    SetOutPath "$INSTDIR"

    ; Copy your two executables (launcher + real app)
    File "dist\\bin\\packitup.exe"
    File "dist\\bin\\packitup-real.exe"
    
    ; — all the DLLs we need
    File /r "dist\\*.dll"

    ; Copy schemas & translations
    File /r "dist\\share\\glib-2.0\\schemas\\*.*"
    File /r "dist\\share\\locale\\pt_BR\\LC_MESSAGES\\*.mo"
    File /r "dist\\share\\locale\\en_US\\LC_MESSAGES\\*.mo"

    ; Copy your icon for the shortcut
    File "dist\\packitup.ico"

    ; Create shortcuts
    CreateDirectory "$SMPROGRAMS\\PackItUP"
    CreateShortcut "$SMPROGRAMS\\PackItUP\\PackItUP.lnk" "$INSTDIR\\packitup.exe" "" "$INSTDIR\\packitup.ico"
    CreateShortcut "$DESKTOP\\PackItUP.lnk" "$INSTDIR\\packitup.exe" "" "$INSTDIR\\packitup.ico"

    WriteUninstaller "$INSTDIR\\Uninstall.exe"
SectionEnd

Section "Uninstall"
    Delete "$INSTDIR\${EXE_NAME}"
    ; remove all file and directories
    RMDir /r "$INSTDIR"

    Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
    RMDir "$SMPROGRAMS\${APP_NAME}"

    Delete "$DESKTOP\${APP_NAME}.lnk"
SectionEnd
