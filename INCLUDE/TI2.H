/* Filename : TI2.H                                                           *
 * Description : Text Interface/2 Precompiled Header                          *
 * Target : TI2.LIB                                                           *
 * Portability : OS/2, DOS                                                    *
 * Creation date : 10/25/94                                                   *
 * Last modified : 04/29/95                                                   */

/* Display error message if they try to used this module in a non-C++ program */

#ifndef __cplusplus
#error TI/2 is a C++ module.  Please use C++ compilation.
#endif

#ifndef _TI2_H_
#define _TI2_H_

/****************************
 * System dependant defines *
 ****************************/

/* MS-DOS Compilers */
#ifdef __MSDOS__
	/* Define functions and pointers for the correct memory model */
#if defined (__HUGE__) || defined(__LARGE__) || defined(__COMPACT__)
#define FAR 				far
#define FARMALLOC		farmalloc
#define FARCALLOC		farcalloc
#define FARREALLOC 	farrealloc

#define FARFREE(ptr)							\
	{																\
		if (ptr != NULL) {						\
			farfree(ptr);               \
			ptr = NULL;									\
		}															\
	}

#define FARMEMMOVE	_fmemmove
#define FARMEMSET		_fmemset
#define FARMEMCPY		_fmemcpy

#define FREE(ptr)									\
	{																\
		if (ptr != NULL) {						\
			free(ptr);               		\
			ptr = NULL;									\
		}															\
	}

#else
#define FAR
#define FARMALLOC		malloc
#define FARCALLOC		calloc
#define FARREALLOC	realloc

#define FARFREE(ptr)							\
	{																\
		if (ptr != NULL) {						\
			free(ptr);               		\
			ptr = NULL;									\
		}															\
	}

#define FARMEMMOVE	memmove
#define FARMEMSET		memset
#define FARMEMCPY		memcpy

#define FREE(ptr)									\
	{																\
		if (ptr != NULL) {						\
			free(ptr);               		\
			ptr = NULL;									\
		}															\
	}

#endif

#endif

/* OS/2 Compilers */
#ifdef __OS2__
	/* Define functions and pointers for unsegmented memory */
	#define FAR
	#define FARMALLOC		malloc
	#define FARCALLOC		calloc
	#define FARREALLOC	realloc

	#define FARFREE(ptr)							\
		{																\
			if (ptr != NULL) {						\
				free(ptr);               		\
				ptr = NULL;									\
			}															\
		}

	#define FARMEMMOVE	memmove
	#define FARMEMSET		memset
	#define FARMEMCPY		memcpy

	#define FREE(ptr)									\
		{																\
			if (ptr != NULL) {						\
				free(ptr);               		\
				ptr = NULL;									\
			}															\
		}

#endif

/******************
 * Window Defines *
 ******************/
																		 /* Miscellaneous attributes for a window */
#define WIN_ACTIVE       (1<<0)      // Window is active
#define WIN_HIDDEN       (1<<1)      // Window is hidden
#define WIN_TITLE_LEFT   (1<<2)      // Left justify title
#define WIN_TITLE_CENTER (1<<3)      // Center title
#define WIN_TITLE_RIGHT  (1<<4)      // Right justify title
#define WIN_TITLE_BOTTOM (1<<5)			 // Place title on bottom of window instead of top
#define WIN_TEXT_NO_WRAP (1<<6)      // Don't wrap text output from win_printf to next line.
#define WIN_VSCROLL_BAR  (1<<7)      // Has a vertical scroll bar
#define WIN_HSCROLL_BAR  (1<<8)      // Has a horizontal scroll bar
#define WIN_HORIZ_CNTR   (1<<9)			 // Makes window the same height and width as specified, but is centered on screen
#define WIN_VERT_CNTR    (1<<10)		 // Makes window the same height and width as specified, but is centered on screen
#define WIN_SCROLL   		 (1<<11)		 // Scrolling on.  Default is off
#define WIN_CURSOR   		 (1<<12)		 // Cursor on. Default off
#define WIN_SEETHRU			 (1<<13)		 // Window is see thru.
#define WIN_EXPLODE			 (1<<14)		 // Window explodes when opened
#define WIN_IMPLODE			 (1<<15)		 // Window implodes when closed

																		 /* Frame types */
#define WIN_FRAME_NONE    	 0       // Window has no frame
#define WIN_FRAME_DOUBLEBOX  1
#define WIN_FRAME_SINGLEBOX  2
#define WIN_FRAME_MIXEDBOX1	 3
#define WIN_FRAME_MIXEDBOX2  4
#define WIN_FRAME_HATCHBOX1	 5
#define WIN_FRAME_DOTTEDLINE 6
#define WIN_FRAME_HATCHBOX2	 7
#define WIN_FRAME_HATCHBOX3	 8
#define WIN_FRAME_SOLIDBOX1	 9
#define WIN_FRAME_SOLIDBOX2	 10
#define WIN_FRAME_BLANKBOX	 11
																		 /* Old window frame defines kept for compatibility */
