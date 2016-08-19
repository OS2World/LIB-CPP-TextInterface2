/* Filename : INTERNAL.CPP                                                  *
 * Description : System Dependent Internal routines for TI/2                *
 * Target : INTERNAL.OBJ -> TI2WIN.LIB or TI2.DLL                           *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 06/10/95                                                 *
 * Last modified : 06/10/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

/******************************
 * Predefined header includes *
 ******************************/

#if defined(__OS2__) || defined(__EMX__)
	#define INCL_VIO
	#define INCL_NOPMAPI
	#define INCL_DOSSEMAPHORES
	#include <os2.h>
#endif
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ti2.h"
#include "internal.h"

#ifdef __TURBOC__
#include <mem.h>
#endif

#if defined(__EMX__) || defined(__GO32__)
#include <memory.h>
#endif

/* Improved version of RTL2 */

void strpad(char *s, ushort max_len, char padch)
		{
			ushort len;
			char *temp_str;

			/* Get current length of string */
			len = strlen(s);

			/* Pad string */
			if (len < max_len) {
				/* Allocate string that holds padding character */
				if ((temp_str = (char *) malloc(sizeof(char) * (max_len - len + 1))) == NULL) {
					/* Failure to allocate memory for string */
					printf("strpad() error : Unable to allocate memory for temp string.\n");
					exit(0);
				}
				/* Fill string with pad character */
				memset((char *) temp_str, padch, max_len - len);
				/* Terminate string */
				temp_str[max_len - len] = '\0';
				/* Append string */
				strcat(s, temp_str);
				/* Free string that holds padding character */
				FREE(temp_str);
			}
				else
			/* Trim string */
			{
				s[max_len] = '\0';
			}
		}

void strcatch(char *s, char ch)
		{
			char temp_str[2];

			/* Convert character to string */
			temp_str[0] = ch;
			temp_str[1] = '\0';

			/* Append temp string to string */
			strcat(s, temp_str);
		}

void strtrim(char *s, ushort num)
		{
			ushort len;     // Length of string

			/* Store length of string to trim */
			len = strlen(s);

			/* if len is greater than num than trim string */
			if (len > num) s[num] = '\0';
		}

ulong atoul(char *str_ptr)
		{
			char c;        // Character from string
			ulong result;   // Result of number

			/* Initialize variables */
			result = 0;

			/* Skip any whitespace characters */
			while (isspace((c = *str_ptr++)));

			/* Skip over any positive or negative signs */
			if (c == '+' || c == '-') {
				c = *str_ptr++;
			}

			/* accumulate digits, ignore overflow */
			while (c >= '0' && c <= '9') {
				result = result * 10 + c - '0';
				c = *str_ptr++;
			}

			/* Return result */
			return (result);
		}

uint atoui(char *str_ptr)
		{
			return (uint) atoul(str_ptr);
		}

ushort atous(char *str_ptr)
		{
			return (ushort) atoul(str_ptr);
		}

void time_to_str(ulong t, char *str)
		{
			ushort hour = 0,
						 minute = 0,
						 second = 0;
			char hour_str[3],
					 minute_str[3],
					 second_str[3],
					 convert_str[3];
			ulong temp_t;

			/* Parse t into hour, minute, second */
			temp_t = t;
			while(temp_t >= 10000) {
				hour++;
				temp_t -= 10000;
			}
			while (temp_t >= 100) {
				minute++;
				temp_t -= 100;
			}
			while (temp_t) {
				second++;
				temp_t -= 1;
			}

			/* Make sure time passed as a parameter is valid */
			if (hour > 23) hour = 23;
			if (minute > 59) minute = 59;
			if (second > 59) second = 59;

			/* Make hhmmss_str our of numbers */
			strcpy(str, "");
			ITOA(hour, hour_str, 10);
			if (strlen(hour_str) < 2) {
				sprintf(convert_str, "0%s", hour_str);
				strcpy(hour_str, convert_str);
			}
			ITOA(minute, minute_str, 10);
			if (strlen(minute_str) < 2) {
				sprintf(convert_str, "0%s", minute_str);
				strcpy(minute_str, convert_str);
			}
			ITOA(second, second_str, 10);
			if (strlen(second_str) < 2) {
				sprintf(convert_str, "0%s", second_str);
				strcpy(second_str, convert_str);
			}
			sprintf(str, "%s:%s:%s", hour_str, minute_str, second_str);
		}

