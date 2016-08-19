/* Filename : WIN2.CPP                                                      *
 * Description : Windowing module #2 for TI/2                               *
 * Target : WIN2.OBJ -> TI2WIN.LIB or TI2.DLL                               *
 * Portability : OS/2, DOS																									*
 * Creation date : 10/25/94                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995								*/

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
#include "ti2.h"
#include "internal.h"

#ifdef __TURBOC__
  #include <alloc.h>
#endif

#ifdef _USERTL2_
	#include "..\rtl2\rtl2.h"
#else
	#define BEEP               7
	#define CR                13
	#define LF								10
	#define BACK_SPACE         8
	#define ESC								27
#endif

/**************************
 * Window Class Functions *
 **************************/

void win_c::clr_scr(void)
		{
			uchar i, j;		// General counters

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

			/* Clear window where frame and text window is in memory */
			for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
				for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(' ', j, i, curr_fcolor, curr_bcolor);
			}

			/* Update screen */
			display_part(TEXT);

			/* Set window's where X & Y to 1,1 */
			curr_x = 1;
			curr_y = 1;

			EXITCRITSEC();
		}

void win_c::clr_region(uchar x1, uchar y1, uchar x2, uchar y2)
		{
			uchar i, j;		// General counters

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

			/* Clear region in memory */
			for (i = y1; i <= y2; i++) {
				for (j = x1; j <= x2; j++) WIN_TEXT_PUTCH(' ', j, i, curr_fcolor, curr_bcolor);
			}

			/* Update screen */
			display_part(TEXT);

			EXITCRITSEC();
		}

void win_c::nd_clr_region(uchar x1, uchar y1, uchar x2, uchar y2)
		{
			uchar i, j;		// General counters

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

			/* Clear region in memory */
			for (i = y1; i <= y2; i++) {
				for (j = x1; j <= x2; j++) WIN_TEXT_PUTCH(' ', j, i, curr_fcolor, curr_bcolor);
			}

			EXITCRITSEC();
		}

void win_c::clr_eol(void)
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
			for (i = curr_x; i <= WIN_TEXT_WIDTH; i++) {
				WIN_TEXT_PUTCH(' ', i, curr_y, curr_fcolor, curr_bcolor);
				display_ch(i, curr_y, TEXT_CH, visible_ch(i, curr_y, TEXT_CH));
			}

			/* Turn mouse on */
			mouse_on();

			EXITCRITSEC();
		}

void win_c::scroll_up(void)
		{
			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) return;

			scroll_part_up(1, 1, WIN_TEXT_WIDTH, WIN_TEXT_HEIGHT);
		}

void win_c::scroll_part_up(uchar topx, uchar topy, uchar botx, uchar boty)
		{
			uchar i, j;   // General counters

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) return;

			/* Check for active window */
			if(!(WINDOW->attr & WIN_ACTIVE)) return;

			/* Shift text array */
			j = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + (WINDOW->frame ? 1 : 0);

			for (i = topy; i < boty; i++) FARMEMCPY(&WINDOW->text[j + i - 1][(j+topx-1)*2], &WINDOW->text[j + i][(j+topx-1)*2], (botx - topx + 1)*2);
			for (i = 0; i < (botx - topx + 1); i++) WIN_TEXT_PUTCH(' ', topx+i, boty, curr_fcolor, curr_bcolor);

			/* Redisplay text */
			display_part(TEXT);
		}

void win_c::scroll_down(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			scroll_part_down(1, 1, WIN_TEXT_WIDTH, WIN_TEXT_HEIGHT);
			EXITCRITSEC();
		}

void win_c::scroll_part_down(uchar topx, uchar topy, uchar botx, uchar boty)
		{
			uchar i, j;		// General counters

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

			/* Shift text array */
			j = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + (WINDOW->frame ? 1 : 0);

			for (i = (boty - 1); i >= topy; i--) FARMEMCPY(&WINDOW->text[j + i][(j+topx-1)*2], &WINDOW->text[j + i - 1][(j+topx-1)*2], (botx - topx + 1)*2);
			for (i = 0; i < (botx - topx + 1); i++) WIN_TEXT_PUTCH(' ', topx+i, topy, curr_fcolor, curr_bcolor);

			/* Redisplay text */
			display_part(TEXT);
			EXITCRITSEC();
		}

void win_c::scroll_left(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			scroll_part_left(1, 1, WIN_TEXT_WIDTH, WIN_TEXT_HEIGHT);
			EXITCRITSEC();
		}

void win_c::scroll_part_left(uchar topx, uchar topy, uchar botx, uchar boty)
		{
			uchar i, j;		// General counters

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

			/* Shift text array */
			j = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + (WINDOW->frame ? 1 : 0);

			for (i = topy; i <= boty; i++) FARMEMMOVE(&WINDOW->text[j + i - 1][(j+topx-1)*2], &WINDOW->text[j + i - 1][(j+topx)*2], (botx - topx + 1 - 1)*2);
			for (i = topy; i <= boty; i++) WIN_TEXT_PUTCH(' ', botx, i, curr_fcolor, curr_bcolor);

			/* Redisplay text */
			display_part(TEXT);
			EXITCRITSEC();
		}

void win_c::scroll_right(void)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			scroll_part_right(1, 1, text_width(), text_height());
			EXITCRITSEC();
		}

void win_c::scroll_part_right(uchar topx, uchar topy, uchar botx, uchar boty)
		{
			uchar i, j;		// General counters

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

			/* Shift text array */
			j = ((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + (WINDOW->frame ? 1 : 0);

			for (i = topy; i <= boty; i++) FARMEMMOVE(&WINDOW->text[j + i - 1][(j+topx)*2], &WINDOW->text[j + i - 1][(j+topx-1)*2], (botx - topx + 1 - 1)*2);
			for (i = topy; i <= boty; i++) WIN_TEXT_PUTCH(' ', topx, i, curr_fcolor, curr_bcolor);

			/* Redisplay text */
			display_part(TEXT);
			EXITCRITSEC();
		}

void win_c::wputch(char ch)
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

			switch (ch) {
				case BEEP : {
											SOUND(3000, 100);
											break;
										}
				case BACK_SPACE : {
														if (curr_x > 1) curr_x--;
														break;
													}
				case CR : {
										curr_x = 1;
										break;
									}
				case LF : {
										curr_y++;
										break;
									}
				default : {
										/* Turn mouse off */
										mouse_off();

										/* Display character if it should be */
										WIN_TEXT_PUTCH(ch, curr_x, curr_y, curr_fcolor, curr_bcolor);
										if (!(WINDOW->attr & WIN_SEETHRU)) display_ch(curr_x, curr_y, TEXT_CH, visible_ch(curr_x, curr_y, TEXT_CH));
										curr_x++;

										/* Turn mouse on */
										mouse_on();
										break;
									}
			}

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

void win_c::wprintf(char *fmt, ...)
		{
			va_list ap;
			char s[256];
      char *p;   // Pointer to string

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Switch to window */
			focus();

			p = &s[0];

			/* Turn mouse off */
			mouse_off();

			while (*p != 0) {
				switch (*p) {
					case BEEP : {
												SOUND(3000, 100);
												break;
											}
					case BACK_SPACE : {
															if (curr_x > 1) curr_x--;
															break;
														}
					case CR : {
											curr_x = 1;
											break;
										}
					case LF : {
											curr_y++;
											break;
										}
					default : {
											/* Display character if it should be */
											WIN_TEXT_PUTCH(*p, curr_x, curr_y, curr_fcolor, curr_bcolor);
											if (!(WINDOW->attr & WIN_SEETHRU)) display_ch(curr_x, curr_y, TEXT_CH, visible_ch(curr_x, curr_y, TEXT_CH));
											curr_x++;
											break;
										}
				}

				/* line wrap? */
				if (curr_x > WIN_TEXT_WIDTH) {
					curr_x = 1;
					curr_y += (scroll_mode ? 1 : 0);
				}

				/* screen wrap ? */
				if (curr_y > WIN_TEXT_HEIGHT) {
					scroll_up();
					curr_y--;
				}

				/* Increased string pointer */
				p++;
			}

			/* Turn mouse on */
			mouse_on();
			EXITCRITSEC();
		}

void win_c::wputs(char *s)
		{
      char *p;   // Pointer to string

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

			/* Switch to window */
			focus();

			p = s;

			/* Turn mouse off */
			mouse_off();

			while (*p != 0) {
				switch (*p) {
					case BEEP : {
												SOUND(3000, 100);
												break;
											}
					case BACK_SPACE : {
															if (curr_x > 1) curr_x--;
															break;
														}
					case CR : {
											curr_x = 1;
											break;
										}
					case LF : {
											curr_y++;
											break;
										}
					default : {
											/* Display character if it should be */
											WIN_TEXT_PUTCH(*p, curr_x, curr_y, curr_fcolor, curr_bcolor);
											if (!(WINDOW->attr & WIN_SEETHRU)) display_ch(curr_x, curr_y, TEXT_CH, visible_ch(curr_x, curr_y, TEXT_CH));
											curr_x++;
											break;
										}
				}

				/* line wrap? */
				if (curr_x > WIN_TEXT_WIDTH) {
					curr_x = 1;
					curr_y += (scroll_mode ? 1 : 0);
				}

				/* screen wrap ? */
				if (curr_y > WIN_TEXT_HEIGHT) {
					scroll_up();
					curr_y--;
				}

				/* Increased string pointer */
				p++;
			}

			/* Turn mouse on */
			mouse_on();
			EXITCRITSEC();
		}

void win_c::nd_putch(char ch)
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

			switch (ch) {
				case BEEP : {
											SOUND(3000, 100);
											break;
										}
				case BACK_SPACE : {
														if (curr_x > 1) curr_x--;
														break;
													}
				case CR : {
										curr_x = 1;
										break;
									}
				case LF : {
										curr_y++;
										break;
									}
				default : {
										/* Update character in memory only */
										WIN_TEXT_PUTCH(ch, curr_x, curr_y, curr_fcolor, curr_bcolor);
										curr_x++;
										break;
									}
			}

			/* line wrap? */
			if (curr_x > WIN_TEXT_WIDTH) {
				curr_x = 1;
				curr_y++;
			}

			/* screen wrap ? */
			if (curr_y > WIN_TEXT_HEIGHT) {
				/* Toggle hidden so scroll won't redisplay the text */
				WINDOW->attr |= WIN_HIDDEN;
				scroll_up();
				WINDOW->attr &= ~(WIN_HIDDEN);
				curr_y--;
			}
			EXITCRITSEC();
		}

void win_c::nd_putctrlch(char ch)
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

			/* Display character if it should be */
			WIN_TEXT_PUTCH(ch, curr_x, curr_y, curr_fcolor, curr_bcolor);
			curr_x++;

			/* line wrap? */
			if (curr_x > WIN_TEXT_WIDTH) {
				curr_x = 1;
				curr_y++;
			}

			/* screen wrap ? */
			if (curr_y > WIN_TEXT_HEIGHT) {
				/* Toggle hidden so scroll won't redisplay the text */
				WINDOW->attr |= WIN_HIDDEN;
				scroll_up();
				WINDOW->attr &= ~(WIN_HIDDEN);
				curr_y--;
			}
			EXITCRITSEC();
		}