#define WIN_FRAME_SS      	 2       // Single line on both sides
#define WIN_FRAME_DD         1       // Double line on both sides
#define WIN_FRAME_DS      	 4       // Double line on side and single on top
#define WIN_FRAME_SD      	 3       // Single line on side and double on top

																		 /* Shadow types */
#define WIN_SHADOW_NONE   0          // No shadow
#define WIN_SHADOW_RIGHT  1          // Shadow on bottom and right side
#define WIN_SHADOW_LEFT   2          // Shadow on top and left side

#define TOP													1
#define BOTTOM											2
#define LEFT												3
#define RIGHT												4

/********************
 * General Typedefs *
 ********************/

typedef unsigned char uchar;
typedef unsigned short int ushort;
typedef unsigned int uint;
typedef unsigned long int ulong;
typedef uchar * puchar;

#ifndef _SWDEFS_H
	typedef struct {                /* Fidonet Address */
									 ushort zone;
									 ushort net;
									 ushort node;
									 ushort point;
								 } faddr_t;
#endif

/***********************
 * Function Prototypes *
 ***********************/

#ifdef _EXTPDL_

void error_msg 	 								(uchar *func_name, uchar *error_msg);
void fatal_error_msg						(uchar *func_name, uchar *error_msg);

#else

#define error_msg(func_name, error_msg)	\
		{																		\
			if(func_name || error_msg) {}			\
		}

#define fatal_error_msg(func_name, error_msg)	\
		{																		\
			if(func_name || error_msg) {}			\
		}

#endif


/*******************
 * Debug Functions *
 *******************/

void  ti2_debug_init		(void);
void  ti2_debug_deinit	(void);
void  ti2_debug_printf	(char *fmt, ...);

/**************************
 * Debug Global Variables *
 **************************/

extern FILE *ti2_debug_file;				// Stream for debug information file

/********************
 * Window Functions *
 ********************/

void 	win_deinit				(void);
uchar win_init					(void);
void  win_redraw_all		(void);
void 	win_text_mode			(int newmode);
uchar win_xyxy_in_xyxy	(uchar x1, uchar y1, uchar x2, uchar y2, uchar x3, uchar y3, uchar x4, uchar y4);

/***************************
 * Window Class Definition *
 ***************************/

typedef struct {
								 uchar 						 handle;				// Handle for this window.  The class will also have the same value in handle
								 uchar 						 topxpos,       // X & Y positions of window on screen including
																	 topypos,       // border etc. in it
																	 botxpos,
																	 botypos;
								 uchar 						 frametopxpos,  // Position of frame corners
																	 frametopypos,
																	 framebotxpos,
																	 framebotypos;
								 uchar 						 texttopxpos,   // Position of text part of window
																	 texttopypos,
																	 textbotxpos,
																	 textbotypos;
								 uchar FAR * FAR * text;			 		// Buffer that holds window image. A copy of everything is stored here
								 uchar 						 shadow;        // Shadow type: 0=none, 1=bottem&right, 2=top&left
								 ushort 					 attr;          // Miscellaneous attributes for window.  See below for more information
								 uchar 						 frame;         // Type of frame.  See WIN_FRAME_ below for more information
							 } win_t;

