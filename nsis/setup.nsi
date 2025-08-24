; -----------------------------------------------------------------------------
; Script Corrected with a Robust, Unified Path Management Function
;
; Author:            Engon Ken Morel
; Email:             engonkenmorel@gmail.com
; Website:           https://engon.cm
; Application Name:  gama
; App Identifier:    cm.engon.gama
; -----------------------------------------------------------------------------

;--------------------------------
; Include Modern UI & Logic Library
!include "MUI2.nsh"
!include "LogicLib.nsh"

;--------------------------------
; General
Name "gama"
OutFile "../bin/gama-installer.exe"
InstallDir "$PROFILE\.gama"
InstallDirRegKey HKCU "Software\cm.engon.gama" "InstallDir"
RequestExecutionLevel user

; --- MUI Settings ---
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

;--------------------------------
; Installer Section
Section "Install gama" SecInstall
  SetOutPath "$INSTDIR"
  
  File "..\bin\gama.exe"
  File /r "..\assets"

  ; --- CORRECTED Add to PATH logic ---
  ; Use a robust, unified function to safely add the directory to the PATH.
  Push "add"
  Push "$INSTDIR"
  Call EnvVarUpdate
  
  ; --- Registry entries for Add/Remove Programs ---
  WriteRegStr HKCU "Software\cm.engon.gama" "InstallDir" "$INSTDIR"
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
  CreateDirectory "$SMPROGRAMS\gama"
  CreateShortCut "$SMPROGRAMS\gama\gama.lnk" "$INSTDIR\gama.exe"
  CreateShortCut "$DESKTOP\gama.lnk" "$INSTDIR\gama.exe"
  
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "DisplayName" "gama"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama" "NoRepair" 1

SectionEnd

;--------------------------------
; Uninstaller Section
Section "Uninstall"
  Delete "$INSTDIR\gama.exe"
  Delete "$INSTDIR\uninstall.exe"
  RMDir /r "$INSTDIR\assets"

  ; --- CORRECTED Remove from PATH logic ---
  ; Call the uninstaller's copy of the robust function.
  Push "remove"
  Push "$INSTDIR"
  Call un.EnvVarUpdate

  ; --- Clean up shortcuts and directories ---
  Delete "$SMPROGRAMS\gama\gama.lnk"
  Delete "$DESKTOP\gama.lnk"
  RMDir "$SMPROGRAMS\gama"
  
  ; --- Clean up registry keys ---
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\cm.engon.gama"
  DeleteRegKey HKCU "Software\cm.engon.gama"
  
  RMDir "$INSTDIR"
SectionEnd

;=============================================================================
; Installer's Robust Path Management Function
;=============================================================================
Function EnvVarUpdate
  Exch $R1 ; $R1 contains the directory (e.g., C:\MyDir)
  Exch
  Exch $R0 ; $R0 contains the action ("add" or "remove")
  
  Push $R2 ; Original PATH
  Push $R3 ; New, rebuilt PATH
  Push $R4 ; A single path entry
  Push $R5 ; Flag to check if the directory was found

  ; WARNING: NSIS has a default string size limit (usually 1024 chars).
  ; If the user's PATH is longer, ReadRegStr will truncate it, and this
  ; script will corrupt the PATH. For maximum safety, compile the installer
  ; using a "large strings" build of NSIS.
  ReadRegStr $R2 HKCU "Environment" "Path"
  StrCpy $R3 "" ; Initialize new path
  StrCpy $R5 0   ; Initialize found flag

  loop:
    ; Grab the next entry from the path string
    StrCpy $R4 $R2 -1
    StrCmp $R4 "" get_last_entry
    StrCpy $R2 $R2 -1
    StrCmp $R4 ";" loop ; Found a separator, process the next entry
    Goto loop

  get_last_entry:
    ; Process the entry
    ${If} $R4 != ""
      ${If} $R4 == $R1
        StrCpy $R5 1 ; Directory found in PATH
        ${If} $R0 == "remove"
          StrCpy $R4 "" ; Clear the variable so it's not added to the new path
        ${EndIf}
      ${EndIf}
      ${If} $R4 != ""
        ${If} $R3 == ""
          StrCpy $R3 $R4
        ${Else}
          StrCpy $R3 "$R4;$R3"
        ${EndIf}
      ${EndIf}
    ${EndIf}
    StrCmp $R2 "" done
    Goto loop
  
  done:
    ; If we were adding and the path was NOT found, add it now.
    ${If} $R0 == "add"
    ${AndIf} $R5 == 0
      ${If} $R3 == ""
        StrCpy $R3 "$R1"
      ${Else}
        StrCpy $R3 "$R3;$R1"
      ${EndIf}
    ${EndIf}

  WriteRegExpandStr HKCU "Environment" "Path" "$R3"
  SendMessage ${HWND_BROADCAST} ${WM_SETTINGCHANGE} 0 "STR:Environment" /TIMEOUT=5000

  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Pop $R0
  Pop $R1
FunctionEnd

;=============================================================================
; Uninstaller's Robust Path Management Function
;
; NOTE: This is a required duplicate of the function above. The uninstaller
; is a standalone .exe and needs its own copy of any functions it calls.
;=============================================================================
Function un.EnvVarUpdate
  Exch $R1 ; $R1 contains the directory (e.g., C:\MyDir)
  Exch
  Exch $R0 ; $R0 contains the action ("add" or "remove")
  
  Push $R2 ; Original PATH
  Push $R3 ; New, rebuilt PATH
  Push $R4 ; A single path entry
  Push $R5 ; Flag to check if the directory was found

  ReadRegStr $R2 HKCU "Environment" "Path"
  StrCpy $R3 "" ; Initialize new path
  StrCpy $R5 0   ; Initialize found flag

  loop:
    ; Grab the next entry from the path string
    StrCpy $R4 $R2 -1
    StrCmp $R4 "" get_last_entry
    StrCpy $R2 $R2 -1
    StrCmp $R4 ";" loop ; Found a separator, process the next entry
    Goto loop

  get_last_entry:
    ; Process the entry
    ${If} $R4 != ""
      ${If} $R4 == $R1
        StrCpy $R5 1 ; Directory found in PATH
        ${If} $R0 == "remove"
          StrCpy $R4 "" ; Clear the variable so it's not added to the new path
        ${EndIf}
      ${EndIf}
      ${If} $R4 != ""
        ${If} $R3 == ""
          StrCpy $R3 $R4
        ${Else}
          StrCpy $R3 "$R4;$R3"
        ${EndIf}
      ${EndIf}
    ${EndIf}
    StrCmp $R2 "" done
    Goto loop
  
  done:
    ; If we were adding and the path was NOT found, add it now.
    ${If} $R0 == "add"
    ${AndIf} $R5 == 0
      ${If} $R3 == ""
        StrCpy $R3 "$R1"
      ${Else}
        StrCpy $R3 "$R3;$R1"
      ${EndIf}
    ${EndIf}

  WriteRegExpandStr HKCU "Environment" "Path" "$R3"
  SendMessage ${HWND_BROADCAST} ${WM_SETTINGCHANGE} 0 "STR:Environment" /TIMEOUT=5000

  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Pop $R0
  Pop $R1
FunctionEnd
