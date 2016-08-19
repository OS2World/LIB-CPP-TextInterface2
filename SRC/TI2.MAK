#=============================================================
#
#	TI2.MAK - Makefile for project D:\SW\LIBS\PDL\TI2.PRJ
#		Created on 06/26/95 at 07:57
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +TI2.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = D:\BCOS2\LIB
INCLUDEPATH = D:\BCOS2\INCLUDE


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
 string2.obj \
 stdlib2.obj \
 datetime.obj \
 internal.obj \
 form2.obj \
 form1.obj \
 menu.obj \
 timeslc.obj \
 mouse.obj \
 win2.obj \
 win.obj \
 globdata.obj

#=============================================================
#		Explicit Rules
#=============================================================
ti2.dll: ti2.cfg $(EXE_DEPENDENCIES)
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
ti2.dll,ti2
D:\BCOS2\LIB\C2MT.LIB+
D:\BCOS2\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
STRING2.obj: ti2.cfg ..\rtl2\STRING2.CPP 
	$(CC) -c ..\rtl2\STRING2.CPP

STDLIB2.obj: ti2.cfg ..\rtl2\STDLIB2.CPP 
	$(CC) -c ..\rtl2\STDLIB2.CPP

DATETIME.obj: ti2.cfg ..\rtl2\DATETIME.CPP 
	$(CC) -c ..\rtl2\DATETIME.CPP

internal.obj: ti2.cfg internal.cpp 

form2.obj: ti2.cfg form2.cpp 

form1.obj: ti2.cfg form1.cpp 

menu.obj: ti2.cfg menu.cpp 

timeslc.obj: ti2.cfg timeslc.cpp 

mouse.obj: ti2.cfg mouse.cpp 

win2.obj: ti2.cfg win2.cpp 

win.obj: ti2.cfg win.cpp 

globdata.obj: ti2.cfg globdata.cpp 

#=============================================================
#		Compiler Configuration File
#=============================================================
ti2.cfg: ti2.mak
  copy &&|
-R
-L$(LIBPATH)
-I$(INCLUDEPATH)
-sd
-vi-
-sm
-v
| ti2.cfg


