
README.TXT

NOTES ON MS-DOS 6
=================

This file provides important information not included in the
MICROSOFT MS-DOS 6 USER'S GUIDE or in MS-DOS Help.

This file is divided into the following major sections:

1. Setup
2. MemMaker and Memory Management
3. Windows
4. Hardware Compatibility with MS-DOS 6
5. Microsoft Programs
6. Third-Party Programs
7. DoubleSpace

If the subject you need information about doesn't appear in
this file, you might find it in one of the following text
files included with MS-DOS:

* NETWORKS.TXT, which describes how to update your network software
  for use with MS-DOS 6.

This file contains the following topics:

1. Notes on Setup
   1.1  You deleted files from the directory that Setup needs
	to install the optional programs.
   1.2  You need to install MS-DOS manually.
   1.3  How to set up the Icelandic keyboard
2. MemMaker and Memory Management
   2.1  Intel Expanded-Memory Driver (EMM.SYS)
   2.2  Running MemMaker on a Computer with PC-NFS
   2.3  You have a Super VGA display and want to conserve memory.
   2.4  Additional MemMaker Troubleshooting
   2.5  Using MemMaker with IBM LAN
   2.6  MemMaker and Adaptec SCSI devices
3. Windows
   3.1  The "Cannot run Windows in standard mode" message appears.
4. Hardware Compatibility with MS-DOS 6
   4.1  Hardcard
   4.2  Inboard 386/PC
   4.3  Olivetti and AT&T Systems with CGA Video Boards
5. Microsoft Programs
   5.1  Microsoft Anti-Virus
   5.2  Microsoft Backup
   5.3  Microsoft CD-ROM Extensions 
   5.4  Microsoft Defragmenter
6. Third-party Programs
   6.1  4DOS and NDOS
   6.2  Above Board 286 and Above Board Plus Installation
	Programs
   6.3  After Dark for DOS (AD-DOS)
   6.4  CodeView
   6.5  Colorado Tape Backup
   6.6  Fastback Plus
   6.7  Norton Desktop for Windows 2.0
   6.8  Norton Utilities
   6.9  PC Tools
   6.10 QEMM-386 (6.02 or earlier)
7. DoubleSpace
   7.1  Converting Your Disk-Compression Software to DoubleSpace
   7.2  DoubleSpace Setup indicates that your computer is running an
	incompatible disk-caching program.
   7.3  Your compressed drive runs out of disk space.
   7.4  Your uncompressed drive runs out of disk space.
   7.5  DoubleSpace did not compress all of your files because
	the drive ran out of disk space.
   7.6  Removing DoubleSpace from Your Computer
   7.7  Windows displays the message "The permanent swap file is corrupt." 
   7.8  EXTDISK.SYS displays a warning about drive letters.
   7.9  You need a special device driver to use your startup drive.
   7.10 Defragmenting Uncompressed Drives After Changing File Attributes
   7.11 Files DoubleSpace Cannot Compress
   7.12 Microsoft Defragmenter runs out of memory while you are 
	compressing a drive.
   7.13 DoubleSpace and PC-Vault
   7.14 DoubleSpace displayed the "Drive X is too fragmented to resize,"
	you followed the message's instructions, and they didn't work.
   7.15 Running DoubleSpace on a Windows NT Flexboot System
   7.16 Maximum size of a compressed drive
   7.17 DoubleSpace displays the message "A CVF is damaged
   7.18 DoubleSpace finishes installation, but you cannot access your
        Hardcard.

1. NOTES ON SETUP
================

1.1 You deleted files from the directory that Setup needs
    to install the optional programs.
---------------------------------------------------------
If you typed SETUP /E at the command prompt, Setup displayed
the "Setup did not find the files it needs in the directory
specified" message, and you think you deleted the files from
your hard disk, carry out the following procedure.

1. Reinstall MS-DOS 6.

   Follow the instructions on your screen. When Setup displays
   the "This version of MS-DOS 6 is designed for computers that
   do not yet have an operating system" message, choose to
   continue Setup.

2. After Setup is finished, insert Setup Disk 1 in drive A,
   and type A:SETUP /E at the command prompt.

1.2 You need to install MS-DOS manually.
----------------------------------------
If you need to install MS-DOS manually, carry out the following procedure:

1. Insert Setup Disk 1 in drive, and restart your computer.

2. After the first screen appears, quit Setup by pressing F3 twice.

3. Transfer your system files to your hard disk by typing SYS A: C:
   at the command prompt.

4. Create a directory on your hard disk that will contain your MS-DOS 6   
   files.

5. Copy all the decompressed files (files without an underscore in
   their extension) on Setup Disk 1 to your MS-DOS directory.

6. Use the EXPAND command to decompress the rest of the files on your Setup
   disks and copy them to your MS-DOS directory. You must expand the
   compressed files one at a time, and specify a new extension for each
   one. For example, to decompress the EGA.SY_  file and copy it to the 
   DOS directory, type the following at the command prompt:

   A:EXPAND EGA.SYS C:\DOS\EGA.SYS
   
   For more information about the EXPAND command and the location and
   extensions of specific files, see the PACKING.LST file on the Setup
   disks.
   
