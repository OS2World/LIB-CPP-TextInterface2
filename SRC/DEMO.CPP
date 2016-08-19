/* Filename : DEMO.CPP                                                      *
 * Description : Small Demo of TI/2                                         *
 * Target : DEMO.OBJ -> DEMO.EXE                                            *
 * Portability : OS/2, DOS                                                  *
 * Creation date : 04/15/95                                                 *
 * Last modified : 05/09/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995               */

/******************************
 * Predefined header includes *
 ******************************/

#ifdef __OS2__
#define INCL_NOPMAPI
#define INCL_DOSDATETIME
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#include <os2.h>
#endif

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include "ti2.h"

void timed_getch(void)
		{
			ushort millisec_passed = 0;   // Milliseconds passed counter

			/* Clear keyboard buffer */
			while (kbhit()) getch();

			/* Loop until user presses a key or timer expires */
			while (millisec_passed < 1500) {
				/* Check for keyboard being pressed */
				if (kbhit()) {
					/* Clear buffer and return */
					while (kbhit()) getch();
					return;
				}
				/* Pause for 100 milliseconds */
				#ifdef __OS2__
				DosSleep(100);
				#else
				delay(100);
				#endif
				/* Update counter */
				millisec_passed += 100;
			}
		}

void intro(void)
		{
			win_c win;    // Displays text that describes what TI/2 can do

			/* Create the window */
			win.create(1, 1, 70, 23, CYAN, BLACK, LIGHTGRAY, BLACK, " TI/2 Demo ",
								 YELLOW, BLUE, WIN_FRAME_MIXEDBOX1, WIN_SHADOW_RIGHT,
								 WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR | WIN_EXPLODE | WIN_IMPLODE);

			/* Display information in window */
			win.cntr_printf(2, "Text Interface/2");
			win.cntr_printf(3, "Copyright (c) Jonathan Tew and Revolutionary Software 1995");
			win.cntr_printf(4, "All Rights Reserved");

			win.goto_xy(1, 6);

			win.nd_puts(" This is a small demo of the Text Interface/2 development library.\n\r");
			win.nd_puts(" This gives a brief overview of the various taks that can be\n\r");
			win.nd_puts(" accomplished with TI/2.  These examples include windows, menus,\n\r");
			win.nd_puts(" entry forms, and windowed input.\n\r\n\r");

      win.nd_puts(" TI/2 has libraries for Turbo C++ v3.0 and Borland C++ 2.0 for\n\r");
			win.nd_puts(" OS/2.  Support for many more compilers will be available soon.\n\r\n\r");

			win.nd_puts(" The TI/2 order form can be found in the file ORDER.FRM.  \n\r\n\r");

			win.nd_puts(" Mail: Revolutionary Software     BBS: The Fallen Fortress\n\r");
			win.nd_puts("       2689 Hickory Cove               (404) 469-6752\n\r");
			win.nd_puts("       Lilburn, GA 30247\n\r");

			win.cntr_printf(win.text_height(), "Press any key to continue.");

			win.redisplay_text();

			/* Wait for key to be pressed */
			while (kbhit()) getch();
			while (!kbhit()) desktop.wputs(" TI/2 ");
			while (kbhit()) getch();

			/* Clear the desktop */
			win.destroy();
		}

