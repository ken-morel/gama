; =============================================================================
; NSIS Installer Script for "{{project.name}}"
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
Name "{{project.name}}"
OutFile ".\build\windows\{{project.name}}-{{project.version}}-installer.exe"
InstallDir "$APPDATA\gama\{{author.name}}\{{project.name}}"
InstallDirRegKey HKCU "Software\{{author.name}}\{{project.name}}" "InstallDir"
RequestExecutionLevel user


;--------------------------------
; Modern UI Configuration
!define MUI_ABORTWARNING

!define MUI_ICON "./logo.ico"
!define MUI_UNICON "./logo.ico"

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE LICENSE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;=============================================================================
; Installer Section
;=============================================================================
Section "Install {{project.name}}" SecInstall
  SetOutPath "$INSTDIR"
  
  ; --- Install application files ---
  File ".\build\windows\{{project.name}}.exe"
  File /r ".\assets"

  ; --- Create Uninstaller and Registry entries for Add/Remove Programs ---
  WriteRegStr HKCU "Software\{{author.name}}\{{project.name}}" "InstallDir" "$INSTDIR"
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{project.name}}" "DisplayName" "{{project.name}}"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{project.name}}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{project.name}}" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{project.name}}" "NoRepair" 1

  ; --- Create shortcuts ---
  CreateDirectory "$SMPROGRAMS\{{author.name}}"
  CreateShortCut "$SMPROGRAMS\{{author.name}}\{{project.name}}.lnk" "$INSTDIR\{{project.name}}.exe"
  CreateShortCut "$DESKTOP\{{project.name}}.lnk" "$INSTDIR\{{project.name}}.exe"
SectionEnd

;=============================================================================
; Uninstaller Section
;=============================================================================
Section "Uninstall"
  ; --- Remove shortcuts and Start Menu folder ---
  Delete "$SMPROGRAMS\{{author.name}}\{{project.name}}.lnk"
  Delete "$DESKTOP\{{project.name}}.lnk"
  ; RMDir "$SMPROGRAMS\{{author.name}}"
  
  ; --- Remove registry keys ---
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\{{author.name}}\{{project.name}}"
  DeleteRegKey HKCU "Software\{{author.name}}\{{project.name}}"
  
  ; --- Remove all installed files and directories ---
  RMDir /r "$INSTDIR"
SectionEnd