7. Create an AUTOEXEC.BAT and CONFIG.SYS file, and store them in the root 
   directory of drive C. For more information about creating these files, 
   see the chapter "Configuring Your System" in the MICROSOFT MS-DOS 6
   USER'S GUIDE.

8. Restart your computer.

1.3 How to set up the Icelandic keyboard 
----------------------------------------
To set up the Icelandic keyboard, carry out the following procedure:

1. Expand and copy the KEYBOARD.IC_, COUNTRY.IC_, and EGA.IC_ files
   from your Setup disks to the directory that contains your MS-DOS files. 
   For the location of the files on the disks and for instructions on how
   to use the EXPAND command, see the PACKING.LST file on Setup Disk 1.
   When you use the EXPAND command, name the files KEYBOARD.SYS, COUNTRY.SYS,
   and EGA.CPI

2. In your CONFIG.SYS file, add the two following lines. For example, if the 
   directory that contains your MS-DOS files is DOS, add the following 
   lines:

   COUNTRY=354,861,C:\DOS\COUNTRY.SYS
   DEVICEHIGH=C:\DOS\DISPLAY.SYS CON=(EGA,,1)

3. In your AUTOEXEC.BAT file, add the following line:

   MODE CON CP PREPARE=((861) C:\DOS\EGA.CPI)
   MODE CON CP SELECT=861
   KEYB IS,,C:\DOS\KEYBOARD.SYS

4. If you also need support for codepage 850, then add the following 
   to your CONFIG.SYS file:
   
   COUNTRY=354,861,C:\DOS\COUNTRY.SYS
   DEVICE=C:\DOS\DISPLAY.SYS CON=(EGA,,2)

   Add the following to your AUTOEXEC.BAT file:

   NLSFUNC
   MODE CON CP PREPARE=((861,850) C:\DOS\EGA.CPI)
   MODE CON CP SELECT=861
   KEYB IS,,C:\DOS\KEYBOARD.SYS
   
5. Restart your computer.

2. MEMMAKER AND MEMORY MANAGEMENT
=================================

2.1 Intel Expanded-Memory Driver (EMM.SYS)
------------------------------------------
If you use Intel's EMM.SYS driver with Aboveboard, use EMM.SYS 
version 4.0 revision E if you an ISA system, or EMM.SYS version 4.0 
revision D if you have an MCA or other system. Contact Intel for a 
free upgrade.

2.2 Running MemMaker on a Computer with PC-NFS
----------------------------------------------
If you use PC-NFS network software, carry out the following procedure
before you run MemMaker:

1. Open your MEMMAKER.INF file by using any text editor. This file is
   in the directory that contains your MS-DOS files.

2. Add the following line to the file:

   *NET

3. Save the file, and then run MemMaker.

2.3 You have a Super VGA display and want to conserve memory.
-------------------------------------------------------------
If you have an 80386 computer with extended memory and a Super
VGA display, you can use the MONOUMB.368 file, in conjunction 
with MemMaker, to conserve conventional memory for running programs.

To conserve memory if you have a Super VGA display:

1. Open your Windows SYSTEM.INI file, and find the [386Enh] section.

2. Add a DEVICE command for the MONOUMB.386 file, which is located
   in your MS-DOS directory. For example, if your MS-DOS files are in
   C:\DOS, add the following line to this section:

   DEVICE=C:\DOS\MONOUMB.386

3. Save the file, and restart your computer.
   
4. After your computer starts, run MemMaker by typing MEMMAKER at the 
   command prompt. 

5. Choose Custom Setup. On the Advanced Options screen, answer Yes to 
   the question "Use monochrome region (B000-B7FF) for running programs?"
   
   Follow the instructions on your screen.

2.4 Additional MemMaker Troubleshooting Information
---------------------------------------------------
If you tried the troubleshooting procedures in the chapter "Making
More Memory Available," and you were referred to this file,
determine if one of the following conditions applies to your 
computer:

o If your computer uses an Adaptec controller card, you might not be
  able to use EMM386 unless you have the ADAPTEC.SYS device driver.
  To obtain the device driver, contact Adaptec or your user-group
  bulletin board (BBS) on the Association of PC User User Groups
  (APCUG) network.

o If your computer uses an SCSI (small computer system interface)
  hard disk or other device, or an EDSI (enhanced system device
  interface), or an MCI (Micro Channel Architecture) device, you
  might need to use double-buffering with EMM386.EXE. For more
  information about adding double-buffering, restart your computer
  with EMM386.EXE, and then type HELP DOUBLE-BUFFERING at the 
  command prompt.

2.5 Using MemMaker with IBM LAN
-------------------------------
Before you run MemMaker, you might need to make adjustments to the 
parameters specified for your IBM LAN support driver, DXMC0MOD.SYS. In 
your CONFIG.SYS file, make sure that the DEVICE command that loads 
DXMC0MOD.SYS includes one of the following before you run MemMaker:

o The first three parameters, as in the following example:

  DEVICE=C:\DXMC0MOD.SYS 400000000001,D800,1

o No parameters, as in the following example:
 
  DEVICE=C:\DXMC0MOD.SYS

o Some parameters and enough commas to fill in the first three 
  parameters, as in the following example:

  DEVICE=C:\DXMC0MOD.SYS 400000000001,,