void creating(void)
		{
			win_c win1;   // Demonstration window
			win_c win2;   // Demonstration window

			/* Create two example windows */
			win1.create(1, 1, 30, 10, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 1 ", YELLOW, BLUE, WIN_FRAME_DOUBLEBOX,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win2.create(49, 15, 79, 24, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 2 ", YELLOW, BLUE, WIN_FRAME_DOUBLEBOX,
									WIN_SHADOW_LEFT, WIN_TITLE_CENTER);

			/* Explain shadows */
			win1.cntr_printf(win1.text_height() / 2, "Windows can have shadows...");
			win2.cntr_printf(win2.text_height() / 2, "either on the top and left");
			win2.cntr_printf(win2.text_height() / 2 + 1, "or bottom and right.");

			timed_getch();

			/* Display different types of frames */
			win1.clr_scr();
			win2.clr_scr();
			win1.cntr_printf(win1.text_height() / 2, "Frames can even be changed.");
			timed_getch();
			win1.frame_change(WIN_FRAME_SINGLEBOX);
			win2.frame_change(WIN_FRAME_MIXEDBOX1);
			timed_getch();
			win1.frame_change(WIN_FRAME_MIXEDBOX2);
			win2.frame_change(WIN_FRAME_HATCHBOX1);
			timed_getch();
			win1.frame_change(WIN_FRAME_HATCHBOX2);
			win2.frame_change(WIN_FRAME_HATCHBOX3);
			timed_getch();
			win1.frame_change(WIN_FRAME_SOLIDBOX1);
			win2.frame_change(WIN_FRAME_DOTTEDLINE);

			timed_getch();
			win1.frame_change(WIN_FRAME_SOLIDBOX2);
			timed_getch();

			/* Show titles example */
			win1.clr_scr();
			win1.cntr_printf(3, "Titles can be...");
			win1.cntr_printf(4, "centered.");
			timed_getch();
			win1.destroy();
			win1.create(1, 1, 30, 10, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 1 ", YELLOW, BLUE, WIN_FRAME_DOUBLEBOX,
									WIN_SHADOW_RIGHT, WIN_TITLE_LEFT);
			win1.clr_scr();
			win1.cntr_printf(3, "Titles can be...");
			win1.cntr_printf(4, "left justified.");
			timed_getch();
			win1.destroy();
			win1.create(1, 1, 30, 10, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 1 ", YELLOW, BLUE, WIN_FRAME_DOUBLEBOX,
									WIN_SHADOW_RIGHT, WIN_TITLE_RIGHT);
			win1.clr_scr();
			win1.cntr_printf(3, "Titles can be...");
			win1.cntr_printf(4, "right justified.");
			timed_getch();

			/* Show centering demo */
			win1.clr_scr();
			win2.clr_scr();
			win2.cntr_printf(win2.text_height() / 2, "Windows can be centered...");
			win2.cntr_printf(win2.text_height() / 2 + 1, "horizontally");
			win1.center(WIN_HORIZ_CNTR);
			timed_getch();
			win1.move(1, 1);
			win2.clr_scr();
			win2.cntr_printf(win2.text_height() / 2, "Windows can be centered...");
			win2.cntr_printf(win2.text_height() / 2 + 1, "vertically");
			win1.center(WIN_VERT_CNTR);
			timed_getch();
			win2.clr_scr();
			win2.cntr_printf(win2.text_height() / 2, "Windows can be centered...");
			win2.cntr_printf(win2.text_height() / 2 + 1, "vert. & horiz.");
			win1.center(WIN_VERT_CNTR | WIN_HORIZ_CNTR);
			timed_getch();

			/* Destroy two example windows */
			win1.destroy();
			win2.destroy();
		}

#define TAB                9
#define SHIFT_TAB         15  // Extended character
#define ESC               27
#define UPARROW           72  // Extended character
#define DOWNARROW         80  // Extended character
#define LEFTARROW         75  // Extended character
#define RIGHTARROW        77  // Extended character

void seethru(void)
		{
			uchar x, y;   // Temporary coordinate
			win_c win;    // Window that is slide around the screen;
			uchar is_seethru = 1;   // Tells whether window is see through.
			win_c desc_win;   // Window that holds instructions
			uchar ch;       // Character entered by user
			uchar is_on_top = 1;  // Tells which window is on top

			/* Create description window and display instructions */
			desc_win.create(1, 1, 40, 9, CYAN, BLACK, LIGHTGRAY, BLACK, " Instructions ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
											WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);
			desc_win.wputs("\n\r");
			desc_win.wputs(" Navigate the other window around\n\r");
			desc_win.wputs(" with the arrow keys.  Hit TAB to\n\r");
			desc_win.wputs(" toggle between normal or see-thru,\n\r");
			desc_win.wputs(" SHIFT-TAB to toggle window on top,\n\r");
			desc_win.wputs(" and ESC to stop.");

			/* Create see-thru window */
			win.create(1, 1, 15, 15, CYAN, BLACK, LIGHTGRAY, BLACK, " See-thru ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX2,
								 WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_SEETHRU);

			while ((ch = getch()) != ESC) {
				x = win.where_win_x();
				y = win.where_win_y();
				switch(ch) {
					case 0 : {
										 switch(getch()) {
											 case UPARROW : {
																				y--;
																				win.slide(x, y);
																				break;
																			}
											 case DOWNARROW : {
																					y++;
																					win.slide(x, y);
																					break;
																				}
											 case LEFTARROW : {
																					x--;
																					win.slide(x, y);
																					break;
																				}
											 case RIGHTARROW : {
																					 x++;
																					 win.slide(x, y);
																					 break;
																				 }
											 case SHIFT_TAB : {
																					/* Popup the correct window */
																					if (is_on_top) {
																						is_on_top = 0;
																						desc_win.popup();
																					}
																						else

																					{
																						is_on_top = 1;
																						win.popup();
																					}
																				}
										 }
										 break;
									 }
					case TAB : {
											 /* Toggle window see-thru and normal and change title to reflect the state */
											 if (is_seethru) {
												 win.make_normal();
												 win.title_change(" Normal ");
												 is_seethru = 0;
											 }
												 else
											 {
												 win.make_seethru();
												 win.title_change( " See-thru ");
												 is_seethru = 1;
											 }
											 break;
										 }
				}
			}

			/* Destroy description and see-thru window */
			desc_win.destroy();
			win.destroy();
		}

