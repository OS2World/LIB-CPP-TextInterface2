@ECHO OFF
REM
REM   Erase all old libraries.
REM      
erase RELEASE\REG\TC30\LARGE\*.LIb
REM
REM   Compile TI/2 Window module
REM      
MAKE -fTCDOSWIN.MAK -DMODEL=l
REM
REM   Compile TI/2 Menu module
REM      
MAKE -fTCDOSMNU.MAK -DMODEL=l
REM
REM   Compile TI/2 Form module
REM      
MAKE -fTCDOSFRM.MAK -DMODEL=l
REM
REM   Compile TI/2 Demo
REM      
MAKE -fDOS_DEMO.MAK              
