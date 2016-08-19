#=============================================================
#
#	TI2.MAK - Makefile for project D:\SW\LIBS\PDL\ti2.prj
#		Created on 06/09/95 at 08:47
#
#=============================================================

.AUTODEPEND

!ifndef MODEL
!Error MODEL must be specified
!endif

!if $d(MODEL)
!if $(MODEL)!=s && $(MODEL)!=m
!Error MODEL must be s or m
!endif
!if $(MODEL) == s
TARGETPATH2 = SINGLE
!endif
!if $(MODEL) == m
TARGETPATH2 = MULTI
!endif
!endif

!if $d(SHAREWARE)
  COMPILE_DEFINES = _SHAREWARE_
  TARGETPATH1 = SW
  DEFFILE = TI2SW.DEF
!else
  COMPILE_DEFINES = _COMMERCIAL_
  TARGETPATH1 = REG
  DEFFILE = TI2REG.DEF
!endif

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +BCOS2DLL.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = D:\bcos2\LIB
INCLUDEPATH = D:\bcos2\INCLUDE


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


EXE_DEPENDENCIES =  \
 form2.obj \
 form1.obj \
 menu.obj \
 timeslc.obj \
 mouse.obj \
 win2.obj \
 win.obj \
 globdata.obj \
 internal.obj \
 datetime.obj \
 string2.obj \
 stdlib2.obj \
 $(DEFFILE)

#=============================================================
#		Explicit Rules
#=============================================================
RELEASE\$(TARGETPATH1)\bcos2_15\$(TARGETPATH2)\DLL\ti2.dll: bcos2dll.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /Tod /ap /L$(LIBPATH) @&&|
D:\BCOS2\LIB\C02D.OBJ+
string2.obj+
stdlib2.obj+
datetime.obj+
internal.obj+
form2.obj+
form1.obj+
menu.obj+
timeslc.obj+
mouse.obj+
win2.obj+
win.obj+
globdata.obj
RELEASE\$(TARGETPATH1)\bcos2_15\$(TARGETPATH2)\DLL\ti2.dll,ti2
D:\BCOS2\LIB\C2MT.LIB+
D:\BCOS2\LIB\OS2.LIB,
$(DEFFILE)

|

#=============================================================
#		Individual File Dependencies
#=============================================================
form2.obj: bcos2dll.cfg form2.cpp

form1.obj: bcos2dll.cfg form1.cpp

menu.obj: bcos2dll.cfg menu.cpp

timeslc.obj: bcos2dll.cfg timeslc.cpp

mouse.obj: bcos2dll.cfg mouse.cpp

win2.obj: bcos2dll.cfg win2.cpp

win.obj: bcos2dll.cfg win.cpp

globdata.obj: bcos2dll.cfg globdata.cpp

internal.obj: bcos2dll.cfg internal.cpp

datetime.obj: bcos2dll.cfg \sw\libs\rtl2\datetime.cpp

string2.obj: bcos2dll.cfg \sw\libs\rtl2\string2.cpp

stdlib2.obj: bcos2dll.cfg \sw\libs\rtl2\stdlib2.cpp

#=============================================================
#		Compiler Configuration File
#=============================================================
bcos2dll.cfg: bcos2dll.mak
  copy &&|
-R
-L$(LIBPATH)
-I$(INCLUDEPATH)
-sd
-vi-
-sm
-v
| bcos2dll.cfg