ulong str_to_time(char *str)
		{
			uchar hour = 0,
						minute = 0,
						second = 0;
			char hour_str[3],
					 minute_str[3],
					 second_str[3];
			ulong temp_t;

			/* Parse string into hour, minute, second strings */
			sprintf(hour_str, "%c%c", str[0], str[1]);
			sprintf(minute_str, "%c%c", str[3], str[4]);
			sprintf(second_str, "%c%c", str[6], str[7]);

			/* Convert strings to numbers */
			hour = (uchar) atous(hour_str);
			minute = (uchar) atous(minute_str);
			second = (uchar) atous(second_str);

			/* Make sure time passed as a parameter is valid */
			if (hour > 23) hour = 23;
			if (minute > 59) minute = 59;
			if (second > 59) second = 59;

			/* Make HHMMSS number out of hour, minute, and second numbers */
			temp_t = 0;
			temp_t += ((ulong) hour * 10000);
			temp_t += ((ulong) minute * 100);
			temp_t += ((ulong) second * 1);

			return(temp_t);
		}

void date_to_str(ulong d, char *str)
		{
			ushort year = 0;
			uchar  month = 0;
			uchar  day = 0;
			char  year_str[5],
						month_str[3],
						day_str[3],
						convert_str[5];
			ulong  temp_d;

			/* Parse t into hour, minute, second */
			temp_d = d;
			while(temp_d >= 10000) {
				year++;
				temp_d -= 10000;
			}
			while (temp_d >= 100) {
				month++;
				temp_d -= 100;
			}
			while (temp_d) {
				day++;
				temp_d -= 1;
			}

			/* Make sure date passed as a parameter is valid */
			if (month > 12) month = 12;
			if (day > 31) day = 31;

			/* Make mmddyyyy_str out of numbers */
			strcpy(str, "");
			ITOA(year, year_str, 10);
			ITOA(month, month_str, 10);
			if (strlen(month_str) < 2) {
				sprintf(convert_str, "0%s", month_str);
				strcpy(month_str, convert_str);
			}
			ITOA(day, day_str, 10);
			if (strlen(day_str) < 2) {
				sprintf(convert_str, "0%s", day_str);
				strcpy(day_str, convert_str);
			}
			sprintf(str, "%s/%s/%s", month_str, day_str, year_str);
		}

ulong str_to_date(char *str)
		{
			ushort year = 0;
			uchar  month = 0,
						 day = 0;
			char year_str[5],
					 month_str[3],
					 day_str[3];
			ulong temp_d;

			/* Parse string into hour, minute, second strings */
			sprintf(month_str, "%c%c", str[0], str[1]);
			sprintf(day_str, "%c%c", str[3], str[4]);
			sprintf(year_str, "%c%c%c%c", str[6], str[7], str[8], str[9]);

			/* Convert strings to numbers */
			year = atous(year_str);
			month = (uchar) atous(month_str);
			day = (uchar) atous(day_str);

			/* Make sure time passed as a parameter is valid */
			if (month > 12) month = 12;
			if (day > 31) day = 31;

			/* Make YYYMMDD number out of year, month, and day numbers */
			temp_d = 0;
			temp_d += ((ulong) year * 10000);
			temp_d += ((ulong) month * 100);
			temp_d += ((ulong) day * 1);

			return(temp_d);
		}

/* Borland/Turbo C++ for DOS & OS/2 */
#if defined(__TURBOC__) || defined(__DJGPP__)

int screenheight(void)
		{
			struct text_info scrn_info;   // Holds screen information

			/* Get screen information */
			gettextinfo(&scrn_info);
			return(scrn_info.screenheight);
		}

int screenwidth(void)
		{
			struct text_info scrn_info;   // Holds screen information

			/* Get screen information */
			gettextinfo(&scrn_info);
			return(scrn_info.screenwidth);
		}

#endif

/* EMX for OS/2 */
#ifdef __EMX__