2.6 MemMaker and Adaptec SCSI devices
-------------------------------------
MemMaker is compatible with Adaptec SCSI devices. However, if you use
the ASPI4DOS.SYS driver to support your SCSI device, MemMaker by default
will exclude this driver from the optimization process. If you don't
use the SCSI device as your startup or boot drive, you can include the
ASPI4DOS.SYS driver in the optimization process. To do so, remove the
*ASPI4DOS entry in the MEMMAKER.INF file, and then run MemMaker.

3. WINDOWS
==========

3.1 The "Cannot run Windows in standard mode" message appears.
--------------------------------------------------------------
If you have a DEVICE=EMM386.EXE command in your CONFIG.SYS file
that includes the RAM or NOEMS switch, you cannot run
Windows 3.0 in standard mode. Either remove the RAM or NOEMS
switch and add the AUTO or OFF switch, or run Windows in real or 
enhanced mode.

NOTE  Using EMM386 may reduce the amount of extended memory
available to Windows and cause Windows to start in
standard mode. To prevent this, reduce the amount of
extended memory that other device drivers use. For example,
if you are using SMARTDrive, try setting the MinCacheSize
parameter to 0.

4. HARDWARE COMPATIBILITY WITH MS-DOS
=====================================

4.1 Hardcard
------------
To use DoubleSpace on a system that uses its Hardcard drive as its
startup drive, see section 7.9. For additional Hardcard
information, see the following:

a) Hardcard II

   If you can't use Plus Development Hardcard II or Hardcard
   II XL when running EMM386.EXE, specify the exclude (x=)
   switch to prevent EMM386 from conflicting with the card's
   BIOS address.

   To configure EMM386, run MemMaker.

b) Hardcard 40 or Passport

   If you are using Hardcard 40 or a Passport removable
   disk, and you have a DEVICE command in your CONFIG.SYS file 
   for PLUSDRV.SYS, disable or remove the DEVICE command.
   Then run MS-DOS 6 Setup. After Setup is complete, reenable or 
   restore the DEVICE command for PLUSDRV.SYS. Make it the last line 
   in the file.

c) If you are unable to access your compressed drive after installing
   DoubleSpace on your Hardcard, try the following:

   1) Add a DBLSPACE /MOUNT command to your AUTOEXEC.BAT file to mount
      the compressed volume file on the Hardcard every time you start
      your computer.

   2) Or, ensure that there is at least one device driver (for example,
      ANSI.SYS) loaded in your CONFIG.SYS file AFTER the ATDOSXL.SYS
      driver but BEFORE the DBLSPACE.SYS driver.

4.2 Inboard 386/PC
-------------------
The Inboard 386/PC is incompatible with MemMaker and the EMM386
memory manager that comes with MS-DOS 6.

4.3 Olivetti and AT&T Systems with CGA Video Boards
---------------------------------------------------
An Olivetti or AT&T system with a CGA video board might not scroll
correctly. Add the /S switch to the DEVICE=ANSI.SYS command
in your CONFIG.SYS file.

5. MICROSOFT PROGRAMS
=====================

5.1 Microsoft Anti-Virus
------------------------
Before cleaning a program file, make sure you have a backup copy of it.
If you clean a program file, and the program is corrupted, reinstall the
program. If Anti-Virus again detects a virus, there is a chance the 
detection is in error; contact your software vendor to determine if an
updated version of the program is available.

5.2 Microsoft Backup
--------------------
The topics contained in this section are "Running Microsoft Backup
for MS-DOS under Windows," "Backup for Windows stops running at the end 
of the compatibility test," and "Running Microsoft with TI4000 and
Gateway NOMAD computers."

Running Microsoft Backup for MS-DOS under Windows
-------------------------------------------------
You should not run Backup for MS-DOS while Windows is running. However, 
if you do, and if your system is not configured correctly, Backup
might display the following symptoms:

o Backup appears to stop running--for example, the clock runs, but
  there is no disk activity.

o Backup reports problems while testing the second floppy disk during
  the compatibility test. For example, it might display a "Diskette
  not properly seated" message or ask for Disk 1 repeatedly.

o Windows displays a message that the DMA buffer size needs to be 
  increased.

o Backup quits unexpectedly.

You can correctly configure your system by carrying out the following
procedure.

1. Open your SYSTEM.INI file, and add a DMABufferSize=32 line to the
   [386Enh] section.

2. When you run Backup, make sure that you specify with the Settings
   command from the Control menu that Backup run with the Exclusive
   and Full Screen options.

Backup for Windows stops running at the end of the compatibility test
---------------------------------------------------------------------
If Backup for Windows stops running at the end of the
Compatibility Test, you might be loading an incompatible third-party
backup driver in your SYSTEM.INI file. Carry out the following
procedure.

NOTE  This procedure disables your third-party backup program.

1. Open your SYSTEM.INI file, and locate the [386Enh] section.

2. Determine whether any of the following lines are included in this
   section:

   DEVICE=FASTBACK.386
   DEVICE=VFD.386
   DEVICE=CPBVXD.386
   DEVICE=VIRWT.386

3. If you find any of these lines, add a semicolon (;) to the front
   of the line.

4. Save the file, restart Windows, and run Backup for Windows again.

