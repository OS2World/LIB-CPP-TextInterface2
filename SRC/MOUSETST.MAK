#=============================================================
#
# OS2_DEMO.MAK - Makefile for project D:\sw\libs\pdl\os2_mousetst.prj
#		Created on 05/17/95 at 18:43
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +OS2_DEMO.CFG
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
 release\reg\bcos2_15\single\ti2win.lib \
 mousetst.obj

#=============================================================
#		Explicit Rules
#=============================================================
mousetst.exe: mousetst.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /Toe /ap /L$(LIBPATH) @&&|
D:\BCOS2_20\LIB\C02.OBJ+
mousetst.obj
mousetst,mousetst
win.lib+
D:\BCOS2_20\LIB\C2.LIB+
D:\BCOS2_20\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
mousetst.obj: mousetst.cfg mousetst.cpp

#=============================================================
#		Compiler Configuration File
#=============================================================
mousetst.cfg: mousetst.mak
  copy &&|
-R
-L$(LIBPATH)
-I$(INCLUDEPATH)
-vi-
-v
| mousetst.cfg


