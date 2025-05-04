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

    ; this is reduntand but safe
    CreateDirectory "$INSTDIR\share\glib-2.0\schemas"
    CreateDirectory "$INSTDIR\share\locale\pt_BR\LC_MESSAGES"

    ; — install GSettings schemas
    File /r "dist\share\glib-2.0\schemas\*.*"

    ; — install .mo translation
    File "dist\share\locale\pt_BR\LC_MESSAGES\packitup.mo"
    File "dist\share\locale\en_US\LC_MESSAGES\packitup.mo"

     ; — write a small launcher .bat
    ;   this wrapper sets GSETTINGS_SCHEMA_DIR for your exe
    FileOpen $0 "$INSTDIR\packitup.bat" w
    FileWrite $0 '@echo off$\r$\n'
    FileWrite $0 'set "GSETTINGS_SCHEMA_DIR=%~dp0share\glib-2.0\schemas"$\r$\n'
    FileWrite $0 '"%~dp0\packitup.exe" %*$\r$\n'
    FileClose $0

    ; — put shortcuts to the .bat instead of the raw exe
    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" \
                   "$INSTDIR\packitup.bat"
    CreateShortcut "$DESKTOP\${APP_NAME}.lnk" \
                   "$INSTDIR\packitup.bat"

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