void win_c::nd_printf(char *fmt, ...)
		{
			va_list ap;
			char s[256];

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Call nd_puts */
			nd_puts(s);

			EXITCRITSEC();
		}

void win_c::nd_puts(char *s)
		{
      char *p;   // Pointer to string

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

			/* Switch to window */
			focus();

			p = s;

			while (*p != 0) {
				switch (*p) {
					case BEEP : {
												SOUND(3000, 100);
												break;
											}
					case BACK_SPACE : {
															if (curr_x > 1) curr_x--;
															break;
														}
					case CR : {
											curr_x = 1;
											break;
										}
					case LF : {
											curr_y++;
											break;
										}
					default : {
											/* Update character in memory only */
											WIN_TEXT_PUTCH(*p, curr_x, curr_y, curr_fcolor, curr_bcolor);
											curr_x++;
											break;
										}
				}

				/* line wrap? */
				if (curr_x > WIN_TEXT_WIDTH) {
					curr_x = 1;
					curr_y += (scroll_mode ? 1 : 0);
				}

				/* screen wrap ? */
				if (curr_y > WIN_TEXT_HEIGHT) {
					/* Toggle hidden so scroll won't redisplay the text */
					WINDOW->attr |= WIN_HIDDEN;
					scroll_up();
					WINDOW->attr &= ~(WIN_HIDDEN);
					curr_y--;
				}

				/* Increased string pointer */
				p++;
			}
			EXITCRITSEC();
		}

void win_c::nd_width_puts(uchar width, char *s)
		{
			int i, j;

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

			/* Switch to window */
			focus();

			/* Write text to screen */
			j = strlen(s);
			if (j <= width) {
				nd_puts(s);
				for (i = 0; i < (width - j); i++) nd_putch(' ');
			}
				else
			{
				for (i = 0; i < width; i++) nd_putch(s[i]);
			}
			EXITCRITSEC();
		}

void win_c::nd_width_printf(uchar width, char *fmt, ...)
		{
			va_list ap;
			char s[256];

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Call other width print function */
			nd_width_puts(width, s);
			EXITCRITSEC();
		}

void win_c::width_puts(uchar width, char *s)
		{
			int i, j;

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

			/* Switch to window */
			focus();

			/* Write text to screen */
			j = strlen(s);
			if (j <= width) {
				wputs(s);
				for (i = 0; i < (width - j); i++) wputch(' ');
			}
				else
			{
				for (i = 0; i < width; i++) wputch(s[i]);
			}
			EXITCRITSEC();
		}

void win_c::width_printf(uchar width, char *fmt, ...)
		{
			va_list ap;
			char s[256];

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Call other width print function */
			width_puts(width, s);
			EXITCRITSEC();
		}

void win_c::cntr_printf(uchar y, char *fmt, ...)
		{
			va_list ap;
			char s[256];

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Switch to window */
			focus();

			/* Gotoxy correct position on screen */
			goto_xy((WIN_TEXT_WIDTH / 2) - (strlen(s)/2) + 1, y);

			/* Print string */
			wputs(s);

			EXITCRITSEC();
		}

void win_c::col_cntr_printf(uchar x, uchar y, char *fmt, ...)
		{
			va_list ap;
			char s[512];

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Switch to window */
			focus();

			/* Gotoxy correct position on screen */
			goto_xy(x - (strlen(s)/2) + 1, y);

			/* Print string */
			wputs(s);

			EXITCRITSEC();
		}

void win_c::rj_printf(uchar x, uchar y, char *fmt, ...)
		{
			va_list ap;
			char s[512];

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

			va_start(ap, fmt);
			vsprintf(s, fmt, ap);
			va_end(ap);

			/* Switch to window */
			focus();

			/* Gotoxy correct position on screen */
			goto_xy(x - strlen(s) + 1, y);

			/* Print string */
			wputs(s);
			EXITCRITSEC();
		}

void win_c::text_color(uchar new_t_fcolor)
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

			/* Set new text color */
			curr_fcolor = new_t_fcolor;

			/* Switch to window */
			focus();

      EXITCRITSEC();
		}

void win_c::text_background(uchar new_t_bcolor)
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

			/* Set new text background color */
			curr_bcolor = new_t_bcolor;

			/* Switch to window */
			focus();

			EXITCRITSEC();
		}

void win_c::frame_textcolor(uchar new_f_fcolor)
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

			/* Set new text color */
			f_fcolor = new_f_fcolor;

			/* Redraw window frame */
			if (WINDOW->frame) {
				redraw();
				display_part(FRAME);
			}
			EXITCRITSEC();
		}

void win_c::frame_textbackground(uchar new_f_bcolor)
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

			/* Set new text background color */
			f_bcolor = new_f_bcolor;

			/* Redraw window frame */
			if (WINDOW->frame) {
				redraw();
				display_part(FRAME);
			}
			EXITCRITSEC();
		}

void win_c::title_textcolor(uchar new_title_fcolor)
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

			/* Set new title text color */
			title_fcolor = new_title_fcolor;

			/* Redraw window frame */
			if (WINDOW->frame) {
				redraw();
				display_part((WINDOW->attr & WIN_TITLE_BOTTOM) ? FRAME_BOTTOM : FRAME_TOP);
			}
			EXITCRITSEC();
		}

void win_c::title_textbackground(uchar new_title_bcolor)
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

			/* Set new text background color */
			title_bcolor = new_title_bcolor;

			/* Redraw window frame */
			if (WINDOW->frame) {
				redraw();
				display_part((WINDOW->attr & WIN_TITLE_BOTTOM) ? FRAME_BOTTOM : FRAME_TOP);
			}
			EXITCRITSEC();
		}

