/* Filename : TIMESLC.CPP                                                   *
 * Description : Time slice routines for TI/2                               *
 * Target : TIMESLC.OBJ -> TI2WIN.LIB or TI2.DLL                            *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 04/30/95                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

/******************************
 * Predefined header includes *
 ******************************/

#if defined(__OS2__) || defined(__EMX__)
	#define INCL_NOPMAPI
	#define INCL_DOSDATETIME
	#define INCL_DOSPROCESS
	#define INCL_DOSSEMAPHORES
	#include <os2.h>
#endif
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include "ti2.h"
#include "internal.h"

/********************
 * Global Variables *
 ********************/

#ifdef __MSDOS__
uchar os_type = 0;        // Type of operating system
#endif

/**********************
 * Time Slice Defines *
 **********************/

#define OS_DOS     0
#define OS_OS2     1
#define OS_DV      2
#define OS_WINS    3
#define OS_WIN3    4

/************************
 * Time Slice Functions *
 ************************/

void _export init_timeslice(void)
		{
			#if defined(__MSDOS__) && !defined(__GO32__)
			union REGS t_regs;

			os_type = OS_DOS;

			/* Test for OS/2 */
			if (_osmajor >= 10) {
				os_type = OS_OS2;
				return;
			}

			/* Test for Windows */
			t_regs.x.ax = 0x4680;
			int86(0x2F, &t_regs, &t_regs);
			if (t_regs.x.ax == 0x0000) os_type = OS_WINS;
			 else
			{
				/* Text for Windows/386 mode */
				t_regs.x.ax = 0x1600 ;
				int86(0x2F, &t_regs, &t_regs);

				switch (t_regs.h.al) {
					case 0x00 :
					case 0x80 :
					case 0x01 :
					case 0xFF : {
												break;
											}
					default   : {
												os_type = OS_WIN3;
												break;
											}
				}
			}

			/* Test for DESQview */
			t_regs.x.cx = 0x4445;     /* load incorrect date */
			t_regs.x.dx = 0x5351;
			t_regs.x.ax = 0x2B01;     /*  DV set up call     */

			intdos(&t_regs, &t_regs);
			if (t_regs.h.al != 0xFF) os_type = OS_DV;
			#endif
		}

void _export timeslice(void)
		{
			#ifdef __MSDOS__
			union REGS t_regs;

			switch (os_type) {
				case OS_OS2  :
				case OS_WIN3 :
				case OS_WINS : {
												 /* Call Time slice function */
												 t_regs.x.ax = 0x1680;
												 int86(0x2f,&t_regs,&t_regs);
												 break;
											 }
				case OS_DV   : {
												 /* Call DV Time slice function */
												 t_regs.x.ax = 0x1000;
												 int86(0x15,&t_regs,&t_regs);
												 break;
											 }
			}
			#endif

			#if defined(__OS2__) || defined(__EMX__)
				/* Call OS/2 API time slice function */
				DosSleep(5);
			#endif
		}