class win_c {                                	/* Window Class */
							private:
								/*************
								 * Variables *
								 *************/
								uchar   handle;										 // Handle for this window
								int     pos;											 // Position in the window array
								uchar   f_fcolor,               	 // Current foreground and background color of frame
												f_bcolor;
								uchar   t_fcolor,               	 // Current foreground and background color of text
												t_bcolor;
								uchar * title,              			 // Title
												title_fcolor,           	 // Current foreground and background color of title
												title_bcolor;
								uchar   scroll_fcolor,         		 // Foreground and background colors for scroll bars
												scroll_bcolor;
								uchar   input_norm_fcolor,     		 // Colors used by all input functions
												input_norm_bcolor,
												input_high_fcolor,
												input_high_bcolor,
												input_blank_fcolor,
												input_blank_bcolor,
												input_text_fcolor,
												input_text_bcolor;
								uchar   curr_fcolor,
												curr_bcolor;							 // Current foreground and background colors
								uchar   curr_x,                		 // Where X position is in window
												curr_y;                		 // Where Y position is in window
								uchar   scroll_mode;							 // True scrolling is on, False scrolling off
								uchar   cursor_mode;							 // True cursor is on, False cursor is off
								ushort  vscroll_cur,           		 // Numbers that determine scroll bar display
												vscroll_max;           		 // Number that determines the maximum value for vscroll_max
								ushort  hscroll_cur,
												hscroll_max;
								/*************
								 * Functions *
								 *************/
								uchar  completely_visible						(void);
								void 	 display_buffer_ch            (uchar *scrn_buffer, uchar x, uchar y, uchar ch_type, uchar visibility);
								void   display_ch										(uchar x, uchar y, uchar ch_type, uchar visibility);
								void   display_part     						(uchar part);
								void   focus                   		 	(void);
								uchar  get_handle										(void);
								void 	 get_hidden_ch								(uchar x, uchar y, uchar ch_type, uchar *buffer);
								void   redraw												(void);
								void   unhide_buffer_ch             (uchar *scrn_buffer, uchar x, uchar y, uchar ch_type);
								void 	 unhide_ch                    (uchar x, uchar y, uchar ch_type);
								uchar  visible_ch										(uchar x, uchar y, uchar ch_type);
								void   get_pos											(void);
							public:
								/*************
								 * Functions *
								 *************/
								void   center                       	(ushort type);
								void 	 clr_eoc												(void);
								void   clr_eol                  			(void);
								void   clr_region                   	(uchar x1, uchar y1, uchar x2, uchar y2);
								void   clr_scr                  			(void);
								void   cntr_printf						 				(uchar y, char *fmt, ...);
								void   col_cntr_printf              	(uchar x, uchar y, char *fmt, ...);
								uchar  create     										(uchar new_topx, uchar new_topy, uchar new_botx, uchar new_boty, uchar new_f_fcolor,
																											 uchar new_f_bcolor, uchar new_t_fcolor, uchar new_t_bcolor, uchar *new_title,
																											 uchar new_title_fcolor, uchar new_title_bcolor, uchar new_frame, uchar new_shadow,
																											 ushort new_attr);
								void   cursor_off	 										(void);
								void   cursor_on											(void);
								void   destroy                 				(void);
								void   explode												(uchar x1, uchar y1, uchar x2, uchar y2);
								void   fill                    				(uchar ch);
								void   fill_col												(uchar col, uchar ch);
								void   fill_row												(uchar row, uchar ch);
								void   frame_change            				(uchar new_frame_type);
								void   frame_textbackground         	(uchar new_f_bcolor);
								void   frame_textcolor              	(uchar new_f_fcolor);
								uchar  frame_x_relative               (uchar abs_x);
								uchar  frame_y_relative               (uchar abs_y);
								ushort get_attr										 		(void);
								void   goto_xy                  			(uchar x, uchar y);
								void   hide                    				(void);
								void   hscroll_create          				(uchar scroll_fcolor, uchar scroll_bcolor, ushort hscroll_cur, ushort hscroll_max);
								void   hscroll_destroy         				(void);
								uchar  hscroll_left            		 		(ushort num);
								ushort hscroll_mouse_move							(void);
								uchar  hscroll_right           				(ushort num);
								void   hscroll_update          				(void);
								void   hscroll_update_cur			 				(ushort new_hscroll_cur);
								void   hscroll_update_cur_and_max			(ushort new_hscroll_cur, ushort new_hscroll_max);
								void   hscroll_update_max			 				(ushort new_hscroll_max);
								void   implode												(void);
								void   make_normal										(void);
								void   make_seethru										(void);
								void   move                    				(uchar newx, uchar newy);
								void   nd_printf                  		(char *fmt, ...);
								void   nd_putch                   		(char ch);
								void   nd_puts												(char *s);
								void   popup													(void);
								void   redisplay											(void);
								void	 redisplay_text									(void);
								void   resize                       	(uchar side, uchar width);
								void   scroll_down             				(void);
								void   scroll_left             				(void);
								void   scroll_off											(void);
								void   scroll_on											(void);
								void   scroll_part_down				 				(uchar topx, uchar topy, uchar botx, uchar boty);
								void   scroll_part_left				 				(uchar topx, uchar topy, uchar botx, uchar boty);
								void   scroll_part_right			 				(uchar topx, uchar topy, uchar botx, uchar boty);
								void   scroll_part_up					 				(uchar topx, uchar topy, uchar botx, uchar boty);
								void   scroll_right            				(void);
								void   scroll_up               				(void);
								void   set_attr												(ushort new_attr);
								void   set_pos												(uchar new_pos);
								void   slide                        	(uchar newx, uchar newy);
								void   read_attr                    	(uchar *buffer, uchar x, uchar y, ushort len);
								void   read_char                    	(uchar *buffer, uchar x, uchar y, ushort len);
								void   read_char_and_attr           	(uchar *buffer, uchar x, uchar y, ushort len);
								void   rj_printf                	  	(uchar x, uchar y, char *fmt, ...);
								void   text_background          			(uchar new_t_bcolor);
								void   text_color               			(uchar new_t_fcolor);
								uchar  text_height						 				(void);
								uchar  text_width						 			 		(void);
								uchar  text_x_relative                (uchar abs_x);
								uchar  text_y_relative                (uchar abs_y);
								void   title_change										(uchar *new_title);
								void   title_textbackground         	(uchar new_title_bcolor);
								void   title_textcolor              	(uchar new_title_fcolor);
								void   unhide                  				(void);
								void   vscroll_create          				(uchar new_scroll_fcolor, uchar new_scroll_bcolor, ushort new_vscroll_cur, ushort new_vscroll_max);
								void   vscroll_destroy         				(void);
								uchar  vscroll_down            				(ushort num);
								ushort vscroll_mouse_move							(void);
								uchar  vscroll_up              				(ushort num);
								void   vscroll_update          				(void);
								void   vscroll_update_cur			 				(ushort new_vscroll_cur);
								void   vscroll_update_cur_and_max			(ushort new_vscroll_cur, ushort new_vscroll_max);
								void   vscroll_update_max			 				(ushort new_vscroll_max);
								uchar  where_win_x										(void);
								uchar  where_win_y										(void);
								uchar  where_x								 				(void);
								uchar  where_y								 				(void);
								void   width_printf										(uchar width, char *fmt, ...);
								void   wprintf                  			(char *fmt, ...);
								void   wputch                   			(char ch);
								void   wputctrlch											(char ch);
								void   wputs													(char *s);
								uchar  write_text_to_file	          	(uchar *filename, uchar newline);