void win_c::hide(void)
		{
      uchar i, j;                   // General counter
      char *vert_shadow_buffer;     // Holds parts of window to be redisplayed
      char *horiz_shadow_buffer;    // Holds parts of window to be redisplayed
      char *frame_buffer;           // Holds parts of window to be redisplayed
      ulong buffer_pos = 0;         // Position in buffer
      char *buffer;                 // Holds image of the screen where window is

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

			/* Hide all characters in this window */
			if (completely_visible()) {
				/* Shadow */
				if (WINDOW->shadow == WIN_SHADOW_LEFT) {
					/* Reset variables */
					buffer_pos = 0;
					/* Allocate a buffer that can hold the horizontal shadow */
          if ((horiz_shadow_buffer = (char *) malloc((WIN_ABS_WIDTH-1) * 2)) == NULL) {
						/* Error creating buffer */
						printf("win_c::hide() error : Unable to allocate buffer for puttext.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Copy data for horizontal shadow to buffer */
					for (i = 1; i < WIN_ABS_WIDTH; i++) {
						get_hidden_ch(i, 1, SHADOW_CH, &horiz_shadow_buffer[buffer_pos]);
						buffer_pos += 2;
					}

					/* Reset variables */
					buffer_pos = 0;
					/* Allocate a buffer that can hold the vertical shadow */
          if ((vert_shadow_buffer = (char *) malloc((WIN_ABS_HEIGHT-1) * 2)) == NULL) {
						/* Error creating buffer */
						printf("win_c::hide() error : Unable to allocate buffer for puttext.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Copy data for vertical shadow to buffer */
					for (i = 1; i < WIN_ABS_HEIGHT; i++) {
						get_hidden_ch(1, i, SHADOW_CH, &vert_shadow_buffer[buffer_pos]);
						buffer_pos += 2;
					}
				}
					else
				if (WINDOW->shadow == WIN_SHADOW_RIGHT) {
					/* Reset variables */
					buffer_pos = 0;
					/* Allocate a buffer that can hold the horizontal shadow */
          if ((horiz_shadow_buffer = (char *) malloc((WIN_ABS_WIDTH-1) * 2)) == NULL) {
						/* Error creating buffer */
						printf("win_c::hide() error : Unable to allocate buffer for puttext.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Copy data for horizontal shadow to buffer */
					for (i = 2; i <= WIN_ABS_WIDTH; i++) {
						get_hidden_ch(i, WIN_ABS_HEIGHT, SHADOW_CH, &horiz_shadow_buffer[buffer_pos]);
						buffer_pos += 2;
					}

					/* Reset variables */
					buffer_pos = 0;
					/* Allocate a buffer that can hold the vertical shadow */
          if ((vert_shadow_buffer = (char *) malloc((WIN_ABS_HEIGHT-1) * 2)) == NULL) {
						/* Error creating buffer */
						printf("win_c::hide() error : Unable to allocate buffer for puttext.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Copy data for vertical shadow to buffer */
					for (i = 2; i <= WIN_ABS_HEIGHT; i++) {
						get_hidden_ch(WIN_ABS_WIDTH, i, SHADOW_CH, &vert_shadow_buffer[buffer_pos]);
						buffer_pos += 2;
					}
				}
				/* Frame and Text */
				/* Reset variables */
				buffer_pos = 0;
				/* Allocate a buffer that can hold the text */
        if ((frame_buffer = (char *) malloc(WIN_FRAME_HEIGHT * WIN_FRAME_WIDTH * 2)) == NULL) {
					/* Error creating buffer */
					printf("win_c::hide() error : Unable to allocate buffer for puttext.\n");
					EXITCRITSEC();
					exit(0);
				}
				/* Copy data from text array to buffer */
				for (i = 1; i <= WIN_FRAME_HEIGHT; i++) {
					for (j = 1; j <= WIN_FRAME_WIDTH; j++) {
						get_hidden_ch(j, i, FRAME_CH, &frame_buffer[buffer_pos]);
						buffer_pos += 2;
					}
				}

				/* Turn mouse off */
				mouse_off();

				/* Display shadow */
				if (WINDOW->shadow == WIN_SHADOW_LEFT) {
					PUTTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos-1, WINDOW->topypos, horiz_shadow_buffer);
					PUTTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->topxpos, WINDOW->botypos-1, vert_shadow_buffer);
				}
					else
				if (WINDOW->shadow == WIN_SHADOW_RIGHT) {
					PUTTEXT(WINDOW->topxpos+1, WINDOW->botypos, WINDOW->botxpos, WINDOW->botypos, horiz_shadow_buffer);
					PUTTEXT(WINDOW->botxpos, WINDOW->topypos+1, WINDOW->botxpos, WINDOW->botypos, vert_shadow_buffer);
				}
				/* Display frame and text */
				PUTTEXT(WINDOW->frametopxpos, WINDOW->frametopypos, WINDOW->framebotxpos, WINDOW->framebotypos, frame_buffer);

				/* Free memory allocated */
				if ((WINDOW->shadow == WIN_SHADOW_RIGHT) || (WINDOW->shadow == WIN_SHADOW_LEFT)) {
					FREE(vert_shadow_buffer);
					FREE(horiz_shadow_buffer);
				}
				FREE(frame_buffer);

				/* Turn mouse on */
				mouse_on();
			}
				else
			{
				/* Allocate buffer that is the size of the whole window on the screen */
        if ((buffer = (char *) malloc(WIN_ABS_HEIGHT * WIN_ABS_WIDTH * 2)) == NULL) {
					/* Error creating buffer */
					printf("win_c::hide() error : Unable to allocate buffer for puttext.\n");
					EXITCRITSEC();
					exit(0);
				}

				/* Turn mouse off */
				mouse_off();

				/* Get the area of the screen where this window appears */
				GETTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, buffer);

				/* Turn mouse on */
				mouse_on();

				/* Hide right shadow if we have one */
				if (WINDOW->shadow == WIN_SHADOW_RIGHT) {
					/* Horizontal */
					for (i = 2; i <= WIN_ABS_WIDTH; i++) unhide_buffer_ch(buffer, i, WIN_ABS_HEIGHT, SHADOW_CH);
					/* Vertical */
					for (i = 2; i <= WIN_ABS_HEIGHT; i++) unhide_buffer_ch(buffer, WIN_ABS_WIDTH, i, SHADOW_CH);
				}
					else
				if (WINDOW->shadow == WIN_SHADOW_LEFT) {
					/* Horizontal */
					for (i = 1; i < WIN_ABS_WIDTH; i++) unhide_buffer_ch(buffer, i, 1, SHADOW_CH);
					/* Vertical */
					for (i = 1; i < WIN_ABS_HEIGHT; i++) unhide_buffer_ch(buffer, 1, i, SHADOW_CH);
				}
				/* Make sure they have a frame */
				if (WINDOW->frame) {
					/* Top */
					for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_buffer_ch(buffer, i, 1, FRAME_CH);
					/* Left */
					for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_buffer_ch(buffer, 1, i, FRAME_CH);
					/* Bottom */
					for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_buffer_ch(buffer, i, WIN_FRAME_HEIGHT, FRAME_CH);
					/* Right */
					for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_buffer_ch(buffer, WIN_FRAME_WIDTH, i, FRAME_CH);
				}
				/* hide text */
				for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
					for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
						unhide_buffer_ch(buffer, j, i, TEXT_CH);
					}
				}

				/* Turn mouse off */
				mouse_off();

				/* Put the updated area of the screen where this window appears back */
				PUTTEXT(WINDOW->topxpos, WINDOW->topypos, WINDOW->botxpos, WINDOW->botypos, buffer);

				/* Turn mouse on */
				mouse_on();

				/* Free buffer */
				FREE(buffer);
			}

			/* Set hidden window attribute */
			WINDOW->attr |= WIN_HIDDEN;
			EXITCRITSEC();
		}

void win_c::unhide(void)
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

			/* Check if window is hidden */
			if(!(WINDOW->attr & WIN_HIDDEN)) {
				EXITCRITSEC();
				return;
			}

			/* Strip hidden window attribute */
			WINDOW->attr &= ~(WIN_HIDDEN);

			/* Update shadow */
			redraw();

			/* Unhide window */
			display_part(SHADOW | FRAME | TEXT);
			EXITCRITSEC();
		}

void win_c::move(uchar newx, uchar newy)
		{
			int diffx,
					diffy;
			uchar min_x, min_y, max_x, max_y;		// Maximum and minimum values that this window can be moved to->

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

			/* Calucalute maximum and miniums */
			if (WINDOW->shadow == WIN_SHADOW_LEFT) min_x = min_y = 2;
				else min_x = min_y = 1;
      max_x = (SCREENWIDTH() - WIN_FRAME_WIDTH - ((WINDOW->shadow == WIN_SHADOW_RIGHT) ? 1 : 0) + 1);
      max_y = (SCREENHEIGHT() - WIN_FRAME_HEIGHT - ((WINDOW->shadow == WIN_SHADOW_RIGHT) ? 1 : 0) + 1);

			/* Make sure new coordinates are valid */
			if (newx < min_x) newx = min_x;
			if (newx > max_x) newx = max_x;
			if (newy < min_y) newy = min_y;
			if (newy > max_y) newy = max_y;

			diffx = WINDOW->frametopxpos - newx;
			diffy = WINDOW->frametopypos - newy;

			hide();

			/* Change all X,Y variables */
			WINDOW->topxpos -= diffx;
			WINDOW->topypos -= diffy;
			WINDOW->botxpos -= diffx;
			WINDOW->botypos -= diffy;
			WINDOW->frametopxpos -= diffx;
			WINDOW->frametopypos -= diffy;
			WINDOW->framebotxpos -= diffx;
			WINDOW->framebotypos -= diffy;
			WINDOW->texttopxpos -= diffx;
			WINDOW->texttopypos -= diffy;
			WINDOW->textbotxpos -= diffx;
			WINDOW->textbotypos -= diffy;

			unhide();
			EXITCRITSEC();
		}

void win_c::slide(uchar newx, uchar newy)
		{
			uchar i;		// General counter
			uchar min_x, min_y, max_x, max_y;		// Maximum and minimum values that this window can be moved to->

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

			/* Calucalute maximum and miniums */
			if (WINDOW->shadow == WIN_SHADOW_LEFT) min_x = min_y = 2;
				else min_x = min_y = 1;
      max_x = (SCREENWIDTH() - WIN_FRAME_WIDTH - ((WINDOW->shadow == WIN_SHADOW_RIGHT) ? 1 : 0) + 1);
      max_y = (SCREENHEIGHT() - WIN_FRAME_HEIGHT - ((WINDOW->shadow == WIN_SHADOW_RIGHT) ? 1 : 0) + 1);

			/* Make sure new coordinates are valid */
			if (newx < min_x) newx = min_x;
			if (newx > max_x) newx = max_x;
			if (newy < min_y) newy = min_y;
			if (newy > max_y) newy = max_y;

			/* Slide window until it ends up where it should */
			while ((WINDOW->frametopxpos != newx) || (WINDOW->frametopypos != newy)) {
				/* See if we are sliding to the right */
				if (WINDOW->frametopxpos < newx) {
					/* Unhide left hand column of the window */
					if (WINDOW->shadow) {
						for (i = 1; i <= WIN_ABS_HEIGHT; i++) unhide_ch(1, i, SHADOW_CH);
						unhide_ch(1, WIN_FRAME_HEIGHT, FRAME_CH);
						unhide_ch(2, WIN_ABS_HEIGHT, SHADOW_CH);
					}
						else
					{
						for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_ch(1, i, FRAME_CH);
					}

					WINDOW->topxpos += 1;
					WINDOW->botxpos += 1;
					WINDOW->frametopxpos += 1;
					WINDOW->framebotxpos += 1;
					WINDOW->texttopxpos += 1;
					WINDOW->textbotxpos += 1;
				}
					else
				/* See if we are sliding left */
				if (WINDOW->frametopxpos > newx) {
					/* Unhide right hand column of the window */
					if (WINDOW->shadow) {
						for (i = 1; i <= WIN_ABS_HEIGHT; i++) unhide_ch(WIN_ABS_WIDTH, i, SHADOW_CH);
						unhide_ch(WIN_FRAME_WIDTH, 1, FRAME_CH);
						unhide_ch(WIN_ABS_WIDTH, 1, SHADOW_CH);
						unhide_ch(WIN_ABS_WIDTH, 2, SHADOW_CH);
					}
						else
					{
						for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_ch(WIN_FRAME_WIDTH, i, FRAME_CH);
					}

					/* Change all X,Y variables */
					WINDOW->topxpos -= 1;
					WINDOW->botxpos -= 1;
					WINDOW->frametopxpos -= 1;
					WINDOW->framebotxpos -= 1;
					WINDOW->texttopxpos -= 1;
					WINDOW->textbotxpos -= 1;
				}

				/* See if we are sliding down */
				if (WINDOW->frametopypos < newy) {
					/* Unhide top row of the window */
					if (WINDOW->shadow) {
						for (i = 1; i <= WIN_ABS_WIDTH; i++) unhide_ch(i, 1, SHADOW_CH);
						unhide_ch(WIN_FRAME_WIDTH, 1, FRAME_CH);
						unhide_ch(WIN_ABS_WIDTH, 1, SHADOW_CH);
						unhide_ch(WIN_ABS_WIDTH, 2, SHADOW_CH);
					}
						else
					{
						for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_ch(i, 1, FRAME_CH);
					}

					/* Change all X,Y variables */
					WINDOW->topypos += 1;
					WINDOW->botypos += 1;
					WINDOW->frametopypos += 1;
					WINDOW->framebotypos += 1;
					WINDOW->texttopypos += 1;
					WINDOW->textbotypos += 1;
				}
					else
				/* See if we are sliding up */
				if (WINDOW->frametopypos > newy) {
					/* Unhide bottom row of the window */
					if (WINDOW->shadow) {
						for (i = 1; i <= WIN_ABS_WIDTH; i++) unhide_ch(i, WIN_ABS_HEIGHT, SHADOW_CH);
						unhide_ch(1, WIN_FRAME_HEIGHT, FRAME_CH);
						unhide_ch(WIN_FRAME_WIDTH, 1, FRAME_CH);
						unhide_ch(WIN_ABS_HEIGHT - 1, 1, SHADOW_CH);
					}
						else
					{
						for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_ch(i, WIN_FRAME_HEIGHT, FRAME_CH);
					}

					/* Change all X,Y variables */
					WINDOW->topypos -= 1;
					WINDOW->botypos -= 1;
					WINDOW->frametopypos -= 1;
					WINDOW->framebotypos -= 1;
					WINDOW->texttopypos -= 1;
					WINDOW->textbotypos -= 1;
				}

				/* Redisplay the whole window */
				redraw();
				display_part(SHADOW | FRAME | TEXT);
			}
			EXITCRITSEC();
		}

void win_c::resize(uchar side, uchar width)
		{
			uchar i, j;										// General counters
			int   diff;										// General difference holder
      char buffer[2];               // Holds a character and attribute

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

			/* Resize correct side */
			switch(side) {
				case LEFT : {
											/* Calculate the difference of the new width and the current width */
											diff = (width - WIN_FRAME_WIDTH);
											/* If there is a different than resize the window */
											if (diff) {
												/* If window is shorter than unhide part that will no longer be there */
												if (diff < 0) {
													for (j = 0; j < abs(diff); j++) {
														/* Unhide all the columns that will not be part of the window anymore */
														if (WINDOW->shadow) {
															for (i = 1; i <= WIN_ABS_HEIGHT; i++) unhide_ch(WIN_ABS_WIDTH-j, i, SHADOW_CH);
															unhide_ch(WIN_ABS_WIDTH-j-1, 1, SHADOW_CH);
														}
															else
														{
															for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_ch(WIN_FRAME_WIDTH-j, i, FRAME_CH);
														}
													}
												}
												/* Change coordinates */
												WINDOW->botxpos += diff;
												WINDOW->framebotxpos += diff;
												WINDOW->textbotxpos += diff;
												/* Reallocate text array */
												for (i = 0; i < WIN_ABS_HEIGHT; i++) {            //FARFREE(WINDOW->text[i]);
													/* Reallocate text array*/
                          if ((WINDOW->text[i] = (char FAR *) FARREALLOC(WINDOW->text[i], sizeof(char) * 2 * WIN_ABS_WIDTH)) == NULL) {
														/* Error reallocating memory */
														printf("win_c::resize(LEFT) error : Unable to reallocate text buffer.\n");
														EXITCRITSEC();
														exit(0);
													}
												}
												/* Redraw the shadow and frame in memory */
												redraw();
												/* If window expanded clear the new area */
												if (diff > 0) {
													for (i = (WIN_TEXT_WIDTH - diff + 1); i <= WIN_TEXT_WIDTH; i++) {
														for (j = 1; j <= WIN_TEXT_HEIGHT; j++) WIN_TEXT_PUTCH(' ', i, j, curr_fcolor, curr_bcolor);
													}
												}

												/* Redraw window parts of window */
												if (diff > 0) display_part(TEXT | FRAME_RIGHT | FRAME_TOP | FRAME_BOTTOM | SHADOW);
													else display_part(FRAME_RIGHT | FRAME_TOP | FRAME_BOTTOM | SHADOW);
											}
											break;
										}
				case RIGHT : {
											/* Calculate the difference of the new width and the current width */
											diff = (width - WIN_FRAME_WIDTH);
											/* If there is a different than resize the window */
											if (diff) {
												/* If window is shorter than unhide part that will no longer be there */
												if (diff < 0) {
													for (j = 0; j < abs(diff); j++) {
														/* Unhide all the columns that will not be part of the window anymore */
														if (WINDOW->shadow) {
															for (i = 1; i <= WIN_ABS_HEIGHT; i++) unhide_ch(j+1, i, SHADOW_CH);
															unhide_ch(j+2, WIN_ABS_HEIGHT, SHADOW_CH);
														}
															else
														{
															for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_ch(j+1, i, FRAME_CH);
														}
													}
												}
												/* Shift text over */
												if (diff < 0) {
													for (i = 0; i < WIN_TEXT_HEIGHT; i++) {
														for (j = 0; j < (WIN_TEXT_WIDTH - abs(diff)); j++) {
															read_char_and_attr(buffer, j+abs(diff)+1, i+1, 1);
															WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j+1, i+1);
														}
													}
												}
												/* Change coordinates */
												WINDOW->topxpos -= diff;
												WINDOW->frametopxpos -= diff;
												WINDOW->texttopxpos -= diff;
												/* Reallocate text array */
												for (i = 0; i < WIN_ABS_HEIGHT; i++) {
													/* Reallocate text array*/
                          if ((WINDOW->text[i] = (char FAR *) FARREALLOC(WINDOW->text[i], sizeof(uchar) * 2 * WIN_ABS_WIDTH)) == NULL) {
														/* Error reallocating memory */
														printf("win_c::resize(RIGHT) error : Unable to reallocate text buffer.\n");
														EXITCRITSEC();
														exit(0);
													}
												}
												if (diff > 0) {
													for (i = 0; i < WIN_TEXT_HEIGHT; i++) {
														for (j = WIN_TEXT_WIDTH; j > diff; j--) {
															read_char_and_attr(buffer, j-diff, i+1, 1);
															WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j, i+1);
														}
													}
												}

												/* Redraw the shadow and frame in memory */
												redraw();
												/* If window expanded clear the new area */
												if (diff > 0) {
													for (i = 0; i < diff; i++) {
														for (j = 1; j <= WIN_TEXT_HEIGHT; j++) WIN_TEXT_PUTCH(' ', i+1, j, curr_fcolor, curr_bcolor);
													}
												}
												/* Redraw window parts of window */
												if (diff > 0) display_part(TEXT | FRAME_LEFT | FRAME_TOP | FRAME_BOTTOM | SHADOW);
													else display_part(FRAME_LEFT | FRAME_TOP | FRAME_BOTTOM | SHADOW);
											}
											break;
										}
				case TOP : {
										 /* Calculate the difference of the new width and the current width */
										 diff = (width - WIN_FRAME_HEIGHT);
										 /* If there is a different than resize the window */
										 if (diff) {
											 /* If window is shorter than unhide part that will no longer be there */
											 if (diff < 0) {
												 for (j = 0; j < abs(diff); j++) {
													 /* Unhide all the columns that will not be part of the window anymore */
													 if (WINDOW->shadow) {
														 for (i = 1; i <= WIN_ABS_WIDTH; i++) unhide_ch(i, WIN_ABS_HEIGHT-j, SHADOW_CH);
														 unhide_ch(1, WIN_ABS_HEIGHT-j-1, SHADOW_CH);
													 }
														 else
													 {
														 for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_ch(i, WIN_FRAME_HEIGHT-j, FRAME_CH);
													 }
												 }
											 }
											 /* Reallocate text array */
											 if (diff < 0) {
												 for (i = 0; i < abs(diff); i++) FARFREE(WINDOW->text[WIN_ABS_HEIGHT - i - 1]);
											 }
											 /* Change coordinates */
											 WINDOW->botypos += diff;
											 WINDOW->framebotypos += diff;
											 WINDOW->textbotypos += diff;
											 /* Reallocate text array */
                       if ((WINDOW->text = (char FAR * FAR *) FARREALLOC(WINDOW->text, sizeof(char FAR *) * WIN_ABS_HEIGHT)) == NULL) {
												 /* Error allocating array */
												 printf("win_c::resize(TOP) error : Unable to reallocate buffer for text.\n");
												 EXITCRITSEC();
												 exit(0);
											 }
											 /* Allocate rows of text */
											 if (diff > 0) {
												 for (i = 0; i < abs(diff); i++) {
                           if ((WINDOW->text[WIN_ABS_HEIGHT - i - 1] = (char FAR *) FARMALLOC(sizeof(char) * 2 * WIN_ABS_WIDTH)) == NULL) {
														 /* Error allocating memory */
														 printf("win_c::resize(TOP) error : Unable to reallocate buffer for text.\n");
														 EXITCRITSEC();
														 exit(0);
													 }
												 }
											 }
											 /* Redraw the shadow and frame in memory */
											 redraw();
											 /* Clear text area */
											 if (diff > 0) {
												 for (i = 0; i < diff; i++) {
													 for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(' ', j, WIN_TEXT_HEIGHT - i, curr_fcolor, curr_bcolor);
												 }
											 }
											 /* Redraw window parts of window */
											 if (diff > 0) display_part(TEXT | FRAME_RIGHT | FRAME_LEFT | FRAME_BOTTOM | SHADOW);
												 else display_part(FRAME_RIGHT | FRAME_LEFT | FRAME_BOTTOM | SHADOW);
										 }
										 break;
									 }
				case BOTTOM : {
												/* Calculate the difference of the new width and the current width */
												diff = (width - WIN_FRAME_HEIGHT);
												/* If there is a different than resize the window */
												if (diff) {
													/* If window is shorter than unhide part that will no longer be there */
													if (diff < 0) {
														for (j = 0; j < abs(diff); j++) {
															/* Unhide all the columns that will not be part of the window anymore */
															if (WINDOW->shadow) {
																for (i = 1; i <= WIN_ABS_WIDTH; i++) unhide_ch(i, j+1, SHADOW_CH);
																unhide_ch(WIN_ABS_WIDTH, j+2, SHADOW_CH);
															}
																else
															{
																for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_ch(i, j+1, FRAME_CH);
															}
														}
													}
													/* Shift text up */
													if (diff < 0) {
														for (i = 0; i < (WIN_TEXT_HEIGHT - abs(diff)); i++) {
															for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
																read_char_and_attr(buffer, j, i+abs(diff)+1, 1);
																WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j, i+1);
															}
														}
													}
													/* Deallocate rows of text */
													if (diff < 0) {
														for (i = 0; i < abs(diff); i++) FARFREE(WINDOW->text[WIN_ABS_HEIGHT - i - 1]);
													}
													/* Change coordinates */
													WINDOW->topypos -= diff;
													WINDOW->frametopypos -= diff;
													WINDOW->texttopypos -= diff;
													/* Reallocate text array */
                          if ((WINDOW->text = (char FAR * FAR *) FARREALLOC(WINDOW->text, sizeof(char FAR *) * WIN_ABS_HEIGHT)) == NULL) {
														/* Error allocating array */
														printf("win_c::resize(BOTTOM) error : Unable to allocate buffer for new text.\n");
														EXITCRITSEC();
														exit(0);
													}
													/* Allocate rows of text */
													if (diff > 0) {
														for (i = 0; i < abs(diff); i++) {
                              if ((WINDOW->text[WIN_ABS_HEIGHT - i - 1] = (char FAR *) FARMALLOC(sizeof(char) * 2 * WIN_ABS_WIDTH)) == NULL) {
																/* Error allocating array */
																printf("win_c::resize(BOTTOM) error : Unable to allocate memory for new text.\n");
																EXITCRITSEC();
																exit(0);
															}
														}
													}

													/* Shift text down */
													if (diff > 0) {
														for (i = WIN_TEXT_HEIGHT; i > diff; i--) {
															for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
																read_char_and_attr(buffer, j, i-diff, 1);
																WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j, i);
															}
														}
													}

													/* Redraw the shadow and frame in memory */
													redraw();
													/* Clear text area */
													if (diff > 0) {
														for (i = 1; i <= diff; i++) {
															for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(' ', j, i, curr_fcolor, curr_bcolor);
														}
													}
													/* Redraw window parts of window */
													if (diff > 0) display_part(TEXT | FRAME_RIGHT | FRAME_LEFT | FRAME_TOP | SHADOW);
														else display_part(FRAME_RIGHT | FRAME_LEFT | FRAME_TOP | SHADOW);
												}
												break;
											}
			}
			EXITCRITSEC();
		}

