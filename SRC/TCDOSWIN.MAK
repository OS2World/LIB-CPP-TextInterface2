.NOAUTODEPEND

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
CC = tcc -D$(COMPILE_DEFINES) -m$(MODEL) +tcdoswin.CFG
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

WIN_LIB_dependencies =  \
 win.obj \
 win2.obj \
 mouse.obj \
 timeslc.obj \
 globdata.obj

#   *Explicit Rules*
RELEASE\$(TARGETPATH1)\TC30\$(TARGETPATH2)\TI2WIN.lib: tcdoswin.cfg $(WIN_LIB_dependencies)
	$(TLIB) $< /P16 @&&|
+win.obj &
+win2.obj &
+mouse.obj &
+timeslc.obj &
+globdata.obj
|

#   *Individual File Dependencies*
win.obj: tcdoswin.cfg win.cpp

win2.obj: tcdoswin.cfg win2.cpp

mouse.obj: tcdoswin.cfg mouse.cpp

timeslc.obj: tcdoswin.cfg timeslc.cpp

globdata.obj: tcdoswin.cfg globdata.cpp

#   *Compiler Configuration File*
tcdoswin.cfg: tcdoswin.mak
	copy &&|
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
| tcdoswin.cfg


