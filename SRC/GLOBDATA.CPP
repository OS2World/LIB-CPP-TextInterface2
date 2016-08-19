/* Filename : GLOBDATA.CPP                                                  *
 * Description : Global Data used by TI/2                                   *
 * Target : GLOBDATA.OBJ -> TI2WIN.LIB or TI2.DLL                           *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 06/08/95                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

#if defined(__OS2__) || defined(__EMX__)
	#define INCL_MOU
	#define INCL_NOPMAPI
	#define INCL_DOSPROCESS
	#define INCL_DOSDATETIME
	#define INCL_DOSSEMAPHORES
	#include <os2.h>
#endif
#include "ti2.h"
#include "internal.h"

/* Window */
win_t FAR * FAR * 						ti2_windows;         			// Array of windows
uchar 												ti2_num_windows = 0;     	// Total number of windows in the array
win_c 								_export desktop;                 	// Desktop window.  Has no frame and covers the whole screen.
uchar 												ti2_destroy_all = 0;     	// True if win_deinit function is running
win_default_colors_t 					ti2_win_default_colors;  	// Default colors for windows
#if defined(__OS2__) || defined(__EMX__)
HMTX 													ti2_hmtx;									// Mutually exclusive semaphore
#endif
/* Form */
form_default_colors_t 				ti2_form_default_colors;  // Default colors for forms
/* Mouse */
uchar 								_export mouse_exist  = 0;  				// True if mouse exists and is initialized
uchar 								_export mouse_left   = 0;  				// True if left button pressed
uchar 								_export mouse_center = 0;  				// True if center button pressed
uchar 								_export mouse_right  = 0;   			// True if right button pressed
uchar 								_export mouse_row    = 0;   			// Current row of mouse pointer
uchar 								_export mouse_col    = 0;   			// Current columnb of mouse pointer
/* Menu */
menu_default_colors_t 				ti2_menu_default_colors;  // Default colors for menu

