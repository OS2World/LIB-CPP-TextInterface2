/* Filename : INTERNAL.H                                                    *
 * Description : Internal header file for TI/2.                             *
 * Target : N/A                                                             *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 10/25/94                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

/****************************
 * System dependant defines *
 ****************************/

/* If Turbo-C for DOS we need to define min, max functions for C++ */
#if (defined(__MSDOS__) && defined (__TURBOC__)) || defined(__EMX__)  || defined(__GO32__)
	#define max(value1, value2) ((value1 > value2) ? value1 : value2)
	#define min(value1, value2) ((value1 < value2) ? value1 : value2)
#endif

/* Function */
#ifdef _USERTL2_
	#define GETCH kb_getch
	#define KBHIT kb_hit

	/* Function prototypes for improved RTL2 functions */

	void    strpad      (char *s, ushort max_len, char padch);
	void    strcatch    (char *s, char ch);
	void    strtrim     (char *s, ushort num);
	ulong   atoul       (char *str_ptr);
	uint    atoui       (char *str_ptr);
	ushort  atous       (char *str_ptr);
	void    time_to_str (ulong t, char *str);
	ulong   str_to_time (char *str);
	void    date_to_str (ulong d, char *str);
	ulong   str_to_date (char *str);
#endif

/* Borland/Turbo compilers for DOS and OS/2 */
#if defined(__TURBOC__) || defined(__GO32__)
#define PUTTEXT(x1, y1, x2, y2, buffer) puttext(x1, y1, x2, y2, buffer)
#define GETTEXT(x1, y1, x2, y2, buffer) gettext(x1, y1, x2, y2, buffer)
#define GOTOXY(x, y)                    gotoxy(x, y)
#define TEXTMODE(mode)                  textmode(mode)
#define SETCURSORTYPE(mode)             _setcursortype(mode)
#define SCREENHEIGHT                    screenheight
#define SCREENWIDTH                     screenwidth
#define ITOA                            itoa
#define GETCH                           getch
#define KBHIT                           kbhit

int screenheight(void);
int screenwidth(void);

#endif

/* EMX v0.9a */
#ifdef __EMX__

#define PUTTEXT(x1, y1, x2, y2, buffer) puttext(x1, y1, x2, y2, buffer)
#define GETTEXT(x1, y1, x2, y2, buffer) gettext(x1, y1, x2, y2, buffer)
#define GOTOXY(x, y)                    gotoxy(x, y)
#define TEXTMODE(mode)                  textmode(mode)
#define SETCURSORTYPE(mode)             setcursortype(mode)
#define SCREENHEIGHT                    screenheight
#define SCREENWIDTH                     screenwidth
#define ITOA                            _itoa
#define GETCH                           emx_getch
#define KBHIT                           emx_kbhit

#define _NOCURSOR     0
#define _NORMALCURSOR 2

void init_video(void);
int puttext(int left, int top, int right, int bottom, void *source);
int gettext(int left, int top, int right, int bottom, void *destin);
void gotoxy(int x, int y);
void textmode(int newmode);
void setcursortype(int cur_t);
int screenheight(void);
int screenwidth(void);
int emx_kbhit(void);
int emx_getch(void);

#endif

/* MS-DOS Compilers */
#ifdef __MSDOS__
	#ifdef __TURBOC__
		/* Define functions and pointers for the correct memory model */
		#if defined (__HUGE__) || defined(__LARGE__) || defined(__COMPACT__)
			#define FAR         far
			#define FARMALLOC   farmalloc
			#define FARCALLOC   farcalloc
			#define FARREALLOC  farrealloc

			#define FARFREE(ptr)              \
				{                               \
					if (ptr != NULL) {            \
						farfree(ptr);               \
						ptr = NULL;                 \
					}                             \
				}

			#define FARMEMMOVE  _fmemmove
			#define FARMEMSET   _fmemset
			#define FARMEMCPY   _fmemcpy

			#define FREE(ptr)                 \
				{                               \
					if (ptr != NULL) {            \
						free(ptr);                  \
						ptr = NULL;                 \
					}                             \
				}
		#else
			#define FAR
			#define FARMALLOC   malloc
			#define FARCALLOC   calloc
			#define FARREALLOC  realloc

			#define FARFREE(ptr)              \
				{                               \
					if (ptr != NULL) {            \
						free(ptr);                  \
						ptr = NULL;                 \
					}                             \
				}     

			#define FARMEMMOVE  memmove
			#define FARMEMSET   memset
			#define FARMEMCPY   memcpy

			#define FREE(ptr)                 \
				{                               \
					if (ptr != NULL) {            \
						free(ptr);                  \
						ptr = NULL;                 \
					}                             \
				}
		#endif
	#endif
	#ifdef __GO32__
		#define FAR
		#define FARMALLOC   malloc
		#define FARCALLOC   calloc
		#define FARREALLOC  realloc

		#define FARFREE(ptr)              \
			{                               \
				if (ptr != NULL) {            \
					free(ptr);                  \
					ptr = NULL;                 \
				}                             \
			}     

		#define FARMEMMOVE  memmove
		#define FARMEMSET   memset
		#define FARMEMCPY   memcpy

		#define FREE(ptr)                 \
			{                               \
				if (ptr != NULL) {            \
					free(ptr);                  \
					ptr = NULL;                 \
				}                             \
			}
	#endif
