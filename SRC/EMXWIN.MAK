#
# /emx/src/lib/lib0.mak
#

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
COMPILE_DEFINES = _COMMERCIAL_
TARGETPATH1 = REG
!endif

GCC=gcc -O2 -v -D$(COMPILE_DEFINES)
AR=ar


.SUFFIXES: .cpp .c .imp .o .obj .s

.cpp.o:
  $(GCC) -c $<

.c.o:
	$(GCC) -c $<

.s.o:
	$(GCC) -c -x assembler-with-cpp -I. $<

LIBWIN=ti2win.a
LIBMENU=ti2menu.a
LIBFORM=ti2form.a

default: clean win menu form

clean:
  -del *.o
  -del ti2win.a
  -del ti2form.a
  -del ti2menu.a

win: $(LIBWIN)
menu: $(LIBMENU)
form: $(LIBFORM)

win.o: win.cpp
win2.o: win2.cpp
mouse.o: mouse.cpp
timeslc.o: timeslc.cpp
globdata.o: globdata.cpp
internal.o: internal.cpp

menu.o: menu.cpp

form1.o: form1.cpp
form2.o: form2.cpp

$(LIBWIN): win.o win2.o mouse.o timeslc.o globdata.o internal.o
  -del release/$(TARGETPATH1)/emx09a.os2/$(TARGETPATH2)/$(LIBWIN)
  $(AR) r release/$(TARGETPATH1)/emx09a.os2/$(TARGETPATH2)/$(LIBWIN) win.o win2.o mouse.o timeslc.o globdata.o internal.o

$(LIBMENU): menu.o
  -del release/$(TARGETPATH1)/emx09a.os2/$(TARGETPATH2)/$(LIBMENU)
  $(AR) r release/$(TARGETPATH1)/emx09a.os2/$(TARGETPATH2)/$(LIBMENU) menu.o

$(LIBFORM): form1.o form2.o
  -del release/$(TARGETPATH1)/emx09a.os2/$(TARGETPATH2)/$(LIBFORM)
  $(AR) r release/$(TARGETPATH1)/emx09a.os2/$(TARGETPATH2)/$(LIBFORM) form1.o form2.o
