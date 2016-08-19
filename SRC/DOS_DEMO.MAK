.AUTODEPEND

#   *Translator Definitions*
CC = tcc +DOS_DEMO.CFG
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


EXE_dependencies =  \
 demo.obj \
 release\reg\tc30\large\ti2form.lib \
 release\reg\tc30\large\ti2menu.lib \
 release\reg\tc30\large\ti2win.lib

#   *Explicit Rules*
dos_demo.exe: dos_demo.cfg $(EXE_dependencies)
	$(TLINK) /v/x/c/L$(LIBPATH) @&&|
c0l.obj+
demo.obj
dos_demo
		# no map file
release\reg\tc30\large\ti2form.lib+
release\reg\tc30\large\ti2menu.lib+
release\reg\tc30\large\ti2win.lib+
emu.lib+
mathl.lib+
cl.lib
|


#   *Individual File Dependencies*
demo.obj: dos_demo.cfg demo.cpp 

#   *Compiler Configuration File*
dos_demo.cfg: dos_demo.mak
	copy &&|
-ml
-v
-vi-
-w-ret
-w-nci
-w-inl
-wpin
-wamb
-wamp
-w-par
-wasm
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
| dos_demo.cfg


