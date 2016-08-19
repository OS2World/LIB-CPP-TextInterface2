.AUTODEPEND

#   *Translator Definitions*
CC = tcc +tcmoutst.CFG
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
 mousetst.obj \
 release\reg\tc30\large\ti2win.lib

#   *Explicit Rules*
tcmoutst.exe: tcmoutst.cfg $(EXE_dependencies)
	$(TLINK) /v/x/c/L$(LIBPATH) @&&|
c0l.obj+
mousetst.obj
tcmoutst
		# no map file
release\reg\tc30\large\ti2win.lib+
emu.lib+
mathl.lib+
cl.lib
|


#   *Individual File Dependencies*
mousetst.obj: tcmoutst.cfg mousetst.cpp 

#   *Compiler Configuration File*
tcmoutst.cfg: tcmoutst.mak
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
| tcmoutst.cfg


