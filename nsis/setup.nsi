; =============================================================================
; NSIS Installer Script for "gama"
;
; Features:
; - Standard installer without PATH modification.
; - Compiles cleanly with no warnings.
; - Complete and clean uninstaller for all files, shortcuts, and registry keys.
; =============================================================================

!include "MUI2.nsh"
!include "LogicLib.nsh"

;--------------------------------
; General
Name "gama"
OutFile "../bin/gama-installer.exe"
InstallDir "$PROFILE\.gama"
InstallDirRegKey HKCU "Software\cm.engon.gama" "InstallDir"
RequestExecutionLevel user

;--------------------------------
; Modern UI Configuration
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;=============================================================================
; Installer Section
;=============================================================================
Section "Install gama" SecInstall
  SetOutPath "$INSTDIR"
  
  ; --- Install application files ---
  File "..\bin\gama.exe"
  File /r "..\assets\fonts"
  File /r "..\assets\gama"
  File /r "..\assets\templates"
  
  ; --- Create Uninstaller and Registry entries for Add/Remove Programs ---
  WriteRegStr HKCU "Software\cm.engon.gama" "InstallDir" "$INSTDIR"
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "DisplayName" "gama"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "NoRepair" 1

  ; --- Create shortcuts ---
  CreateDirectory "$SMPROGRAMS\gama"
  CreateShortCut "$SMPROGRAMS\gama\gama.lnk" "$INSTDIR\gama.exe"
  CreateShortCut "$DESKTOP\gama\uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortCut "$DESKTOP\gama.lnk" "$INSTDIR\gama.exe"
SectionEnd

;=============================================================================
; Uninstaller Section
;=============================================================================
Section "Uninstall"
  ; --- Remove shortcuts and Start Menu folder ---
  Delete "$SMPROGRAMS\gama\gama.lnk"
  Delete "$DESKTOP\gama.lnk"
  RMDir "$SMPROGRAMS\gama"
  
  ; --- Remove registry keys ---
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama"
  DeleteRegKey HKCU "Software\cm.engon.gama"
  
  ; --- Remove all installed files and directories ---
  RMDir /r "$INSTDIR"
SectionEnd