								void   input_4d_addr		 					 	 	(uchar x, uchar y, faddr_t *addr);
								void   input_byte				 						 	(uchar x, uchar y, uchar *num, uchar min, uchar max);
								void   input_frmt_string		 				 	(uchar x, uchar y, uchar *str, uchar *ctrl_str);
								void   input_hhmmss									 	(uchar x, uchar y, ulong *t);
								void   input_init										  (uchar new_input_norm_fcolor, uchar new_input_norm_bcolor,
																											 uchar new_input_high_fcolor, uchar new_input_high_bcolor,
																											 uchar new_input_blank_fcolor, uchar new_input_blank_bcolor,
																											 uchar new_input_text_fcolor, uchar new_input_text_bcolor);
								void   input_string  								 	(uchar x, uchar y, char *s, uchar max_len);
								void   input_toggle_uchar_bit_yes_no 	(uchar x, uchar y, uchar *toggle, uchar bit);
								void   input_toggle_uchar_yes_no		 	(uchar x, uchar y, uchar *toggle);
								void   input_toggle_ulong_bit_yes_no 	(uchar x, uchar y, ulong *toggle, ulong bit);
								void   input_toggle_ushort_bit_yes_no (uchar x, uchar y, ushort *toggle, ushort bit);
								void   input_ulong	 		 			  	 		(uchar x, uchar y, ulong *num, ulong min, ulong max);
								void 	 input_upcase_string 					 	(uchar x, uchar y, char *s, uchar max_len);
								void   input_ushort		 		 			  	 	(uchar x, uchar y, ushort *num, ushort min, ushort max);
						};

/***************************
 * Window Global Variables *
 ***************************/

extern win_t FAR * FAR * ti2_windows;  		// Array of windows
extern uchar 						 ti2_num_windows; // Total number of windows in the array
extern uchar             ti2_destroy_all; // True if win_deinit function is running
extern win_c 						 desktop;     		// Desktop window.  Has no frame and covers the whole screen.

/*****************
 * Menu Typedefs *
 *****************/

typedef struct {                                /* Menu command */
								 ushort  id;                    // Unique ID number for this menu command
								 uchar * title;               	// Title of menu cmd
								 uchar   hotkey;                // Hotkey of this command
								 uchar   active;                // Active menucommand
								 uchar   type;                  // 0=menucmd, 1=sperator, 2=submenu
								 ushort  submenu;               // Number of submenu
							 } menucmd_t;

typedef struct {                                		/* Submenu */
								 ushort 			parent_id;						// Unique ID number of submenu which this submenu is under
								 ushort 			id;                   // Unique ID number of submenu
								 uchar  			title_pos;            // Position on menu bar to put title
								 uchar  			active;               // Active submenu
								 win_c  			win;                  // Window to draw submenu in
								 uchar  			menucmd_cur;          // Current menu command
								 uchar  		  menucmd_max;          // Maximum number of menucmds in this submenu
								 ushort FAR * menucmd;           		// Array of menucmd ids that are in this submenu
								 uchar  		  longest_menucmd_name; // Length of longest menu command name
								 uchar *		  title;               	// Title of submenu
								 uchar  		  hotkey;
							 } submenu_t;

typedef struct {                                /* Menu command index */
								 ushort id;                     // Unique ID of menucmd
								 ushort pos;                    // Position in menu->menucmds array
							 } menucmds_idx_t;

typedef struct {                                /* Submenu index */
								 ushort id;                     // Unique ID of submenu
								 ushort pos;                    // Position in menu->submenus array
							 } submenus_idx_t;

/*************************
 * Menu Class Definition *
 *************************/

