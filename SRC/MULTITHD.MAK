#=============================================================
#
#	MULTITHD.MAK - Makefile for project D:\sw\libs\pdl\multithd.prj
#		Created on 06/08/95 at 17:02
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +MULTITHD.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = D:\BCOS2_20\LIB
INCLUDEPATH = D:\BCOS2_20\INCLUDE


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
 release\reg\bcos2_15\multi\ti2.lib \
 multithd.obj

#=============================================================
#		Explicit Rules
#=============================================================
multithd.exe: multithd.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /Toe /ap /L$(LIBPATH) @&&|
D:\BCOS2_20\LIB\C02.OBJ+
multithd.obj
multithd,multithd
release\reg\bcos2_15\multi\ti2.lib+
D:\BCOS2_20\LIB\C2MT.LIB+
D:\BCOS2_20\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
multithd.obj: multithd.cfg multithd.cpp 

#=============================================================
#		Compiler Configuration File
#=============================================================
multithd.cfg: multithd.mak
  copy &&|
-R
-L$(LIBPATH)
-I$(INCLUDEPATH)
-vi-
-sm
-v
| multithd.cfg


