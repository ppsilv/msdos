@ECHO OFF
ECHO.
ECHO Boot Disk Maker for Super MS-DOS 7.10 Boot Disk
ECHO ===============================================
ECHO.
ECHO Note: If you are now in the WinNT/2K/XP+ console,
ECHO then it's recommanded to use WinImage or similar instead.
ECHO.
ECHO Insert a 1.44M floppy disk, and
PAUSE
IF NOT EXIST MSDOS710.IMG GOTO ERROR
IF NOT EXIST DI.EXE GOTO ERROR
ECHO.
ECHO Start to make the boot disk..
DI /R MSDOS710.IMG A:
ECHO.
ECHO Done.
GOTO END
:ERROR
ECHO Error: Can't find the disk image(r)!
:END
ECHO.
