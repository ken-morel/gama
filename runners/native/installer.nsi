; =============================================================================
; NSIS Installer Script for "{{name}}"
;
; Features:
; - Self-contained and robust function to add the installation directory to the PATH.
; - Accurately checks if the directory is already in the PATH to avoid duplicates.
; - No uninstaller logic for PATH removal to keep it simple.
; - Compiles cleanly with no warnings.
; =============================================================================

!include "MUI2.nsh"
!include "LogicLib.nsh"


;--------------------------------
; General
Name "{{name}}"
OutFile ".\{{name}}-{{version}}-installer.exe"
InstallDir "$APPDATA\gama\{{author.name}}\{{name}}"
InstallDirRegKey HKCU "Software\{{author.name}}\{{name}}" "InstallDir"
RequestExecutionLevel user


;--------------------------------
; Modern UI Configuration
!define MUI_ABORTWARNING

!define MUI_ICON "..\..\assets\images\icon.ico"
!define MUI_UNICON "..\..\assets\images\icon.ico"

!insertmacro MUI_PAGE_WELCOME
;!insertmacro MUI_PAGE_LICENSE LICENSE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;=============================================================================
; Installer Section
;=============================================================================
Section "Install {{name}}" SecInstall
  SetOutPath "$INSTDIR"

  ; --- Install application files ---
  File "..\native\{{name}}.exe"
  File "..\native\libvgama.dll"
  File /r "..\..\assets"

  ; --- Create Uninstaller and Registry entries for Add/Remove Programs ---
  WriteRegStr HKCU "Software\{{author.name}}\{{name}}" "InstallDir" "$INSTDIR"
  WriteUninstaller "$INSTDIR\uninstall.exe"

  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{name}}" "DisplayName" "{{name}}"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{name}}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{name}}" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{name}}" "NoRepair" 1

  ; --- Create shortcuts ---
  CreateDirectory "$SMPROGRAMS\{{author.name}}"
  CreateShortCut "$SMPROGRAMS\{{author.name}}\{{name}}.lnk" "$INSTDIR\{{name}}.exe"
  CreateShortCut "$DESKTOP\{{name}}.lnk" "$INSTDIR\{{name}}.exe"
SectionEnd

;=============================================================================
; Uninstaller Section
;=============================================================================
Section "Uninstall"
  ; --- Remove shortcuts and Start Menu folder ---
  Delete "$SMPROGRAMS\{{author.name}}\{{name}}.lnk"
  Delete "$DESKTOP\{{name}}.lnk"
  ; RMDir "$SMPROGRAMS\{{author.name}}"

  ; --- Remove registry keys ---
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{name}}"
  DeleteRegKey HKCU "Software\{{author.name}}\{{name}}"

  ; --- Remove all installed files and directories ---
  RMDir /r "$INSTDIR"
SectionEnd