Running Microsoft Backup with TI4000 and Gateway NOMAD computers
----------------------------------------------------------------
To avoid a conflict between the Turbo feature and Microsoft Backup
for Windows or MS-DOS, add a /L0 switch to the DEVICE command in
your CONFIG.SYS that loads the BATTERY.PRO file. Or, before you
run Microsoft Backup, type SETPOWER /L0 at the command prompt.

5.3 Microsoft CD-ROM Extensions
-------------------------------
Use the MSCDEX.EXE file that comes with MS-DOS 6.

If you load MSCDEX.EXE with a batch file or program,
make sure it loads the MSCDEX.EXE file that comes with 
MS-DOS 6. The MS-DOS Setup program copies this file
to the directory that contains your MS-DOS files.

5.4 Microsoft Defragmenter
--------------------------
If you carried out the procedures in "An MS-DOS program displays
an out-of-memory message" in the chapter "Diagnosing and Solving
Problems" in the MICROSOFT MS-DOS 6 USER'S GUIDE, and still receive
an out-of-memory message when you run the Defragmenter, there might
be too many files on your hard disk for the Defragmenter to organize. 
For the Defragmenter to work correctly, you might need to
delete some files or move them to a floppy disk or a network drive.


6. THIRD-PARTY PROGRAMS
=======================

6.1 4DOS and NDOS
-----------------
4DOS (prior to 4.02) and NDOS (prior to Norton Utilities 7.0) are
compatible with MS-DOS 6.  However, to use some of the new features
in MS-DOS 6 (such as MemMaker, the LOADHIGH /L switch, DIR compression
switches, and so on), contact JP Software for an update to 4DOS 4.02 or
later, or Symantec for an update to Norton Utilities 7.0 or later.

If you use multiple configurations, 4DOS or NDOS will not automatically
run your AUTOEXEC.BAT file unless you include a /P on the SHELL line in
your CONFIG.SYS file.

To use MS-DOS 6 Help instead of 4DOS or NDOS Help, start it by using
COMMAND /C HELP, or define a 4DOS or NDOS alias to run HELP.COM from
your MS-DOS 6 directory.

6.2 Above Board 286 and Above Board Plus Installation
    Programs
-----------------------------------------------------
Do not use an Above Board installation program dated May 1989
or earlier until you disable programs that use extended memory,
such as SMARTDrive or RAMDrive. You might lose data if you leave
these programs enabled. After you have installed Above Board,
you can reenable these programs.

6.3 After Dark for DOS (AD-DOS)
-------------------------------
If you run After Dark for DOS (AD-DOS) at the same time you 
run programs that use DPMI, you might have problems with 
MS-DOS--for example, your computer might stop running or you
might receive "packed-file corrupt" messages. Examples of programs
that use DPMI are Microsoft MASM61, C7, and FoxPro25.

6.4 CodeView
-----------
CAUTION  Using versions 3.0 to 3.13 of the CodeView CV.EXE
file may cause data loss if your system has an 80386 memory
manager (such as EMM386.EXE) and device drivers or programs
that use extended memory. To determine which version you
have, type CV.EXE at the command prompt.

6.5 Colorado Tape Backup
------------------------
If you receive a message that you have two versions of the
VFINTD.386 file loaded, you probably need to edit your
SYSTEM.INI file. To do so, carry out the following procedure:

1. Open your SYSTEM.INI file and locate the [386Enh] section.
   You should see two lines similar to the following:

   DEVICE=C:\TAPE\CMSDTAPE.386
   DEVICE=C:\DOS\VFINTD.386

2. If you plan to use your Colorado Tape Backup program, add a semi-
   colon (;) before the DEVICE command for the VFINTD.386 file. If you
   plan to use Microsoft Backup, add a semi-colon before the DEVICE
   command for the CMSDTAPE.386 file.

3. Save the file, and restart Windows.

6.6 Fastback Plus
-----------------
If you have a version of Fifth Generation Systems Fastback Pluse earlier
than 3.0, use the LOADFIX command before running Fastback Plus or the 
Fastback Plus installation program to ensure that you don't lose data. To 
do so, type the following at the command prompt:

LOADFIX FB.EXE

or

LOADFIX FBINSTAL.EXE

6.7 Norton Desktop for Windows 2.0
----------------------------------
Setup adds a second Tools menu which contains Backup and
Antivirus commands (if you installed these Windows programs),
as well as a DoubleSpace Info command.

6.8 Norton Utilities
--------------------
The "Clear Space" option of Norton SpeedDisk (prior to version 7.0)
is incompatible with DoubleSpace drives. For an updated version of 
Norton SpeedDisk, contact your software vendor.

Do not use the WipeInfo utility on DoubleSpace drives. It can cause 
lost clusters. If you have already used this option, use the
DBLSPACE /CHKDSK /F command to fix the lost clusters.

6.9 PC Tools
-------------
As reported by DoubleSpace CHKDSK, the DISKFIX /SCAN option
in PC Tools can cause lost clusters on DoubleSpace drives. If you
have already used this option, use the DBLSPACE /CHKDSK /F command 
to fix the lost clusters.

The FORMAT command of PC Tools version 7.1 conflicts with
MS-DOS 6. Use the FORMAT command that comes with MS-DOS 6.

The COMPRESS command of PC Tools versions 6.0 and 5.5  
is incompatible with DoubleSpace.