class menu_c { 																					/* Menu Class */
							 private:
								/*************
								 * Variables *
								 *************/
								 uchar 							  line;             // Line menu bar is on
								 win_c 							  title_bar;				// Window for titlebar
								 uchar 							  submenu_cur;      // Current submenu
								 uchar 							  submenu_max;      // Maximum number of sub menus
								 ushort FAR *				  submenu;          // Array of submenu->id which are directly off of the menu bar
								 uchar 						 	  pulldown;         // Wether menu is pulled down or just scrolling titles
								 uchar 							  reloading;				// True if reloading menu after a menu command has run
								 ushort 						  result;						// Menu command id
								 uchar 							  hotkey_fcolor;    // Color of hotkey (character immediately after ~ in title)
								 uchar 							  norm_fcolor;      // Normal foreground and background color
								 uchar 							  norm_bcolor;
								 uchar 							  high_fcolor;      // Highlighted foreground and background color
								 uchar 							  high_bcolor;
								 uchar 							  inactive_fcolor;  // Inactive foreground and background color
								 uchar 							  inactive_bcolor;
								 uchar 							  frame_fcolor;			// Frame color
								 uchar 							  frame_bcolor;
								 menucmd_t FAR *      menucmds;       	// Array of menu commands
								 ushort               menucmds_max;     // Total number of menucmds for this menu
								 menucmds_idx_t FAR * menucmds_idx;  		// Index of all the menucmds
								 ushort 				      menucmds_idx_max;	// Total number of menucmds indexes
								 submenu_t 			FAR * submenus;       	// Array of submenus
								 ushort 			        submenus_max;     // Total number of submenus for this menu
								 submenus_idx_t FAR * submenus_idx;  		// Index of all the submenus
								 ushort 				      submenus_idx_max; // Total number of submenus indexes
								/*************
								 * Functions *
								 *************/
								 void  bar_redraw             		(void);
								 void  next_submenu           		(uchar direction);
								 int   hotkey                 		(uchar key);
								 void  submenu_redraw_menucmds		(submenu_t FAR *curr_submenu);
								 void  next_menucmd           		(submenu_t FAR *curr_submenu, uchar direction);
								 uchar submenu_clicked						(void);
								 uchar submenu_or_menucmd_clicked	(submenu_t FAR *curr_submenu, uchar *num);
								 void  goto_submenu             	(uchar submenu_num);
								 void  goto_menucmd             	(submenu_t FAR *curr_submenu, uchar menucmd_num);
								 int   submenu_hotkey         		(submenu_t FAR *curr_submenu, uchar key);
								 uchar load_submenu           		(submenu_t FAR *curr_submenu, uchar x, uchar y, uchar leftright);
							 public:
								/*************
								 * Functions *
								 *************/
								 void   add_menucmd_func       	(ushort parent_submenu_id, ushort new_id, uchar *new_title,
																								 uchar new_hotkey);
								 void   add_menucmd_separator  	(ushort parent_submenu_id, ushort new_id);
								 void   add_menucmd_submenu			(ushort parent_submenu_id, ushort new_id, ushort new_submenu_id,
																								 uchar *new_title, uchar new_hotkey);
								 void   add_submenu							(ushort new_id, uchar new_title_pos, uchar *new_title, uchar new_hotkey);
								 void   create                 	(uchar new_line, uchar new_hotkey_fcolor,
																								 uchar new_norm_fcolor, uchar new_norm_bcolor,
																								 uchar new_high_fcolor, uchar new_high_bcolor,
																								 uchar new_inactive_fcolor, uchar new_inactive_bcolor,
																								 uchar new_frame_fcolor, uchar new_frame_bcolor);
								 void   destroy                	(void);
								 ushort load               			(void);
								 void   menucmd_active				 	(ushort id);
								 void   menucmd_inactive			 	(ushort id);
								 void   submenu_active				 	(ushort id);
								 void   submenu_inactive			 	(ushort id);
						 };

/*****************
 * Form Typedefs *
 *****************/

typedef struct {
								 ushort 		id;						// Unique ID of field
								 uchar  		active;				// Wether this field is active
								 uchar  		type;					// Type of field.  See FIELD_ defines.
								 void FAR * data;					// Point to unique data for this field
							 } form_field_t;

typedef struct {
								 ushort id;								// Unique ID for field
								 uchar  ch;            		// Character code to respond to
								 uchar  extended;					// Character is an extended character
								 uchar  active;
							 } form_char_t;

typedef struct {
								 uchar   x;								// Coordinates inside window for button
								 uchar   y;
								 uchar * title;						// Title of button
								 uchar   down;						// True if button depressed
							 } field_button_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar entry_width;		// Width excluding leading and trailing space for input field
								 uchar *num;
								 uchar min_num;
								 uchar max_num;
							 } field_byte_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar entry_width;		// Width excluding leading and trailing space for input field
								 ushort *num;
								 ushort min_num;
								 ushort max_num;
							 } field_ushort_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar entry_width;		// Width excluding leading and trailing space for input field
								 ulong *num;
								 ulong min_num;
								 ulong max_num;
							 } field_ulong_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar entry_width;		// Width excluding leading and trailing space for input field
								 char *num;
								 char min_num;
								 char max_num;
							 } field_char_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar entry_width;		// Width excluding leading and trailing space for input field
								 int *num;
								 int min_num;
								 int max_num;
							 } field_int_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar entry_width;		// Width excluding leading and trailing space for input field
								 long *num;
								 long min_num;
								 long max_num;
							 } field_long_t;

typedef struct {
								 uchar id;
								 uchar *title;
								 uchar x;
								 uchar y;
								 uchar checked;
							 } field_checkbox_entry_t;

typedef struct {
								 uchar title_x;   					// Coordinates for title
								 uchar title_y;
								 uchar *title;							// Title
								 uchar boxtopx;							// Coordinates for box where entries are
								 uchar boxtopy;
								 uchar boxbotx;
								 uchar boxboty;
								 field_checkbox_entry_t FAR *entrys;	// Array of entries
								 uchar num_entrys;					// Number of entries
								 uchar cur_entrys;					// Current entry highlighted
							 } field_checkbox_t;

