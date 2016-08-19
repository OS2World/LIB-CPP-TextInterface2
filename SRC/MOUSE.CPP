/* Filename : MOUSE.CPP                                                     *
 * Description : Mouse routines for TI/2                                    *
 * Target : MOUSE.OBJ -> TI2WIN.LIB or TI2.DLL                              *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 04/30/95                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

/******************************
 * Predefined header includes *
 ******************************/

#if defined(__OS2__) || defined(__EMX__)
	#define INCL_MOU
	#define INCL_NOPMAPI
	#define INCL_DOSPROCESS
	#define INCL_DOSDATETIME
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

uchar ti2_mouse_click_delay = 100;        // Milliseconds to delay after first click to see if the button is down or just clicked

#if defined(__OS2__) || defined(__EMX__)
HMOU ti2_hmou;                            // OS/2 mouse handle
#endif

/*******************
 * Mouse Functions *
 *******************/

void _export mouse_init(void)
		{
      #if defined(__OS2__) || defined(__EMX__)
			USHORT events;      // Holds events that mouse should report

			/* Open default mouse driver */
      if (MouOpen(NULL, &ti2_hmou)) {
				/* Error opening mouse */
				mouse_exist = 0;
				return;
			}

			/* Set mouse events */
			events = MOUSE_MOTION | MOUSE_BN1_DOWN | MOUSE_BN2_DOWN | MOUSE_BN3_DOWN | MOUSE_MOTION_WITH_BN1_DOWN | MOUSE_MOTION_WITH_BN2_DOWN | MOUSE_MOTION_WITH_BN3_DOWN;
      MouSetEventMask(&events, ti2_hmou);

			/* Toggle mouse existance */
			mouse_exist = 1;

			/* Make mouse pointer visible to start */
			mouse_on();

			/* Read mouse event just to initialize */
			mouse_read_event();
			#endif

			#ifdef __MSDOS__
			union REGS inregs, outregs;       // Registers used by int86x
			struct SREGS segregs;

			/* Call interrupt to see if mouse exists */
			inregs.x.ax = 0x0000;
			int86x(0x33, &inregs, &outregs, &segregs);

			/* Check result of initialization */
			if (outregs.x.ax == 0xFFFF) mouse_exist = 1;
				else return;

			/* Make mouse pointer visible to start */
			mouse_on();

			/* Read mouse event just to initialize */
			mouse_read_event();
			#endif
		}

void _export mouse_on(void)
		{
      #if defined(__OS2__) || defined(__EMX__)
			/* Unhide mouse */
      if (mouse_exist) MouDrawPtr(ti2_hmou);
			#endif

			#ifdef __MSDOS__
			union REGS inregs, outregs;       // Registers used by int86x
			struct SREGS segregs;

			/* Call interrupt to see if mouse exists. Call twice to make sure cursor gets unhidden */
			inregs.x.ax = 0x0001;
			int86x(0x33, &inregs, &outregs, &segregs);
			int86x(0x33, &inregs, &outregs, &segregs);
			#endif
		}

void _export mouse_off(void)
		{
      #if defined(__OS2__) || defined(__EMX__)
			NOPTRRECT np;                 // Contains coordinates where mouse won't be displayed inside of

			/* Hide mouse */
			if (mouse_exist) {
				/* Set no pointer display coordinates */
				np.row = 0;
				np.col = 0;
        np.cRow = SCREENHEIGHT() - 1;
        np.cCol = SCREENWIDTH() - 1;
				/* Remove mouse pointer from screen */
        MouRemovePtr(&np, ti2_hmou);
			}
			#endif

			#ifdef __MSDOS__
			union REGS inregs, outregs;       // Registers used by int86x
			struct SREGS segregs;

			/* Call interrupt to see if mouse exists */
			inregs.x.ax = 0x0002;
			int86x(0x33, &inregs, &outregs, &segregs);
			#endif
		}

uchar _export mouse_read_event(void)
		{
      #if defined(__OS2__) || defined(__EMX__)
			USHORT eventflag = MOU_WAIT;
			MOUEVENTINFO info;
			MOUQUEINFO queinfo;

			/* Check for mouse existance */
			if (!mouse_exist) return(0);

			/* Get number of events in que */
      MouGetNumQueEl(&queinfo, ti2_hmou);

			/* Read event */
			if (queinfo.cEvents) {
        MouReadEventQue(&info, &eventflag, ti2_hmou);

				/* Convert results to our variables */
				mouse_row = info.row + 1;
				mouse_col = info.col + 1;

				/* Left button */
				if (info.fs & MOUSE_BN1_DOWN) {
					mouse_left = BUTTON_DOWN;
          DELAY(ti2_mouse_click_delay);
					return(0);
				}
					else
				if (info.fs & MOUSE_MOTION_WITH_BN1_DOWN) mouse_left = BUTTON_DOWN;
					else
				if (mouse_left == BUTTON_DOWN) mouse_left = BUTTON_CLICKED;
					else
				if (mouse_left == BUTTON_CLICKED) mouse_left = 0;

				/* Center button */
				if (info.fs & MOUSE_BN3_DOWN) {
					mouse_center = BUTTON_CLICKED;
          DELAY(ti2_mouse_click_delay);
					return(0);
				}
					else
				if (info.fs & MOUSE_MOTION_WITH_BN3_DOWN) mouse_center = BUTTON_DOWN;
					else
				if (mouse_center == BUTTON_DOWN) mouse_center = BUTTON_CLICKED;
					else
				if (mouse_center == BUTTON_CLICKED) mouse_center = 0;

				/* Right button */
				if (info.fs & MOUSE_BN2_DOWN) {
					mouse_right = BUTTON_CLICKED;
          DELAY(ti2_mouse_click_delay);
					return(0);
				}
					else
				if (info.fs & MOUSE_MOTION_WITH_BN2_DOWN) mouse_right = BUTTON_DOWN;
					else
				if (mouse_right == BUTTON_DOWN) mouse_right = BUTTON_CLICKED;
					else
				if (mouse_right == BUTTON_CLICKED) mouse_right = 0;

				/* Return true since a mouse event occured */
				return(1);
			}
				else
			if ((mouse_left == BUTTON_DOWN) || (mouse_center == BUTTON_DOWN) || (mouse_right == BUTTON_DOWN)) return(1);
				else
			if ((mouse_left == BUTTON_CLICKED) || (mouse_center == BUTTON_CLICKED) || (mouse_right == BUTTON_CLICKED)) {
				if (mouse_left == BUTTON_CLICKED) mouse_left = 0;
				if (mouse_center == BUTTON_CLICKED) mouse_center = 0;
				if (mouse_right == BUTTON_CLICKED) mouse_right = 0;
				return(1);
			}
				else
			{
				/* Return false since no mouse event has occured */
				return(0);
			}
			#endif

			#ifdef __MSDOS__
			#define LEFT_BUTTON   (1<<0)
			#define RIGHT_BUTTON  (1<<1)
			#define CENTER_BUTTON (1<<2)

			union REGS inregs, outregs;       // Registers used by int86x
			struct SREGS segregs;
			uchar event_occured = 0;

			/* Update position on screen information */
			inregs.x.ax = 0x0003;
			int86x(0x33, &inregs, &outregs, &segregs);
			outregs.x.cx /= 8;
			outregs.x.cx++;
			outregs.x.dx /= 8;
			outregs.x.dx++;
			if (mouse_col != outregs.x.cx) {
				mouse_col = outregs.x.cx;
				event_occured = 1;
			}
			if (mouse_row != outregs.x.dx) {
				mouse_row = outregs.x.dx;
				event_occured = 1;
			}

			/* Check left button */
			inregs.x.ax = 0x0005;
			inregs.x.bx = 0x0000;
			int86x(0x33, &inregs, &outregs, &segregs);
			if (outregs.x.ax & LEFT_BUTTON) {
				if (!(mouse_left == BUTTON_DOWN)) {
					mouse_left = BUTTON_DOWN;
          DELAY(ti2_mouse_click_delay);
					return(0);
				} 
					else mouse_left = BUTTON_DOWN;
				event_occured = 1;
			}
				else
			{
				if (mouse_left == BUTTON_DOWN) {
					mouse_left = BUTTON_CLICKED;
					event_occured = 1;
				}
					else
				if (mouse_left == BUTTON_CLICKED) {
					mouse_left = 0;
					event_occured = 1;
				}
			}

			/* Check center button */
			inregs.x.ax = 0x0005;
			inregs.x.bx = 0x0002;
			int86x(0x33, &inregs, &outregs, &segregs);
			if (outregs.x.ax & CENTER_BUTTON) {
				if (!(mouse_center == BUTTON_DOWN)) {
					mouse_center = BUTTON_DOWN;
          DELAY(ti2_mouse_click_delay);
					return(0);
				}
					else mouse_center = BUTTON_DOWN;
				event_occured = 1;
			}
				else
			{
				if (mouse_center == BUTTON_DOWN) {
					mouse_center = BUTTON_CLICKED;
					event_occured = 1;
				}
					else
				if (mouse_center == BUTTON_CLICKED) {
					mouse_center = 0;
					event_occured = 1;
				}
			}

			/* Check right button */
			inregs.x.ax = 0x0005;
			inregs.x.bx = 0x0001;
			int86x(0x33, &inregs, &outregs, &segregs);
			if (outregs.x.ax & RIGHT_BUTTON) {
				if (!(mouse_right == BUTTON_DOWN)) {
					mouse_right = BUTTON_DOWN;
          DELAY(ti2_mouse_click_delay);
					return(0);
				}
					else mouse_right = BUTTON_DOWN;
				event_occured = 1;
			}
				else
			{
				if (mouse_right == BUTTON_DOWN) {
					mouse_right = BUTTON_CLICKED;
					event_occured = 1;
				}
					else
				if (mouse_right == BUTTON_CLICKED) {
					mouse_right = 0;
					event_occured = 1;
				}
			}

			if (event_occured) return(1);
				else return(0);
			#undef LEFT_BUTTON
			#undef RIGHT_BUTTON
			#undef CENTER_BUTTON
			#endif
		}

void _export mouse_deinit()
		{
      #if defined(__OS2__) || defined(__EMX__)
			/* Deinit mouse driver */
			if (mouse_exist) {
        MouClose(ti2_hmou);
				mouse_exist = 0;
			}
			#endif

			#ifdef __MSDOS__
			union REGS inregs, outregs;       // Registers used by int86x
			struct SREGS segregs;

			/* Call interrupt to reset mouse driver */
			inregs.x.ax = 0x0000;
			int86x(0x33, &inregs, &outregs, &segregs);
			#endif
		}