6.10 QEMM-386 (6.02 or earlier)
-------------------------------
Quarterdeck's QEMM memory manager is compatible with MS-DOS 6.
Quarterdeck has a Technical Note #166, MSDOS6.TEC, which discusses
how to get the most out of running their products with MS-DOS 6. This
technote can be obtained from their BBS, Compuserve (GO QUARTERDECK,
Library 2), BIX (JOIN DESQVIEW), and their Q/FAX automated fax-back
service.

7. DOUBLESPACE
==============

7.1  Converting Your Disk-Compression Software to DoubleSpace
-------------------------------------------------------------
To convert your disk-compression software, carry out the
following procedure.

NOTE If your computer uses XtraDrive disk compression, use its
uninstallation program to remove the compression, and then
install DoubleSpace.

1. Install MS-DOS 6 if you haven't already done so.

2. Use Microsoft Backup for MS-DOS to back up the files on your
   hard disk. If you didn't install Backup for MS-DOS during Setup, 
   see the chapter "Getting Started" in the MICROSOFT MS-DOS 6 USER'S
   GUIDE for instructions on installing it.

3. If your Setup disks are compatible with drive A, insert
   Setup Disk 1 in drive A, and restart your computer. After 
   Setup displays the first screen, quit Setup by pressing F3 twice.

   If your Setup disks are not compatible with drive A,
   create a startup floppy disk for drive A. To do this, insert 
   Setup Disk 1 in drive B, and a blank floppy disk in drive A.
   Then type B:SETUP /F at the command prompt.
   
   When prompted, choose to install MS-DOS on the floppy disk
   in drive A. After Setup is finished, leave the disk in drive A,
   and restart your computer. 

4. Use the FORMAT command to format the drive that contains the
   file that contains all of your compressed files. If you don't know
   where this file is located, see your disk-compression documentation.

   If you are reformatting drive C, include the /S switch to transfer
   system files to it.

5. If you formatted drive C, make sure Setup Disk 1 is in drive A
   or B, and type A:SETUP or B:SETUP at the command prompt.

    Follow the instructions on your screen.

6. After Setup is complete, install DoubleSpace by typing DBLSPACE
   at the command prompt. Follow the instructions on your screen.

7. Use Backup for MS-DOS to restore the files you backed up.

   NOTE  When you run Microsoft Backup, you will have to configure it
   again. Also, you will need to retrieve the catalog file from your
   backup floppy disks. To do so, choose the Catalog button in the
   Restore dialog box.

7.2 DoubleSpace Setup indicates that your computer is running an
    incompatible disk-caching program.
----------------------------------------------------------------
If DoubleSpace Setup displays a message indicating your computer
is running an incompatible disk-caching program, open your CONFIG.SYS
or AUTOEXEC.BAT file, and delete the DEVICE command that loads your 
disk-caching program. In your AUTOEXEC.BAT file, add a line for the MS-DOS 
6 SMARTDRV program. For example, if your MS-DOS files are in a directory
named DOS, add the following line:

   C:\DOS\SMARTDRV.EXE

Quit your text editor, and restart your computer. Run DoubleSpace again.

7.3 Your compressed drive runs out of disk space.
-------------------------------------------------
If your compressed drive runs out of free disk space, you can
use the following techniques to free some space on the drive:

o Specify a higher estimated compression ratio for that drive.
o Enlarge that drive.
o Carry out the DBLSPACE /DEFRAG /F and DBLSPACE /DEFRAG commands
  on that drive.

The rest of this section explains each technique.

Specifying a Higher Estimated Compression Ratio
-----------------------------------------------
If a compressed drive's estimated compression ratio is too small,
MS-DOS might report that there is no more free space on that drive,
when some space is in fact available. (Raising the estimated compression
ratio is most effective if the files you are trying to store are highly
compressible. It will have no effect if the drive really is out of space.)

To specify a larger estimated compression ratio:

1. Start the DoubleSpace program by typing DBLSPACE at the command
   prompt.

2. Select the compressed drive you want to change, and then choose
   the Change Ratio command from the Drive menu.

   The Change Compression Ratio dialog box appears.

3. Specify a higher compression ratio, and then choose OK. For more
   information, choose the Help button.

You can also change the estimated compression ratio from the command prompt.
For more information, type HELP DBLSPACE /RATIO at the command prompt.

Enlarging a Compressed Drive
----------------------------
You can enlarge a compressed drive to make more space available on it.
Enlarging a compressed drive uses free space on the uncompressed (host) 
drive.

To enlarge the compressed drive:

1. Start the DoubleSpace program by typing DBLSPACE at the command prompt.

2. Select the compressed drive you want to enlarge, and then choose
   the Change Size command from the Drive menu.

   The Change Size dialog box appears. The New Free Space line shows how 
   much free space the compressed and uncompressed drives will have if you 
   choose OK.

3. Specify a smaller number for New Free Space on the uncompressed
   drive. Notice that as you change this number, DoubleSpace adjusts
   the New Free Space amount for the compressed drive. When the New Free
   Space amount for both drives is what you want, choose OK.

   DoubleSpace enlarges the compressed drive.