typedef struct {
								 uchar id;
								 uchar *title;
								 uchar x;
								 uchar y;
							 } field_radiobutton_entry_t;

typedef struct {
								 uchar title_x;   					// Coordinates for title
								 uchar title_y;
								 uchar *title;							// Title
								 uchar boxtopx;							// Coordinates for box where entries are
								 uchar boxtopy;
								 uchar boxbotx;
								 uchar boxboty;
								 field_radiobutton_entry_t FAR *entrys;	// Array of entries
								 uchar num_entrys;					// Number of entries
								 uchar cur_entrys;					// Current entry highlighted
							 } field_radiobutton_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar *str;
								 uchar view_len;
								 uchar max_len;
							 } field_string_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar *str;
								 uchar *ctrl_str;
								 uchar view_len;
								 uchar max_len;
							 } field_control_string_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 faddr_t *addr;
								 uchar view_len;
							 } field_faddr_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 ulong *t;
							 } field_hhmmss_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 ulong *d;
							 } field_mmddyyyy_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar *str;
								 uchar *ctrl_str;
								 uchar max_len;
								 uchar curr_pos;
								 uchar min_pos;
								 uchar max_pos;
							 } field_frmt_string_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar *t;
							 } field_toggle_uchar_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 uchar *t;
								 uchar bit;
							 } field_toggle_uchar_bit_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 ushort *t;
								 ushort bit;
							 } field_toggle_ushort_bit_t;

typedef struct {
								 uchar title_x;
								 uchar title_y;
								 uchar *title;
								 uchar entry_x;
								 uchar entry_y;
								 ulong *t;
								 ulong bit;
							 } field_toggle_ulong_bit_t;

typedef union {														// Pointers to data type specific information
								field_button_t 	 	 	 			FAR * button;
								field_string_t 	  	 			FAR * string;
								field_frmt_string_t  			FAR * frmt_string;
								field_byte_t 	 			 			FAR * field_byte;
								field_ushort_t 			 			FAR * field_ushort;
								field_ulong_t  			 			FAR * field_ulong;
								field_checkbox_t 	 	 			FAR * checkbox;
								field_radiobutton_t  			FAR * radiobutton;
								field_faddr_t	 			 			FAR * field_faddr;
								field_hhmmss_t 						FAR * field_hhmmss;
								field_mmddyyyy_t 					FAR * field_mmddyyyy;
								field_toggle_uchar_t 			FAR * field_toggle_uchar;
								field_toggle_uchar_bit_t 	FAR * field_toggle_uchar_bit;
								field_toggle_ushort_bit_t FAR * field_toggle_ushort_bit;
								field_toggle_ulong_bit_t 	FAR * field_toggle_ulong_bit;
								field_char_t 							FAR * field_char;
								field_int_t 							FAR * field_int;
								field_long_t 							FAR * field_long;
								field_control_string_t 		FAR * control_string;
							} field_ptr_t;

/****************
 * Form Defines *
 ****************/

#define FIELD_BUTTON     				1
#define FIELD_STRING      			2
#define FIELD_UPCASE_STRING 		3
#define FIELD_FORMAT_STRING 		4
#define FIELD_BYTE							5
#define FIELD_USHORT						6
#define FIELD_ULONG							7
#define FIELD_CHECKBOX					8
#define FIELD_RADIOBUTTON				9
#define FIELD_4D_ADDR						10
#define FIELD_HHMMSS						11
#define FIELD_MMDDYYYY					12
#define FIELD_TOGGLE_UCHAR			13
#define FIELD_TOGGLE_UCHAR_BIT 	14
#define FIELD_TOGGLE_USHORT_BIT	15
#define FIELD_TOGGLE_ULONG_BIT	16
#define FIELD_CHAR							17
#define FIELD_INT								18
#define FIELD_LONG							19
#define FIELD_CONTROL_STRING		20

/*******************************
 * Entry Form Class Definition *
 *******************************/