#endif

/* OS/2 Compilers */
#if defined(__OS2__) || defined(__EMX__)
	/* Define functions and pointers for unsegmented memory */
	#define FAR
	#define FARMALLOC   malloc
	#define FARCALLOC   calloc
	#define FARREALLOC  realloc

	#define FARFREE(ptr)              \
		{                               \
			if (ptr != NULL) {            \
				free(ptr);                  \
				ptr = NULL;                 \
			}                             \
		}

	#define FARMEMMOVE  memmove
	#define FARMEMSET   memset
	#define FARMEMCPY   memcpy

	#define FREE(ptr)                 \
		{                               \
			if (ptr != NULL) {            \
				free(ptr);                  \
				ptr = NULL;                 \
			}                             \
		}

#endif

/*******************
 * Library Defines *
 *******************/

#define VERSION "0.99e"                    // Current revision of TI/2
//#define _TI2DEBUG_    1                    // Undefine to turn off debugging
//#define _SHAREWARE_ 1                    // Undefine to create commercial version

/**********************
 * Window Global Data *
 **********************/

#if defined(__OS2__) || defined(__EMX__)
extern HMTX   ti2_hmtx;               // Mutually exclusive semaphore
#endif

/************************
 * Window Class Defines *
 ************************/

#define SHADOW                      (1<<0)
#define FRAME_TOP                   (1<<1)
#define FRAME_LEFT                  (1<<2)
#define FRAME_BOTTOM                (1<<3)
#define FRAME_RIGHT                 (1<<4)
#define TEXT                        (1<<5)
#define FRAME                       FRAME_TOP | FRAME_LEFT | FRAME_BOTTOM | FRAME_RIGHT

																				/* Used for calling display_ch and visible_ch */
#define SHADOW_CH                   0
#define FRAME_CH                    1
#define TEXT_CH                     2

																				/* Visibility of a character */
#define NOT_VISIBLE                 0
#define VISIBLE                     1
#define SHADOW_VISIBLE              2
#define SHADOW_NOT_VISIBLE          3

#define WINDOW                      ti2_windows[pos]
#define MAX_WINDOWS                 250
#define WIN_SHADOW_FCOLOR           DARKGRAY
#define WIN_SHADOW_BCOLOR           BLACK
#define WIN_ABS_WIDTH               (WINDOW->botxpos - WINDOW->topxpos + 1)
#define WIN_ABS_HEIGHT              (WINDOW->botypos - WINDOW->topypos + 1)
#define WIN_FRAME_WIDTH             (WINDOW->framebotxpos - WINDOW->frametopxpos + 1)
#define WIN_FRAME_HEIGHT            (WINDOW->framebotypos - WINDOW->frametopypos + 1)
#define WIN_TEXT_WIDTH              (WINDOW->textbotxpos - WINDOW->texttopxpos + 1)
#define WIN_TEXT_HEIGHT             (WINDOW->textbotypos - WINDOW->texttopypos + 1)
#define MAKE_TEXT_ATTR(fore, back)  ((fore & 0x8F) | ((back<< 4) & 0x7F))

#define WIN_XY_IN_RANGE(x, y, x1, y1, x2, y2)   ((x >= x1) && (x <= x2) && (y >= y1) && (y <= y2))
#define WIN_SHADOW_ABS_X(x)                     (WINDOW->topxpos + x - 1)
#define WIN_SHADOW_ABS_Y(y)                     (WINDOW->topypos + y - 1)

#define WIN_FRAME_PUTCH(ch, x, y, fore, back)                           \
		{                                                                   \
			uchar offx, offy;                                                 \
																																				\
			offx = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - 1);   \
			offy = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - 1);   \
			offx *= 2;                                                        \
																																				\
			WINDOW->text[offy][offx] = ch;                                    \
			WINDOW->text[offy][offx+1] = MAKE_TEXT_ATTR(fore, back);          \
		}

