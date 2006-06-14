; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "MeshLab"
!define PRODUCT_VERSION "0.7"
!define PRODUCT_PUBLISHER "Paolo Cignoni VCG - ISTI - CNR"
!define PRODUCT_WEB_SITE "http://vcg.isti.cnr.it/~cignoni"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\meshlab.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "..\..\docs\gpl.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\meshlab.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "MeshLabSetup.exe"
InstallDir "$PROGRAMFILES\VCG\MeshLab"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show
Var qt_base
Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\meshlab\release\meshlab.exe"
  CreateDirectory "$SMPROGRAMS\MeshLab"
  CreateShortCut "$SMPROGRAMS\MeshLab\MeshLab.lnk" "$INSTDIR\meshlab.exe"
  CreateShortCut "$DESKTOP\MeshLab.lnk" "$INSTDIR\meshlab.exe"
  SetOutPath "$INSTDIR\shaders"
  File "..\meshlab\shaders\Cook-Torrance.frag"
  File "..\meshlab\shaders\Cook-Torrance.gdp"
  File "..\meshlab\shaders\Cook-Torrance.vert"
  File "..\meshlab\shaders\dimple.frag"
  File "..\meshlab\shaders\dimple.gdp"
  File "..\meshlab\shaders\dimple.vert"
  File "..\meshlab\shaders\electronic microscope.gdp"
  File "..\meshlab\shaders\envmap.frag"
  File "..\meshlab\shaders\envmap.gdp"
  File "..\meshlab\shaders\envmap.vert"
  File "..\meshlab\shaders\glass.frag"
  File "..\meshlab\shaders\glass.gdp"
  File "..\meshlab\shaders\glass.vert"
  File "..\meshlab\shaders\gooch.frag"
  File "..\meshlab\shaders\gooch.gdp"
  File "..\meshlab\shaders\gooch.vert"
  File "..\meshlab\shaders\Hatch.frag"
  File "..\meshlab\shaders\Hatch.gdp"
  File "..\meshlab\shaders\Hatch.vert"
  File "..\meshlab\shaders\lattice.frag"
  File "..\meshlab\shaders\lattice.gdp"
  File "..\meshlab\shaders\lattice.vert"
  File "..\meshlab\shaders\Oren-Nayar.frag"
  File "..\meshlab\shaders\Oren-Nayar.gdp"
  File "..\meshlab\shaders\Oren-Nayar.vert"
  File "..\meshlab\shaders\phong.frag"
  File "..\meshlab\shaders\phong.gdp"
  File "..\meshlab\shaders\phong.vert"
  File "..\meshlab\shaders\polkadot3d.frag"
  File "..\meshlab\shaders\polkadot3d.gdp"
  File "..\meshlab\shaders\polkadot3d.vert"
  File "..\meshlab\shaders\SEM.frag"
  File "..\meshlab\shaders\SEM.vert"
  File "..\meshlab\shaders\stripes2.frag"
  File "..\meshlab\shaders\stripes2.gdp"
  File "..\meshlab\shaders\stripes2.vert"
  File "..\meshlab\shaders\toon.frag"
  File "..\meshlab\shaders\toon.gdp"
  File "..\meshlab\shaders\toon.vert"
  File "..\meshlab\shaders\xray.frag"
  File "..\meshlab\shaders\xray.gdp"
  File "..\meshlab\shaders\xray.vert"
  SetOutPath "$INSTDIR\plugins"
  File "..\meshlab\plugins\meshio.dll"
  File "..\meshlab\plugins\meshselect.dll"
  File "..\meshlab\plugins\meshrender.dll"
  File "..\meshlab\plugins\meshfilter.dll"
  File "..\meshlab\plugins\meshedit.dll"
  File "..\meshlab\plugins\meshcolorize.dll"
  SetOutPath "$INSTDIR\textures"
  File "..\meshlab\textures\chrome.png"
  SetOutPath "$INSTDIR\samples"
  File "..\sample\texturedknot.ply"
  File "..\sample\texturedknot.obj"
  File "..\sample\texturedknot.mtl"
  File "..\sample\TextureDouble_A.png"
  File "..\sample\Laurana50k.ply"
  SetOutPath "$INSTDIR\plugins\imageformats"
  File "C:\Qt\4.1.2\plugins\imageformats\qjpeg1.dll"
  SetOutPath "$INSTDIR"
  File "C:\Qt\4.1.2\bin\QtCore4.dll"
  File "C:\Qt\4.1.2\bin\QtGui4.dll"
  File "C:\Qt\4.1.2\bin\QtOpenGL4.dll"
  File "C:\Qt\4.1.2\bin\QtXml4.dll"
  File "C:\Qt\4.1.2\bin\QtNetwork4.dll"
  File "C:\MinGW\bin\mingwm10.dll"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\MeshLab\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\MeshLab\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\meshlab.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\meshlab.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\xray.vert"
  Delete "$INSTDIR\xray.gdp"
  Delete "$INSTDIR\xray.frag"
  Delete "$INSTDIR\toon.vert"
  Delete "$INSTDIR\toon.gdp"
  Delete "$INSTDIR\toon.frag"
  Delete "$INSTDIR\stripes2.vert"
  Delete "$INSTDIR\stripes2.gdp"
  Delete "$INSTDIR\stripes2.frag"
  Delete "$INSTDIR\SEM.vert"
  Delete "$INSTDIR\SEM.frag"
  Delete "$INSTDIR\polkadot3d.vert"
  Delete "$INSTDIR\polkadot3d.gdp"
  Delete "$INSTDIR\polkadot3d.frag"
  Delete "$INSTDIR\phong.vert"
  Delete "$INSTDIR\phong.gdp"
  Delete "$INSTDIR\phong.frag"
  Delete "$INSTDIR\Oren-Nayar.vert"
  Delete "$INSTDIR\Oren-Nayar.gdp"
  Delete "$INSTDIR\Oren-Nayar.frag"
  Delete "$INSTDIR\LightworkDesign-license.txt"
  Delete "$INSTDIR\lattice.vert"
  Delete "$INSTDIR\lattice.gdp"
  Delete "$INSTDIR\lattice.frag"
  Delete "$INSTDIR\Hatch.vert"
  Delete "$INSTDIR\Hatch.gdp"
  Delete "$INSTDIR\Hatch.frag"
  Delete "$INSTDIR\gooch.vert"
  Delete "$INSTDIR\gooch.gdp"
  Delete "$INSTDIR\gooch.frag"
  Delete "$INSTDIR\glass.vert"
  Delete "$INSTDIR\glass.gdp"
  Delete "$INSTDIR\glass.frag"
  Delete "$INSTDIR\envmap.vert"
  Delete "$INSTDIR\envmap.gdp"
  Delete "$INSTDIR\envmap.frag"
  Delete "$INSTDIR\electronic microscope.gdp"
  Delete "$INSTDIR\dimple.vert"
  Delete "$INSTDIR\dimple.gdp"
  Delete "$INSTDIR\dimple.frag"
  Delete "$INSTDIR\CVS\Root"
  Delete "$INSTDIR\CVS\Repository"
  Delete "$INSTDIR\CVS\Entries.Old"
  Delete "$INSTDIR\CVS\Entries.Extra.Old"
  Delete "$INSTDIR\CVS\Entries.Extra"
  Delete "$INSTDIR\CVS\Entries"
  Delete "$INSTDIR\Cook-Torrance.vert"
  Delete "$INSTDIR\Cook-Torrance.gdp"
  Delete "$INSTDIR\Cook-Torrance.frag"
  Delete "$INSTDIR\3Dlabs-license.txt"
  Delete "$INSTDIR\Example.file"
  Delete "$INSTDIR\meshlab.exe"

  Delete "$SMPROGRAMS\MeshLab\Uninstall.lnk"
  Delete "$SMPROGRAMS\MeshLab\Website.lnk"
  Delete "$DESKTOP\MeshLab.lnk"
  Delete "$SMPROGRAMS\MeshLab\MeshLab.lnk"

  RMDir "$SMPROGRAMS\MeshLab"
  RMDir "$INSTDIR\CVS"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd