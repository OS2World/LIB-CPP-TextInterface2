
/* Filename : WIN.CPP                                                       *
 * Description : Windowing module #1 for TI/2                               *
 * Target : WIN.OBJ -> TI2WIN.LIB or TI2.DLL                                *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 10/25/94                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

/******************************
 * Predefined header includes *
 ******************************/

#if defined(__OS2__) || defined(__EMX__)
	#define INCL_NOPMAPI
	#define INCL_DOSPROCESS
	#define INCL_DOSSEMAPHORES
	#include <os2.h>
#endif

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <dos.h>
#include <math.h>
#include "ti2.h"
#include "internal.h"

#ifdef __TURBOC__
  #include <alloc.h>
#endif

#ifdef _USERTL2_
	#include "..\rtl2\rtl2.h"
#else
#endif

/**********************
 * Window Global Data *
 **********************/

uchar ti2_initialized = 0;     // True if window system initialized

/********************
 * Window Functions *
 ********************/

void _export win_set_default_colors (uchar f_fcolor, uchar f_bcolor, uchar t_fcolor,
                                     uchar t_bcolor, uchar title_fcolor, uchar title_bcolor,
                                     uchar scroll_fcolor, uchar scroll_bcolor)
		{
			ENTERCRITSEC();

			/* Copy default colors */
			ti2_win_default_colors.f_fcolor = f_fcolor;
			ti2_win_default_colors.f_bcolor = f_bcolor;
			ti2_win_default_colors.t_fcolor = t_fcolor;
			ti2_win_default_colors.t_bcolor = t_bcolor;
			ti2_win_default_colors.title_fcolor = title_fcolor;
			ti2_win_default_colors.title_bcolor = title_bcolor;
			ti2_win_default_colors.scroll_fcolor = scroll_fcolor;
			ti2_win_default_colors.scroll_bcolor = scroll_bcolor;

			EXITCRITSEC();
		}

uchar win_xyxy_in_xyxy(uchar x1, uchar y1, uchar x2, uchar y2, uchar x3, uchar y3, uchar x4, uchar y4)
		{
			uchar i;  // General counter

			/* See if any of the coordinates of the first box are inside the second box */
			if (WIN_XY_IN_RANGE(x1, y1, x3, y3, x4, y4) || WIN_XY_IN_RANGE(x2, y1, x3, y3, x4, y4) ||
					WIN_XY_IN_RANGE(x1, y2, x3, y3, x4, y4) || WIN_XY_IN_RANGE(x2, y2, x3, y3, x4, y4)) return(1);

			/* See if any of the coordiantes of the second box are within the first box */
			if (WIN_XY_IN_RANGE(x3, y3, x1, y1, x2, y2) || WIN_XY_IN_RANGE(x4, y3, x1, y1, x2, y2) ||
					WIN_XY_IN_RANGE(x3, y4, x1, y1, x2, y2) || WIN_XY_IN_RANGE(x4, y4, x1, y1, x2, y2)) return(1);

			/* See if any of borders of the first window are inside the second */
			for (i = (x1 + 1); i < x2; i++) {
				if (WIN_XY_IN_RANGE(i, y1, x3, y3, x4, y4)) return(1);
			}
			for (i = (x1 + 1); i < x2; i++) {
				if (WIN_XY_IN_RANGE(i, y2, x3, y3, x4, y4)) return(1);
			}
			for (i = (y1 + 1); i < y2; i++) {
				if (WIN_XY_IN_RANGE(x1, i, x3, y3, x4, y4)) return(1);
			}
			for (i = (y1 + 1); i < y2; i++) {
				if (WIN_XY_IN_RANGE(x2, i, x3, y3, x4, y4)) return(1);
			}

			/* Return no overlap */
			return(0);
		}

uchar _export win_init(void)
		{
      /* Initialize video system for EMX */
      #ifdef __EMX__
      init_video();
      #endif

			/* Create Semaphore which coordinates window writing */
			#ifdef __OS2__
			DosCreateMutexSem(NULL, &ti2_hmtx, 0, FALSE);
			#endif

			ENTERCRITSEC();

			/* Initialize variables */
			ti2_num_windows = 0;
			ti2_initialized = 1;

			/* Create desktop window */
      if (!desktop.create(1, 1, SCREENWIDTH(), SCREENHEIGHT(), LIGHTGRAY, BLACK, LIGHTGRAY, BLACK, "",
													LIGHTGRAY, BLACK, WIN_FRAME_NONE, WIN_SHADOW_NONE, WIN_SCROLL)) {
				EXITCRITSEC();
				return(0);
			}

			/* Initialize mouse driver */
			mouse_init();

			/* Initialize time slice system */
			init_timeslice();

			/* If this is shareware show copyright notice etc. */
#ifdef _SHAREWARE_
			win_c win;    // Holds shareware notice

      win.create(1, 1, 70, 21, WHITE, BLACK, WHITE, BLACK, " NOTICE ", RED+BLINK, BLACK,
								 WIN_FRAME_MIXEDBOX1, WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);

			/* Display copyright notice */
			win.cntr_printf(2, "Text Interface/2");
			win.cntr_printf(3, "Copyright (c) Jonathan Tew and Revolutionary Software 1995");
			win.cntr_printf(4, "All Rights Reserved");
			win.cntr_printf(6, "Version %s", VERSION);

			/* Display warning */
			win.cntr_printf(8, "This is a shareware evaluation edition of TI/2.  Any program");
			win.cntr_printf(9, "written with this unregistered library is not to be released.");

			/* Display Contact address */
			win.goto_xy(1, 11);
			win.wputs("    Contact Revolutionary Software at:\n\r\n\r");
			win.wputs("    Mailing Address:  Revolutionary Sofware\n\r");
			win.wputs("                      2689 Hickory Cove\n\r");
			win.wputs("                      Lilburn, GA 30247\n\r\n\r");
			win.wputs("    BBS:              The Fallen Fortress\n\r");
			win.wputs("                      (404) 469-6752");

			/* Empty the keyboard buffer */
			while (KBHIT()) GETCH();

			/* Wait for the user to press a key */
			GETCH();

			/* Destroy the window */
			win.destroy();
#endif

			/* Return successful */
			EXITCRITSEC();
			return(1);
		}

void _export win_deinit(void)
    { 
      uchar i, j;   // General counters

			ENTERCRITSEC();

			/* Destroy desktop window */
			desktop.destroy();

			/* Free all other memory allocated by unclosed windows */
			for (i = 0; i < ti2_num_windows; i++) {
				for (j = 0; j < (ti2_windows[i]->botypos - ti2_windows[i]->topypos + 1); j++) {
					FARFREE(ti2_windows[i]->text[j]);
				}
				FARFREE(ti2_windows[i]->text);
				FARFREE(ti2_windows[i]);
			}

			/* Deallocate windows and reset variables */
			FARFREE(ti2_windows);
			ti2_num_windows = 0;
			ti2_initialized = 0;

			/* Deinitialize mouse driver */
			mouse_deinit();

			EXITCRITSEC();
			/* Destroy TI/2 Event Sempahore */
			#ifdef __OS2__
			DosCloseMutexSem(ti2_hmtx);
			#endif
		}

void _export win_redraw_all(void)
		{
			uchar i;                  // General counter
			uchar total_num_windows;  // Stores actual total number of windows
			win_c win;                // Instance of window

			ENTERCRITSEC();

			/* Store ti2_num_windows */
			total_num_windows = ti2_num_windows;

			/* Redisplay all windows */
			for (i = 0; i < total_num_windows; i++) {
				ti2_num_windows = (i + 1);
				win.set_pos(i);
				win.redisplay();
			}

			/* Restore ti2_num_windows */
			ti2_num_windows = total_num_windows;
			EXITCRITSEC();
		}

void _export win_text_mode(int newmode)
		{
			ENTERCRITSEC();

			/* Change text mode */
			TEXTMODE(newmode);

			/* Resize desktop */
      desktop.resize(TOP, SCREENHEIGHT());
      desktop.resize(LEFT, SCREENWIDTH());

			/* Redraw the whole screen */
			win_redraw_all();
			EXITCRITSEC();
		}

/**************************
 * Window Class Functions *
 **************************/

uchar win_c::text_x_relative(uchar abs_x)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for hidden window */
			if (WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return(0);
			}

			/* Return the coordinate in the window that the absolute x is */
			if ((abs_x >= WINDOW->texttopxpos) && (abs_x <= WINDOW->textbotxpos)) {
				EXITCRITSEC();
				return(abs_x - WINDOW->texttopxpos + 1);
			}
				else
			{
				EXITCRITSEC();
				return(0);
			}
		}

uchar win_c::text_y_relative(uchar abs_y)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for hidden window */
			if (WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return(0);
			}

			/* Return the coordinate in the window that the absolute x is */
			if ((abs_y >= WINDOW->texttopypos) && (abs_y <= WINDOW->textbotypos)) {
				EXITCRITSEC();
				return(abs_y - WINDOW->texttopypos + 1);
			}
				else
			{
				EXITCRITSEC();
				return(0);
			}
		}

uchar win_c::frame_x_relative(uchar abs_x)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for hidden window */
			if (WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return(0);
			}

			/* Return the coordinate in the window that the absolute x is */
			if ((abs_x >= WINDOW->frametopxpos) && (abs_x <= WINDOW->framebotxpos)) {
				EXITCRITSEC();
				return(abs_x - WINDOW->frametopxpos + 1);
			}
				else
			{
				EXITCRITSEC();
				return(0);
			}
		}

uchar win_c::frame_y_relative(uchar abs_y)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for hidden window */
			if (WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return(0);
			}

			/* Return the coordinate in the window that the absolute x is */
			if ((abs_y >= WINDOW->frametopypos) && (abs_y <= WINDOW->framebotypos)) {
				EXITCRITSEC();
				return(abs_y - WINDOW->frametopypos + 1);
			}
				else
			{
				EXITCRITSEC();
				return(0);
			}
		}

void win_c::get_pos(void)
		{
			uchar i;

			if ((pos < ti2_num_windows) && (pos >= 0)) {
				if (ti2_windows[pos]->handle != handle) {
					pos = -1;
					for (i = 0; i < ti2_num_windows; i++) {
						if (ti2_windows[i]->handle == handle) {
							pos = i;
							break;
						}
					}
				}
			}
				else
			{
				pos = -1;
				for (i = 0; i < ti2_num_windows; i++) {
					if (ti2_windows[i]->handle == handle) {
						pos = i;
						break;
					}
				}
			}
		}

void win_c::make_seethru(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Toggle see thru bit on */
			if (!(WINDOW->attr & WIN_SEETHRU)) {
				WINDOW->attr |= WIN_SEETHRU;

				/* Redisplay text */
				display_part(TEXT);
			}
			EXITCRITSEC();
		}

void win_c::make_normal(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Toggle see thru bit on */
			if (WINDOW->attr & WIN_SEETHRU) {
				WINDOW->attr &= ~(WIN_SEETHRU);

				/* Redisplay text */
				display_part(TEXT);
			}
			EXITCRITSEC();
		}

void win_c::popup(void)
		{
			win_t FAR *temp_win;    // Temporary pointer to window information
			uchar visible;          // Tells windows is completely visible.

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check to see if we are the top window already */
			if ((pos + 1) == ti2_num_windows) {
				EXITCRITSEC();
				return;
			}

			/* If window is completely visible there is no need to hide and unhide */
			visible = completely_visible();

			/* Hide this window */
			if (!visible) hide();

			temp_win = ti2_windows[pos];

			/* Store pointer to current window and shift it to the top */
			FARMEMMOVE(&ti2_windows[pos], &ti2_windows[pos + 1], sizeof(win_t FAR *) * (ti2_num_windows - pos - 1));

			ti2_windows[ti2_num_windows - 1] = temp_win;

			/* Update position in window array */
			get_pos();

			/* Unhide window on top */
			if (!visible) unhide();
			EXITCRITSEC();
		}

void win_c::set_pos(uchar new_pos)
		{
			ENTERCRITSEC();

			if (new_pos >= ti2_num_windows) {
				EXITCRITSEC();
				return;
			}

			/* Set new position */
			pos = new_pos;

			/* Set handle to the one at the new position */
			handle = WINDOW->handle;
			EXITCRITSEC();
		}

void win_c::redisplay(void)
		{
			ENTERCRITSEC();
			/* Update position in window array */
			get_pos();

			/* Redisplay the whole window */
			display_part(SHADOW | FRAME | TEXT);
			EXITCRITSEC();
		}

void win_c::redisplay_text(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Redisplay the whole window */
			display_part(TEXT);
			EXITCRITSEC();
		}

uchar win_c::get_handle(void)
		{
			int next_handle = 0;   // Value of next handle
			uchar i;                // General counter

			/* If we have 250 windows than return false */
			if (ti2_num_windows >= MAX_WINDOWS) return(0);

			/* Loop through all windows and find highest handle */
			for (i = 0; i < ti2_num_windows; i++) {
				/* See if this window has a greater handle than our highest */
				if (ti2_windows[i]->handle > next_handle) next_handle = ti2_windows[i]->handle;
			}
			next_handle++;

			/* Reallocate array of pointers */
			ti2_num_windows++;
			if ((ti2_windows = (win_t FAR * FAR *) FARREALLOC(ti2_windows, sizeof(win_t FAR *) * ti2_num_windows)) == NULL) {
				/* Error reallocating windows array */
				printf("\nwin_get_handle() error : Unable to reallocate windows array");
				getch();
				return(0);
			}
			if ((ti2_windows[ti2_num_windows - 1] = (win_t FAR *) FARMALLOC(sizeof(win_t))) == NULL) {
				/* Error reallocating windows array */
				FARFREE(ti2_windows);
				printf("win_get_handle() error : Unable to reallocate windows array");
				getch();
				return(0);
			}

			/* Set window class handle */
			handle = next_handle;
			ti2_windows[ti2_num_windows - 1]->handle = next_handle;

			/* Return successful */
			return(1);
		}

uchar win_c::completely_visible(void)
		{
			uchar k;    // General counter

			/* See if this is the top window.  If so than return true */
			if ((pos + 1) == ti2_num_windows) return(1);

			/* Search all windows above this one to see if they overlap at all */
			for (k = (pos + 1); k < ti2_num_windows; k++) {
				/* See if window that we are checking is hidden */
				if (!(ti2_windows[k]->attr & WIN_HIDDEN)) {
					/* See if there is a see-thru window above and our window is completely inside the see-thru part */
					if (ti2_windows[k]->attr & WIN_SEETHRU) {
						if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->topxpos, ti2_windows[k]->topypos, ti2_windows[k]->botxpos, ti2_windows[k]->botypos)) {
							/* Our window lies below the window above, but is it completely in the see thru part */
							/* Check shadow */
							if (ti2_windows[k]->shadow == WIN_SHADOW_RIGHT) {
								if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->botxpos, ti2_windows[k]->topypos+1, ti2_windows[k]->botxpos, ti2_windows[k]->botypos) ||
										win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->topxpos+1, ti2_windows[k]->botypos, ti2_windows[k]->botxpos, ti2_windows[k]->botypos)) {
									/* There is shadow overlap */
									return(0);
								}
							}
								else
							if (ti2_windows[k]->shadow == WIN_SHADOW_LEFT) {
								if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->topxpos, ti2_windows[k]->topypos, ti2_windows[k]->botxpos-1, ti2_windows[k]->topypos) ||
										win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->topxpos, ti2_windows[k]->topypos, ti2_windows[k]->topxpos, ti2_windows[k]->botypos-1)) {
									/* There is shadow overlap */
									return(0);
								}
							}
							/* Check frame overlap */
							if (ti2_windows[k]->frame) {
								/* Check all sides for overlap */
								/* Top */
								if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->frametopxpos, ti2_windows[k]->frametopypos, ti2_windows[k]->framebotxpos, ti2_windows[k]->frametopypos)) return(0);
								/* Bottom */
								if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->frametopxpos, ti2_windows[k]->framebotypos, ti2_windows[k]->framebotxpos, ti2_windows[k]->framebotypos)) return(0);
								/* Left */
								if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->frametopxpos, ti2_windows[k]->frametopypos, ti2_windows[k]->frametopxpos, ti2_windows[k]->framebotypos)) return(0);
								/* Right */
								if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->framebotxpos, ti2_windows[k]->frametopypos, ti2_windows[k]->framebotxpos, ti2_windows[k]->framebotypos)) return(0);
							}
						}
					}
						else
					/* If any overlap return false */
					if (win_xyxy_in_xyxy(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, ti2_windows[k]->topxpos, ti2_windows[k]->topypos, ti2_windows[k]->botxpos, ti2_windows[k]->botypos)) return(0);
				}
			}

			/* Return completely visible */
			return(1);
		}

uchar win_c::visible_ch(uchar x, uchar y, uchar ch_type)
		{
			uchar i;    // General counter
			uchar result = 0;

			/* Change X Y coordinates to make them absolute to the screen */
			switch(ch_type) {
				case SHADOW_CH : {
													 x = (WINDOW->topxpos+x-1);
													 y = (WINDOW->topypos+y-1);
													 break;
												 }
				case FRAME_CH : {
													x = (WINDOW->frametopxpos+x-1);
													y = (WINDOW->frametopypos+y-1);
													break;
												}
				case TEXT_CH : {
												 x = (WINDOW->texttopxpos+x-1);
												 y = (WINDOW->texttopypos+y-1);
												 break;
											 }
			}

			/* Check to see if we are the top window.  If so it has to be visible */
			if (completely_visible()) return(VISIBLE);

			result = VISIBLE;

			/* Check to see if any of the ti2_windows that have a higher z position overlap this one */
			for (i = (pos + 1); i < ti2_num_windows; i++) {
				/* See if window that we are checking is hidden and wether our point could even fall inside of this window */
				if (!(ti2_windows[i]->attr & WIN_HIDDEN) && WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
					/* See if we are in the frame of the window on top. Also check for see-thru window */
					if (ti2_windows[i]->attr & WIN_SEETHRU) {
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
							/* Our window lies below the window above, but is it completely in the see thru part */
							/* Check frame overlap */
							if (ti2_windows[i]->frame) {
								/* Check all sides for overlap */
								/* Top */
								if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos) ||
								/* Bottom */
										WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos) ||
								/* Left */
										WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos) ||
								/* Right */
										WIN_XY_IN_RANGE(x, y, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {

									if (result == SHADOW_VISIBLE) return(SHADOW_NOT_VISIBLE);
										else return(NOT_VISIBLE);
								}
							}
						}
					}
						else
					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
						if (result == SHADOW_VISIBLE) return(SHADOW_NOT_VISIBLE);
							else return(NOT_VISIBLE);
					}

					/* See if we are in the shadow of the window on top */
					if (ti2_windows[i]->shadow == WIN_SHADOW_RIGHT) {
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->botxpos, ti2_windows[i]->topypos+1, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
							/* This character is visible, but in the shadow of another window */
							result = SHADOW_VISIBLE;
						}
							else
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos+1, ti2_windows[i]->botypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
							/* This character is visible, but in the shadow of another window */
							result = SHADOW_VISIBLE;
						}
					}
						else
					if (ti2_windows[i]->shadow == WIN_SHADOW_LEFT) {
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos-1, ti2_windows[i]->topypos)) {
							/* This character is visible, but in the shadow of another window */
							result = SHADOW_VISIBLE;
						}
							else
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->topxpos, ti2_windows[i]->botypos-1)) {
							/* This character is visible, but in the shadow of another window */
							result = SHADOW_VISIBLE;
						}
					}
				}
			}

			/* Return the result */
			return(result);
		}

void win_c::get_hidden_ch(uchar x, uchar y, uchar ch_type, char *buffer)
		{
			int i;                  // General counter
			uchar hidden_window;    // Position in array of window beneath ours with the hidden character
			uchar result = 0;
			uchar offx, offy;       // X/Y offsets in buffer of hidden character

			/* Initialize variables */
			buffer[0] = ' ';
			buffer[1] = 0;

			/* Make coordinates absolte */
			switch(ch_type) {
				case SHADOW_CH : {
													 x = (WINDOW->topxpos+x-1);
													 y = (WINDOW->topypos+y-1);
													 break;
												 }
				case FRAME_CH : {
													x = (WINDOW->frametopxpos+x-1);
													y = (WINDOW->frametopypos+y-1);
													break;
												}
				case TEXT_CH : {
												 x = (WINDOW->texttopxpos+x-1);
												 y = (WINDOW->texttopypos+y-1);
												 break;
											 }
			}

			/* Search ti2_windows under this one for a character */
			for (i = (pos - 1); i >= 0; i--) {
				/* See if window that we are checking is hidden */
				if (!(ti2_windows[i]->attr & WIN_HIDDEN)) {
					/* See if we are in the frame of the window beneath us and if we are a see-thru window */
					if (ti2_windows[i]->attr & WIN_SEETHRU) {
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
							/* Check frame overlap */
							if (ti2_windows[i]->frame) {
								/* Check all sides for overlap */
								/* Top */
								if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos) ||
								/* Bottom */
										WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos) ||
								/* Left */
										WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos) ||
								/* Right */
										WIN_XY_IN_RANGE(x, y, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {

									hidden_window = i;
									result = 1;
									break;
								}
							}
						}
					}
						else
					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
						hidden_window = i;
						result = 1;
						break;
					}

					/* See if we are in the shadow of the window on top */
					if (ti2_windows[i]->shadow == WIN_SHADOW_RIGHT) {
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->botxpos, ti2_windows[i]->topypos+1, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
							/* This character is visible, but in the shadow of another window */
							hidden_window = i;
							result = 1;
							break;
						}
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos+1, ti2_windows[i]->botypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
							/* This character is visible, but in the shadow of another window */
							hidden_window = i;
							result = 1;
							break;
						}
					}
						else
					if (ti2_windows[i]->shadow == WIN_SHADOW_LEFT) {
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos-1, ti2_windows[i]->topypos)) {
							/* This character is visible, but in the shadow of another window */
							hidden_window = i;
							result = 1;
							break;
						}
						if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->topxpos, ti2_windows[i]->botypos-1)) {
							/* This character is visible, but in the shadow of another window */
							hidden_window = i;
							result = 1;
							break;
						}
					}
				}
			}

			/* If we found the hidden character in this window than copy it to buffer and return */
			if (result) {
				offx = (x - ti2_windows[hidden_window]->topxpos);
				offy = (y - ti2_windows[hidden_window]->topypos);

				buffer[0] = ti2_windows[hidden_window]->text[offy][(offx * 2)];
				buffer[1] = ti2_windows[hidden_window]->text[offy][(offx * 2) + 1];
			}
		}

void win_c::unhide_ch(uchar x, uchar y, uchar ch_type)
		{
			int i;                  // General counter
			uchar result = 0;
			uchar offx, offy;       // X/Y offsets in buffer of hidden character
			uchar visibility;       // Holds visibility of this hidden character
      char buffer[2];         // Holds character and attribute of character being unhidden

			/* Get visibility */
			visibility = visible_ch(x, y, ch_type);

			/* Get hidden character */
			get_hidden_ch(x, y, ch_type, buffer);

			/* Make coordinates absolte */
			switch(ch_type) {
				case SHADOW_CH : {
													 x = (WINDOW->topxpos+x-1);
													 y = (WINDOW->topypos+y-1);
													 break;
												 }
				case FRAME_CH : {
													x = (WINDOW->frametopxpos+x-1);
													y = (WINDOW->frametopypos+y-1);
													break;
												}
				case TEXT_CH : {
												 x = (WINDOW->texttopxpos+x-1);
												 y = (WINDOW->texttopypos+y-1);
												 break;
											 }
			}

			/* Display unhidden character */
			switch(visibility) {
				case VISIBLE : {
												 /* Turn mouse off */
												 mouse_off();

												 PUTTEXT(x, y, x, y, buffer);

												 /* Turn mouse on */
												 mouse_on();
												 break;
											 }
				case SHADOW_VISIBLE :
				case SHADOW_NOT_VISIBLE : {
																		/* Make the color of the character shadow */
																		buffer[1] = MAKE_TEXT_ATTR(WIN_SHADOW_FCOLOR, WIN_SHADOW_BCOLOR);

																		/* Undate shadow of the ti2_windows that shadow ours */
																		for (i = (pos + 1); i < ti2_num_windows; i++) {
																			/* Reset variable */
																			result = 0;
																			/* See if window that we are checking is hidden and wether our point could even fall inside of this window */
																			if (!(ti2_windows[i]->attr & WIN_HIDDEN) && WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																				/* If this is a see-thru window see if our falls in the frame */
																				if (ti2_windows[i]->attr & WIN_SEETHRU) {
																					/* See if there is any chance we fall inside the frame or see thru part */
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																						/* Check frame overlap */
																						if (ti2_windows[i]->frame) {
																							/* Check all sides for overlap */
																							/* Top */
																							if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos) ||
																							/* Bottom */
																								 WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos) ||
																							/* Left */
																								 WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos) ||
																							/* Right */
																								 WIN_XY_IN_RANGE(x, y, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																								break;
																							}
																						}
																					}
																				}
																					else
																				/* See if we are in the frame of the window on top */
																				if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																					break;
																				}

																				/* See if we are in the shadow of the window on top */
																				if (ti2_windows[i]->shadow == WIN_SHADOW_RIGHT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->botxpos, ti2_windows[i]->topypos+1, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos+1, ti2_windows[i]->botypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}
																					else
																				if (ti2_windows[i]->shadow == WIN_SHADOW_LEFT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos-1, ti2_windows[i]->topypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->topxpos, ti2_windows[i]->botypos-1)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}

																				/* Update the shadow of this window if we found one */
																				if (result) {
																					offx = (x - ti2_windows[i]->topxpos);
																					offy = (y - ti2_windows[i]->topypos);

																					ti2_windows[i]->text[offy][(offx * 2)] = buffer[0];
																					ti2_windows[i]->text[offy][(offx * 2) + 1] = buffer[1];
																				}
																			}
																		}
																		/* If this character is visible display it */
																		if (visibility == SHADOW_VISIBLE) {
																			/* Turn mouse off */
																			mouse_off();

																			PUTTEXT(x, y, x, y, buffer);

																			/* Turn mouse on */
																			mouse_on();
																		}
																		break;
																	}
			}
		}

void win_c::unhide_buffer_ch(char *scrn_buffer, uchar x, uchar y, uchar ch_type)
		{
			int i;                  // General counter
			uchar result = 0;
			uchar offx, offy;       // X/Y offsets in buffer of hidden character
			uchar visibility;       // Holds visibility of this hidden character
      char buffer[2];         // Holds character and attribute of character being unhidden
      ulong scrn_offset;      // Offset into screen buffer
      uchar win_x, win_y;     // Offset of x and y in the window

			/* Calculate local x and y */
			switch(ch_type) {
				case SHADOW_CH : {
													 win_x = x;
													 win_y = y;
													 break;
												 }
				case FRAME_CH : {
													win_y = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y;
													win_x = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x;
													break;
												}
				case TEXT_CH : {
												 win_y = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y + (WINDOW->frame ? 1 : 0);
												 win_x = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x + (WINDOW->frame ? 1 : 0);
												 break;
											 }
			}

			/* Get visibility */
			visibility = visible_ch(x, y, ch_type);

			/* Get hidden character */
			get_hidden_ch(x, y, ch_type, buffer);

			/* Make coordinates absolte */
			switch(ch_type) {
				case SHADOW_CH : {
													 x = (WINDOW->topxpos+x-1);
													 y = (WINDOW->topypos+y-1);
													 break;
												 }
				case FRAME_CH : {
													x = (WINDOW->frametopxpos+x-1);
													y = (WINDOW->frametopypos+y-1);
													break;
												}
				case TEXT_CH : {
												 x = (WINDOW->texttopxpos+x-1);
												 y = (WINDOW->texttopypos+y-1);
												 break;
											 }
			}

			/* Display unhidden character */
			switch(visibility) {
				case VISIBLE : {
												 /* Calculate offset into screen buffer */
												 scrn_offset = ((win_y - 1) * WIN_ABS_WIDTH * 2) + ((win_x - 1) * 2);
												 scrn_buffer[scrn_offset] = buffer[0];
												 scrn_buffer[scrn_offset + 1] = buffer[1];
												 break;
											 }
				case SHADOW_VISIBLE :
				case SHADOW_NOT_VISIBLE : {
																		/* Make the color of the character shadow */
																		buffer[1] = MAKE_TEXT_ATTR(WIN_SHADOW_FCOLOR, WIN_SHADOW_BCOLOR);

																		/* Undate shadow of the ti2_windows that shadow ours */
																		for (i = (pos + 1); i < ti2_num_windows; i++) {
																			/* Reset variable */
																			result = 0;
																			/* See if window that we are checking is hidden and wether our point could even fall inside of this window */
																			if (!(ti2_windows[i]->attr & WIN_HIDDEN) && WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																				/* If this is a see-thru window see if our falls in the frame */
																				if (ti2_windows[i]->attr & WIN_SEETHRU) {
																					/* See if there is any chance we fall inside the frame or see thru part */
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																						/* Check frame overlap */
																						if (ti2_windows[i]->frame) {
																							/* Check all sides for overlap */
																							/* Top */
																							if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos) ||
																							/* Bottom */
																								 WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos) ||
																							/* Left */
																								 WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->frametopxpos, ti2_windows[i]->framebotypos) ||
																							/* Right */
																								 WIN_XY_IN_RANGE(x, y, ti2_windows[i]->framebotxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																								break;
																							}
																						}
																					}
																				}
																					else
																				/* See if we are in the frame of the window on top */
																				if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																					break;
																				}

																				/* See if we are in the shadow of the window on top */
																				if (ti2_windows[i]->shadow == WIN_SHADOW_RIGHT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->botxpos, ti2_windows[i]->topypos+1, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos+1, ti2_windows[i]->botypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}
																					else
																				if (ti2_windows[i]->shadow == WIN_SHADOW_LEFT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos-1, ti2_windows[i]->topypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->topxpos, ti2_windows[i]->botypos-1)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}

																				/* Update the shadow of this window if we found one */
																				if (result) {
																					offx = (x - ti2_windows[i]->topxpos);
																					offy = (y - ti2_windows[i]->topypos);

																					ti2_windows[i]->text[offy][(offx * 2)] = buffer[0];
																					ti2_windows[i]->text[offy][(offx * 2) + 1] = buffer[1];
																				}
																			}
																		}
																		/* If this character is visible display it */
																		if (visibility == SHADOW_VISIBLE) {
																			/* Calculate offset into screen buffer */
																			scrn_offset = ((win_y - 1) * WIN_ABS_WIDTH * 2) + ((win_x - 1) * 2);
																			scrn_buffer[scrn_offset] = buffer[0];
																			scrn_buffer[scrn_offset + 1] = buffer[1];
																		}
																		break;
																	}
			}
		}

void win_c::center(ushort type)
		{
			uchar diff;                   // General difference holder

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if (WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Hide window if they specified horizontal or vertical centering */
			if ((type & WIN_HORIZ_CNTR) || (type & WIN_VERT_CNTR)) hide();
				else
			{
				EXITCRITSEC();
				return;
			}

			/* Change coordinates of ti2_windows */
			if (type & WIN_HORIZ_CNTR) {
        diff = ((SCREENWIDTH() / 2) - (WIN_FRAME_WIDTH / 2) + 1) - WINDOW->frametopxpos;
				if (diff) {
					WINDOW->topxpos += diff;
					WINDOW->frametopxpos += diff;
					WINDOW->texttopxpos += diff;
					WINDOW->botxpos += diff;
					WINDOW->framebotxpos += diff;
					WINDOW->textbotxpos += diff;
				}
			}
			if (type & WIN_VERT_CNTR) {
        diff = ((SCREENHEIGHT() / 2) - (WIN_FRAME_HEIGHT / 2) + 1) - WINDOW->frametopypos;
				if (diff) {
					WINDOW->topypos += diff;
					WINDOW->frametopypos += diff;
					WINDOW->texttopypos += diff;
					WINDOW->botypos += diff;
					WINDOW->framebotypos += diff;
					WINDOW->textbotypos += diff;
				}
			}

			/* Unhide window in new position */
			unhide();

			EXITCRITSEC();
		}

uchar win_c::def_create(uchar new_topx, uchar new_topy, uchar new_botx, uchar new_boty, char *new_title,
												uchar new_frame, uchar new_shadow, ushort new_attr)
		{
			uchar result;		// Result from window creation

			ENTERCRITSEC();

			/* Call other create functions with the default colors */
			result = create(new_topx, new_topy, new_botx, new_boty, ti2_win_default_colors.f_fcolor,
											ti2_win_default_colors.f_bcolor, ti2_win_default_colors.t_fcolor,
											ti2_win_default_colors.t_bcolor, new_title, ti2_win_default_colors.title_fcolor,
											ti2_win_default_colors.title_bcolor, new_frame, new_shadow, new_attr);

			EXITCRITSEC();
			return(result);
		}

uchar win_c::create(uchar new_topx,     uchar new_topy,         uchar new_botx,         uchar new_boty,
										uchar new_f_fcolor, uchar new_f_bcolor,     uchar new_t_fcolor,     uchar new_t_bcolor,
                    char *new_title,    uchar new_title_fcolor, uchar new_title_bcolor, uchar new_frame,
										uchar new_shadow,   ushort new_attr)
		{
			int width;                    // Used when centering ti2_windows on screen
			int height;                   // Used when centering ti2_windows on screen
			uchar i, j;                   // General counter
			uchar exp_x1, exp_y1, exp_x2, exp_y2;

			ENTERCRITSEC();

			/* If window system not initialized return unsuccessful */
			if (!ti2_initialized) {
				EXITCRITSEC();
				return(0);
			}

			/* Get a handle */
			if (!get_handle()) {
				/* To many ti2_windows or memory allocation error */
        printf("\nwin_create() error : Too many ti2_windows or allocation error.\n");
				EXITCRITSEC();
				return(0);
			}

			/* Get position */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Make sure text window will be at least one high and two wide */
			if (new_frame) {
				if ((new_boty - new_topy + 1) < 3) new_boty = (new_topy + 2);
				if ((new_botx - new_topx + 1) < 4) new_botx = (new_topx + 3);
			}
				else
			{
				if ((new_boty - new_topy + 1) < 1) new_boty = new_topy;
				if ((new_botx - new_topx + 1) < 2) new_botx = (new_topx + 1);
			}

			/* Check for WIN_HORIZ_CNTR and WIN_VERT_CNTR and change coordinates */
			if (new_attr & WIN_HORIZ_CNTR) {
				width = new_botx - new_topx + 1;
        new_topx = (SCREENWIDTH() /2) - (width/2) + 1;
				new_botx = new_topx + width - 1;
			}
			if (new_attr & WIN_VERT_CNTR) {
				height = new_boty - new_topy + 1;
        new_topy = (SCREENHEIGHT() /2) - (height/2) + 1;
				new_boty = new_topy + height - 1;
			}

			/* Exploding window coordinate preparation */
			if (new_attr & WIN_EXPLODE) {
				/* Store final coordinates */
				exp_x1 = new_topx;
				exp_y1 = new_topy;
				exp_x2 = new_botx;
				exp_y2 = new_boty;
				/* Calculate new coordinates */
				while (((new_botx - new_topx + 1) > 3) || ((new_boty - new_topy + 1) > 3)) {
					if ((new_botx - new_topx + 1) > 3) new_topx++;
					if ((new_botx - new_topx + 1) > 3) new_botx--;
					if ((new_boty - new_topy + 1) > 3) new_topy++;
					if ((new_boty - new_topy + 1) > 3) new_boty--;
				}
				/* Make sure starting exploding window isn't bigger than the final one */
				if (new_topx < exp_x1) new_topx = exp_x1;
				if (new_topy < exp_y1) new_topy = exp_y1;
				if (new_botx > exp_x2) new_botx = exp_x2;
				if (new_boty > exp_y2) new_boty = exp_y2;
			}

			/* Initialize window structure */
			WINDOW->topxpos = new_topx;
			WINDOW->topypos = new_topy;
			WINDOW->botxpos = new_botx;
			WINDOW->botypos = new_boty;

			WINDOW->frametopxpos = new_topx;
			WINDOW->frametopypos = new_topy;
			WINDOW->framebotxpos = new_botx;
			WINDOW->framebotypos = new_boty;

			WINDOW->frame = new_frame;
			f_fcolor = new_f_fcolor;
			f_bcolor = new_f_bcolor;
			t_fcolor = new_t_fcolor;
			t_bcolor = new_t_bcolor;

			WINDOW->text = NULL;
			title = NULL;

			vscroll_cur = vscroll_max = hscroll_cur = hscroll_max = 0;

			WINDOW->shadow = new_shadow;

			if (WINDOW->frame) {            // Set text window part to correct value
				WINDOW->texttopxpos = new_topx + 1;
				WINDOW->texttopypos = new_topy + 1;
				WINDOW->textbotxpos = new_botx - 1;
				WINDOW->textbotypos = new_boty - 1;
			}
				else
			{
				WINDOW->texttopxpos = new_topx;
				WINDOW->texttopypos = new_topy;
				WINDOW->textbotxpos = new_botx;
				WINDOW->textbotypos = new_boty;
			}
			switch(WINDOW->shadow) {
				case 0 : {
									 break;
								 }
				case 1 : {
									 WINDOW->botxpos += 1;
									 WINDOW->botypos += 1;
									 break;
								 }
				case 2 : {
									 WINDOW->topxpos -= 1;
									 WINDOW->topypos -= 1;
									 break;
								 }
			}

      /* Check function parameters for valid arguments */
      if (new_topx < 1) {
        printf("\nwin_create() error : topx = %d < 1\n", new_topx);
        EXITCRITSEC();
        return(0);
      }
      if (new_topx > SCREENWIDTH()) {
        printf("\nwin_create() error : topx = %d > screenwidth = %d\n", new_topx, SCREENWIDTH());
        EXITCRITSEC();
        return(0);
      }
      if (new_topy < 1) {
        printf("\nwin_create() error : topy = %d < 1\n", new_topy);
        EXITCRITSEC();
        return(0);
      }
      if (new_topy > SCREENHEIGHT()) {
        printf("\nwin_create() error : topy = %d > screenheight = %d\n", new_topy, SCREENHEIGHT());
        EXITCRITSEC();
        return(0);
      }
      if (new_botx < 1) {
        printf("\nwin_create() error : botx = %d < 1\n", new_botx);
        EXITCRITSEC();
        return(0);
      }
      if (new_botx > SCREENWIDTH()) {
        printf("\nwin_create() error : botx = %d > screenwidht = %d\n", new_botx, SCREENWIDTH());
        EXITCRITSEC();
        return(0);
      }
      if (new_boty < 1) {
        printf("\nwin_create() error : boty = %d < 1\n", new_boty);
        EXITCRITSEC();
        return(0);
      }
      if (new_boty > SCREENHEIGHT()) {
        printf("\nwin_create() error : boty = %d > screenheight = %d\n", new_boty, SCREENHEIGHT());
        EXITCRITSEC();
        return(0);
      }
      if (new_botx < new_topx) {
        printf("\nwin_create() error : botx = %d < topx = %d\n", new_botx, new_topx);
        EXITCRITSEC();
        return(0);
      }
      if (new_boty < new_topy) {
        printf("\nwin_create() error : boty = %d < topy = %d\n", new_boty, new_topy);
        EXITCRITSEC();
        return(0);
      }
      if (new_frame > 11) {
        printf("\nwin_create() error : frame = %d > 11\n", new_frame);
        EXITCRITSEC();
        return(0);
      }
      if (new_shadow > 2) {
        printf("\nwin_create() error : shadow = %d > 2\n", new_shadow);
        EXITCRITSEC();
        return(0);
      }

			/* Copy the title */
      title = (char *) malloc(sizeof(char) * (strlen(new_title) + 1));
			strcpy(title, new_title);
			title_fcolor = new_title_fcolor;
			title_bcolor = new_title_bcolor;

			WINDOW->attr = new_attr;

			/* Make sure they didn't set WIN_ACTIVE or any other conflicting attributes in the attribute */
			WINDOW->attr &= ~(WIN_ACTIVE);
			if (WINDOW->attr & WIN_TITLE_LEFT) WINDOW->attr &= ~(WIN_TITLE_CENTER | WIN_TITLE_RIGHT);
				else
			if (WINDOW->attr & WIN_TITLE_CENTER) WINDOW->attr &= ~(WIN_TITLE_RIGHT);

			/* Set cursor mode */
			if (WINDOW->attr & WIN_CURSOR) cursor_mode = 1;
				else cursor_mode = 0;
			/* Set scroll mode */
			if (WINDOW->attr & WIN_SCROLL) scroll_mode = 1;
				else scroll_mode = 0;

			/* Initialize array to hold text in window */
      if ((WINDOW->text = (char FAR * FAR *) FARCALLOC(WIN_ABS_HEIGHT, sizeof(char FAR *))) == NULL) {
				/* Error allocating array */
        printf("\nwin_create() error : Allocating buffer to store text.\n");
				EXITCRITSEC();
				return(0);
			}
			for (i = 0; i < WIN_ABS_HEIGHT; i++) {
        if ((WINDOW->text[i] = (char FAR *) FARCALLOC(WIN_ABS_WIDTH*2, sizeof(char))) == NULL) {
					/* Error allocating array */
					FARFREE(WINDOW->text);
          printf("\nwin_create() error : Allocating buffer to store text.\n");
					EXITCRITSEC();
					return(0);
				}
			}

			/* Initialize window array */
			for (i = 0; i < WIN_ABS_HEIGHT; i++) FARMEMSET(WINDOW->text[i], 0, WIN_ABS_WIDTH*2);

			/* Set WINDOW.attr to WIN_ACTIVE since window was successfully created */
			WINDOW->attr |= WIN_ACTIVE;

			/* Clear window where frame and text window is */
			for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
				for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(' ', j, i, t_fcolor, t_bcolor);
			}

			/* Redraw frame etc. */
			redraw();

			/* Draw text window */
			curr_x = 1;
			curr_y = 1;

			/* Set curr foreground and background colors */
			text_color(t_fcolor);
			text_background(t_bcolor);

			/* Display window */
			display_part(SHADOW | FRAME | TEXT);

			/* Explode window if it is supposed to */
			if (WINDOW->attr & WIN_EXPLODE) explode(exp_x1, exp_y1, exp_x2, exp_y2);

			EXITCRITSEC();
			return(1);
		}

void win_c::redraw(void)
		{
			int i, j, k;        // General counters
      char scrnpnt[2];    // Buffer to hold shadow characters

																												/* Characters for the bolders */
      const char frame_char_topleft[12]         = {' ','É','Ú','Õ','Ö','°','-','±','²','Û','Û',' '};
      const char frame_char_topright[12]        = {' ','»','¿','¸','·','°','-','±','²','Û','Û',' '};
      const char frame_char_bottomleft[12]      = {' ','È','À','Ô','Ó','°','-','±','²','Û','Û',' '};
      const char frame_char_bottomright[12]     = {' ','¼','Ù','¾','½','°','-','±','²','Û','Û',' '};
      const char frame_char_horiztopside[12]    = {' ','Í','Ä','Í','Ä','°','-','±','²','ß','Û',' '};
      const char frame_char_horizbottomside[12] = {' ','Í','Ä','Í','Ä','°','-','±','²','Ü','Û',' '};
      const char frame_char_vertside[12]        = {' ','º','³','³','º','°','|','±','²','Û','Û',' '};

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) return;

			/* Check if window is hidden */
			if (WINDOW->attr & WIN_HIDDEN) return;

			/* Draw frame */
			if (WINDOW->frame) {
				/* Top left corner */
				WIN_FRAME_PUTCH(frame_char_topleft[WINDOW->frame], 1, 1, f_fcolor, f_bcolor);
				/* Top line */
				for (j = 1; j <= (WIN_FRAME_WIDTH - 2); j++) WIN_FRAME_PUTCH(frame_char_horiztopside[WINDOW->frame], j+1, 1, f_fcolor, f_bcolor);
				/* Top right corner */
				WIN_FRAME_PUTCH(frame_char_topright[WINDOW->frame], WIN_FRAME_WIDTH, 1, f_fcolor, f_bcolor);
				/* Left line */
				for (j = 1; j <= (WIN_FRAME_HEIGHT - 2); j++) WIN_FRAME_PUTCH(frame_char_vertside[WINDOW->frame], 1, j+1, f_fcolor, f_bcolor);
				/* Bottom left corner */
				WIN_FRAME_PUTCH(frame_char_bottomleft[WINDOW->frame], 1, WIN_FRAME_HEIGHT, f_fcolor, f_bcolor);
				/* Bottom line */
				for (j = 1; j <= (WIN_FRAME_WIDTH - 2); j++) WIN_FRAME_PUTCH(frame_char_horizbottomside[WINDOW->frame], j+1, WIN_FRAME_HEIGHT, f_fcolor, f_bcolor);
				/* Bottom right corner */
				WIN_FRAME_PUTCH(frame_char_bottomright[WINDOW->frame], WIN_FRAME_WIDTH, WIN_FRAME_HEIGHT, f_fcolor, f_bcolor);
				/* Right line */
				for (j = 1; j <= (WIN_FRAME_HEIGHT - 2); j++) WIN_FRAME_PUTCH(frame_char_vertside[WINDOW->frame], WIN_FRAME_WIDTH, j+1, f_fcolor, f_bcolor);
			}

			/* Draw WINDOW.shadow */
			switch(WINDOW->shadow) {
				case WIN_SHADOW_NONE : {
																 break;
															 }
				case WIN_SHADOW_RIGHT : { // Bottom and right WINDOW.shadow
																	for (i = 2; i <= WIN_ABS_WIDTH; i++) {
																		get_hidden_ch(i, WIN_ABS_HEIGHT, SHADOW_CH, scrnpnt);
																		WIN_SHADOW_PUTCH(scrnpnt[0], i, WIN_ABS_HEIGHT);
																	}

																	for (i = 2; i <= WIN_ABS_HEIGHT; i++) {
																		get_hidden_ch(WIN_ABS_WIDTH, i, SHADOW_CH, scrnpnt);
																		WIN_SHADOW_PUTCH(scrnpnt[0], WIN_ABS_WIDTH, i);
																	}
																	break;
																}
				case WIN_SHADOW_LEFT : {  // Top and left WINDOW.shadow
																 for (i = 1; i < WIN_ABS_WIDTH; i++) {
																	 get_hidden_ch(i, 1, SHADOW_CH, scrnpnt);
																	 WIN_SHADOW_PUTCH(scrnpnt[0], i, 1);
																 }

																 for (i = 1; i < WIN_ABS_HEIGHT; i++) {
																	 get_hidden_ch(1, i, SHADOW_CH, scrnpnt);
																	 WIN_SHADOW_PUTCH(scrnpnt[0], 1, i);
																 }
																 break;
															 }
			}

			/* Place title on top or bottom of window frame.  If WIN_TITLE_LEFT, RIGHT or CENTER is specified than print no title */
			if (WINDOW->frame) {
				j = min ((uchar) strlen(title), (uchar) (WIN_FRAME_WIDTH - 2));
				if (WINDOW->attr & WIN_TITLE_LEFT) {
					for (i = 0; i < j; i++) WIN_FRAME_PUTCH(title[i], 2+i, ((WINDOW->attr & WIN_TITLE_BOTTOM) ? WIN_FRAME_HEIGHT : 1), title_fcolor, title_bcolor);
				}
					else
				if (WINDOW->attr & WIN_TITLE_CENTER) {
					k = (WIN_FRAME_WIDTH / 2) + 1 - (j / 2);
					for (i = 0; i < j; i++) WIN_FRAME_PUTCH(title[i], k+i, ((WINDOW->attr & WIN_TITLE_BOTTOM) ? WIN_FRAME_HEIGHT : 1), title_fcolor, title_bcolor);
				}
					else
				if (WINDOW->attr & WIN_TITLE_RIGHT) {
					k = (WIN_FRAME_WIDTH - j);
					for (i = 0; i < j; i++) WIN_FRAME_PUTCH(title[i], k+i, ((WINDOW->attr & WIN_TITLE_BOTTOM) ? WIN_FRAME_HEIGHT : 1), title_fcolor, title_bcolor);
				}
			}

			/* Update scroll bars if there are scroll bars for this window. */
			if (WINDOW->attr & WIN_VSCROLL_BAR) vscroll_update();
			if (WINDOW->attr & WIN_HSCROLL_BAR) hscroll_update();
		}

uchar win_c::write_text_to_file(char *filename, uchar newline)
		{
			FILE *out;      // File handle for text file
			uchar ch;       // Holds character to be written
			uchar i, j;     // General counters

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Out output file */
			if ((out = fopen(filename, "wt")) == NULL) {
				/* Error opening return */
				EXITCRITSEC();
				return(0);
			}

			/* Write all the text to the file */
			for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
				/* Write all the characters for this line */
				for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
					WIN_TEXT_GETCH(ch, j, i);
					fputc(ch, out);
				}
				/* Write line feed if we are supposed to */
				if (newline) fputc('\n', out);
			}

			/* Close file */
			fclose(out);

			/* Return successful */
			EXITCRITSEC();
			return(1);
		}

void win_c::read_char(char *buffer, uchar x, uchar y, ushort len)
		{
			ushort buffer_pos = 0;    // Position in buffer

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Read characters into buffer */
			while (len > 0) {
				/* Check for line wrap or bottom of the screen */
				if (x > WIN_TEXT_WIDTH) {
					y++;
					x = 1;
				}
				if (y > WIN_TEXT_HEIGHT) break;

				/* Load character into buffer */
				WIN_TEXT_GETCH(buffer[buffer_pos], x, y);
				buffer_pos++;
				len--;
				x++;
			}
			EXITCRITSEC();
		}

void win_c::read_attr(char *buffer, uchar x, uchar y, ushort len)
		{
			ushort buffer_pos = 0;    // Position in buffer

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Read attributes into buffer */
			while (len > 0) {
				/* Check for line wrap or bottom of the screen */
				if (x > WIN_TEXT_WIDTH) {
					y++;
					x = 1;
				}
				if (y > WIN_TEXT_HEIGHT) break;

				/* Load attribute into buffer */
				WIN_TEXT_GETATTR(buffer[buffer_pos], x, y);
				buffer_pos++;
				len--;
				x++;
			}
			EXITCRITSEC();
		}

void win_c::read_char_and_attr(char *buffer, uchar x, uchar y, ushort len)
		{
			ushort buffer_pos = 0;    // Position in buffer

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Read characters and attributes into buffer */
			while (len > 0) {
				/* Check for line wrap or bottom of the screen */
				if (x > WIN_TEXT_WIDTH) {
					y++;
					x = 1;
				}
				if (y > WIN_TEXT_HEIGHT) break;

				/* Load character into buffer */
				WIN_TEXT_GETCH(buffer[buffer_pos], x, y);
				buffer_pos++;
				/* Load attribute into buffer */
				WIN_TEXT_GETATTR(buffer[buffer_pos], x, y);
				buffer_pos++;
				len--;
				x++;
			}
			EXITCRITSEC();
		}

void win_c::display_part(uchar part)
		{
      char *buffer;         // Holds a buffer that puttext will write directly to the screen
			ulong buffer_pos = 0; // Current position in buffer;
			uchar i, j;           // General counters

			/* Update position in window array */
			get_pos();

			/* Check for active window */
			if (!(WINDOW->attr & WIN_ACTIVE)) return;

			/* Check for hidden window */
			if (WINDOW->attr & WIN_HIDDEN) return;

			/* Turn mouse off */
			mouse_off();

			/* See if the whole window is visible, else draw whatever is */
			if (completely_visible()) {
				/* Find offset because of shadow */
				j = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0);

				/* Display correct parts of window */
				if (part & SHADOW) {
					/* Display shadow */
					if (WINDOW->shadow == WIN_SHADOW_LEFT) {
						/* Reset variables */
						buffer_pos = 0;
						/* Allocate a buffer that can hold the vertical and horizontal shadow */
            if ((buffer = (char *) malloc(max(WIN_ABS_HEIGHT-1, WIN_ABS_WIDTH-1) * 2)) == NULL) {
							/* Error creating buffer */
              printf("\nwin_display_abs() error : Allocating buffer for puttext.\n");
							return;
						}
						/* Display horizontal shadow */
						FARMEMCPY(buffer, &WINDOW->text[0][0], CALC_SCRN_BUFFER(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos-1, WINDOW->topypos));
						PUTTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos-1, WINDOW->topypos, buffer);
						/* Copy data from vertical shadow to buffer */
						for (i = 0; i < WIN_ABS_HEIGHT - 1; i++) {
							FARMEMCPY(&buffer[buffer_pos], &WINDOW->text[i][0], 2);
							buffer_pos += 2;
						}
						/* Display vertical shadow */
						PUTTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->topxpos, WINDOW->botypos-1, buffer);
						/* Free buffer */
						FREE(buffer);
					}
						else
					if (WINDOW->shadow == WIN_SHADOW_RIGHT) {
						/* Reset variables */
						buffer_pos = 0;
						/* Allocate a buffer that can hold the vertical shadow */
            if ((buffer = (char *) malloc(max(WIN_ABS_HEIGHT-1, WIN_ABS_WIDTH-1) * 2)) == NULL) {
							/* Error creating buffer */
              printf("\nwin_display_abs() error : Allocating buffer for puttext.\n");
							return;
						}
						/* Display horizontal shadow */
						FARMEMCPY(buffer, &WINDOW->text[WIN_ABS_HEIGHT - 1][2], (WIN_ABS_WIDTH - 1) * 2);//CALC_SCRN_BUFFER(WINDOW.topxpos+1, WINDOW.botypos, WINDOW.botxpos, WINDOW.botypos));
						PUTTEXT(WINDOW->topxpos+1, WINDOW->botypos, WINDOW->botxpos, WINDOW->botypos, buffer);
						/* Copy data from vertical shadow to buffer */
						buffer_pos = 0;
						for (i = 1; i < WIN_ABS_HEIGHT; i++) {
							FARMEMCPY((void FAR *) &buffer[buffer_pos], &WINDOW->text[i][(WIN_ABS_WIDTH - 1)*2], 2);
							buffer_pos += 2;
						}
						/* Display vertical shadow */
						PUTTEXT(WINDOW->botxpos, WINDOW->topypos+1, WINDOW->botxpos, WINDOW->botypos, buffer);
						/* Free buffer */
						FREE(buffer);
					}
				}
				/* Make sure they have a frame */
				if (WINDOW->frame) {
					/* Top */
					if (part & FRAME_TOP) {
						/* Allocate a buffer that can hold the horizontal part of frame */
            if ((buffer = (char *) malloc(WIN_FRAME_WIDTH * 2)) == NULL) {
							/* Error creating buffer */
              printf("\nwin_display_abs() error : Allocating buffer for puttext.\n");
							return;
						}
						FARMEMCPY(buffer, &WINDOW->text[j][j*2], CALC_SCRN_BUFFER(WINDOW->frametopxpos, WINDOW->frametopypos, WINDOW->framebotxpos, WINDOW->frametopypos));
						PUTTEXT(WINDOW->frametopxpos, WINDOW->frametopypos, WINDOW->framebotxpos, WINDOW->frametopypos, buffer);
						FREE(buffer);
					}
					/* Bottom */
					if (part & FRAME_BOTTOM) {
            if ((buffer = (char *) malloc(WIN_FRAME_WIDTH * 2)) == NULL) {
							/* Error creating buffer */
              printf("\nwin_display_abs() error : Allocating buffer for puttext.\n");
							return;
						}
						FARMEMCPY(buffer, &WINDOW->text[j+WIN_FRAME_HEIGHT-1][j*2], CALC_SCRN_BUFFER(WINDOW->frametopxpos, WINDOW->framebotypos, WINDOW->framebotxpos, WINDOW->framebotypos));
						PUTTEXT(WINDOW->frametopxpos, WINDOW->framebotypos, WINDOW->framebotxpos, WINDOW->framebotypos, buffer);
						FREE(buffer);
					}
					/* Allocate buffer if we are displaying either of the sides */
					if ((part & FRAME_LEFT) || (part & FRAME_RIGHT)) {
						/* Allocate a buffer that can hold the vertical part of frame */
            if ((buffer = (char *) malloc(WIN_FRAME_HEIGHT * 2)) == NULL) {
							/* Error creating buffer */
              printf("\nwin_display_abs() error : Allocating buffer for puttext.\n");
							return;
						}
						/* Left */
						if (part & FRAME_LEFT) {
							/* Reset variables */
							buffer_pos = 0;
							/* Copy data from vertical shadow to buffer */
							for (i = 0; i < WIN_FRAME_HEIGHT; i++) {
								FARMEMCPY(&buffer[buffer_pos], &WINDOW->text[j+i][j*2], 2);
								buffer_pos += 2;
							}
							/* Display vertical shadow */
							PUTTEXT(WINDOW->frametopxpos, WINDOW->frametopypos, WINDOW->frametopxpos, WINDOW->framebotypos, buffer);
						}
						/* Right */
						if (part & FRAME_RIGHT) {
							/* Reset variables */
							buffer_pos = 0;
							/* Copy data from vertical shadow to buffer */
							for (i = 0; i < WIN_FRAME_HEIGHT; i++) {
								FARMEMCPY(&buffer[buffer_pos], &WINDOW->text[j+i][(j + WIN_FRAME_WIDTH - 1) * 2], 2);
								buffer_pos += 2;
							}
							/* Display vertical shadow */
							PUTTEXT(WINDOW->framebotxpos, WINDOW->frametopypos, WINDOW->framebotxpos, WINDOW->framebotypos, buffer);
						}
						/* Free buffer */
						FREE(buffer);
					}
				}
				if (part & TEXT) {
					/* Reset variables */
					buffer_pos = 0;
					/* Allocate a buffer that can hold the text */
          if ((buffer = (char *) malloc(WIN_TEXT_HEIGHT * WIN_TEXT_WIDTH * 2)) == NULL) {
						/* Error creating buffer */
            printf("\nwin_display_abs() error : Allocating buffer for puttext.\n");
						return;
					}
					/* Copy data from text array to buffer */
					if (!(WINDOW->attr & WIN_SEETHRU)) {
						for (i = 0; i < WIN_TEXT_HEIGHT; i++) {
							FARMEMCPY(&buffer[buffer_pos], &WINDOW->text[j + i + (WINDOW->frame ? 1 : 0)][(j + (WINDOW->frame ? 1 : 0)) *2], WIN_TEXT_WIDTH * 2);
							buffer_pos += (WIN_TEXT_WIDTH * 2);
						}
					}
						else
					{
						/* Get all the characters that are in the see thru window */
						for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
							for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
								get_hidden_ch(j, i, TEXT_CH, &buffer[buffer_pos]);
								buffer_pos += 2;
							}
						}
					}
					/* Put Text to screen */
					PUTTEXT(WINDOW->texttopxpos, WINDOW->texttopypos, WINDOW->textbotxpos, WINDOW->textbotypos, buffer);
					/* Free buffer */
					FREE(buffer);
				}
			}
				else
			{
				/* Allocate buffer that is the size of the whole window on the screen */
        if ((buffer = (char *) malloc(WIN_ABS_HEIGHT * WIN_ABS_WIDTH * 2)) == NULL) {
					/* Error creating buffer */
          printf("win_display_abs() error : Allocating buffer for puttext.\n");
					return;
				}

				/* Get the area of the screen where this window appears */
				GETTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, buffer);

				/* Display correct parts of window */
				if (part & SHADOW) {
					/* Display right shadow if we have one */
					if (WINDOW->shadow == WIN_SHADOW_RIGHT) {
						/* Horizontal */
						for (i = 2; i <= WIN_ABS_WIDTH; i++) display_buffer_ch(buffer, i, WIN_ABS_HEIGHT, SHADOW_CH, visible_ch(i, WIN_ABS_HEIGHT, SHADOW_CH));
						/* Vertical */
						for (i = 2; i <= WIN_ABS_HEIGHT; i++) display_buffer_ch(buffer, WIN_ABS_WIDTH, i, SHADOW_CH, visible_ch(WIN_ABS_WIDTH, i, SHADOW_CH));
					}
						else
					if (WINDOW->shadow == WIN_SHADOW_LEFT) {
						/* Horizontal */
						for (i = 1; i < WIN_ABS_WIDTH; i++) display_buffer_ch(buffer, i, 1, SHADOW_CH, visible_ch(i, 1, SHADOW_CH));
						/* Vertical */
						for (i = 1; i < WIN_ABS_HEIGHT; i++) display_buffer_ch(buffer, 1, i, SHADOW_CH, visible_ch(1, i, SHADOW_CH));
					}
				}
				if (WINDOW->frame) {
					/* Top */
					if (part & FRAME_TOP) for (i = 1; i <= WIN_FRAME_WIDTH; i++) display_buffer_ch(buffer, i, 1, FRAME_CH, visible_ch(i, 1, FRAME_CH));
					/* Left */
					if (part & FRAME_LEFT) for (i = 1; i <= WIN_FRAME_HEIGHT; i++) display_buffer_ch(buffer, 1, i, FRAME_CH, visible_ch(1, i, FRAME_CH));
					/* Bottom */
					if (part & FRAME_BOTTOM) for (i = 1; i <= WIN_FRAME_WIDTH; i++) display_buffer_ch(buffer, i, WIN_FRAME_HEIGHT, FRAME_CH, visible_ch(i, WIN_FRAME_HEIGHT, FRAME_CH));
					/* Right */
					if (part & FRAME_RIGHT) for (i = 1; i <= WIN_FRAME_HEIGHT; i++) display_buffer_ch(buffer, WIN_FRAME_WIDTH, i, FRAME_CH, visible_ch(WIN_FRAME_WIDTH, i, FRAME_CH));
				}
				if (part & TEXT) {
					/* Display text.  Display_buffer_ch will get the characters for a see-thru window */
					for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
						for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
							display_buffer_ch(buffer, j, i, TEXT_CH, visible_ch(j, i, TEXT_CH));
						}
					}
				}

				/* Get the area of the screen where this window appears */
				PUTTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, buffer);

				/* Free Buffer */
				FREE(buffer);
			}

			/* Turn mouse on */
			mouse_on();
		}

void win_c::display_ch(uchar x, uchar y, uchar ch_type, uchar visibility)
		{
      char buffer[2];   // Holds a buffer that puttext will write directly to the screen
			uchar i;          // General counter
			uchar result = 0; // True if shadow found
			uchar offx, offy; // X, Y offset of window that has the shadow that overlays ours

			/* Copy text to buffer */
			switch(ch_type) {
				case SHADOW_CH : {
													 buffer[0] = WINDOW->text[y - 1][(x - 1) * 2];
													 buffer[1] = WINDOW->text[y - 1][((x - 1) * 2) + 1];
													 break;
												 }
				case FRAME_CH : {
													buffer[0] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - 1][(((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - 1) * 2];
													buffer[1] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - 1][((((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - 1) * 2) + 1];
													break;
												}
				case TEXT_CH : {
												 buffer[0] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)][(((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)) * 2];
												 buffer[1] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)][((((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)) * 2) + 1];
												 break;
											 }
			}

			/* Make coordinates absolute to screen */
			switch(ch_type) {
				case SHADOW_CH : {
													 x = (WINDOW->topxpos + x - 1);
													 y = (WINDOW->topypos + y - 1);
													 break;
												 }
				case FRAME_CH : {
													 x = (WINDOW->frametopxpos + x - 1);
													 y = (WINDOW->frametopypos + y - 1);
													break;
												}
				case TEXT_CH : {
													 x = (WINDOW->texttopxpos + x - 1);
													 y = (WINDOW->texttopypos + y - 1);
												 break;
											 }
			}

			/* Put text to screen in the correct way */
			switch(visibility) {
				case VISIBLE : {
												 PUTTEXT(x, y, x, y, buffer);
												 break;
											 }
				case SHADOW_VISIBLE :
				case SHADOW_NOT_VISIBLE : {
																		/* Make the color of the character shadow */
																		buffer[1] = MAKE_TEXT_ATTR(WIN_SHADOW_FCOLOR, WIN_SHADOW_BCOLOR);

																		/* Undate shadow of the ti2_windows that shadow ours */
																		for (i = (pos + 1); i < ti2_num_windows; i++) {
																			/* Reset variable */
																			result = 0;
																			/* See if window that we are checking is hidden and wether our point could even fall inside of this window */
																			if (!(ti2_windows[i]->attr & WIN_HIDDEN) && WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																				/* See if we are in the frame of the window on top */
																				if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																					break;
																				}

																				/* See if we are in the shadow of the window on top */
																				if (ti2_windows[i]->shadow == WIN_SHADOW_RIGHT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->botxpos, ti2_windows[i]->topypos+1, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos+1, ti2_windows[i]->botypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}
																					else
																				if (ti2_windows[i]->shadow == WIN_SHADOW_LEFT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos-1, ti2_windows[i]->topypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->topxpos, ti2_windows[i]->botypos-1)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}

																				/* Update the shadow of this window if we found one */
																				if (result) {
																					offx = (x - ti2_windows[i]->topxpos);
																					offy = (y - ti2_windows[i]->topypos);

																					ti2_windows[i]->text[offy][(offx * 2)] = buffer[0];
																					ti2_windows[i]->text[offy][(offx * 2) + 1] = buffer[1];
																				}
																			}
																		}
																		/* If this character is visible display it */
																		if (visibility == SHADOW_VISIBLE) {
																			PUTTEXT(x, y, x, y, buffer);
																		}
																		break;
																	}
			}
		}

void win_c::display_buffer_ch(char *scrn_buffer, uchar x, uchar y, uchar ch_type, uchar visibility)
		{
      char buffer[2];     // Holds a buffer that puttext will write directly to the screen
      uchar i;            // General counter
      uchar result = 0;   // True if shadow found
      uchar offx, offy;   // X, Y offset of window that has the shadow that overlays ours
			ulong scrn_offset;  // Offset into screen buffer
			uchar win_x, win_y; // Offset of x and y in the window

			/* Calculate local x and y */
			switch(ch_type) {
				case SHADOW_CH : {
													 win_x = x;
													 win_y = y;
													 break;
												 }
				case FRAME_CH : {
													win_y = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y;
													win_x = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x;
													break;
												}
				case TEXT_CH : {
												 win_y = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y + (WINDOW->frame ? 1 : 0);
												 win_x = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x + (WINDOW->frame ? 1 : 0);
												 break;
											 }
			}

			/* Copy text to buffer */
			switch(ch_type) {
				case SHADOW_CH : {
													 buffer[0] = WINDOW->text[y - 1][(x - 1) * 2];
													 buffer[1] = WINDOW->text[y - 1][((x - 1) * 2) + 1];
													 break;
												 }
				case FRAME_CH : {
													buffer[0] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - 1][(((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - 1) * 2];
													buffer[1] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - 1][((((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - 1) * 2) + 1];
													break;
												}
				case TEXT_CH : {
												 /* If window is see-thru load the character from the window beneath, else load the one from the text array */
												 if (!(WINDOW->attr & WIN_SEETHRU)) {
													 buffer[0] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)][(((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)) * 2];
													 buffer[1] = WINDOW->text[((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)][((((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)) * 2) + 1];
												 }
													 else
												 {
													 get_hidden_ch(x, y, TEXT_CH, buffer);
												 }
												 break;
											 }
			}

			/* Make coordinates absolute to screen */
			switch(ch_type) {
				case SHADOW_CH : {
													 x = (WINDOW->topxpos + x - 1);
													 y = (WINDOW->topypos + y - 1);
													 break;
												 }
				case FRAME_CH : {
													 x = (WINDOW->frametopxpos + x - 1);
													 y = (WINDOW->frametopypos + y - 1);
													break;
												}
				case TEXT_CH : {
													 x = (WINDOW->texttopxpos + x - 1);
													 y = (WINDOW->texttopypos + y - 1);
												 break;
											 }
			}

			/* Put text to screen in the correct way */
			switch(visibility) {
				case VISIBLE : {
												 /* Calculate offset into screen buffer */
												 scrn_offset = ((win_y - 1) * WIN_ABS_WIDTH * 2) + ((win_x - 1) * 2);
												 scrn_buffer[scrn_offset] = buffer[0];
												 scrn_buffer[scrn_offset + 1] = buffer[1];
												 break;
											 }
				case SHADOW_VISIBLE :
				case SHADOW_NOT_VISIBLE : {
																		/* Make the color of the character shadow */
																		buffer[1] = MAKE_TEXT_ATTR(WIN_SHADOW_FCOLOR, WIN_SHADOW_BCOLOR);

																		/* Undate shadow of the ti2_windows that shadow ours */
																		for (i = (pos + 1); i < ti2_num_windows; i++) {
																			/* Reset variable */
																			result = 0;
																			/* See if window that we are checking is hidden and wether our point could even fall inside of this window */
																			if (!(ti2_windows[i]->attr & WIN_HIDDEN) && WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																				/* See if we are in the frame of the window on top */
																				if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->frametopxpos, ti2_windows[i]->frametopypos, ti2_windows[i]->framebotxpos, ti2_windows[i]->framebotypos)) {
																					break;
																				}

																				/* See if we are in the shadow of the window on top */
																				if (ti2_windows[i]->shadow == WIN_SHADOW_RIGHT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->botxpos, ti2_windows[i]->topypos+1, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos+1, ti2_windows[i]->botypos, ti2_windows[i]->botxpos, ti2_windows[i]->botypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}
																					else
																				if (ti2_windows[i]->shadow == WIN_SHADOW_LEFT) {
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->botxpos-1, ti2_windows[i]->topypos)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																						else
																					if (WIN_XY_IN_RANGE(x, y, ti2_windows[i]->topxpos, ti2_windows[i]->topypos, ti2_windows[i]->topxpos, ti2_windows[i]->botypos-1)) {
																						/* This character is visible, but in the shadow of another window */
																						result = 1;
																					}
																				}

																				/* Update the shadow of this window if we found one */
																				if (result) {
																					offx = (x - ti2_windows[i]->topxpos);
																					offy = (y - ti2_windows[i]->topypos);

																					ti2_windows[i]->text[offy][(offx * 2)] = buffer[0];
																					ti2_windows[i]->text[offy][(offx * 2) + 1] = buffer[1];
																				}
																			}
																		}
																		/* If this character is visible display it */
																		if (visibility == SHADOW_VISIBLE) {
																			scrn_offset = ((win_y - 1) * WIN_ABS_WIDTH * 2) + ((win_x - 1) * 2);
																			scrn_buffer[scrn_offset] = buffer[0];
																			scrn_buffer[scrn_offset + 1] = buffer[1];
																		}
																		break;
																	}
			}
		}

void win_c::title_change(char *new_title)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* See if this window has a frame/title */
			if (!WINDOW->frame) {
				EXITCRITSEC();
				return;
			}

			/* Change title */
			FREE(title);
      title = (char *) malloc(sizeof(char) * (strlen(new_title) + 1));
			strcpy(title, new_title);
			/* Redraw window */
			redraw();
			display_part(SHADOW | ((WINDOW->attr & WIN_TITLE_BOTTOM) ? FRAME_BOTTOM : FRAME_TOP));
			EXITCRITSEC();
		}

void win_c::cursor_off(void)
		{
			ENTERCRITSEC();

			/* Set cursor mode */
			cursor_mode = 0;
			/* Turn cursor back on */
      SETCURSORTYPE(_NOCURSOR);

			EXITCRITSEC();
		}

void win_c::cursor_on(void)
		{
			ENTERCRITSEC();

			/* Set cursor mode */
			cursor_mode = 1;
			/* Turn cursor back on */
      SETCURSORTYPE(_NORMALCURSOR);

			EXITCRITSEC();
		}

void win_c::scroll_off(void)
		{
			/* Set scroll mode */
			scroll_mode = 0;
		}

void win_c::scroll_on(void)
		{
			/* Set scroll mode */
			scroll_mode = 1;
		}

void win_c::vscroll_create(uchar new_scroll_fcolor, uchar new_scroll_bcolor,
													 ushort new_vscroll_cur, ushort new_vscroll_max)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			 /* Check for active window */
			 if(!(WINDOW->attr & WIN_ACTIVE)) {
				 EXITCRITSEC();
				 return;
			 }

			 /* Update window structure to reflect the presence of the scroll bar */
			 WINDOW->attr |= (WIN_VSCROLL_BAR);

			 scroll_fcolor = new_scroll_fcolor;
			 scroll_bcolor = new_scroll_bcolor;

			 if (!new_vscroll_cur) new_vscroll_cur = 1;
			 if (!new_vscroll_max) new_vscroll_max = 1;

			 if (new_vscroll_cur > new_vscroll_max) new_vscroll_cur = new_vscroll_max;

			 vscroll_cur = new_vscroll_cur;
			 vscroll_max = new_vscroll_max;

			 vscroll_update();
			 EXITCRITSEC();
		}

void win_c::vscroll_destroy(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			 /* Check for active window */
			 if(!(WINDOW->attr & WIN_ACTIVE)) {
				 EXITCRITSEC();
				 return;
			 }

			 /* Check for presence of a vertical scroll bar */
			 if(!(WINDOW->attr & WIN_VSCROLL_BAR)) {
				 EXITCRITSEC();
				 return;
			 }

			 /* Clear has vertical scroll bar attribute */
			 WINDOW->attr &= ~(WIN_VSCROLL_BAR);

			 redraw();
			 display_part(FRAME_RIGHT);
			 EXITCRITSEC();
		}

ushort win_c::vscroll_mouse_move(void)
		{
			ushort vscroll_pos;
			float perc;           // Percentage of bar where clicked

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for presence of a horizontal scroll bar */
			if(!(WINDOW->attr & WIN_VSCROLL_BAR)) {
				EXITCRITSEC();
				return(0);
			}

			/* See if any of the buttons in this event were even pressed */
			if ((mouse_left == BUTTON_DOWN) || (mouse_right == BUTTON_DOWN) || (mouse_center == BUTTON_DOWN)) {
				/* See if coordinates of event are on the arrow keys of the scroll bar */
				if ((frame_x_relative(mouse_col) == WIN_FRAME_WIDTH) && (frame_y_relative(mouse_row) == 3)) {
					/* They clicked on the left arrow */
					if (vscroll_cur > 1) {
						vscroll_cur--;
						vscroll_update();
						DELAY(100);
						EXITCRITSEC();
						return(vscroll_cur);
					}
					EXITCRITSEC();
					return(0);
				}
				/* See if coordinates of event are on the arrow keys of the scroll bar */
				if ((frame_x_relative(mouse_col) == WIN_FRAME_WIDTH) && (frame_y_relative(mouse_row) == (WIN_FRAME_HEIGHT - 2))) {
					/* They clicked on the left arrow */
					if (vscroll_cur < vscroll_max) {
						vscroll_cur++;
						vscroll_update();
						DELAY(100);
						EXITCRITSEC();
						return(vscroll_cur);
					}
					EXITCRITSEC();
					return(0);
				}
				/* See if coordinates of event are on the actual scroll bar */
				if ((frame_y_relative(mouse_row) >= 4) && (frame_y_relative(mouse_row) <= (WIN_FRAME_HEIGHT - 3)) && (frame_x_relative(mouse_col) == WIN_FRAME_WIDTH)) {
					/* Calculate what number is represented by that position on the bar */
					perc = (((float) (frame_y_relative(mouse_row) - 3) / (float) (WIN_FRAME_HEIGHT - 6)) * (float) 100);
					perc = ceil(perc);
					perc = (perc / (float) 100);
					vscroll_pos = (ushort) (perc * (float) vscroll_max);
					if (!vscroll_pos) vscroll_pos = 1;
					/* Slide bar towards the new position */
					if (vscroll_pos != vscroll_cur) {
						if (vscroll_pos < vscroll_cur) vscroll_cur--;
							else vscroll_cur++;
						/* Make sure scroll bar position isn't out of range */
						if (vscroll_cur > vscroll_max) vscroll_cur = vscroll_max;
						/* Update scroll bar */
						vscroll_update();
						DELAY(100);
						EXITCRITSEC();
						return(vscroll_cur);
					}
				}
			}
			EXITCRITSEC();
			return(0);
		}

void win_c::vscroll_update_cur(ushort new_vscroll_cur)
		{
			ENTERCRITSEC();
			vscroll_update_cur_and_max(new_vscroll_cur, vscroll_max);
			EXITCRITSEC();
		}


void win_c::vscroll_update_max(ushort new_vscroll_max)
		{
			ENTERCRITSEC();
			vscroll_update_cur_and_max(vscroll_cur, new_vscroll_max);
			EXITCRITSEC();
		}

void win_c::vscroll_update_cur_and_max(ushort new_vscroll_cur, ushort new_vscroll_max)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			 /* Check for active window */
			 if(!(WINDOW->attr & WIN_ACTIVE)) {
				 EXITCRITSEC();
				 return;
			 }

			 if (!new_vscroll_cur) new_vscroll_cur = 1;
			 if (!new_vscroll_max) new_vscroll_max = 1;

			 vscroll_cur = new_vscroll_cur;
			 vscroll_max = new_vscroll_max;

			 if (vscroll_cur > new_vscroll_max) new_vscroll_max = vscroll_cur;
			 if (vscroll_cur > vscroll_max) vscroll_cur = vscroll_max;

			vscroll_update();
			EXITCRITSEC();
		}


void win_c::vscroll_update(void)
		{
			int top, bottom;  // Top and bottom part of scroll bar including arrow characters
			int bar_height;   // Height of scroll bar
			int bar_pos;          // Position of bar
			int i;

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Check if window has a vertical scroll bar to update */
			if(!(WINDOW->attr & WIN_VSCROLL_BAR)) {
				EXITCRITSEC();
				return;
			}

			/* Make sure the current isn't greater than the max */
			if (vscroll_cur > vscroll_max) vscroll_cur = vscroll_max;

			/* Calculate coordinates for top and bottom of scroll bar */
			top = 3;
			bottom = WIN_FRAME_HEIGHT - 2;
			/* Calculate bar height */
			bar_height = bottom - top + 1 - 2;

			bar_pos = (int)((float)bar_height * ((float)vscroll_cur / (float)vscroll_max));
			if (bar_pos < 1) bar_pos = 1;
			if (vscroll_cur == 1) bar_pos = 1;

			/* Hide mouse */
			mouse_off();

			/* Draw bar */
			WIN_FRAME_PUTCH(24, WIN_FRAME_WIDTH, top, scroll_fcolor, scroll_bcolor);
			display_ch(WIN_FRAME_WIDTH, top, FRAME_CH, visible_ch(WIN_FRAME_WIDTH, top, FRAME_CH));
			for(i = 0;i < bar_height; i++) {
				/* Draw normal dotted box, unless it is the current position then draw the square */
				if (bar_pos == (i+1)) {
					/* Bar position indicator */
					WIN_FRAME_PUTCH('þ', WIN_FRAME_WIDTH, top+bar_pos, scroll_fcolor, scroll_bcolor);
					display_ch(WIN_FRAME_WIDTH, top+bar_pos, FRAME_CH, visible_ch(WIN_FRAME_WIDTH, top+bar_pos, FRAME_CH));
				}
					else
				{
					/* Bar */
					WIN_FRAME_PUTCH('°', WIN_FRAME_WIDTH, top+i+1, scroll_fcolor, scroll_bcolor);
					display_ch(WIN_FRAME_WIDTH, top+i+1, FRAME_CH, visible_ch(WIN_FRAME_WIDTH, top+i+1, FRAME_CH));
				}
			}
			WIN_FRAME_PUTCH(25, WIN_FRAME_WIDTH, bottom, scroll_fcolor, scroll_bcolor);
			display_ch(WIN_FRAME_WIDTH, bottom, FRAME_CH, visible_ch(WIN_FRAME_WIDTH, bottom, FRAME_CH));

			/* Unhide mouse */
			mouse_on();
			EXITCRITSEC();
		}

uchar win_c::vscroll_up(ushort num)
		{
			ushort old_vscroll_cur;

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			old_vscroll_cur = vscroll_cur;

			vscroll_cur -= num;
			if (vscroll_cur < 1) vscroll_cur = 1;
			if (vscroll_cur > vscroll_max) vscroll_cur = vscroll_max;

			if (old_vscroll_cur == vscroll_cur) {
				EXITCRITSEC();
				return(0);
			}

			vscroll_update();

			EXITCRITSEC();
			return(1);
		}

uchar win_c::vscroll_down(ushort num)
		{
			ushort old_vscroll_cur;

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			old_vscroll_cur = vscroll_cur;

			vscroll_cur += num;
			if (vscroll_cur < 1) vscroll_cur = 1;
			if (vscroll_cur > vscroll_max) vscroll_cur = vscroll_max;

			if (old_vscroll_cur == vscroll_cur) {
				EXITCRITSEC();
				return(0);
			}

			vscroll_update();

			EXITCRITSEC();
			return(1);
		}


void win_c::hscroll_create(uchar new_scroll_fcolor, uchar new_scroll_bcolor,
													 ushort new_hscroll_cur, ushort new_hscroll_max)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Update window structure to reflect the presence of the scroll bar */
			WINDOW->attr |= (WIN_HSCROLL_BAR);

			scroll_fcolor = new_scroll_fcolor;
			scroll_bcolor = new_scroll_bcolor;

			if (!new_hscroll_cur) new_hscroll_cur = 1;
			if (!new_hscroll_max) new_hscroll_max = 1;

			if (new_hscroll_cur > new_hscroll_max) new_hscroll_cur = new_hscroll_max;

			hscroll_cur = new_hscroll_cur;
			hscroll_max = new_hscroll_max;

			hscroll_update();
			EXITCRITSEC();
		}

void win_c::hscroll_destroy(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			 /* Check for active window */
			 if(!(WINDOW->attr & WIN_ACTIVE)) {
				 EXITCRITSEC();
				 return;
			 }

			 /* Check for presence of a horizontal scroll bar */
			 if(!(WINDOW->attr & WIN_HSCROLL_BAR)) {
				 EXITCRITSEC();
				 return;
			 }

			 /* Clear has horizontal scroll bar attribute */
			 WINDOW->attr &= ~(WIN_HSCROLL_BAR);

			 redraw();
			 display_part(FRAME_BOTTOM);
			 EXITCRITSEC();
		}

ushort win_c::hscroll_mouse_move(void)
		{
			ushort hscroll_pos;
			float perc;           // Percentage of bar where clicked

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for presence of a horizontal scroll bar */
			if(!(WINDOW->attr & WIN_HSCROLL_BAR)) {
				EXITCRITSEC();
				return(0);
			}

			/* See if any of the buttons in this event were even pressed */
			if ((mouse_left == BUTTON_DOWN) || (mouse_right == BUTTON_DOWN) || (mouse_center == BUTTON_DOWN)) {
				/* See if coordinates of event are on the arrow keys of the scroll bar */
				if ((frame_x_relative(mouse_col) == 3) && (frame_y_relative(mouse_row) == WIN_FRAME_HEIGHT)) {
					/* They clicked on the left arrow */
					if (hscroll_cur > 1) {
						hscroll_cur--;
						hscroll_update();
						DELAY(100);
						EXITCRITSEC();
						return(hscroll_cur);
					}
					EXITCRITSEC();
					return(0);
				}
				/* See if coordinates of event are on the arrow keys of the scroll bar */
				if ((frame_x_relative(mouse_col) == (WIN_FRAME_WIDTH - 2)) && (frame_y_relative(mouse_row) == WIN_FRAME_HEIGHT)) {
					/* They clicked on the left arrow */
					if (hscroll_cur < hscroll_max) {
						hscroll_cur++;
						hscroll_update();
						DELAY(100);
						EXITCRITSEC();
						return(hscroll_cur);
					}
					EXITCRITSEC();
					return(0);
				}
				/* See if coordinates of event are on the actual scroll bar */
				if ((frame_x_relative(mouse_col) >= 4) && (frame_x_relative(mouse_col) <= (WIN_FRAME_WIDTH - 3)) && (frame_y_relative(mouse_row) == WIN_FRAME_HEIGHT)) {
					/* Calculate what number is represented by that position on the bar */
					perc = (((float) (frame_x_relative(mouse_col) - 3) / (float) (WIN_FRAME_WIDTH - 6)) * (float) 100);
					perc = ceil(perc);
					perc = (perc / (float) 100);
					hscroll_pos = (ushort) (perc * (float) hscroll_max);
					if (!hscroll_pos) hscroll_pos = 1;
					/* Slide bar towards the new position */
					if (hscroll_pos != hscroll_cur) {
						if (hscroll_pos < hscroll_cur) hscroll_cur--;
							else hscroll_cur++;
						/* Make sure scroll bar position isn't out of range */
						if (hscroll_cur > hscroll_max) hscroll_cur = hscroll_max;
						/* Update scroll bar */
						hscroll_update();
						DELAY(100);
						EXITCRITSEC();
						return(hscroll_cur);
					}
				}
			}
			EXITCRITSEC();
			return(0);
		}

void win_c::hscroll_update(void)
		{
			int left, right;  // Top and bottom part of scroll bar including arrow characters
			int bar_width;      // wideth of scroll bar
			int bar_pos;          // Position of bar
			int i;

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Check if window has a horizontal scroll bar to update */
			if(!(WINDOW->attr & WIN_HSCROLL_BAR)) {
				EXITCRITSEC();
				return;
			}

			/* Make sure the current isn't greater than the max */
			if (hscroll_cur > hscroll_max) hscroll_cur = hscroll_max;

			/* Calculate coordinates for top and bottom of scroll bar */
			left = 3;
			right = WIN_FRAME_WIDTH - 2;
			/* Calculate bar height */
			bar_width = right - left + 1 - 2;

			bar_pos = (int)((float)bar_width * ((float)hscroll_cur / (float)hscroll_max));
			if (bar_pos < 1) bar_pos = 1;
			if (hscroll_cur == 1) bar_pos = 1;

			/* Hide mouse */
			mouse_off();

			/* Draw bar */
			WIN_FRAME_PUTCH(27, left, WIN_FRAME_HEIGHT, scroll_fcolor, scroll_bcolor);
			display_ch(left, WIN_FRAME_HEIGHT, FRAME_CH, visible_ch(left, WIN_FRAME_HEIGHT, FRAME_CH));
			for(i = 0;i < bar_width; i++) {
				/* Draw normal dotted box, unless it is the current position then draw the square */
				if (bar_pos == (i+1)) {
					/* Bar position indicator */
					WIN_FRAME_PUTCH('þ', left+bar_pos, WIN_FRAME_HEIGHT, scroll_fcolor, scroll_bcolor);
					display_ch(left+bar_pos, WIN_FRAME_HEIGHT, FRAME_CH, visible_ch(left+bar_pos, WIN_FRAME_HEIGHT, FRAME_CH));
				}
					else
				{
					/* Bar */
					WIN_FRAME_PUTCH('°', left+i+1, WIN_FRAME_HEIGHT, scroll_fcolor, scroll_bcolor);
					display_ch(left+i+1, WIN_FRAME_HEIGHT, FRAME_CH, visible_ch(left+i+1, WIN_FRAME_HEIGHT, FRAME_CH));
				}
			}
			WIN_FRAME_PUTCH(26, right, WIN_FRAME_HEIGHT, scroll_fcolor, scroll_bcolor);
			display_ch(right, WIN_FRAME_HEIGHT, FRAME_CH, visible_ch(right, WIN_FRAME_HEIGHT, FRAME_CH));

			/* Unhide mouse */
			mouse_on();
			EXITCRITSEC();
		}

void win_c::hscroll_update_cur(ushort new_hscroll_cur)
		{
			ENTERCRITSEC();
			hscroll_update_cur_and_max(new_hscroll_cur, hscroll_max);
			EXITCRITSEC();
		}

void win_c::hscroll_update_max(ushort new_hscroll_max)
		{
			ENTERCRITSEC();
			hscroll_update_cur_and_max(hscroll_cur, new_hscroll_max);
			EXITCRITSEC();
		}

void win_c::hscroll_update_cur_and_max(ushort new_hscroll_cur, ushort new_hscroll_max)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			 /* Check for active window */
			 if(!(WINDOW->attr & WIN_ACTIVE)) {
				 EXITCRITSEC();
				 return;
			 }

			 if (!new_hscroll_cur) new_hscroll_cur = 1;
			 if (!new_hscroll_max) new_hscroll_max = 1;

			 hscroll_cur = new_hscroll_cur;
			 hscroll_max = new_hscroll_max;

			 if (hscroll_cur > new_hscroll_max) new_hscroll_max = hscroll_cur;
			 if (hscroll_cur > hscroll_max) hscroll_cur = hscroll_max;

			 hscroll_update();
			 EXITCRITSEC();
		}

uchar win_c::hscroll_left(ushort num)
		{
			ushort old_hscroll_cur;

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			old_hscroll_cur = hscroll_cur;

			hscroll_cur -= num;
			if (hscroll_cur < 1) hscroll_cur = 1;
			if (hscroll_cur > hscroll_max) hscroll_cur = hscroll_max;

			if (old_hscroll_cur == hscroll_cur) {
				EXITCRITSEC();
				return(0);
			}

			hscroll_update();

			EXITCRITSEC();
			return(1);
		}

uchar win_c::hscroll_right(ushort num)
		{
			ushort old_hscroll_cur;

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return(0);
			}

			old_hscroll_cur = hscroll_cur;

			hscroll_cur += num;
			if (hscroll_cur < 1) hscroll_cur = 1;
			if (hscroll_cur > hscroll_max) hscroll_cur = hscroll_max;

			if (old_hscroll_cur == hscroll_cur) {
				EXITCRITSEC();
				return(0);
			}

			hscroll_update();

			EXITCRITSEC();
			return(1);
		}

void win_c::focus(void)
		{
			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) return;

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) return;

			/* Make window the active one and set colors*/
			GOTOXY(WINDOW->texttopxpos+curr_x-1, WINDOW->texttopypos+curr_y-1);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::goto_xy(uchar x, uchar y)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Save new coordinates */
			curr_x = x;
			curr_y = y;

			/* Update screen */
			focus();
			EXITCRITSEC();
		}

uchar win_c::where_x(void)
		{
			return(curr_x);
		}

uchar win_c::where_y(void)
		{
			return(curr_y);
		}

uchar win_c::where_win_x(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Return coordinates of top left corner of FRAME or if there is no frame the TEXT */
			EXITCRITSEC();
			return(WINDOW->frametopxpos);
		}

uchar win_c::where_win_y(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* Return coordinates of top left corner of FRAME or if there is no frame the TEXT */
			EXITCRITSEC();
			return(WINDOW->frametopypos);
		}

uchar win_c::text_height(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			EXITCRITSEC();
			return(WIN_TEXT_HEIGHT);
		}

uchar win_c::text_width(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return(0);
			}

			EXITCRITSEC();
			return(WIN_TEXT_WIDTH);
		}

void win_c::wputctrlch(char ch)
		{
			ENTERCRITSEC();

			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Switch to window */
			focus();

			/* Turn mouse off */
			mouse_off();

			/* Display character if it should be */
			WIN_TEXT_PUTCH(ch, curr_x, curr_y, curr_fcolor, curr_bcolor);
			if (!(WINDOW->attr & WIN_SEETHRU)) display_ch(curr_x, curr_y, TEXT_CH, visible_ch(curr_x, curr_y, TEXT_CH));
			curr_x++;

			/* Turn mouse on */
			mouse_on();

			/* line wrap? */
			if (curr_x > WIN_TEXT_WIDTH) {
				curr_x = 1;
				curr_y++;
			}

			/* screen wrap ? */
			if (curr_y > WIN_TEXT_HEIGHT) {
				scroll_up();
				curr_y--;
			}
			EXITCRITSEC();
		}

void win_c::clr_eoc(void)
		{
			uchar i; // General counter

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Switch to current window */
			focus();

			/* Turn mouse off */
			mouse_off();

			/* Clear to end of line */
			for (i = curr_y; i <= WIN_TEXT_HEIGHT; i++) {
				WIN_TEXT_PUTCH(' ', curr_x, i, curr_fcolor, curr_bcolor);
				display_ch(curr_x, i, TEXT_CH, visible_ch(curr_x, i, TEXT_CH));
			}

			/* Turn mouse on */
			mouse_on();

			EXITCRITSEC();
		}

void win_c::fill_col(uchar col, char ch)
		{
			uchar i; // General counter

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Switch to current window */
			focus();

			/* Turn mouse off */
			mouse_off();

			/* Clear to end of line */
			for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
				WIN_TEXT_PUTCH(ch, col, i, curr_fcolor, curr_bcolor);
				display_ch(col, i, TEXT_CH, visible_ch(col, i, TEXT_CH));
			}

			/* Turn mouse on */
			mouse_on();
			EXITCRITSEC();
		}

void win_c::fill_row(uchar row, char ch)
		{
			uchar i; // General counter

			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) {
				EXITCRITSEC();
				return;
			}

			/* Check if window is hidden */
			if(WINDOW->attr & WIN_HIDDEN) {
				EXITCRITSEC();
				return;
			}

			/* Switch to current window */
			focus();

			/* Turn mouse off */
			mouse_off();

			/* Clear to end of line */
			for (i = 1; i <= WIN_TEXT_WIDTH; i++) {
				WIN_TEXT_PUTCH(ch, i, row, curr_fcolor, curr_bcolor);
				display_ch(i, row, TEXT_CH, visible_ch(i, row, TEXT_CH));
			}

			/* Turn mouse on */
			mouse_on();
			EXITCRITSEC();
		}