typedef USHORT APIRET16;

#define DEFAULT_ROWS    25
#define DEFAULT_COLS    80
#define DEFAULT_ATTRIB  7

typedef struct {                      /* Abbreviated Mode Data */
								 ushort  length;
								 uchar   type;
								 uchar   color;
								 ushort  col;
								 ushort  row;
							 } AMD;

typedef struct {
								 uchar currmode;
								 uchar screenheight;
								 uchar screenwidth;
							 } VIDEOREC;

#define iTYPE   3   /* Color, B&W, Monochrome   */
#define iROWS   7   /* 14, 21, 25, 28, 43, 50, 60   */
#define iCOLS   2   /* 40, 80           */

VIDEOREC   video = {0};
int        LastMode;          /* Most recent TEXT mode        */
int        C4350Mode;         /* The mode that C4350 maps to  */
int        directvideo = 1;
static AMD original;          /* Startup video mode info      */

static char ModeTable[iTYPE][iROWS][iCOLS] = {
																							 {{C40X14, C80X14},
																								{C40X21, C80X21},
																								{C40   , C80   },
																								{C40X28, C80X28},
																								{C40X43, C80X43},
																								{C40X50, C80X50},
																								{C40X60, C80X60}},

																							 {{BW40X14, BW80X14},
																								{BW40X21, BW80X21},
																								{BW40   , BW80   },
																								{BW40X28, BW80X28},
																								{BW40X43, BW80X43},
																								{BW40X50, BW80X50},
																								{BW40X60, BW80X60}},

																							 {{MONO14, MONO14},
																								{MONO21, MONO21},
																								{MONO  , MONO  },
																								{MONO28, MONO28},
																								{MONO43, MONO43},
																								{MONO50, MONO50},
																								{MONO50, MONO60}}
																						 };

static char TypeLookUp[iTYPE]  = {1, 5, 0};
static char ColorLookUp[iTYPE] = {4, 2, 0};
static char RowLookUp[iROWS]   = {14, 21, 25, 28, 43, 50, 60};

static int emx_kbdch = -1;  // Holds character read from EMX keyboard handler

int emx_getch(void)
		{
			int ch;     // Holds temporary character

			/* See if there is already a character read */
			if (emx_kbdch >= 0) {
				ch = emx_kbdch;
				emx_kbdch = -1;
				return(ch);
			}
				else
			{
				/* Return the next character from the keyboard */
				return(_read_kbd(0, 1, 0));
			}
		}

int emx_kbhit(void)
		{
			/* See if there is already a character read */
			if (emx_kbdch >= 0) return(1);
				else
			{
				/* Try to read in a character.  No wait read */
				emx_kbdch = _read_kbd(0, 0, 0);
				if (emx_kbdch >= 0) return(1);
					else return(0);
			}
		}

static int WhichMode(UCHAR type, unsigned rows, unsigned cols)
		{
			int T, R, C;

			T = ((type & VGMT_DISABLEBURST) == 0) ? 0 : ((type & VGMT_OTHER) ? 1 : 2); /* default = MONO */

			/* Look up the number of rows in the table. */
			for (R = 0; R < sizeof(RowLookUp) && RowLookUp[R] != (char)rows; R++)
				;
			if (R == sizeof(RowLookUp)) return(-1);  /* not found */

			/* Columns must be 40 or 80. */
			if (cols == 40) C = 0;
				else
			if (cols == 80) C = 1;
				else return(-1);

			return(ModeTable[T][R][C]);
		}

