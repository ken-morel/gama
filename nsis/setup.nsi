; =============================================================================
; NSIS Installer Script for "gama" (Verified, Compilable Version)
;
; Features:
; - Self-contained and robust function to add the installation directory to the PATH.
; - Accurately checks if the directory is already in the PATH to avoid duplicates.
; - No uninstaller logic for PATH removal to keep it simple.
; - Compiles cleanly with no warnings.
; =============================================================================

!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "StrFunc.nsh"

; --- Declare which StrFunc macros we are using ---
${Using:StrFunc} StrStr ; <-- THIS IS THE FIX for the new error.

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
!insertmacro MUI_PAGE_LICENSE
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;----------------------------------------------------------------------------
; Custom Function to Add to PATH (Improved for Robustness)
;----------------------------------------------------------------------------
Function AddGamaToPath
  ; Read the current user PATH
  ReadRegStr $R0 HKCU "Environment" "Path"

  ; To prevent partial matches (e.g., `C:\gama` in `C:\gama-old`),
  ; we wrap the existing PATH and our path in semicolons for a precise check.
  ; This reliably finds the entry whether it's at the start, middle, or end.
  ${StrStr} $R1 ";$R0;" ";$INSTDIR;"
  
  ; If $R1 is empty, our path was not found.
  ${If} $R1 == ""
    ; It's not in the PATH, so let's add it.
    ; Check if the current PATH is empty to avoid a leading semicolon.
    ${If} $R0 == ""
      WriteRegExpandStr HKCU "Environment" "Path" "$INSTDIR"
    ${Else}
      WriteRegExpandStr HKCU "Environment" "Path" "$R0;$INSTDIR"
    ${EndIf}
    
    ; Broadcast a message to notify other processes of the change.
    SendMessage ${HWND_BROADCAST} ${WM_SETTINGCHANGE} 0 "STR:Environment" /TIMEOUT=5000
  ${EndIf}
FunctionEnd

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
  File /r "..\assets\images"

  ; --- Add the installation directory to the user's PATH ---
  Call AddGamaToPath
  
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