#define WIN_TEXT_PUTCH(ch, x, y, fore, back)                                                  \
		{                                                                                         \
			uchar offx, offy;                                                                       \
																																															\
			offx = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1));   \
			offy = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1));   \
			offx *= 2;                                                                              \
																																															\
			WINDOW->text[offy][offx] = ch;                                                          \
			WINDOW->text[offy][offx+1] = MAKE_TEXT_ATTR(fore, back);                                \
		}

#define WIN_TEXT_PUTCH_AND_ATTR(ch, attr, x, y)                                             \
		{                                                                                       \
			uchar offx, offy;                                                                     \
																																														\
			offx = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)); \
			offy = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)); \
			offx *= 2;                                                                            \
																																														\
			WINDOW->text[offy][offx] = ch;                                                        \
			WINDOW->text[offy][offx+1] = attr;                                                    \
		}

#define WIN_TEXT_GETCH(ch, x, y)                                                            \
		{                                                                                       \
			uchar offx, offy;                                                                     \
																																														\
			offx = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)); \
			offy = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)); \
			offx *= 2;                                                                            \
																																														\
			ch = WINDOW->text[offy][offx];                                                       \
		}

#define WIN_TEXT_GETATTR(ch, x, y)                                                          \
		{                                                                                       \
			uchar offx, offy;                                                                     \
																																														\
			offx = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + x - (WINDOW->frame ? 0 : 1)); \
			offy = (((WINDOW->shadow == WIN_SHADOW_LEFT) ? 1 : 0) + y - (WINDOW->frame ? 0 : 1)); \
			offx *= 2;                                                                            \
																																														\
			ch = WINDOW->text[offy][offx + 1];                                                   \
		}

#define WIN_SHADOW_PUTCH(ch, x, y)                                                        \
		{                                                                                     \
			uchar offx, offy;                                                                   \
																																													\
			offx = x - 1;                                                                       \
			offy = y - 1;                                                                       \
			offx *= 2;                                                                          \
																																													\
			WINDOW->text[offy][offx] = ch;                                                      \
			WINDOW->text[offy][offx+1] = MAKE_TEXT_ATTR(WIN_SHADOW_FCOLOR, WIN_SHADOW_BCOLOR);  \
		}

#define WIN_SHADOW_PUTCHATTR(ch, x, y, attr)  \
		{                                         \
			uchar offx, offy;                       \
																							\
			offx = x - 1;                           \
			offy = y - 1;                           \
			offx *= 2;                              \
																							\
			WINDOW->text[offy][offx] = ch;          \
			WINDOW->text[offy][offx+1] = attr;      \
		}

#ifdef __MSDOS__
	#define SOUND(freq, duration)   \
		{                             \
			sound(freq);                \
			delay(duration);            \
			nosound();                  \
		}
#else
	#define SOUND(freq, duration)   \
		{                             \
			DosBeep(freq, duration);    \
		}
#endif

#ifdef __MSDOS__
	#define DELAY(milliseconds) \
	{                           \
		delay(milliseconds);      \
	}
#else
	#define DELAY(milliseconds) \
	{                           \
		DosSleep(milliseconds);   \
	}
#endif

#define CALC_SCRN_BUFFER(x1, y1, x2, y2) ((y2 - y1 + 1) * (x2 - x1 + 1) * 2)

#if (defined(__OS2__) || defined(__EMX__)) && defined(__MT__)
	#define ENTERCRITSEC()                                   \
			{                                                    \
				DosRequestMutexSem(ti2_hmtx, SEM_INDEFINITE_WAIT); \
			}
	#define EXITCRITSEC()                 \
			{                                 \
				DosReleaseMutexSem(ti2_hmtx);   \
			}
#else
	#define ENTERCRITSEC()    \
			{                     \
			}
	#define EXITCRITSEC()     \
			{                     \
			}
#endif

/* Function prototypes for improved RTL2 functions */

void    strpad      (char *s, ushort max_len, char padch);
void    strcatch    (char *s, char ch);
void    strtrim     (char *s, ushort num);
ulong   atoul       (char *str_ptr);
uint    atoui       (char *str_ptr);
ushort  atous       (char *str_ptr);
void    time_to_str (ulong t, char *str);
ulong   str_to_time (char *str);
void    date_to_str (ulong d, char *str);
ulong   str_to_date (char *str);
