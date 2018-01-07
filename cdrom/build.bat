@ECHO OFF
DEL *.ISO
DEL *.IMG
DEL *.TOC
CLS
ECHO Building PS1DEV.IMG...
"../../../cdrom/buildcd.exe" -l -iPS1DEV.IMG PS1DEV.CTI
REM PAUSE
ECHO Converting PS1DEV.IMG to PS1DEV.ISO...
"../../../cdrom/stripiso.exe" S 2352 PS1DEV.IMG PS1DEV.ISO
ECHO.
ECHO The PS1DEV CD-ROM Image was Created Successfully!
"../../../cdrom/psxlicense.exe" /eu /i PS1DEV.ISO