Carrying Out the DBLSPACE /DEFRAG /F and DBLSPACE /DEFRAG Commands
on your Compressed Drive
------------------------------------------------------------------
You can sometimes free additional space on a compressed drive by
having DoubleSpace defragment that drive twice, once with and once
without the /F switch. 

NOTE  You might want to carry out the following procedure overnight, since
defragmenting a large or badly fragmented drive can take a long time.
(To carry out the entire procedure overnight, create a batch file that 
contains both the commands in the procedure.)

To free space by defragmenting the drive twice:

1. Make the compressed drive your current drive.

2. Type DBLSPACE /DEFRAG /F at the command prompt.

   DoubleSpace defragments the drive and fixes any errors it finds.

3. When DoubleSpace finishes, type DBLSPACE /DEFRAG at the command prompt.
   
   DoubleSpace consolidates the free space on the drive so there is as 
   much free space as possible.

7.4 Your uncompressed drive runs out of disk space
--------------------------------------------------
If your uncompressed (host) drive runs out of free disk space, you
can enlarge it by reducing the size of any compressed drives that are
stored on that uncompressed drive. Of course, this will reduce the
amount of free space on the compressed drive(s).

To enlarge the uncompressed (host) drive:

1. Start the DoubleSpace program by typing DBLSPACE at the command prompt.

2. Select the compressed drive whose size you want to reduce, and then
   choose the Change Size command from the Drive menu. (Select
   a compressed drive that is stored on the uncompressed drive that's
   out of space. To find out which compressed drives are stored
   on that uncompressed drive, type DBLSPACE /LIST at the command prompt.)

   The Change Size dialog box appears. The New Free Space line shows 
   how much free space the compressed and uncompressed drives will have 
   if you choose OK.

3. Specify a larger number for the New Free Space on the uncompressed
   drive. Notice that as you change this number, DoubleSpace adjusts
   the New Free Space amount for the compressed drive. When the New Free
   Space amount for both drives is what you want, choose OK.

   DoubleSpace reduces the size of the compressed drive, which makes
   more free space available on the corresponding uncompressed drive.

7.5 DoubleSpace did not compress all of your files because
    the drive ran out of disk space.
----------------------------------------------------------
If DoubleSpace indicates that it could not compress some
of your files because there was not enough disk space, carry
out the following procedure.

1. To determine which drive is your uncompressed drive, type
   DBLSPACE /LIST at the command prompt.

2. Using Microsoft Backup, back up to floppy disks the files
   on the uncompressed drive that were not compressed.

3. Delete the files on the uncompressed drive that were not compressed.

4. Type DBLSPACE at the command prompt.

5. From the Drive menu, choose Change Size.

6. To increase the size of your compressed drive, decrease the size
   of your uncompressed drive, and choose OK.

7. From the Drive menu, choose Exit, and use Backup to restore
   the files you backed up to your compressed drive. If you run out of
   space again, repeat steps 5 through 7 until the compressed drive
   is large enough.

7.6 Removing DoubleSpace from your computer
-------------------------------------------
There is no method for automatically removing DoubleSpace. There are 
two ways to remove it manually:
 
  * Back up the files on all your compressed drives, remove 
    DoubleSpace, and then restore the backed-up files onto
    your uncompressed drive. (Note that all the files currently 
    on your compressed drive(s) might not fit on your hard disk 
    after you remove DoubleSpace.)

  * Move as many files as possible from your compressed drive 
    to your uncompressed drive, reduce the size of your
    compressed drive to free space on the uncompressed drive, 
    and keep moving files and shrinking the compressed drive until 
    no more files remain on the compressed drive. Then, remove 
    DoubleSpace.

NOTE: The drive letter of your uncompressed drive may change after you 
      remove DoubleSpace. If it does, any files or programs configured 
      for use on the uncompressed drive (for example, your Windows permanent 
      swap file) will need to be reconfigured.

The procedures in this section explain each method of removing DoubleSpace.

To remove DoubleSpace by using the back-up-and-restore method:

1. Delete any unnecessary files from your compressed drives.

2. Back up the files on all compressed drives. If your backup
   program is located on a compressed drive, make sure you copy
   the backup program files to another uncompressed drive or to a 
   floppy disk.

   If you are using Microsoft Backup for MS-DOS, the program files
   you need to copy are the following:

   MSBACKUP.EXE   MSBACKUP.OVL   MSBACKUP.INI   MSBACKDB.OVL
   MSBACKDR.OVL   MSBACKFB.OVL   MSBACKFR.OVL   MSBCONFG.OVL
   DEFAULT.SET    MSBACKUP.LOG   MSBACKUP.RST

3. To determine which drive is your uncompressed drive, type
   DBLSPACE /LIST at the command prompt. The uncompressed drive
   is listed under the CVF Filename column. For example, if
   H:\DBLSPACE.000 is the CVF Filename associated with drive C,
   drive H is the uncompressed drive.

4. If you are removing DoubleSpace from your startup drive, copy the 
   COMMAND.COM file from your compressed drive to the root directory of 
   your uncompressed drive.