void win_c::implode(void)
		{
			uchar i, j;										// General counters
			int   diff;										// General difference holder
      char buffer[2];               // Holds a character and attribute

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

			/* Implode window */
			while ((WIN_TEXT_WIDTH > 1) || (WIN_TEXT_HEIGHT > 1)) {
				/* Resize all the sides */
				/* LEFT */
				if (WIN_TEXT_WIDTH > 1) {
					/* Calculate the difference of the new width and the current width */
					if (WIN_TEXT_WIDTH > 3)	diff = -2;
						else diff = -1;
					/* Unhide part that will no longer be there */
					for (j = 0; j < abs(diff); j++) {
						/* Unhide all the columns that will not be part of the window anymore */
						if (WINDOW->shadow) {
							for (i = 1; i <= WIN_ABS_HEIGHT; i++) unhide_ch(WIN_ABS_WIDTH-j, i, SHADOW_CH);
							unhide_ch(WIN_ABS_WIDTH-j-1, 1, SHADOW_CH);
						}
							else
						{
							for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_ch(WIN_FRAME_WIDTH-j, i, FRAME_CH);
						}
					}
					/* Change coordinates */
					WINDOW->botxpos += diff;
					WINDOW->framebotxpos += diff;
					WINDOW->textbotxpos += diff;
					/* Reallocate text array */
					for (i = 0; i < WIN_ABS_HEIGHT; i++) {
						/* Reallocate text array*/
            if ((WINDOW->text[i] = (char FAR *) FARREALLOC(WINDOW->text[i], sizeof(uchar) * 2 * WIN_ABS_WIDTH)) == NULL) {
							/* Error reallocating memory */
							printf("win_c::implode(LEFT) error : Unable to reallocate text buffer.\n");
							EXITCRITSEC();
							exit(0);
						}
					}
					/* Redraw the shadow and frame in memory */
					redraw();
				}
				/* RIGHT */
				if (WIN_TEXT_WIDTH > 1) {
					/* Calculate the difference of the new width and the current width */
					if (WIN_TEXT_WIDTH > 3) diff = -2;
						else diff = -1;
					/* Unhide part that will no longer be there */
					for (j = 0; j < abs(diff); j++) {
						/* Unhide all the columns that will not be part of the window anymore */
						if (WINDOW->shadow) {
							for (i = 1; i <= WIN_ABS_HEIGHT; i++) unhide_ch(j+1, i, SHADOW_CH);
							unhide_ch(j+2, WIN_ABS_HEIGHT, SHADOW_CH);
						}
							else
						{
							for (i = 1; i <= WIN_FRAME_HEIGHT; i++) unhide_ch(j+1, i, FRAME_CH);
						}
					}
					/* Shift text over */
					for (i = 0; i < WIN_TEXT_HEIGHT; i++) {
						for (j = 0; j < (WIN_TEXT_WIDTH - abs(diff)); j++) {
							read_char_and_attr(buffer, j+abs(diff)+1, i+1, 1);
							WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j+1, i+1);
						}
					}
					/* Change coordinates */
					WINDOW->topxpos -= diff;
					WINDOW->frametopxpos -= diff;
					WINDOW->texttopxpos -= diff;
					/* Reallocate text array */
					for (i = 0; i < WIN_ABS_HEIGHT; i++) {
						/* Reallocate text array*/
            if ((WINDOW->text[i] = (char FAR *) FARREALLOC(WINDOW->text[i], sizeof(uchar) * 2 * WIN_ABS_WIDTH)) == NULL) {
							/* Error reallocating memory */
							printf("win_c::resize(RIGHT) error : Unable to reallocate text buffer.\n");
							EXITCRITSEC();
							exit(0);
						}
					}
					/* Redraw the shadow and frame in memory */
					redraw();
				}
				/* TOP */
				if (WIN_TEXT_HEIGHT > 1) {
					/* Calculate the difference of the new width and the current width */
					diff = -1;
					/* Unhide part that will no longer be there */
					for (j = 0; j < abs(diff); j++) {
						/* Unhide all the columns that will not be part of the window anymore */
						if (WINDOW->shadow) {
							for (i = 1; i <= WIN_ABS_WIDTH; i++) unhide_ch(i, WIN_ABS_HEIGHT-j, SHADOW_CH);
							unhide_ch(1, WIN_ABS_HEIGHT-j-1, SHADOW_CH);
						}
							else
						{
							for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_ch(i, WIN_FRAME_HEIGHT-j, FRAME_CH);
						}
					}
					/* Reallocate text array */
					for (i = 0; i < abs(diff); i++) FARFREE(WINDOW->text[WIN_ABS_HEIGHT - i - 1]);
					/* Change coordinates */
					WINDOW->botypos += diff;
					WINDOW->framebotypos += diff;
					WINDOW->textbotypos += diff;
					/* Reallocate text array */
          if ((WINDOW->text = (char FAR * FAR *) FARREALLOC(WINDOW->text, sizeof(uchar FAR *) * WIN_ABS_HEIGHT)) == NULL) {
						/* Error allocating array */
						printf("win_c::resize(TOP) error : Unable to reallocate buffer for text.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Redraw the shadow and frame in memory */
					redraw();
				}
				/* BOTTOM */
				if (WIN_TEXT_HEIGHT > 1) {
					/* Calculate the difference of the new width and the current width */
					diff = -1;
					/* Unhide part that will no longer be there */
					for (j = 0; j < abs(diff); j++) {
						/* Unhide all the columns that will not be part of the window anymore */
						if (WINDOW->shadow) {
							for (i = 1; i <= WIN_ABS_WIDTH; i++) unhide_ch(i, j+1, SHADOW_CH);
							unhide_ch(WIN_ABS_WIDTH, j+2, SHADOW_CH);
						}
							else
						{
							for (i = 1; i <= WIN_FRAME_WIDTH; i++) unhide_ch(i, j+1, FRAME_CH);
						}
					}
					/* Shift text up */
					for (i = 0; i < (WIN_TEXT_HEIGHT - abs(diff)); i++) {
						for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
							read_char_and_attr(buffer, j, i+abs(diff)+1, 1);
							WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j, i+1);
						}
					}
					/* Deallocate rows of text */
					for (i = 0; i < abs(diff); i++) FARFREE(WINDOW->text[WIN_ABS_HEIGHT - i - 1]);
					/* Change coordinates */
					WINDOW->topypos -= diff;
					WINDOW->frametopypos -= diff;
					WINDOW->texttopypos -= diff;
					/* Reallocate text array */
          if ((WINDOW->text = (char FAR * FAR *) FARREALLOC(WINDOW->text, sizeof(uchar FAR *) * WIN_ABS_HEIGHT)) == NULL) {
						/* Error allocating array */
						printf("win_c::resize(BOTTOM) error : Unable to allocate buffer for new text.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Redraw the shadow and frame in memory */
					redraw();
				}

				/* Redisplay the whole window */
				redisplay();
			}
			EXITCRITSEC();
		}

void win_c::explode(uchar x1, uchar y1, uchar x2, uchar y2)
		{
			uchar i, j;										// General counters
			int   diff;										// General difference holder
      char buffer[2];               // Holds a character and attribute

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

			/* Explode window */
			while ((WINDOW->frametopxpos > x1) || (WINDOW->frametopypos > y1) || (WINDOW->framebotxpos < x2) || (WINDOW->framebotypos < y2)) {
				/* Resize all the sides until window is the correct size */
				/* LEFT */
				if (WINDOW->framebotxpos < x2) {
					/* Calculate the difference of the new width and the current width */
					if ((WINDOW->framebotxpos + 2) <= x2) diff = 2;
						else diff = 1;
					/* Change coordinates */
					WINDOW->botxpos += diff;
					WINDOW->framebotxpos += diff;
					WINDOW->textbotxpos += diff;
					/* Reallocate text array */
					for (i = 0; i < WIN_ABS_HEIGHT; i++) {            //FARFREE(WINDOW->text[i]);
						/* Reallocate text array*/
            if ((WINDOW->text[i] = (char FAR *) FARREALLOC(WINDOW->text[i], sizeof(uchar) * 2 * WIN_ABS_WIDTH)) == NULL) {
							/* Error reallocating memory */
							printf("win_c::resize(LEFT) error : Unable to reallocate text buffer.\n");
							EXITCRITSEC();
							exit(0);
						}
					}
					/* Redraw the shadow and frame in memory */
					redraw();
					/* If window expanded clear the new area */
					for (i = (WIN_TEXT_WIDTH - diff + 1); i <= WIN_TEXT_WIDTH; i++) {
						for (j = 1; j <= WIN_TEXT_HEIGHT; j++) WIN_TEXT_PUTCH(' ', i, j, curr_fcolor, curr_bcolor);
					}
				}
				/* RIGHT */
				if (WINDOW->frametopxpos > x1) {
					/* Calculate the difference of the new width and the current width */
					if ((WINDOW->frametopxpos - 2) >= x1) diff = 2;
						else diff = 1;
					/* Change coordinates */
					WINDOW->topxpos -= diff;
					WINDOW->frametopxpos -= diff;
					WINDOW->texttopxpos -= diff;
					/* Reallocate text array */
					for (i = 0; i < WIN_ABS_HEIGHT; i++) {
						/* Reallocate text array*/
            if ((WINDOW->text[i] = (char FAR *) FARREALLOC(WINDOW->text[i], sizeof(uchar) * 2 * WIN_ABS_WIDTH)) == NULL) {
							/* Error reallocating memory */
							printf("win_c::resize(RIGHT) error : Unable to reallocate text buffer.\n");
							EXITCRITSEC();
							exit(0);
						}
					}
					/* Shift text */
					for (i = 0; i < WIN_TEXT_HEIGHT; i++) {
						for (j = WIN_TEXT_WIDTH; j > diff; j--) {
							read_char_and_attr(buffer, j-diff, i+1, 1);
							WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j, i+1);
						}
					}
					/* Redraw the shadow and frame in memory */
					redraw();
					/* Window expanded clear the new area */
					for (i = 0; i < diff; i++) {
						for (j = 1; j <= WIN_TEXT_HEIGHT; j++) WIN_TEXT_PUTCH(' ', i+1, j, curr_fcolor, curr_bcolor);
					}
				}
				/* TOP */
				if (WINDOW->framebotypos < y2) {
					/* Calculate the difference of the new width and the current width */
					diff = 1;
					/* Change coordinates */
					WINDOW->botypos += diff;
					WINDOW->framebotypos += diff;
					WINDOW->textbotypos += diff;
					/* Reallocate text array */
          if ((WINDOW->text = (char FAR * FAR *) FARREALLOC(WINDOW->text, sizeof(uchar FAR *) * WIN_ABS_HEIGHT)) == NULL) {
						/* Error allocating array */
						printf("win_c::resize(TOP) error : Unable to reallocate buffer for text.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Allocate rows of text */
					for (i = 0; i < abs(diff); i++) {
            if ((WINDOW->text[WIN_ABS_HEIGHT - i - 1] = (char FAR *) FARMALLOC(sizeof(char) * 2 * WIN_ABS_WIDTH)) == NULL) {
							/* Error allocating memory */
							printf("win_c::resize(TOP) error : Unable to reallocate buffer for text.\n");
							EXITCRITSEC();
							exit(0);
						}
					}
					/* Redraw the shadow and frame in memory */
					redraw();
					/* Clear text area */
					for (i = 0; i < diff; i++) {
						for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(' ', j, WIN_TEXT_HEIGHT - i, curr_fcolor, curr_bcolor);
					}
				}
				/* BOTTOM */
				if (WINDOW->frametopypos > y1) {
					/* Calculate the difference of the new width and the current width */
					diff = 1;
					/* Change coordinates */
					WINDOW->topypos -= diff;
					WINDOW->frametopypos -= diff;
					WINDOW->texttopypos -= diff;
					/* Reallocate text array */
          if ((WINDOW->text = (char FAR * FAR *) FARREALLOC(WINDOW->text, sizeof(uchar FAR *) * WIN_ABS_HEIGHT)) == NULL) {
						/* Error allocating array */
						printf("win_c::resize(BOTTOM) error : Unable to allocate buffer for new text.\n");
						EXITCRITSEC();
						exit(0);
					}
					/* Allocate rows of text */
					for (i = 0; i < abs(diff); i++) {
            if ((WINDOW->text[WIN_ABS_HEIGHT - i - 1] = (char FAR *) FARMALLOC(sizeof(char) * 2 * WIN_ABS_WIDTH)) == NULL) {
							/* Error allocating array */
							printf("win_c::resize(BOTTOM) error : Unable to allocate memory for new text.\n");
							EXITCRITSEC();
							exit(0);
						}
					}
					/* Shift text down */
					for (i = WIN_TEXT_HEIGHT; i > diff; i--) {
						for (j = 1; j <= WIN_TEXT_WIDTH; j++) {
							read_char_and_attr(buffer, j, i-diff, 1);
							WIN_TEXT_PUTCH_AND_ATTR(buffer[0], buffer[1], j, i);
						}
					}
					/* Redraw the shadow and frame in memory */
					redraw();
					/* Clear text area */
					for (i = 1; i <= diff; i++) {
						for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(' ', j, i, curr_fcolor, curr_bcolor);
					}
				}

				/* Redisplay whole window */
				redisplay();
			}
			EXITCRITSEC();
		}

void win_c::frame_change(uchar new_frame_type)
		{
			uchar j;		// General counter

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

			/* Check for the removal of the frame */
			if (!new_frame_type && WINDOW->frame) {
				/* They decided to remove the frame so clear the border and extend the text window */
				/* Top left corner */
				WIN_FRAME_PUTCH(' ', 1, 1, curr_fcolor, curr_bcolor);
				/* Top line */
				for (j = 1; j <= (WIN_FRAME_WIDTH - 2); j++) WIN_FRAME_PUTCH(' ', j+1, 1, curr_fcolor, curr_bcolor);
				/* Top right corner */
				WIN_FRAME_PUTCH(' ', WIN_FRAME_WIDTH, 1, curr_fcolor, curr_bcolor);
				/* Left line */
				for (j = 1; j <= (WIN_FRAME_HEIGHT - 2); j++) WIN_FRAME_PUTCH(' ', 1, j+1, curr_fcolor, curr_bcolor);
				/* Bottom left corner */
				WIN_FRAME_PUTCH(' ', 1, WIN_FRAME_HEIGHT, curr_fcolor, curr_bcolor);
				/* Bottom line */
				for (j = 1; j <= (WIN_FRAME_WIDTH - 2); j++) WIN_FRAME_PUTCH(' ', j+1, WIN_FRAME_HEIGHT, curr_fcolor, curr_bcolor);
				/* Bottom right corner */
				WIN_FRAME_PUTCH(' ', WIN_FRAME_WIDTH, WIN_FRAME_HEIGHT, curr_fcolor, curr_bcolor);
				/* Right line */
				for (j = 1; j <= (WIN_FRAME_HEIGHT - 2); j++) WIN_FRAME_PUTCH(' ', WIN_FRAME_WIDTH, j+1, curr_fcolor, curr_bcolor);
				WINDOW->texttopxpos -= 1;
				WINDOW->texttopypos -= 1;
				WINDOW->textbotxpos += 1;
				WINDOW->textbotypos += 1;
				display_part(TEXT);
				EXITCRITSEC();
				return;
			}
				else
			/* Check for create of the frame */
			if (new_frame_type && !WINDOW->frame) {
				WINDOW->texttopxpos += 1;
				WINDOW->texttopypos += 1;
				WINDOW->textbotxpos -= 1;
				WINDOW->textbotypos -= 1;
			}

			WINDOW->frame = new_frame_type;

			/* Redraw window */
			redraw();
			display_part(FRAME);
			EXITCRITSEC();
		}

void win_c::fill(char ch)
		{
			uchar i, j;

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

			/* Clear window where frame and text window is in memory */
			for (i = 1; i <= WIN_TEXT_HEIGHT; i++) {
				for (j = 1; j <= WIN_TEXT_WIDTH; j++) WIN_TEXT_PUTCH(ch, j, i, curr_fcolor, curr_bcolor);
			}

			/* Redisplay window */
			display_part(TEXT);
			EXITCRITSEC();
		}

void win_c::set_attr(ushort new_attr)
		{
			ENTERCRITSEC();

			/* Update position in window array */
			get_pos();

			/* See if we found the position */
			if (pos == -1) {
				EXITCRITSEC();
				return;
			}

			WINDOW->attr = new_attr;
			EXITCRITSEC();
		}

ushort win_c::get_attr(void)
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
			return(WINDOW->attr);
		}

void win_c::destroy(void)
		{
			uchar i;          // General Counter

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

			/* Implode window if it is supposed to */
			if (WINDOW->attr & WIN_IMPLODE) implode();

			/* Hide window */
			hide();

			/* Free text array */
			for (i = 0; i < WIN_ABS_HEIGHT; i++) FARFREE(WINDOW->text[i]);
			FARFREE(WINDOW->text);

			/* Free stored background */
			if (!ti2_destroy_all) FREE(title);

			FARFREE(ti2_windows[pos]);

			/* Remove window from window array and shift the rest of the pointers down */
			if ((pos + 1) < ti2_num_windows) {
				FARMEMMOVE(&ti2_windows[pos], &ti2_windows[pos + 1], sizeof(win_t FAR *) * (ti2_num_windows - pos - 1));
			}

			/* Reallocate window array */
			ti2_num_windows--;
			if (((ti2_windows = (win_t FAR * FAR *) FARREALLOC(ti2_windows, sizeof(win_t FAR *) * ti2_num_windows)) == NULL) && (ti2_num_windows)) {
				/* Error reallocating ti2_windows array */
				printf("win_c::destroy() error : Unable to reallocate ti2_windows array.\n");
				EXITCRITSEC();
				exit(0);
			}
			EXITCRITSEC();
		}

void win_c::input_init(uchar new_input_norm_fcolor, uchar new_input_norm_bcolor, uchar new_input_high_fcolor,
											 uchar new_input_high_bcolor, uchar new_input_blank_fcolor, uchar new_input_blank_bcolor,
											 uchar new_input_text_fcolor, uchar new_input_text_bcolor)
		{
			ENTERCRITSEC();
			input_norm_fcolor = new_input_norm_fcolor;
			input_norm_bcolor = new_input_norm_bcolor;
			input_high_fcolor = new_input_high_fcolor;
			input_high_bcolor = new_input_high_bcolor;
			input_blank_fcolor = new_input_blank_fcolor;
			input_blank_bcolor = new_input_blank_bcolor;
			input_text_fcolor = new_input_text_fcolor;
			input_text_bcolor = new_input_text_bcolor;
			EXITCRITSEC();
		}

void win_c::input_string(uchar x, uchar y, char *s, uchar max_len)
		{
			uchar done = 0;
			uchar ch;
			uchar curr_len;
      char *temp_str;
      char *temp_str2;

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

			EXITCRITSEC();

			/* Allocate temp strings */
      temp_str = (char *)malloc(sizeof(char) * (max_len + 1));
      temp_str2 = (char *)malloc(sizeof(char) * (max_len + 1));

			/* Make sure string isn't to long */
			if (strlen(s) > max_len) s[max_len - 1] = '\0';

			/* Copy string into temporary string */
			strcpy(temp_str, s);

			/* Store length of string */
			curr_len = strlen(temp_str);

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Print field */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str2, "");
			strpad(temp_str2, max_len, '°');
			wprintf(temp_str2);

			/* Print string in highlight */
			text_color(input_high_fcolor);
			text_background(input_high_bcolor);
			goto_xy(x, y);
			wprintf(temp_str);

			/* Act special for first character */
			ch = GETCH();
			switch(ch) {
				case BACK_SPACE : {
														if (curr_len > 0){
															curr_len--;
															strtrim(temp_str, curr_len);
															text_color(input_blank_fcolor);
															text_background(input_blank_bcolor);
															wprintf("\b°\b");
															text_color(input_norm_fcolor);
															text_background(input_norm_bcolor);
														}
														break;
													}
				case CR : {
										strcpy(s, temp_str);
										done = 1;
										break;
									}
				case ESC : {
										 done = 1;
										 break;
									 }
				case 0 : { // Extended key like arrows
									 ch = GETCH();
									 sprintf(temp_str, "");
									 curr_len = 0;
									 break;
								 }
				default : {
										sprintf(temp_str, "%c", ch);
										curr_len = 1;
									}
			}

			/* Redraw string */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str2, "");
			strpad(temp_str2, max_len, '°');
			wprintf(temp_str2);
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(temp_str);

			while(!done) {
				ch = GETCH();
				switch (ch) {
					case 0 : {
										 ch = GETCH();
										 break;
									 }
					case CR : {
											strcpy (s, temp_str);
											done = 1;
											break;
										}
					case BACK_SPACE : {
															if (curr_len > 0) {
																curr_len--;
																strtrim(temp_str, curr_len);
																text_color(input_blank_fcolor);
																text_background(input_blank_bcolor);
																wprintf("\b°\b");
																text_color(input_norm_fcolor);
																text_background(input_norm_bcolor);
															}
															break;
														}
					case ESC : {
											 done = 1;
											 break;
										 }
					default : {
											if (curr_len < max_len) {
												strcatch(temp_str, ch);
												curr_len++;
												wputch(ch);
											}
											break;
										}
				}
			}

			/* Print field */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			strcpy(temp_str2, "");
			strpad(temp_str2, max_len, ' ');
			wprintf(temp_str2);

			/* Print string in highlight */
			goto_xy(x, y);
			wprintf(s);

			/* Deallocate temp strings */
			FREE(temp_str);
			FREE(temp_str2);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_upcase_string(uchar x, uchar y, char *s, uchar max_len)
		{
			uchar done = 0;
			uchar ch;
			uchar curr_len;
      char *temp_str;
      char *temp_str2;

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

			EXITCRITSEC();

			/* Allocate temp strings */
      temp_str = (char *)malloc(sizeof(char) * (max_len + 1));
      temp_str2 = (char *)malloc(sizeof(char) * (max_len + 1));

			/* Make sure string isn't to long */
			if (strlen(s) > max_len) s[max_len - 1] = '\0';

			/* Copy string into temporary string */
			strcpy(temp_str, s);

			/* Store length of string */
			curr_len = strlen(temp_str);

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Print field */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str2, "");
			strpad(temp_str2, max_len, '°');
			wprintf(temp_str2);

			/* Print string in highlight */
			text_color(input_high_fcolor);
			text_background(input_high_bcolor);
			goto_xy(x, y);
			wprintf(temp_str);

			/* Act special for first character */
			ch = GETCH();
			switch(ch) {
				case BACK_SPACE : {
														if (curr_len > 0){
															curr_len--;
															strtrim(temp_str, curr_len);
															text_color(input_blank_fcolor);
															text_background(input_blank_bcolor);
															wprintf("\b°\b");
															text_color(input_norm_fcolor);
															text_background(input_norm_bcolor);
														}
														break;
													}
				case CR : {
										strcpy(s, temp_str);
										done = 1;
										break;
									}
				case ESC : {
										 done = 1;
										 break;
									 }
				case 0 : { // Extended key like arrows
									 ch = GETCH();
									 sprintf(temp_str, "");
									 curr_len = 0;
									 break;
								 }
				default : {
										sprintf(temp_str, "%c", toupper(ch));
										curr_len = 1;
									}
			}

			/* Redraw string */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str2, "");
			strpad(temp_str2, max_len, '°');
			wprintf(temp_str2);
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(temp_str);

			while(!done) {
				ch = GETCH();
				switch (ch) {
					case 0 : {
										 ch = GETCH();
										 break;
									 }
					case CR : {
											strcpy (s, temp_str);
											done = 1;
											break;
										}
					case BACK_SPACE : {
															if (curr_len > 0) {
																curr_len--;
																strtrim(temp_str, curr_len);
																text_color(input_blank_fcolor);
																text_background(input_blank_bcolor);
																wprintf("\b°\b");
																text_color(input_norm_fcolor);
																text_background(input_norm_bcolor);
															}
															break;
														}
					case ESC : {
											 done = 1;
											 break;
										 }
					default : {
											if (curr_len < max_len) {
												strcatch(temp_str, toupper(ch));
												curr_len++;
												wputch(toupper(ch));
											}
											break;
										}
				}
			}

			/* Print field */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			strcpy(temp_str2, "");
			strpad(temp_str2, max_len, ' ');
			wprintf(temp_str2);

			/* Print string in highlight */
			goto_xy(x, y);
			wprintf(s);

			/* Deallocate temp strings */
			FREE(temp_str);
			FREE(temp_str2);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_frmt_string(uchar x, uchar y, char *str, char *ctrl_str)
		{
			uchar max_len;
			uchar curr_pos;
			uchar ch;         // Character inputed by user
			uchar min_pos;    // Minimum value curr_pos can be
			uchar max_pos;    // Maximum value curr_pos can be
			uchar done = 0;
      char *temp_str;
			uchar i;

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

			EXITCRITSEC();

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Initialize variables */
			/* Find maximum length of the string we are inputting */
			max_len = strlen(ctrl_str);
			/* Find minimum position for input */
			curr_pos = 1;
			while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos < max_len)) {
				curr_pos++;
			}
			min_pos = curr_pos;
			/* Find maximum position for input */
			curr_pos = max_len;
			while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos > 1)) {
				curr_pos--;
			}
			max_pos = curr_pos;
			/* Fill str correctly and find first point to edit */
			curr_pos = strlen(str);
			/* If current position is one of the valid characters than increase by one */
			if ((ctrl_str[curr_pos - 1] == '#') || (ctrl_str[curr_pos - 1] == 'A') || (ctrl_str[curr_pos - 1] == 'a')) curr_pos++;
			/* Find a position if current position is valid */
			while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos < max_len)) {
				curr_pos++;
			}
			/* If current position is not a valid position and it is at the end seek for a current position to the left */
			if (curr_pos <= max_len) {
				while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos > 1)) {
					curr_pos--;
				}
			}
			/* If current_position is at the end or still not a valid position increase by one */
			//  || ((ctrl_str[curr_pos - 1] != '#')  && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a'))
			if ((curr_pos == max_pos)) curr_pos++;
			/* Search one more time for a current position to the right if current one isn't valid */
			/* if (curr_pos < max_pos) {
				while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos < max_len)) {
					curr_pos++;
				}
			}  */
			/* Fill string with correct characters from ctrl_str */
			strpad(str, max_len, ' ');
			for (i = 0; i < max_len; i++) {
				if ((ctrl_str[i] == '#') || (ctrl_str[i] == 'A') || (ctrl_str[i] == 'a')) {
					if (isalpha(ctrl_str[i])) {
						if (ctrl_str[i] == 'A') str[i] = toupper(str[i]);
							else str[i] = tolower(str[i]);
					}
				}
					else str[i] = ctrl_str[i];
			}
			/* Allocate temporary string and copy contents of str to it */
      temp_str = (char *) malloc(sizeof(char) * (max_len + 1));
			strcpy(temp_str, str);

			/* Print blank field */
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(temp_str);
			goto_xy(x+curr_pos-1, y);

			/* Put cursor at right starting position */
			while (!done) {
				ch = GETCH();
				switch (ch) {
					case 0 : {
										 ch = GETCH();
										 break;
									 }
					case CR : {
											if (curr_pos == (max_pos + 1)) {
												done = 1;
												strcpy(str, temp_str);
											}
											break;
										}
					case BACK_SPACE : {
															if (curr_pos > min_pos) {
																/* Find next position to the left */
																curr_pos--;
																while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos > min_pos)) {
																	curr_pos--;
																}
																/* Goto the input position */
																goto_xy(x+curr_pos-1, y);
																/* Erase the character */
																wprintf(" \b");
															}
															break;
														}
					case ESC : {
											 done = 1;
											 break;
										 }
					default : {
											if (curr_pos <= max_pos) {
												if (((ctrl_str[curr_pos - 1] == '#') && (isdigit(ch))) || ((ctrl_str[curr_pos - 1] == 'A') && (isalpha(ch))) || ((ctrl_str[curr_pos - 1] == 'a') && (isalpha(ch))) ) {
													/* Change case of the character if it supposed to be */
													if(isalpha(ch)) {
														if (ctrl_str[curr_pos - 1] == 'A') ch = toupper(ch);
															else ch = tolower(ch);
													}
													/* printf character */
													wputch(ch);
													/* Update string */
													temp_str[curr_pos - 1] = ch;
													/* Find next curr_pos */
													if (curr_pos < max_pos) {
														curr_pos++;
														while(((ctrl_str[curr_pos - 1] != '#') && (ctrl_str[curr_pos - 1] != 'A') && (ctrl_str[curr_pos - 1] != 'a')) && (curr_pos <= max_pos)) {
															curr_pos++;
														}
													}
														else curr_pos = (max_pos + 1);
													/* Goto new position */
													goto_xy(x+curr_pos-1, y);
												}
											}
											break;
										}
				}
			}

			/* Turn cursor off */
      SETCURSORTYPE(_NOCURSOR);

			/* Deallocate temp_str */
			FREE(temp_str);

			/* Reprint string */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			wprintf(str);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_hhmmss(uchar x, uchar y, ulong *t)
		{
      char hhmmss_str[9];

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

			EXITCRITSEC();

			time_to_str(*t, hhmmss_str);
			input_frmt_string(x, y, hhmmss_str, "##:##:##");
			*t = str_to_time(hhmmss_str);

			/* Reprint string just to show if any correction had to be made to the time they entered */
			time_to_str(*t, hhmmss_str);
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			wprintf(hhmmss_str);
		}

