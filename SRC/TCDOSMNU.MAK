.AUTODEPEND

!ifndef MODEL
!Error MODEL must be specified
!endif

!if $d(MODEL)
!if $(MODEL)!=m && $(MODEL)!=l && $(MODEL)!=h
!Error MODEL must be m, l, or h
!endif
!if $(MODEL) == m
TARGETPATH2 = MEDIUM
!endif
!if $(MODEL) == l
TARGETPATH2 = LARGE
!endif
!if $(MODEL) == h
TARGETPATH2 = HUGE
!endif
!endif

!if $d(SHAREWARE)
COMPILE_DEFINES = _SHAREWARE_
TARGETPATH1 = SW
!else
COMPILE_DEFINES = _COMMERCIAL_
TARGETPATH1 = REG
!endif

#   *Translator Definitions*
CC = tcc -D$(COMPILE_DEFINES) -m$(MODEL) +tcdosmnu.CFG
TASM = TASM
TLIB = tlib
TLINK = tlink
LIBPATH = D:\TC\LIB
INCLUDEPATH = D:\TC\INCLUDE

#   *Implicit Rules*
.c.obj:
	$(CC) -c {$< }

.cpp.obj:
	$(CC) -c {$< }

#   *List Macros*

MENU_LIB_dependencies =  \
 menu.obj

#   *Explicit Rules*
RELEASE\$(TARGETPATH1)\TC30\$(TARGETPATH2)\TI2MENU.lib: tcdosmnu.cfg $(MENU_LIB_dependencies)
	$(TLIB) $< /P16 @&&|
+menu.obj
|

#   *Individual File Dependencies*
menu.obj: tcdosmnu.cfg menu.cpp

#   *Compiler Configuration File*
tcdosmnu.cfg: tcdosmnu.mak
	copy &&|
-m$(MODEL)
-vi-
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
-wcln
-w-cpt
-wdef
-w-dup
-w-pia
-wnod
-w-ill
-w-sus
-wstv
-wuse
-w-ext
-w-ias
-w-ibc
-w-pre
-w-nst
-I$(INCLUDEPATH)
-L$(LIBPATH)
| tcdosmnu.cfg


