#=============================================================
#
#	OS2_DEMO.MAK - Makefile for project D:\sw\libs\pdl\os2_demo.prj
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
 release\reg\bcos2_20\single\ti2win.lib \
 release\reg\bcos2_20\single\ti2menu.lib \
 release\reg\bcos2_20\single\ti2form.lib \
 demo.obj

#=============================================================
#		Explicit Rules
#=============================================================
os2_demo.exe: os2_demo.cfg $(EXE_DEPENDENCIES)
  $(TLINK) /v /Toe /ap /L$(LIBPATH) @&&|
D:\BCOS2_20\LIB\C02.OBJ+
demo.obj
os2_demo,os2_demo
release\reg\bcos2_20\single\ti2win.lib+
release\reg\bcos2_20\single\ti2menu.lib+
release\reg\bcos2_20\single\ti2form.lib+
D:\BCOS2_20\LIB\C2.LIB+
D:\BCOS2_20\LIB\OS2.LIB

|


#=============================================================
#		Individual File Dependencies
#=============================================================
demo.obj: os2_demo.cfg demo.cpp 

#=============================================================
#		Compiler Configuration File
#=============================================================
os2_demo.cfg: os2_demo.mak
  copy &&|
-R
-L$(LIBPATH)
-I$(INCLUDEPATH)
-vi-
-v
| os2_demo.cfg