void output(void)
		{
			win_c win1;   // Demonstration window
			win_c win2;   // Demonstration window
			win_c win3;   // Demonstration window
			uchar i;      // General counter

			/* Create windows that overlap */
			win1.create(10, 5, 40, 12, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 1 ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win2.create(15, 10, 45, 17, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 2 ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win3.create(20, 15, 70, 22, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 3 ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);

			/* Show off regular printing */
			win3.cntr_printf(1, "You can write text to a window...");
			timed_getch();
			win3.cntr_printf(2, "Hello World!");
			timed_getch();

			/* Show changing the colors and printing */
			win3.clr_scr();
			win3.cntr_printf(1, "You can write text with different colors...");
			timed_getch();
			win3.text_color(BLACK);
			win3.text_background(LIGHTGRAY);
			win3.cntr_printf(2, "Hello World!");
			win3.text_color(YELLOW);
			win3.text_background(RED);
			win3.cntr_printf(3, "Hello World!");
			win3.text_color(LIGHTGRAY);
			win3.text_background(BLACK);
			timed_getch();

			/* Show printf function */
			win3.clr_scr();
			win3.cntr_printf(1, "You can even use a printf function for windows!");
			win3.cntr_printf(2, "To demonstrate I will count to twenty...");
			timed_getch();
			win3.clr_scr();
			for (i = 1; i <= 20; i++) {
				win3.wprintf("Count is now = %d\n\r", i);
				timed_getch();
			}

			/* Show right justify */
			win3.clr_scr();
			win3.cntr_printf(1, "You can write Right Justified strings.");
			timed_getch();
			win3.rj_printf(20, 3, "These strings");
			win3.rj_printf(20, 4, "are printed right");
			win3.rj_printf(20, 5, "justified");
			timed_getch();

			/* Show column centering */
			win3.clr_scr();
			win3.cntr_printf(1, "You can center strings on different columns...");
			timed_getch();
			win3.col_cntr_printf(9, 3, "These strings");
			win3.col_cntr_printf(9, 4, "are centered on");
			win3.col_cntr_printf(9, 5, "column 9.");
			timed_getch();
			win3.clr_scr();
			win3.col_cntr_printf(18, 3, "These strings");
			win3.col_cntr_printf(18, 4, "are centered on");
			win3.col_cntr_printf(18, 5, "column 18.");
			timed_getch();
			win3.clr_scr();
			win3.col_cntr_printf(28, 3, "These strings");
			win3.col_cntr_printf(28, 4, "are centered on");
			win3.col_cntr_printf(28, 5, "column 28.");
			timed_getch();

			/* Show background writing */
			win3.clr_scr();
			win3.cntr_printf(1, "You can write strings to background windows...");
			win3.cntr_printf(2, "To demonstrate we will count in all the windows.");
			timed_getch();
			win3.clr_scr();
			for(i = 1; i <= 75; i++) {
				win1.wprintf("Count is now = %d\n\r", i);
				win2.wprintf("Count is now = %d\n\r", i);
				win3.wprintf("Count is now = %d\n\r", i);
			}
			timed_getch();

			/* Destroy windows */
			win1.destroy();
			win2.destroy();
			win3.destroy();
		}

void clearing(void)
		{
			win_c win1;   // Demonstration window
			win_c win2;   // Demonstration window

			/* Create windows */
			win1.create(1, 1, 40, 12, CYAN, BLACK, LIGHTGRAY, BLACK, " Window ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win2.create(10, 18, 70, 22, CYAN, BLACK, LIGHTGRAY, BLACK, " Description ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);

			/* Fill window so they can see it being cleared */
			win1.fill('*');

			/* Show clr_eol function */
			win2.cntr_printf(2, "You can clear from the cursor to the end of the line.");
			timed_getch();
			win1.goto_xy(10, 1);
			win1.clr_eol();

			/* Demonstrate clr_region function */
			win2.clr_scr();
			win2.cntr_printf(2, "You can clear a region of the window.");
			timed_getch();
			win1.clr_region(2, 3, 10, 8);

			/* Demonstrate clr_scr function */
			win2.clr_scr();
			win2.cntr_printf(2, "You can clear the whole window.");
			timed_getch();
			win1.clr_scr();
			timed_getch();

			/* Destroy windows */
			win1.destroy();
			win2.destroy();
		}

void scroll_bar(void)
		{
			win_c win;    // Window that is slide around the screen;
			uchar ch;       // Character entered by user
			uchar done = 0; // True if ESC pressed

			/* Create description window and display instructions */
			win.create(1, 1, 40, 20, CYAN, BLACK, LIGHTGRAY, BLACK, " Instructions ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
								 WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);
			win.wputs("\n\r");
			win.wputs(" Slide the scroll bars around with\n\r");
			win.wputs(" the arrow keys.  Hit ESC to stop.\n\r");

			/* Create scroll bars */
			win.hscroll_create(WHITE, CYAN, 1, 50);
			win.vscroll_create(WHITE, CYAN, 1, 50);

			while (!done) {
				/* Check keyboard and mouse for input */
				for (;;) {
					/* Check keyboard */
					if (kbhit()) {
						ch = getch();
						break;
					}
					/* Check mouse */
					if (mouse_read_event()) {
						/* Call scroll bar mouse events */
						if (!win.hscroll_mouse_move()) win.vscroll_mouse_move();
					}
				}

				switch(ch) {
					case 0 : {
										 switch(getch()) {
											 case UPARROW : {
																				win.vscroll_up(1);
																				break;
																			}
											 case DOWNARROW : {
																					win.vscroll_down(1);
																					break;
																				}
											 case LEFTARROW : {
																					win.hscroll_left(1);
																					break;
																				}
											 case RIGHTARROW : {
																					 win.hscroll_right(1);
																					 break;
																				 }
										 }
										 break;
									 }
					case ESC : {
											 done = 1;
											 break;
										 }
				}
			}

			/* Destroy description and see-thru window */
			win.destroy();
		}

void scrolling(void)
		{
			win_c win1;   // Demonstration window
			win_c win2;   // Demonstration window

			/* Create windows */
			win1.create(1, 1, 40, 12, CYAN, BLACK, LIGHTGRAY, BLACK, " Window ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win2.create(10, 18, 70, 22, CYAN, BLACK, LIGHTGRAY, BLACK, " Description ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);

			/* Fill window so they can see it being scrolled */

			/* Demonstration */
			win2.cntr_printf(2, " You can scroll windows right.");
			win1.fill('*');
			timed_getch();
			win1.scroll_right();
			timed_getch();

			win2.cntr_printf(2, " You can scroll windows left.");
			win1.fill('*');
			timed_getch();
			win1.scroll_left();
			timed_getch();

			win2.cntr_printf(2, " You can scroll windows up.");
			win1.fill('*');
			timed_getch();
			win1.scroll_up();
			timed_getch();
			win2.cntr_printf(2, " You can scroll windows down.");
			win1.fill('*');
			timed_getch();
			win1.scroll_down();
			timed_getch();

			/* Destroy windows */
			win1.destroy();
			win2.destroy();
		}

void hiding(void)
		{
			win_c win1;   // Demonstration window
			win_c win2;   // Demonstration window
			win_c win3;   // Demonstration window
			uchar ch;     // General counter
			uchar is_hidden = 0;    // True if window is hidden

			/* Create windows that overlap */
			win1.create(10, 5, 40, 12, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 1 ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win2.create(15, 10, 45, 17, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 2 ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);
			win3.create(20, 15, 70, 22, CYAN, BLACK, LIGHTGRAY, BLACK, " Window 3 ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER);

			/* Display instructions */
			win3.wputs(" Press space to hide and unhide Window 2.\n\r");
			win3.wputs(" Press ESC to stop.");

			while ((ch = getch()) != ESC) {
				if (ch == ' ') {
					/* Hide or unhide window */
					if (is_hidden) {
						is_hidden = 0;
						win2.unhide();

					}
						else
					{
						is_hidden = 1;
						win2.hide();
					}
				}

			}

			/* Destroy windows */
			win1.destroy();
			win2.destroy();
			win3.destroy();
		}

void strings(void)
		{
			uchar formatted_string[21] = "(800)555-1212";
			uchar normal_string[61] = "This is a very long string that you can edit :)";
			uchar upcased_string[61] = "THIS IS A VERY LONG UPCASED STRING............";
			uchar control_string[61] = "0fff";
			win_c win;
			form_c form;

			/* Defines used for entry form */
			#define F_FORMATTED_STRING 1
			#define F_NORMAL_STRING    2
			#define F_UPCASED_STRING   3
			#define F_CONTROL_STRING   4
			#define B_DONE             5

			/* Create window */
			win.create(1, 1, 50, 13, CYAN, BLACK, LIGHTGRAY, BLACK, " String Input ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);

			form.create(&win);
			form.button_colors(BLACK, GREEN, YELLOW, GREEN, DARKGRAY, GREEN, LIGHTGRAY, BLACK);
			form.field_colors(BLACK, LIGHTGRAY, WHITE, LIGHTGRAY, DARKGRAY, LIGHTGRAY);
			form.string_colors(WHITE, BLUE, WHITE, GREEN, DARKGRAY, LIGHTGRAY);

			form.add_format_string(F_FORMATTED_STRING, 2, 2, " Formatted String ", "(###)###-####", 22, 2);
			form.set_format_string_var(F_FORMATTED_STRING, formatted_string, 1);
			form.add_string(F_NORMAL_STRING, 2, 4, " Normal String    ", 22, 4, 20, 60);
			form.set_string_var(F_NORMAL_STRING, normal_string, 1);
			form.add_upcase_string(F_UPCASED_STRING, 2, 6, " Upcased String   ", 22, 6, 20, 60);
			form.set_upcase_string_var(F_UPCASED_STRING, upcased_string, 1);
			form.add_control_string(F_CONTROL_STRING, 2, 8, " Hex Ctrl String  ", "0123456789abcdefABCDEF", 22, 8, 20, 60);
			form.set_control_string_var(F_CONTROL_STRING, control_string, 1);
			form.add_button(B_DONE, 2, 10, "Done");

			/* Display form and get input */
			form.display();
			form.load();

			/* Cleanup memory allocated by window and entry form */
			form.destroy();
			win.destroy();

			/* Undefine defines used for entry form */
			#undef F_FORMATTED_STRING
			#undef F_NORMAL_STRING
			#undef F_UPCASED_STRING
			#undef F_CONTROL_STRING
			#undef B_DONE
		}

void radiobutton_and_checkboxs(void)
		{
			win_c win;
			form_c form;

			/* Defines used for checkboxes and radiobuttons */
			#define B_DONE             1
			#define F_CHECKBOX         2
				#define CE_ENVIROMENT       1
				#define CE_DESKTOP          2
				#define CE_PROJECT          3
			#define F_RADIOBUTTON      3
				#define RE_CUA              1
				#define RE_ALTERNATE        2
				#define RE_NATIVE           3

			/* Create window */
			win.create(1, 1, 36, 10, CYAN, BLACK, LIGHTGRAY, BLACK, " Radio Buttons / Checkboxes ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);

			form.create(&win);
			form.button_colors(BLACK, GREEN, YELLOW, GREEN, DARKGRAY, GREEN, LIGHTGRAY, BLACK);
			form.field_colors(BLACK, LIGHTGRAY, WHITE, LIGHTGRAY, DARKGRAY, LIGHTGRAY);
			form.radiobutton_colors(BLACK, CYAN, WHITE, CYAN, DARKGRAY, CYAN, CYAN);
			form.checkbox_colors(BLACK, CYAN, WHITE, CYAN, DARKGRAY, CYAN, CYAN);

			form.add_button(B_DONE, 2, 7, "Done");

			form.add_radiobutton(F_RADIOBUTTON, 2, 2, "Command Set", 2, 3, 16, 5);
			form.add_radiobutton_entry(F_RADIOBUTTON, RE_CUA,       2, 1, "CUA", 1);
			form.add_radiobutton_entry(F_RADIOBUTTON, RE_ALTERNATE, 2, 2, "Alternate", 0);
			form.add_radiobutton_entry(F_RADIOBUTTON, RE_NATIVE,    2, 3, "Native", 0);

			form.add_checkbox(F_CHECKBOX, 18, 2, "Save Options", 18, 3, 33, 5);
			form.add_checkbox_entry(F_CHECKBOX, CE_ENVIROMENT, 2, 1, "Enviroment", 1);
			form.add_checkbox_entry(F_CHECKBOX, CE_DESKTOP,    2, 2, "Desktop", 1);
			form.add_checkbox_entry(F_CHECKBOX, CE_PROJECT,    2, 3, "Project", 1);

			/* Display form and get input */
			form.display();
			form.load();

			/* Cleanup memory allocated by window and entry form */
			form.destroy();
			win.destroy();

			/* Undefine defines used for checkboxes and radiobuttons */
			#undef B_DONE
			#undef F_CHECKBOX
			#undef CE_ENVIROMENT
			#undef CE_DESKTOP
			#undef CE_PROJECT
			#undef F_RADIOBUTTON
			#undef RE_CUA
			#undef RE_ALTERNATE
			#undef RE_NATIVE
		}

void listboxs(void)
		{
			win_c win;
			form_c form;

			/* Defines used for checkboxes and radiobuttons */
			#define B_DONE             1
			#define F_LISTBOX         2

			/* Create window */
			win.create(1, 1, 36, 16, CYAN, BLACK, LIGHTGRAY, BLACK, " Listboxes ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);

			form.create(&win);
			form.button_colors(BLACK, GREEN, YELLOW, GREEN, DARKGRAY, GREEN, LIGHTGRAY, BLACK);
			form.field_colors(BLACK, LIGHTGRAY, WHITE, LIGHTGRAY, DARKGRAY, LIGHTGRAY);
			form.listbox_colors(BLACK, CYAN, WHITE, CYAN, CYAN, WHITE, BLUE);

			form.add_button(B_DONE, 2, 13, "Done");

      form.add_listbox(F_LISTBOX, 2, 2, "About Listboxes", 2, 4, win.text_width() - 2, 10, 1, 1);
			form.add_listbox_entry(F_LISTBOX, "Although this listbox is scrolling text, TI/2 provides one", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "of the most powerful listboxes available.  Other programming", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "enviroments allow for only text strings to be stored in a", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "listbox, but TI/2 allows the programmer to store a text", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "representation and data of any type.  This means the programmer", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "doesn't have to parse the entry choosen to figure out what data", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "they would like to manipulate.  The user need only to double-click", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "or press entry on an entry to choose it.  In this case choosing an", NULL, 0);
			form.add_listbox_entry(F_LISTBOX, "entry will close the form.", NULL, 0);

			/* Display form and get input */
			form.display();
			form.load();

			/* Cleanup memory allocated by window and entry form */
			form.destroy();
			win.destroy();

			/* Undefine defines used for checkboxes and radiobuttons */
			#undef B_DONE
			#undef F_LISTBOX
		}

void numbers(void)
		{
			uchar num_byte = 0;
			ushort num_ushort = 0;
			ulong num_ulong = 0;
			char  num_char = 0;
			int num_int = 0;
			long num_long = 0;
			win_c win;
			form_c form;

			/* Defines used for entry form */
			#define F_BYTE    1
			#define F_USHORT  2
			#define F_ULONG   3
			#define F_CHAR    4
			#define F_INT     5
			#define F_LONG    6
			#define B_DONE    7

			/* Create window */
			win.create(1, 1, 25, 17, CYAN, BLACK, LIGHTGRAY, BLACK, " Number Input ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);

			form.create(&win);
			form.button_colors(BLACK, GREEN, YELLOW, GREEN, DARKGRAY, GREEN, LIGHTGRAY, BLACK);
			form.field_colors(BLACK, LIGHTGRAY, WHITE, LIGHTGRAY, DARKGRAY, LIGHTGRAY);
			form.number_colors(WHITE, BLUE, WHITE, GREEN, DARKGRAY, LIGHTGRAY);

			form.add_byte(F_BYTE, 2, 2, " byte   ", 11, 2, 0, 255);
			form.set_byte_var(F_BYTE, &num_byte, 1);
			form.add_ushort(F_USHORT, 2, 4, " ushort ", 11, 4, 0, 65535);
			form.set_ushort_var(F_USHORT, &num_ushort, 1);
			form.add_ulong(F_ULONG, 2, 6, " ulong  ", 11, 6, 0, 4294967295);
			form.set_ulong_var(F_ULONG, &num_ulong, 1);

			form.add_char(F_CHAR, 2, 8, " char   ", 11, 8, -128, 127);
			form.set_char_var(F_CHAR, &num_char, 1);
			form.add_int(F_INT, 2, 10, " int    ", 11, 10, -32768, 32767);
			form.set_int_var(F_INT, &num_int, 1);
			form.add_long(F_LONG, 2, 12, " long   ", 11, 12, -2147483648, 2147647);
			form.set_long_var(F_LONG, &num_long, 1);

			form.add_button(B_DONE, 2, 14, "Done");

			form.display();
			form.load();

			/* Cleanup memory allocated by window and entry form */
			form.destroy();
			win.destroy();

			/* Undefine defines used for entry form */
			#undef F_BYTE
			#undef F_USHORT
			#undef F_ULONG
			#undef F_CHAR
			#undef F_INT
			#undef F_LONG
			#undef B_DONE
		}

void others(void)
		{
			ulong d = 19951225;
			ulong t = 120000;
			faddr_t addr = {1, 133, 5016, 0};
			uchar toggle = 1;
			win_c win;
			form_c form;

			/* Defines used for entry form */
			#define F_TIME    1
			#define F_DATE    2
			#define F_ADDR    3
			#define F_TOGGLE  4
			#define B_DONE    5

			/* Create window */
			win.create(1, 1, 39, 13, CYAN, BLACK, LIGHTGRAY, BLACK, " Other Input ", YELLOW, BLUE, WIN_FRAME_MIXEDBOX1,
									WIN_SHADOW_RIGHT, WIN_TITLE_CENTER | WIN_HORIZ_CNTR | WIN_VERT_CNTR);

			form.create(&win);
			form.button_colors(BLACK, GREEN, YELLOW, GREEN, DARKGRAY, GREEN, LIGHTGRAY, BLACK);
			form.field_colors(BLACK, LIGHTGRAY, WHITE, LIGHTGRAY, DARKGRAY, LIGHTGRAY);
			form.string_colors(WHITE, BLUE, WHITE, GREEN, DARKGRAY, LIGHTGRAY);

			form.add_hhmmss(F_TIME, 2, 2, " Time       ", 15, 2);
			form.set_hhmmss_var(F_TIME, &t, 1);

			form.add_mmddyyyy(F_DATE, 2, 4, " Date ", 15, 4);
			form.set_mmddyyyy_var(F_DATE, &d, 1);

			form.add_4d_addr(F_ADDR, 2, 6, " Fido Addr. ", 15, 6, 20);
			form.set_4d_addr_var(F_ADDR, &addr, 1);

			form.add_toggle_uchar(F_TOGGLE, 2, 8, " Toggle     ", 15, 8);
			form.set_toggle_uchar_var(F_TOGGLE, &toggle, 1);

			form.add_button(B_DONE, 2, 10, "Done");

			form.display();
			form.load();

			/* Cleanup memory allocated by window and entry form */
			form.destroy();
			win.destroy();

			/* Undefine defines used for entry form */
			#undef F_TIME
			#undef F_DATE
			#undef F_ADDR
			#undef F_TOGGLE
			#undef B_DONE
		}


#define SM_WINDOWS    1
#define SM_FORM       2
#define SM_SCREEN     3

#define MC_CREATING   1
#define MC_SEETHRU    2
#define MC_OUTPUT     3
#define MC_CLEARING   4
#define MC_SCROLL_BAR 5
#define MC_SCROLLING  6
#define MC_HIDING     7

#define MC_STRINGS    8
#define MC_NUMBERS    9
#define MC_RADIOBUT   10
#define MC_LISTBOX    11
#define MC_OTHER      12

#define MC_25_ROWS    13
#define MC_50_ROWS    14

void main(void)
		{
			menu_c menu;      // Menu
			uchar done = 0;   // True when user presses ESC to exit menu

			/* Initialize TI/2 */
			win_init();

			/* Call the introduction function */
			intro();

			/* Fill the desktop */
			desktop.fill('þ');

			/* Create menu */
			menu.create(1, YELLOW, LIGHTGRAY, BLACK, BLUE, LIGHTGRAY, DARKGRAY, BLACK, LIGHTGRAY, BLACK);
			menu.add_submenu(SM_WINDOWS, 2, " ~Windows ", 'W');
			menu.add_menucmd_func(SM_WINDOWS, MC_CREATING,   " ~Creating   ", 'C');
			menu.add_menucmd_func(SM_WINDOWS, MC_SEETHRU,    " ~See-thru   ", 'S');
			menu.add_menucmd_func(SM_WINDOWS, MC_OUTPUT,     " ~Output     ", 'O');
			menu.add_menucmd_func(SM_WINDOWS, MC_CLEARING,   " C~learing   ", 'L');
			menu.add_menucmd_func(SM_WINDOWS, MC_SCROLL_BAR, " Sc~roll Bar ", 'S');
			menu.add_menucmd_func(SM_WINDOWS, MC_SCROLLING,  " Scroll~ing  ", 'I');
			menu.add_menucmd_func(SM_WINDOWS, MC_HIDING,     " ~Hiding     ", 'H');

			menu.add_submenu(SM_FORM, 12, " ~Form ", 'F');
			menu.add_menucmd_func(SM_FORM, MC_STRINGS,  " ~String ", 'S');
			menu.add_menucmd_func(SM_FORM, MC_NUMBERS,  " ~Number ", 'N');
			menu.add_menucmd_func(SM_FORM, MC_RADIOBUT, " ~Radiobuttons / Checkboxes ", 'R');
			menu.add_menucmd_func(SM_FORM, MC_LISTBOX,  " ~Listboxes ", 'L');
			menu.add_menucmd_func(SM_FORM, MC_OTHER,    " ~Other ", 'O');

			menu.add_submenu(SM_SCREEN, 19, " ~Screen ", 'S');
			menu.add_menucmd_func(SM_SCREEN, MC_25_ROWS,  " ~25 Rows ", '2');
			menu.add_menucmd_func(SM_SCREEN, MC_50_ROWS,  " ~50 Rows ", '5');

			/* Load menu */
			while (!done) {
				switch(menu.load()) {
					case 0 : {
										 /* Exit menu */
										 done = 1;
										 break;
									 }
					case MC_CREATING : {
															 /* Give creating windows demonstartion */
															 creating();
															 break;
														 }
					case MC_SEETHRU : {
															/* Let them play with see-thru windows */
															seethru();
															break;
														}
					case MC_OUTPUT : {
														 /* Give output demonstration */
														 output();
														 break;
													 }
					case MC_CLEARING : {
															 /* Give clearing demonstration */
															 clearing();
															 break;
														 }
					case MC_SCROLL_BAR : {
																 /* Give scroll bar demonstration */
																 scroll_bar();
																 break;
															 }
					case MC_SCROLLING : {
																/* Give scrolling demonstration */
																scrolling();
																break;
															}
					case MC_HIDING : {
														 /* Give hiding/unhiding demonstration */
														 hiding();
														 break;
													 }
					case MC_STRINGS : {
															/* Give string input demonstration */
															strings();
															break;
														}
					case MC_NUMBERS : {
															/* Give number input demonstration */
															numbers();
															break;
														}
					case MC_RADIOBUT : {
															 /* Give radio buttons and checkboxes demonstration */
															 radiobutton_and_checkboxs();
															 break;
														 }
					case MC_LISTBOX : {
															/* Give listbox demonstration */
															listboxs();
															break;
														}
					case MC_OTHER : {
														/* Give other input demonstration */
														others();
														break;
													}
					case MC_25_ROWS : {
															/* Changes text mode */
															win_text_mode(C80);
															break;
														}
					case MC_50_ROWS : {
															/* Changes text mode */
															win_text_mode(C4350);
															/* Refill desktop since we have 25 more rows */
															desktop.fill('þ');
															break;
														}
				}
			}

			/* Destroy Menu */
			menu.destroy();

			/* Deinitialize TI/2 */
			win_deinit();

			/* Exit gracefully */
			textcolor(LIGHTGRAY);
			textbackground(BLACK);
			clrscr();
			printf("\nThanks for testing out TI/2.  A Revolutionary Software product.\n");
		}