void win_c::input_4d_addr(uchar x, uchar y, faddr_t *addr)
		{
			faddr_t temp_addr;
			char temp_addr_str[24];
      char temp_str[24];
			int field;
			int field_cnt[4];
			char ch;
			int colon;
			int slash;
			int period;
			char field_str[4][6];
			int i;
			uchar done = 0;

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

			EXITCRITSEC();

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Initialize Variables */
			temp_addr.zone = addr->zone;
			temp_addr.net = addr->net;
			temp_addr.node = addr->node;
			temp_addr.point = addr->point;
			sprintf(temp_addr_str, "%d:%d/%d.%d", temp_addr.zone, temp_addr.net, temp_addr.node, temp_addr.point);
			field = 3;
			sprintf(field_str[0], "%d", temp_addr.zone);
			sprintf(field_str[1], "%d", temp_addr.net);
			sprintf(field_str[2], "%d", temp_addr.node);
			sprintf(field_str[3], "%d", temp_addr.point);
			for (i = 0; i < 4; i++) field_cnt[i] = strlen(field_str[i]);
			sprintf(field_str[0], "");
			sprintf(field_str[1], "");
			sprintf(field_str[2], "");
			sprintf(field_str[3], "");
			ch = ' ';
			colon = slash = period = 1;

			/* Print field */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, 23, '°');
			wprintf(temp_str);

			/* Print string in highlight */
			text_color(input_high_fcolor);
			text_background(input_high_bcolor);
			goto_xy(x, y);
			wprintf(temp_addr_str);

			/* Act special for first character */
			ch = GETCH();
			switch(ch) {
				case BACK_SPACE : { // Backspace
														if (strlen(temp_addr_str)) {
															if (field_cnt[field] == 0) {
																switch(field) {
																	case 1 : {
																						 field--;
																						 colon = 0;
																						 break;
																					 }
																	case 2 : {
																						 field--;
																						 slash = 0;
																						 break;
																					 }
																	case 3 : {
																						 field--;
																						 period = 0;
																						 break;
																					 }
																}
															}
																else
															{
																field_cnt[field]--;
															}
															strtrim(temp_addr_str, strlen(temp_addr_str) - 1);
															text_color(input_blank_fcolor);
															text_background(input_blank_bcolor);
															wprintf("\b°\b");
															text_color(input_norm_fcolor);
															text_background(input_norm_bcolor);
														}
														break;
													}
				case CR : {
										addr->zone = temp_addr.zone;
										addr->net  = temp_addr.net;
										addr->node = temp_addr.node;
										addr->point = temp_addr.point;
										done = 1;
										break;
									}
				case ESC : {
										 done = 1;
											break;
									 }
				case 0 : { // Extended key like arrows
									 ch = GETCH();
									 sprintf(temp_addr_str, "");
									 field = colon = slash = period = 0;
									 for (i = 0; i < 4; i++) field_cnt[i] = 0;
									 break;
								 }
				default : {
										if (isdigit(ch)) {
											sprintf(temp_addr_str, "%c", ch);
											field = colon = slash = period = 0;
											for (i = 1; i < 4; i++) field_cnt[i] = 0;
											field_cnt[0] = 1;
										}
											else
										{
											sprintf(temp_addr_str, "");
											field = colon = slash = period = 0;
											for (i = 0; i < 4; i++) field_cnt[i] = 0;
										}
									}
			}

			/* Redraw string */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, 23, '°');
			wprintf(temp_str);
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(temp_addr_str);

			/* Process input */
			while (!done) {
				ch = GETCH();
				if ((isdigit(ch)) || (ch == ':') || (ch == '/') || (ch == '.') || (ch == BACK_SPACE) || (ch == CR) || (ch == ESC)) {
					if (isdigit(ch)) {
						if(field_cnt[field] < 5) {
							strcatch(temp_addr_str, ch);
							wputch(ch);
							field_cnt[field]++;
						}
					}
						else
					if (ch == ':') {
						if (!colon) {
							colon = 1;
							field++;
							wprintf(":");
							strcatch(temp_addr_str, ':');
						}
					}
						else
					if (ch == '/') {
						if ((!slash) && (colon)) {
							slash = 1;
							field++;
							wprintf("/");
							strcatch(temp_addr_str, '/');
						}
					}
						else
					if (ch == '.') {
						if ((!period) && (slash) && (colon)) {
							period = 1;
							field++;
							wprintf("->");
              strcatch(temp_addr_str, ':');
						}
					}
						else
					if (ch == BACK_SPACE) {
						if (strlen(temp_addr_str)) {
							if (field_cnt[field] == 0) {
								switch(field) {
									case 1 : {
														 field--;
														 colon = 0;
														 break;
													 }
									case 2 : {
														 field--;
														 slash = 0;
														 break;
													 }
									case 3 : {
														 field--;
														 period = 0;
														 break;
													 }
								}
							}
								else
							{
								field_cnt[field]--;
							}
							strtrim(temp_addr_str, strlen(temp_addr_str) - 1);
							text_color(input_blank_fcolor);
							text_background(input_blank_bcolor);
							wprintf("\b°\b");
							text_color(input_norm_fcolor);
							text_background(input_norm_bcolor);
						}
					}
						else
					if (ch == CR) {
						field = 0;
						for (i = 0; i < strlen(temp_addr_str); i++) {
							if ((temp_addr_str[i] == ':') || (temp_addr_str[i] == '/') || (temp_addr_str[i] == '.')) field++;
								else strcatch(field_str[field], temp_addr_str[i]);
						}

						temp_addr.zone = atous(field_str[0]);
						temp_addr.net = atous(field_str[1]);
						temp_addr.node = atous(field_str[2]);
						temp_addr.point = atous(field_str[3]);

						addr->zone = temp_addr.zone;
						addr->net  = temp_addr.net;
						addr->node = temp_addr.node;
						addr->point = temp_addr.point;

						done = 1;
					}
						else
					if (ch == ESC) {
						done = 1;
					}
				}
			}

			/* Print field */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, 23, ' ');
			wprintf(temp_str);
			/* Print string in highlight */
			goto_xy(x, y);
			wprintf("%u:%u/%u->%u", addr->zone, addr->net, addr->node, addr->point);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_byte(uchar x, uchar y, uchar *num, uchar min, uchar max)
		{
			uchar done = 0;
			uchar ch;
      char num_str[4];
      char temp_str[4];
			uchar max_len;
			uchar curr_len;

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

			EXITCRITSEC();

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Calculate maximum length of num string */
			sprintf(num_str, "%d", max);
			max_len = strlen(num_str);
			strcpy(num_str, "");

			/* Make sure num passed is within min and max values */
			if (*num < min) *num = min;
			if (*num > max) *num = max;

			/* Copy current number to num_str */
			sprintf(num_str, "%d", *num);
			curr_len = strlen(num_str);

			/* Print field */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, '°');
			wprintf(temp_str);

			/* Print string in highlight */
			text_color(input_high_fcolor);
			text_background(input_high_bcolor);
			goto_xy(x, y);
			wprintf(num_str);

			/* Act special for first character */
			ch = GETCH();
			switch(ch) {
				case BACK_SPACE : {
														if (curr_len > 0) {
															num_str[curr_len - 1] = '\0';
															curr_len--;
															text_color(input_blank_fcolor);
															text_background(input_blank_bcolor);
															wprintf("\b°\b");
															text_color(input_norm_fcolor);
															text_background(input_norm_bcolor);
														}
														break;
													}
				case CR : {
										*num = atoi(num_str);
										done = 1;
										break;
									}
				case ESC : {
										 done = 1;
										 break;
									 }
				case 0 : { // Extended key like arrows
									 ch = GETCH();
									 sprintf(num_str, "");
									 curr_len = 0;
									 break;
								 }
				default : {
										if (isdigit(ch)) {
											sprintf(num_str, "%c", ch);
											curr_len = 1;
										}
											else
										{
											sprintf(num_str, "");
											curr_len = 0;
										}
									}
			}

			/* Redraw string */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, '°');
			wprintf(temp_str);
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(num_str);

			while(!done) {
				ch = GETCH();
				switch (ch) {
					case 0 : {
										 ch = GETCH();
										 break;
									 }
					case CR : {
											*num = atoi(num_str);
											done = 1;
											break;
										}
					case BACK_SPACE : {
															if (curr_len > 0) {
																num_str[curr_len - 1] = '\0';
																curr_len--;
																text_color(input_blank_fcolor);
																text_background(input_blank_bcolor);
																wprintf("\b°\b");
																text_color(input_norm_fcolor);
																text_background(input_norm_bcolor);
															}
															break;
														}
					case ESC : {
											 done = 1;
											 break;
										 }
					default : {
											if ((curr_len < max_len) && (isdigit(ch))) {
												num_str[curr_len] = ch;
												num_str[curr_len + 1] = '\0';
												curr_len++;
												wputch(ch);
											}
											break;
										}
				}
			}

			/* Make sure num returned is within min and max values */
			if (*num < min) *num = min;
			if (*num > max) *num = max;

			/* Print field */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, ' ');
			wprintf(temp_str);

			/* Print string */
			goto_xy(x, y);
			wprintf("%d", *num);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_ushort(uchar x, uchar y, ushort *num, ushort min, ushort max)
		{
			uchar done = 0;
			uchar ch;
      char num_str[6];
      char temp_str[6];
			uchar max_len;
			uchar curr_len;

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

			EXITCRITSEC();

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Calculate maximum length of num string */
			sprintf(num_str, "%u", max);
			max_len = strlen(num_str);
			strcpy(num_str, "");

			/* Make sure num passed is within min and max values */
			if (*num < min) *num = min;
			if (*num > max) *num = max;

			/* Copy current number to num_str */
			sprintf(num_str, "%u", *num);
			curr_len = strlen(num_str);

			/* Print field */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, '°');
			wprintf(temp_str);

			/* Print string in highlight */
			text_color(input_high_fcolor);
			text_background(input_high_bcolor);
			goto_xy(x, y);
			wprintf(num_str);

			/* Act special for first character */
			ch = GETCH();
			switch(ch) {
				case BACK_SPACE : {
														if (curr_len > 0) {
															num_str[curr_len - 1] = '\0';
															curr_len--;
															text_color(input_blank_fcolor);
															text_background(input_blank_bcolor);
															wprintf("\b°\b");
															text_color(input_norm_fcolor);
															text_background(input_norm_bcolor);
														}
														break;
													}
				case CR : {
										*num = atous(num_str);
										done = 1;
										break;
									}
				case ESC : {
										 done = 1;
										 break;
									 }
				case 0 : { // Extended key like arrows
									 ch = GETCH();
									 sprintf(num_str, "");
									 curr_len = 0;
									 break;
								 }
				default : {
										if (isdigit(ch)) {
											sprintf(num_str, "%c", ch);
											curr_len = 1;
										}
											else
										{
											sprintf(num_str, "");
											curr_len = 0;
										}
									}
			}

			/* Redraw string */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, '°');
			wprintf(temp_str);
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(num_str);

			while(!done) {
				ch = GETCH();
				switch (ch) {
					case 0 : {
										 ch = GETCH();
										 break;
									 }
					case CR : {
											*num = atous(num_str);
											done = 1;
											break;
										}
					case BACK_SPACE : {
															if (curr_len > 0) {
																num_str[curr_len - 1] = '\0';
																curr_len--;
																text_color(input_blank_fcolor);
																text_background(input_blank_bcolor);
																wprintf("\b°\b");
																text_color(input_norm_fcolor);
																text_background(input_norm_bcolor);
															}
															break;
														}
					case ESC : {
											 done = 1;
											 break;
										 }
					default : {
											if ((curr_len < max_len) && (isdigit(ch))) {
												num_str[curr_len] = ch;
												num_str[curr_len + 1] = '\0';
												curr_len++;
												wputch(ch);
											}
											break;
										}
				}
			}

			/* Make sure num returned is within min and max values */
			if (*num < min) *num = min;
			if (*num > max) *num = max;

			/* Print field */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, ' ');
			wprintf(temp_str);

			/* Print string */
			goto_xy(x, y);
			wprintf("%u", *num);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_ulong(uchar x, uchar y, ulong *num, ulong min, ulong max)
		{
			uchar done = 0;
			uchar ch;
      char num_str[11];
      char temp_str[11];
			uchar max_len;
			uchar curr_len;

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

			EXITCRITSEC();

			/* Turn cursor on */
      SETCURSORTYPE(_NORMALCURSOR);

			/* Calculate maximum length of num string */
			sprintf(num_str, "%lu", max);
			max_len = strlen(num_str);
			strcpy(num_str, "");

			/* Make sure num passed is within min and max values */
			if (*num < min) *num = min;
			if (*num > max) *num = max;

			/* Copy current number to num_str */
			sprintf(num_str, "%lu", *num);
			curr_len = strlen(num_str);

			/* Print field */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, '°');
			wprintf(temp_str);

			/* Print string in highlight */
			text_color(input_high_fcolor);
			text_background(input_high_bcolor);
			goto_xy(x, y);
			wprintf(num_str);

			/* Act special for first character */
			ch = GETCH();
			switch(ch) {
				case BACK_SPACE : {
														if (curr_len > 0) {
															num_str[curr_len - 1] = '\0';
															curr_len--;
															text_color(input_blank_fcolor);
															text_background(input_blank_bcolor);
															wprintf("\b°\b");
															text_color(input_norm_fcolor);
															text_background(input_norm_bcolor);
														}
														break;
													}
				case CR : {
										*num = atoul(num_str);
										done = 1;
										break;
									}
				case ESC : {
										 done = 1;
										 break;
									 }
				case 0 : { // Extended key like arrows
									 ch = GETCH();
									 sprintf(num_str, "");
									 curr_len = 0;
									 break;
								 }
				default : {
										if (isdigit(ch)) {
											sprintf(num_str, "%c", ch);
											curr_len = 1;
										}
											else
										{
											sprintf(num_str, "");
											curr_len = 0;
										}
									}
			}

			/* Redraw string */
			text_color(input_blank_fcolor);
			text_background(input_blank_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, '°');
			wprintf(temp_str);
			text_color(input_norm_fcolor);
			text_background(input_norm_bcolor);
			goto_xy(x, y);
			wprintf(num_str);

			while(!done) {
				ch = GETCH();
				switch (ch) {
					case 0 : {
										 ch = GETCH();
										 break;
									 }
					case CR : {
											*num = atoul(num_str);
											done = 1;
											break;
										}
					case BACK_SPACE : {
															if (curr_len > 0) {
																num_str[curr_len - 1] = '\0';
																curr_len--;
																text_color(input_blank_fcolor);
																text_background(input_blank_bcolor);
																wprintf("\b°\b");
																text_color(input_norm_fcolor);
																text_background(input_norm_bcolor);
															}
															break;
														}
					case ESC : {
											 done = 1;
											 break;
										 }
					default : {
											if ((curr_len < max_len) && (isdigit(ch))) {
												num_str[curr_len] = ch;
												num_str[curr_len + 1] = '\0';
												curr_len++;
												wputch(ch);
											}
											break;
										}
				}
			}

			/* Make sure num returned is within min and max values */
			if (*num < min) *num = min;
			if (*num > max) *num = max;

			/* Print field */
			text_color(input_text_fcolor);
			text_background(input_text_bcolor);
			goto_xy(x, y);
			strcpy(temp_str, "");
			strpad(temp_str, max_len, ' ');
			wprintf(temp_str);

			/* Print string */
			goto_xy(x, y);
			wprintf("%lu", *num);

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_toggle_uchar_bit_yes_no(uchar x, uchar y, uchar *toggle, uchar bit)
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

			EXITCRITSEC();

			if (*toggle & bit) {    // Current state is yes, change to no
				*toggle &= ~(bit);
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("No ");
			}
				else
			{ // Current state is no, change to yes
				*toggle |= bit;
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("Yes");
			}

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_toggle_ushort_bit_yes_no(uchar x, uchar y, ushort *toggle, ushort bit)
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

			EXITCRITSEC();

			if (*toggle & bit) {    // Current state is yes, change to no
				*toggle &= ~(bit);
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("No ");
			}
				else
			{ // Current state is no, change to yes
				*toggle |= bit;
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("Yes");
			}

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_toggle_ulong_bit_yes_no(uchar x, uchar y, ulong *toggle, ulong bit)
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

			EXITCRITSEC();

			if (*toggle & bit) {    // Current state is yes, change to no
				*toggle &= ~(bit);
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("No ");
			}
				else
			{ // Current state is no, change to yes
				*toggle |= bit;
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("Yes");
			}

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}

void win_c::input_toggle_uchar_yes_no(uchar x, uchar y, uchar *toggle)
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

			EXITCRITSEC();

			if (*toggle) {    // Current state is yes, change to no
				*toggle = 0;
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("No ");
			}
				else
			{ // Current state is no, change to yes
				*toggle = 1;
				text_color(input_text_fcolor);
				text_background(input_text_bcolor);
				goto_xy(x, y);
				wprintf("Yes");
			}

			/* Set cursor back to whatever cursor_mode says */
      if (cursor_mode) SETCURSORTYPE(_NORMALCURSOR);
        else SETCURSORTYPE(_NOCURSOR);
		}