5. Make your uncompressed drive the current drive. For example,
   if drive H is your uncompressed drive, type H: at the command prompt. 
   To change to the root directory, type CD\ at the command prompt. If 
   you want to delete all of your DoubleSpace drives, type the following 
   at the command prompt:

   DELTREE DBLSPACE.*
   
   If you want to delete just one of your DoubleSpace drives, use
   the DELTREE command to delete the CVF for the drive. (The
   DBLSPACE /LIST command also shows the CVF names for your drives.)
   For example, if the CVF is DBLSPACE.000, type the following at
   the command prompt:

   DELTREE DBLSPACE.000
   
6. Restart your computer.

7. Restore your backed-up files. If your Backup program files are on a 
   floppy disk, copy them to the hard disk first. Then run the Backup
   program from your hard disk.

   NOTE  You might need to retrieve your catalog file from your backup 
   floppy disks. To do so, choose the Catalog button in the Restore dialog 
   box.


To remove DoubleSpace by using the move-and-resize method:

1. Delete any unnecessary files from your compressed drives. 

2. To determine which drive is your uncompressed (host) drive, type
   DBLSPACE /LIST at the command prompt. The uncompressed drive
   is listed under the CVF Filename column. For example, if
   H:\DBLSPACE.000 is the CVF Filename associated with drive C,
   drive H is the uncompressed drive.

3. Delete any unnecessary files from the uncompressed drive, including
   your Windows permanent swap file (if any). 

4. Change to your compressed drive, and then type DLBSPACE /SIZE at the 
   command prompt. DoubleSpace will reduce the drive's size as much as
   possible, which will free some space on the uncompressed drive. (If 
   you have more than one compressed drive, carry out this step for each 
   one.)

   If DoubleSpace cannot reduce a compressed drive's size because the
   drive is too fragmented, run Microsoft Defragmenter by typing DEFRAG 
   at the command prompt. When Defragmenter completes, type 
   DBLSPACE /SIZE at the command prompt.

5. Use the MOVE command to move files from the compressed drive to the 
   uncompressed drive until only .5 MB of free space remains on the 
   uncompressed drive.

   Repeat steps 4 and 5 until your compressed drives do not contain any
   files you want to keep.
   
6. If you are removing DoubleSpace from your startup drive, copy the 
   COMMAND.COM file from your compressed drive to the root directory 
   of your uncompressed drive.

7. Make your uncompressed drive the current drive. For example,
   if drive H is your uncompressed drive, type H: at the command prompt. 
   To change to the root directory, type CD\ at the command prompt. If 
   you want to delete all of your DoubleSpace drives, type the following 
   at the command prompt:

   DELTREE DBLSPACE.*
   
   If you want to delete just one of your DoubleSpace drives, use
   the DELTREE command to delete the CVF for the drive. (The
   DBLSPACE /LIST command also shows the CVF names for your drives.)
   For example, if the CVF is DBLSPACE.000, type the following at
   the command prompt:

   DELTREE DBLSPACE.000
   
8. Remove all references to DBLSPACE from your CONFIG.SYS and AUTOEXEC.BAT 
   files.

9. Restart your computer.

7.7 Windows displays the message "The permanent swap file is corrupt."
----------------------------------------------------------------------
If you use a Windows permanent swap file, it must be located on an
uncompressed drive. If your permanent swap file is on a compressed 
drive, Windows displays the message "The permanent swap file is corrupt" 
when it starts.

When you install DoubleSpace, the DoubleSpace Setup program checks for 
the existence of a Windows permanent swap file. If it finds one, 
DoubleSpace Setup moves the swap file to your uncompressed drive.
However, if you install Windows after installing DoubleSpace, or if you use
Control Panel to change the location of your permanent swap file, your
swap file might end up on a compressed drive. (When you specify a drive 
for your permanent swap file, Windows allows you to choose a compressed 
drive.)

To move your permanent swap file to an uncompressed drive:

1. Start Windows. 

2. At the "Permanent swap file is corrupt" screen, type Y in response
   to the question "Do you want to delete this swap file?", and
   then press ENTER.

3. Open Control Panel, and then double-click the 386 Enhanced icon.

4. Choose the Virtual Memory button. Windows displays a dialog box stating 
   that a corrupt swap file was found and asks if you want to set the 
   file's length to zero.

5. Choose the Yes button. Windows displays another Virtual Memory dialog box.

6. Choose the Change button. Windows displays swap-file settings.

7. In the Drive list box, select a drive that is not compressed. In the
   Type list box, select "Permanent."

   If your uncompressed drive does not have enough free space to create a
   permanent swap file, create a temporary swap file on either your
   compressed or uncompressed drives. (For information about freeing
   space on your uncompressed drive, see section 7.4.)

   When you have finished specifying swap-file settings, choose OK twice,
   and follow the instructions on your screen.

7.8 EXTDISK.SYS displays a warning about drive letters.
-------------------------------------------------------
If you are using DoubleSpace on a Compaq computer, and your CONFIG.SYS file 
loads the EXTDISK.SYS device driver, EXTDISK.SYS displays the following 
message when it loads:

WARNING: EXTDISK.SYS is not the first device driver to assign drive
	 letters.  Physical hard drive letters will not be contiguous.

The EXTDISK.SYS driver still works properly. It displays this message
because it expects to be the first module to assign drive letters,
but because DBLSPACE.BIN loads before the CONFIG.SYS file and assigns
some drive letters, EXTDISK.SYS is no longer first. (EXTDISK.SYS
displays the message regardless of when the DBLSPACE.SYS device
driver is loaded in the CONFIG.SYS file.)