void init_video(void)
		{
			AMD         newvideo;
			VIOMODEINFO vinfo;

			/* Get current screen mode. */
			vinfo.cb = sizeof(vinfo);
			VioGetMode(&vinfo, (HVIO) 0);
			/* Store original screen settings */
			original = *(AMD *)&vinfo;
			original.length = sizeof(AMD);

			/* Check to see if we are in graphics mode.  If so try to go to text. */
			newvideo = original;

			if (original.type & VGMT_GRAPHICS) {
				/* Make it text mode */
				newvideo.type &= ~VGMT_GRAPHICS;

				if(VioSetMode((PVIOMODEINFO) &newvideo, (HVIO) 0) != 0) {
					/* Setting video mode failed. Try a basic 80X25 text mode. */
					newvideo.row = DEFAULT_ROWS;
					newvideo.col = DEFAULT_COLS;
					if(VioSetMode((PVIOMODEINFO) &newvideo, (HVIO) 0) != 0) return;
				}

				/* Assign default values since we changed from graphics to text. */
				video.screenwidth      = newvideo.col;
				video.screenheight     = newvideo.row;
			}
				else
			{
				/* Already in text Mode */
				USHORT Row, Col;

				video.screenwidth      = original.col;
				video.screenheight     = original.row;
			}

			/* Figure out which mode C4350 (EGA 43 or VGA 50 line) should map to. */
			if (vinfo.vres <= 350) C4350Mode = C80X43; // EGA = 350 (8 * 43)
				else C4350Mode = C80X50;                 // VGA = 400 (8 * 50)

			LastMode = WhichMode(newvideo.type, newvideo.row, newvideo.col);
			if (LastMode == C4350Mode) LastMode = C4350;  // 43 or 50 row mode?
				else
			if (LastMode == -1) LastMode = ORIGMODE;     // unknown mode?

			video.currmode = LastMode;
		}

void crt_init(int newvideomode)
		{
			AMD newvideo;
			int i, j, k;
			int actual_mode;

			/* Are we being asked to restore the LAST text mode set? */
			if (newvideomode == LASTMODE) newvideomode = LastMode;

			/* If we are asked for C4350 use the correct one for their card */
			actual_mode = ((newvideomode == C4350) ? C4350Mode : newvideomode);

			/* Check the mode table to see if the mode is available */
			for (i=0; i<iTYPE; i++) {
				for (j=0; j<iROWS; j++) {
					for (k=0; k<iCOLS; k++) {
						if (((ModeTable[i][j][k]) == actual_mode) || (newvideomode == ORIGMODE)) {
							/* Found mode.  Construct AMD and set new mode. */
							if (newvideomode == ORIGMODE) newvideo = original;
								else
							{
								newvideo.length = sizeof(AMD);
								newvideo.type   = TypeLookUp[i];
								newvideo.color  = ColorLookUp[i];
								newvideo.row    = RowLookUp[j];
								newvideo.col    = (newvideo.type) ? (k ? 80 : 40) : 80;
							}

							if(VioSetMode((PVIOMODEINFO)&newvideo, (HVIO) 0) == 0) {
								video.currmode = newvideomode;
								video.screenwidth = newvideo.col;
								video.screenheight = newvideo.row;
								LastMode = newvideomode;
							}
							return;
						}
					}
				}
			}
			return;
		}

int screenheight(void)
		{
			return(video.screenheight);
		}

int screenwidth(void)
		{
			return(video.screenwidth);
		}

void textmode(int newmode)
		{
			uchar i, j;                   // General counters
			char scrnpnt[2] = {' ', 0};   // Black blank space on screen

			crt_init(newmode);

			/* Clear entire screen */
			for (i = 1; i <= screenheight(); i++) {
				for (j = 1; j <= screenwidth(); j++) puttext(j, i, j, i, scrnpnt);
			}
		}

void setcursortype(int cur_t)
		{
			if (cur_t) {}
		}

int puttext(int left, int top, int right, int bottom, void *source)
		{
			int i;    // General counter

			for (i = top; i <= bottom; i++) {
				VioWrtCellStr((PCH) source, (USHORT) (2 * (right - left + 1)), (USHORT) i - 1, (USHORT) left - 1, 0);
				source += (2 * (right - left + 1));
			}

			/* Return successful */
			return(1);
		}

int gettext(int left, int top, int right, int bottom, void *destin)
		{
			int i;    // General counter
			USHORT len;

			for (i = top; i <= bottom; i++) {
				len = (2 * (right - left + 1));
				VioReadCellStr((PCH) destin, &len, (USHORT) i - 1, (USHORT) left - 1, 0);
				destin += len;
			}

			/* Return successful */
			return(1);
		}

void gotoxy(int x, int y)
		{
			VioSetCurPos((USHORT) y - 1, (USHORT) x - 1, 0);
		}

#endif
