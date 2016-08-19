#=============================================================
#
# bcos2frm.MAK - Makefile for project D:\sw\libs\pdl\bcos2frm.prj
#   Created on 05/17/95 at 15:38
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
!else
!if $d(USERTL2)
COMPILE_DEFINES = _USERTL2_
TARGETPATH1 = RTL2
!else
COMPILE_DEFINES = _COMMERCIAL_
TARGETPATH1 = REG
!endif
!endif

#=============================================================
#   Translator Definitions
#=============================================================
CC = bcc -D$(COMPILE_DEFINES) +bcos2frm.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = D:\bcos2\LIB
INCLUDEPATH = D:\bcos2\INCLUDE

#=============================================================
#   Implicit Rules
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
#   List Macros
#=============================================================

FORM_LIB_DEPENDENCIES =  \
 form1.obj \
 form2.obj

#=============================================================
#   Explicit Rules
#=============================================================
RELEASE\$(TARGETPATH1)\bcos2_15\$(TARGETPATH2)\ti2form.lib: bcos2frm.cfg $(FORM_LIB_DEPENDENCIES)
#  - del RELEASE\$(TARGETPATH1)\bcos2\$(TARGETPATH2)\ti2form.lib
	$(TLIB) $<  @&&|
+form1.obj &
+form2.obj
|


#=============================================================
#   Individual File Dependencies
#=============================================================

FORM1.obj: bcos2frm.cfg FORM1.CPP
FORM2.obj: bcos2frm.cfg FORM2.CPP

#=============================================================
#   Compiler Configuration File
#=============================================================
bcos2frm.cfg: bcos2frm.mak
	copy &&|
-L$(LIBPATH)
-I$(INCLUDEPATH)
-v
!if $(MODEL) == m
-sm
!endif
| bcos2frm.cfg


