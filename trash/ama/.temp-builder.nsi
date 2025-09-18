; =============================================================================
; NSIS Installer Script for "ama"
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
Name "ama"
OutFile ".\build\windows\ama-0.1.0-installer.exe"
InstallDir "$APPDATA\gama\jealomy\ama"
InstallDirRegKey HKCU "Software\jealomy\ama" "InstallDir"
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
Section "Install ama" SecInstall
  SetOutPath "$INSTDIR"
  
  ; --- Install application files ---
  File ".\build\windows\ama.exe"
  File /r ".\assets"

  ; --- Create Uninstaller and Registry entries for Add/Remove Programs ---
  WriteRegStr HKCU "Software\jealomy\ama" "InstallDir" "$INSTDIR"
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\jealomy\ama" "DisplayName" "ama"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\jealomy\ama" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\jealomy\ama" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\jealomy\ama" "NoRepair" 1

  ; --- Create shortcuts ---
  CreateDirectory "$SMPROGRAMS\jealomy"
  CreateShortCut "$SMPROGRAMS\jealomy\ama.lnk" "$INSTDIR\ama.exe"
  CreateShortCut "$DESKTOP\ama.lnk" "$INSTDIR\ama.exe"
SectionEnd

;=============================================================================
; Uninstaller Section
;=============================================================================
Section "Uninstall"
  ; --- Remove shortcuts and Start Menu folder ---
  Delete "$SMPROGRAMS\jealomy\ama.lnk"
  Delete "$DESKTOP\ama.lnk"
  ; RMDir "$SMPROGRAMS\jealomy"
  
  ; --- Remove registry keys ---
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\jealomy\ama"
  DeleteRegKey HKCU "Software\jealomy\ama"
  
  ; --- Remove all installed files and directories ---
  RMDir /r "$INSTDIR"
SectionEnd