7.9 You need a special device driver to use your startup drive
--------------------------------------------------------------
If your startup hard disk drive requires a device driver in your CONFIG.SYS
file, do not compress that drive. If you do, your computer will not 
start properly, since DoubleSpace will be unable to access your startup 
drive. (This is because MS-DOS loads DBLSPACE.BIN, the portion of MS-DOS 
that accesses compressed drives, before starting any of the device drivers 
in your CONFIG.SYS file.)

To install DoubleSpace on a computer with a startup drive that requires a
special device driver, use DoubleSpace Setup to compress a drive other than
your startup drive, or use DoubleSpace Setup to create a new compressed
drive using free space on any existing drive.

7.10 Defragmenting Uncompressed Drives After Changing File Attributes
---------------------------------------------------------------------
You can safely defragment both your compressed or uncompressed drives,
using the Microsoft Defragmenter or another defragmentation program, as long
as you do not change the attributes of your compressed volume files.
If you change the attributes of a compressed volume file, and then
defragment that uncompressed drive, you might lose data.

7.11 Files DoubleSpace Cannot Compress
--------------------------------------
Some files (such as .ZIP files) are already compressed. DoubleSpace
might not be able to compress such files any further.

Encrypted data files, such as the Microsoft Mail 3.0 .MMF file, are
not compressible and will be stored in uncompressed form, even if
you store such files on a compressed drive.

You might want to store uncompressible files on an uncompressed drive
rather than on a compressed drive. Doing so can sometimes improve
your system's speed.

7.12 Microsoft Defragmenter runs out of memory while you are compressing 
     a drive.
------------------------------------------------------------------------
If the Defragmenter runs out of memory while you are compressing a drive,
quit DoubleSpace, and then carry out the procedures in "An MS-DOS-based
program displays an out-of-memory message" in the chapter "Diagnosing
and Solving Problems" in the MICROSOFT MS-DOS 6 USER'S GUIDE.

If the Defragmenter still runs out of memory after you have tried
these procedures, there might be too many files on your hard disk 
for the Defragmenter to organize. For the program to work correctly, 
you might need to delete some files or move them to a floppy disk or a 
network drive.

7.13 DoubleSpace and PC-Vault
-----------------------------
The PC-Vault and PC-Vault Plus Hard Disk Protection Systems from
Johnson Computer Systems are compatible with DoubleSpace, except
for the Maximum Floppy Boot Protection feature in versions prior
to 4.6. To avoid losing data, do not use Maximum Floppy Boot Protection 
in versions earlier than 4.6 with DoubleSpace. If your version of PC-Vault 
or PC-Vault Plus is earlier than 4.6, contact Johnson Computer Systems.

7.14 DoubleSpace displayed the "Drive X is too fragmented to resize,"
     you followed the message's instructions, and they didn't work.
---------------------------------------------------------------------
If you followed the instructions, and you are still unable to resize
the drive, you might have system files that are preventing
Microsoft Defragmenter from reorganizing your files. Carry out the 
following procedure:
 
1. Change to the drive that DoubleSpace identified in its message.    

2. To find the system files, type DIR /S /A:S|MORE at the
   command prompt.

3. For each filename that DIR displays, type ATTRIB -S FILENAME
   at the command prompt to remove its system attribute.
	      
4. Run DEFRAG /Q /H DRIVE: again.

5. For each file whose system attribute you removed, type 
   ATTRIB +S FILENAME at the command prompt to restore the attribute.

6. To change the size of your drive, run DoubleSpace again.

7.15 Running DoubleSpace on a Windows NT Flexboot System
--------------------------------------------------------
The BOOT.INI, BOOTSECT.DOS, NTLDR, and NTDETECT.COM files should not
be compressed. Add these files to the [SpecialFiles] section of the 
DBLSPACE.INF before you run DoubleSpace. After DoubleSpace Setup
is finished, copy all the files and subdirectories in your Windows NT 
directory on your compressed drive to a Windows NT directory on your 
uncompressed drive.

7.16 Maximum size of a compressed drive 
---------------------------------------
The maximum size that a DoubleSpace-compressed drive can be is
512 megabytes (MB). For example, if you have a disk drive that is 
300 MB, the drive will not be larger than 512 MB after DoubleSpace 
compresses it.

7.17  DoubleSpace displays the message "A CVF is damaged"
---------------------------------------------------------
If the message "A CVF is damaged" appears when you start your computer,
then DoubleSpace has detected problems with a compressed volume file (CVF).
The most common cause of this message is crosslinked files on the
compressed drive. DoubleSpace detects a crosslinked file if two files 
or directories are recorded in the DoubleSpace file allocation table 
as using the same disk space.

To fix this problem:

1. Change to the compressed drive and type DBLSPACE /CHKDSK at the command
   prompt.

   If DoubleSpace Chkdsk finds a crosslinked file, it reports the problem 
   and the name of the file.

2. To fix the cross-linked files, copy the specified files or directories
   elsewhere and delete the original copies of those files. Some of the 
   information in these files may be lost.

7.18  DoubleSpace finishes installation, but you cannot access your Hardcard.
-----------------------------------------------------------------------------
See section 4.1, part C.