class form_c {
							 private:
								/*************
								 * Variables *
								 *************/
								 ushort 				 		 fields_cur;	 	// Current highlighted field
								 ushort 				 		 fields_max;		// Total number of fields
								 ushort 				 		 chars_max;			// Total number of characters
								 form_field_t FAR *  fields;				// Array of fields;
								 form_char_t FAR *   chars;					// Array of characters that will be responded to
								 uchar							 button_norm_fcolor;
								 uchar							 button_norm_bcolor;
								 uchar							 button_high_fcolor;
								 uchar							 button_high_bcolor;
								 uchar 							 button_inactive_fcolor;
								 uchar							 button_inactive_bcolor;
								 uchar							 button_shadow_fcolor;
								 uchar							 button_shadow_bcolor;
								 uchar							 field_norm_fcolor;
								 uchar							 field_norm_bcolor;
								 uchar							 field_high_fcolor;
								 uchar							 field_high_bcolor;
								 uchar							 field_inactive_fcolor;
								 uchar							 field_inactive_bcolor;
								 uchar							 string_norm_fcolor;
								 uchar							 string_norm_bcolor;
								 uchar							 string_high_fcolor;
								 uchar							 string_high_bcolor;
								 uchar							 string_inactive_fcolor;
								 uchar							 string_inactive_bcolor;
								 uchar							 number_norm_fcolor;
								 uchar							 number_norm_bcolor;
								 uchar							 number_high_fcolor;
								 uchar							 number_high_bcolor;
								 uchar							 number_inactive_fcolor;
								 uchar							 number_inactive_bcolor;
								 uchar 							 checkbox_norm_fcolor;
								 uchar							 checkbox_norm_bcolor;
								 uchar							 checkbox_high_fcolor;
								 uchar							 checkbox_high_bcolor;
								 uchar							 checkbox_inactive_fcolor;
								 uchar							 checkbox_inactive_bcolor;
								 uchar							 checkbox_box_bcolor;
								 uchar 							 radiobutton_norm_fcolor;
								 uchar							 radiobutton_norm_bcolor;
								 uchar							 radiobutton_high_fcolor;
								 uchar							 radiobutton_high_bcolor;
								 uchar							 radiobutton_inactive_fcolor;
								 uchar							 radiobutton_inactive_bcolor;
								 uchar							 radiobutton_box_bcolor;
								 win_c FAR *				 win;
								 ushort 						 result;
								/*************
								 * Functions *
								 *************/
								 void   button_click	 					(ushort field_num);
								 void   button_unclick					(ushort field_num);
								 ushort check_user_def_char			(uchar ch, uchar ext);
								 void   draw_checkbox_entry			(ushort field_num, uchar entry_num, uchar highlight);
								 void   draw_radiobutton_entry	(ushort field_num, uchar entry_num, uchar highlight);
								 void   draw_field							(ushort field_num, uchar button_down, uchar draw_data);
								 ushort field_clicked						(void);
								 void 	goto_checkbox_entry			(ushort field_num, uchar entry_num);
								 void 	goto_radiobutton_entry	(ushort field_num, uchar entry_num);
								 void 	goto_field							(ushort field_num, uchar button_down);
								 uchar  load_4d_addr						(void);
								 uchar  load_button							(void);
								 uchar	load_byte								(void);
								 uchar	load_char								(void);
								 uchar 	load_checkbox		  			(void);
								 uchar  load_control_string			(void);
								 uchar  load_format_string			(void);
								 uchar  load_hhmmss							(void);
								 uchar	load_int								(void);
								 uchar	load_long								(void);
								 uchar  load_mmddyyyy						(void);
								 uchar  load_radiobutton				(void);
								 uchar 	load_string							(void);
								 uchar	load_toggle_uchar				(void);
								 uchar	load_toggle_uchar_bit		(void);
								 uchar	load_toggle_ushort_bit	(void);
								 uchar	load_toggle_ulong_bit		(void);
								 uchar	load_ulong							(void);
								 uchar  load_upcase_string			(void);
								 uchar	load_ushort			  			(void);
								 void 	next_checkbox_entry			(ushort field_num, uchar direction);
								 void 	next_radiobutton_entry	(ushort field_num, uchar direction);
								 void   next_field							(uchar direction);
							 public:
								/*************
								 * Functions *
								 *************/
								 void   add_4d_addr								(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 uchar new_view_len);
								 void   add_button		 						(ushort new_id, uchar new_x, uchar new_y, uchar *new_title);
								 void   add_byte									(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 uchar new_min_num, uchar new_max_num);
								 void 	add_char									(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 char new_min_num, char new_max_num);
								 void   add_checkbox							(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_boxtopx, uchar new_boxtopy,
																									 uchar new_boxbotx, uchar new_boxboty);
								 void   add_checkbox_entry			  (ushort checkbox_id, uchar new_id, uchar new_x,
																									 uchar new_y, uchar *new_title, uchar new_checked);
								 void   add_control_string				(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar *new_ctrl_str, uchar new_entry_x, uchar new_entry_y,
																									 uchar new_view_len, uchar new_max_len);
								 void   add_format_string					(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar *new_ctrl_str, uchar new_entry_x, uchar new_entry_y);
								 void   add_hhmmss								(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y);
								 void 	add_int										(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 int new_min_num, int new_max_num);
								 void 	add_long									(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 long new_min_num, long new_max_num);
								 void   add_mmddyyyy							(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y);
								 void   add_radiobutton						(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_boxtopx, uchar new_boxtopy,
																									 uchar new_boxbotx, uchar new_boxboty);
								 void   add_radiobutton_entry			(ushort radiobutton_id, uchar new_id, uchar new_x,
																									 uchar new_y, uchar *new_title, uchar new_cur);
								 void 	add_string								(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 uchar new_view_len, uchar new_max_len);
								 void   add_toggle_uchar					(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y);
								 void 	add_toggle_uchar_bit			(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 uchar new_bit);
								 void 	add_toggle_ulong_bit			(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 ulong new_bit);
								 void 	add_toggle_ushort_bit			(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 ushort new_bit);
								 void 	add_ulong									(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 ulong new_min_num, ulong new_max_num);
								 void   add_upcase_string					(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 uchar new_view_len, uchar new_max_len);
								 void   add_user_def_char					(ushort new_id, uchar new_ch, uchar new_ext);
								 void 	add_ushort								(ushort new_id, uchar new_title_x, uchar new_title_y,
																									 uchar *new_title, uchar new_entry_x, uchar new_entry_y,
																									 ushort new_min_num, ushort new_max_num);
								 void   button_colors 						(uchar new_button_norm_fcolor, uchar new_button_norm_bcolor,
																									 uchar new_button_high_fcolor, uchar new_button_high_bcolor,
																									 uchar new_button_inactive_fcolor, uchar new_button_inactive_bcolor,
																									 uchar new_button_shadow_fcolor, uchar new_button_shadow_bcolor);
								 void   checkbox_colors						(uchar new_checkbox_norm_fcolor, uchar new_checkbox_norm_bcolor,
																									 uchar new_checkbox_high_fcolor, uchar new_checkbox_high_bcolor,
																									 uchar new_checkbox_inactive_fcolor, uchar new_checkbox_inactive_bcolor,
																									 uchar new_checkbox_box_bcolor);
								 uchar  checkbox_entry_status			(ushort field_id, uchar entry_id);
								 void   create				 						(win_c FAR *new_win);
								 void   destroy			 							(void);
								 void   display			 							(void);
								 void   field_colors							(uchar new_field_norm_fcolor, uchar new_field_norm_bcolor,
																									 uchar new_field_high_fcolor, uchar new_field_high_bcolor,
																									 uchar new_field_inactive_fcolor, uchar new_field_inactive_bcolor);
								 ushort get_cur_field							(void);
								 ushort load											(void);
								 void   number_colors							(uchar new_number_norm_fcolor, uchar new_number_norm_bcolor,
																									 uchar new_number_high_fcolor, uchar new_number_high_bcolor,
																									 uchar new_number_inactive_fcolor, uchar new_number_inactive_bcolor);
								 void 	radiobutton_colors				(uchar new_radiobutton_norm_fcolor, uchar new_radiobutton_norm_bcolor,
																									 uchar new_radiobutton_high_fcolor, uchar new_radiobutton_high_bcolor,
																									 uchar new_radiobutton_inactive_fcolor, uchar new_radiobutton_inactive_bcolor,
																									 uchar new_radiobutton_box_bcolor);
								 uchar  radiobutton_status				(ushort field_id);
								 void  	set_4d_addr_var	 					(ushort new_id, faddr_t *new_addr, uchar edit);
								 void 	set_char_var							(ushort new_id, char *new_num, uchar edit);
								 void   set_control_string_var		(ushort new_id, uchar *new_str, uchar edit);
								 void   set_byte_var							(ushort new_id, uchar *new_num, uchar edit);
								 void   set_format_string_var			(ushort new_id, uchar *new_str, uchar edit);
								 void 	set_hhmmss_var						(ushort new_id, ulong *new_t, uchar edit);
								 void 	set_int_var								(ushort new_id, int *new_num, uchar edit);
								 void 	set_long_var							(ushort new_id, long *new_num, uchar edit);
								 void 	set_mmddyyyy_var					(ushort new_id, ulong *new_d, uchar edit);
								 void 	set_string_var						(ushort new_id, uchar *new_str, uchar edit);
								 void 	set_toggle_uchar_var			(ushort new_id, uchar *new_t, uchar edit);
								 void 	set_toggle_uchar_bit_var	(ushort new_id, uchar *new_t, uchar edit);
								 void 	set_toggle_ulong_bit_var	(ushort new_id, ulong *new_t, uchar edit);
								 void 	set_toggle_ushort_bit_var	(ushort new_id, ushort *new_t, uchar edit);
								 void   set_ulong_var							(ushort new_id, ulong *new_num, uchar edit);
								 void   set_upcase_string_var			(ushort new_id, uchar *new_str, uchar edit);
								 void   set_ushort_var						(ushort new_id, ushort *new_num, uchar edit);
								 void   string_colors							(uchar new_string_norm_fcolor, uchar new_string_norm_bcolor,
																									 uchar new_string_high_fcolor, uchar new_string_high_bcolor,
																									 uchar new_string_inactive_fcolor, uchar new_string_inactive_bcolor);
							 };



/*****************
 * Mouse Defines *
 *****************/

#define BUTTON_CLICKED 1
#define BUTTON_DOWN 	 2

/**************************
 * Mouse Global Variables *
 **************************/

extern uchar mouse_exist;			// True if mouse exists and is initialized
extern uchar mouse_left;			// True if left button pressed
extern uchar mouse_center;		// True if center button pressed
extern uchar mouse_right;			// True if right button pressed
extern uchar mouse_row;				// Current row of mouse pointer
extern uchar mouse_col;  			// Current columnb of mouse pointer

/*******************
 * Mouse Functions *
 *******************/

void mouse_init(void);
void mouse_on(void);
void mouse_off(void);
uchar mouse_read_event(void);
void mouse_deinit(void);

/************************
 * Time Slice Functions *
 ************************/
void init_timeslice(void);
void timeslice(void);

#endif /* Add nothing after this line */
