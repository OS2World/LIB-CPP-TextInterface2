/* Filename : FORM2.CPP                                                     *
 * Description : Form module #2 for TI/2                                    *
 * Target : FORM2.OBJ -> TI2FORM.LIB or TI2.DLL                             *
 * Portability : OS/2, DOS																									*
 * Creation date : 05/04/95                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995								*/

/******************************
 * Predefined header includes *
 ******************************/

#if defined(__OS2__) || defined(__EMX__)
	#define INCL_DOSDATETIME
	#define INCL_DOSPROCESS
	#define INCL_NOPMAPI
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
	#define CR                13
	#define ESC								27
	#define BACK_SPACE         8
	#define TAB                9
	#define SHIFT_TAB         15  // Extended character
	#define UPARROW           72  // Extended character
	#define DOWNARROW         80  // Extended character
	#define LEFTARROW         75  // Extended character
	#define RIGHTARROW        77  // Extended character
#endif

/************************
 * Form Class Functions *
 ************************/

ushort form_c::load(void)
		{
			uchar inactive = 0;		// When true function exits

			/* Make sure that entry_fields_cur is set correctly */
			if ((fields_cur < 1) && (fields_max > 0)) fields_cur = 1;
			if (fields_cur > fields_max) fields_cur = fields_max;

			/* Initialize variables */
			result = 0;

			/* Loop and get characters and then process */
			while (!inactive) {
				/* Load the correct field */
				switch(fields[fields_cur - 1].type) {
					case FIELD_BUTTON : {
																switch(load_button()) {
																	case 0 : {
																						 /* Button pressed */
																						 return(result);
																					 }
																	case 1 : {
																						 /* Next field up */
																						 next_field(0);
																						 break;
																					 }
																	case 2 : {
																						 /* Next field down */
																						 next_field(1);
																						 break;
																					 }
																}
																break;
															}
					case FIELD_BYTE : {
															switch(load_byte()) {
																case 0 : {
																					 /* Button pressed */
																					 return(result);
																				 }
																case 1 : {
																					 /* Next field up */
																					 next_field(0);
																					 break;
																				 }
																case 2 : {
																					 /* Next field down */
																					 next_field(1);
																					 break;
																				 }
															}
															break;
														}
					case FIELD_CHAR : {
															switch(load_char()) {
																case 0 : {
																					 /* Button pressed */
																					 return(result);
																				 }
																case 1 : {
																					 /* Next field up */
																					 next_field(0);
																					 break;
																				 }
																case 2 : {
																					 /* Next field down */
																					 next_field(1);
																					 break;
																				 }
															}
															break;
														}
					case FIELD_INT : {
														 switch(load_int()) {
															 case 0 : {
																					/* Button pressed */
																					return(result);
																				}
															 case 1 : {
																					/* Next field up */
																					next_field(0);
																					break;
																				}
															 case 2 : {
																					/* Next field down */
																					next_field(1);
																					break;
																				}
															}
															break;
														}
					case FIELD_LONG : {
															switch(load_long()) {
																case 0 : {
																					 /* Button pressed */
																					 return(result);
																				 }
																case 1 : {
																					 /* Next field up */
																					 next_field(0);
																					 break;
																				 }
																case 2 : {
																					 /* Next field down */
																					 next_field(1);
																					 break;
																				 }
															}
															break;
														}
					case FIELD_USHORT : {
																switch(load_ushort()) {
																	case 0 : {
																						 /* Button pressed */
																						 return(result);
																					 }
																	case 1 : {
																						 /* Next field up */
																						 next_field(0);
																						 break;
																					 }
																	case 2 : {
																						 /* Next field down */
																						 next_field(1);
																						 break;
																					 }
																}
																break;
															}
					case FIELD_ULONG : {
															 switch(load_ulong()) {
																 case 0 : {
																						/* Button pressed */
																						return(result);
																					}
																 case 1 : {
																						/* Next field up */
																						next_field(0);
																						break;
																					}
																 case 2 : {
																						/* Next field down */
																						next_field(1);
																						break;
																					}
															 }
															 break;
														 }
					case FIELD_STRING : {
																switch(load_string()) {
																	case 0 : {
																						 /* Button pressed */
																						 return(result);
																					 }
																	case 1 : {
																						 /* Next field up */
																						 next_field(0);
																						 break;
																					 }
																	case 2 : {
																						 /* Next field down */
																						 next_field(1);
																						 break;
																					 }
																}
																break;
															}
					case FIELD_CONTROL_STRING : {
																				switch(load_control_string()) {
																					case 0 : {
																										 /* Button pressed */
																										 return(result);
																									 }
																					case 1 : {
																										 /* Next field up */
																										 next_field(0);
																										 break;
																									 }
																					case 2 : {
																										 /* Next field down */
																										 next_field(1);
																										 break;
																									 }
																				}
																				break;
																			}
					case FIELD_TOGGLE_UCHAR : {
																			switch(load_toggle_uchar()) {
																				case 0 : {
																									 /* Button pressed */
																									 return(result);
																								 }
																			case 1 : {
																								 /* Next field up */
																								 next_field(0);
																								 break;
																							 }
																			case 2 : {
																								 /* Next field down */
																								 next_field(1);
																								 break;
																							 }
																		}
																		break;
																	}
					case FIELD_TOGGLE_UCHAR_BIT : {
																					switch(load_toggle_uchar_bit()) {
																						case 0 : {
																											 /* Button pressed */
																											 return(result);
																										 }
																						case 1 : {
																											 /* Next field up */
																											 next_field(0);
																											 break;
																										 }
																						case 2 : {
																											 /* Next field down */
																											 next_field(1);
																											 break;
																										 }
																					}
																					break;
																				}
					case FIELD_TOGGLE_USHORT_BIT : {
																					 switch(load_toggle_ushort_bit()) {
																						 case 0 : {
																												/* Button pressed */
																												return(result);
																											}
																						 case 1 : {
																												/* Next field up */
																												next_field(0);
																												break;
																											}
																						 case 2 : {
																												/* Next field down */
																												next_field(1);
																												break;
																											}
																					 }
																					 break;
																				 }
					case FIELD_TOGGLE_ULONG_BIT : {
																					switch(load_toggle_ulong_bit()) {
																						case 0 : {
																											 /* Button pressed */
																											 return(result);
																										 }
																						case 1 : {
																											 /* Next field up */
																											 next_field(0);
																											 break;
																										 }
																						case 2 : {
																											 /* Next field down */
																											 next_field(1);
																											 break;
																										 }
																					}
																					break;
																				}
					case FIELD_HHMMSS : {
																switch(load_hhmmss()) {
																	case 0 : {
																						 /* Button pressed */
																						 return(result);
																					 }
																	case 1 : {
																						 /* Next field up */
																						 next_field(0);
																						 break;
																					 }
																	case 2 : {
																						 /* Next field down */
																						 next_field(1);
																						 break;
																					 }
																}
																break;
															}
					case FIELD_MMDDYYYY : {
																	switch(load_mmddyyyy()) {
																		case 0 : {
																							 /* Button pressed */
																							 return(result);
																						 }
																		case 1 : {
																							 /* Next field up */
																							 next_field(0);
																							 break;
																						 }
																		case 2 : {
																							 /* Next field down */
																							 next_field(1);
																							 break;
																						 }
																	}
																	break;
																}
					case FIELD_UPCASE_STRING : {
																			 switch(load_upcase_string()) {
																				 case 0 : {
																										/* Button pressed */
																										return(result);
																									}
																				 case 1 : {
																										/* Next field up */
																										next_field(0);
																										break;
																									}
																				 case 2 : {
																										/* Next field down */
																										next_field(1);
																										break;
																									}
																			 }
																			 break;
																		 }
					case FIELD_FORMAT_STRING : {
																			 switch(load_format_string()) {
																				 case 0 : {
																										/* Button pressed */
																										return(result);
																									}
																				 case 1 : {
																										/* Next field up */
																										next_field(0);
																										break;
																									}
																				 case 2 : {
																										/* Next field down */
																										next_field(1);
																										break;
																									}
																			 }
																			 break;
																		 }
					case FIELD_LISTBOX : {
																 switch(load_listbox()) {
                                   case 0 : {
                                              /* Button pressed */
                                              return(result);
                                            }
                                   case 1 : {
                                              /* Next field up */
                                              next_field(0);
                                              break;
                                            }
                                   case 2 : {
                                              /* Next field down */
                                              next_field(1);
                                              break;
                                            }
                                 }
                                 break;
                               }
					case FIELD_CHECKBOX : {
																	switch(load_checkbox()) {
																		case 0 : {
																							 /* Button pressed */
																							 return(result);
																						 }
																		case 1 : {
																							 /* Next field up */
																							 next_field(0);
																							 break;
																						 }
																		case 2 : {
																							 /* Next field down */
																							 next_field(1);
																							 break;
																						 }
																	}
																	break;
																}
					case FIELD_RADIOBUTTON : {
																		 switch(load_radiobutton()) {
																			 case 0 : {
																									/* Button pressed */
																									return(result);
																								}
																			 case 1 : {
																									/* Next field up */
																									next_field(0);
																									break;
																								}
																			 case 2 : {
																									/* Next field down */
																									next_field(1);
																									break;
																								}
																		 }
																		 break;
																	 }
					case FIELD_4D_ADDR : {
																 switch(load_4d_addr()) {
																	 case 0 : {
																							/* Button pressed */
																							return(result);
																						}
																	 case 1 : {
																							/* Next field up */
																							next_field(0);
																							break;
																						}
																	 case 2 : {
																							/* Next field down */
																							next_field(1);
																							break;
																						}
																 }
																 break;
															 }
				}
			}
			/* Return result */
			return(result);
		}

ushort form_c::get_cur_field(void)
		{
			/* Return the ID of the current entry field */
			return(fields[fields_cur - 1].id);
		}

uchar form_c::load_button(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked

			/* Initialize variables */
			result = 0;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) return(0);
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == CR) {
						button_click(fields_cur);
						DELAY(100);
						button_unclick(fields_cur);
						result = fields[fields_cur - 1].id;
						/* Return button pressed */
						return(0);
					}
						else
					if (ch == TAB) {
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) return(1);
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_toggle_uchar(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked
			field_toggle_uchar_t FAR * field_toggle_uchar;		// Pointer to toggle uchar information

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to checkbox pointer */
			field_toggle_uchar = (field_toggle_uchar_t FAR *) fields[fields_cur - 1].data;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) return(0);
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == ' ') {
						/* Toggle field */
						if (*field_toggle_uchar->t) *field_toggle_uchar->t = 0;
							else *field_toggle_uchar->t = 1;
						/* Redisplay field */
            draw_field(fields_cur, 0, 1, 0);
					}
						else
					if (ch == TAB) {
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) return(1);
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_toggle_uchar_bit(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked
			field_toggle_uchar_bit_t FAR * field_toggle_uchar_bit;		// Pointer to toggle uchar information

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to checkbox pointer */
			field_toggle_uchar_bit = (field_toggle_uchar_bit_t FAR *) fields[fields_cur - 1].data;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) return(0);
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == ' ') {
						/* Toggle field */
						if (*field_toggle_uchar_bit->t & field_toggle_uchar_bit->bit) *field_toggle_uchar_bit->t &= ~field_toggle_uchar_bit->bit;
							else *field_toggle_uchar_bit->t |= field_toggle_uchar_bit->bit;
						/* Redisplay field */
            draw_field(fields_cur, 0, 1, 0);
					}
						else
					if (ch == TAB) {
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) return(1);
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_toggle_ushort_bit(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked
			field_toggle_ushort_bit_t FAR * field_toggle_ushort_bit;		// Pointer to toggle uchar information

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to checkbox pointer */
			field_toggle_ushort_bit = (field_toggle_ushort_bit_t FAR *) fields[fields_cur - 1].data;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) return(0);
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == ' ') {
						/* Toggle field */
						if (*field_toggle_ushort_bit->t & field_toggle_ushort_bit->bit) *field_toggle_ushort_bit->t &= ~field_toggle_ushort_bit->bit;
							else *field_toggle_ushort_bit->t |= field_toggle_ushort_bit->bit;
						/* Redisplay field */
            draw_field(fields_cur, 0, 1, 0);
					}
						else
					if (ch == TAB) {
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) return(1);
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_toggle_ulong_bit(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked
			field_toggle_ulong_bit_t FAR * field_toggle_ulong_bit;		// Pointer to toggle uchar information

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to checkbox pointer */
			field_toggle_ulong_bit = (field_toggle_ulong_bit_t FAR *) fields[fields_cur - 1].data;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) return(0);
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == ' ') {
						/* Toggle field */
						if (*field_toggle_ulong_bit->t & field_toggle_ulong_bit->bit) *field_toggle_ulong_bit->t &= ~field_toggle_ulong_bit->bit;
							else *field_toggle_ulong_bit->t |= field_toggle_ulong_bit->bit;
						/* Redisplay field */
            draw_field(fields_cur, 0, 1, 0);
					}
						else
					if (ch == TAB) {
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) return(1);
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_checkbox(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked
			field_checkbox_t FAR * checkbox;  // Pointer to checkbox information

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to checkbox pointer */
			checkbox = (field_checkbox_t FAR *) fields[fields_cur - 1].data;

			/* Highlight the current entry */
			draw_checkbox_entry(fields_cur, checkbox->cur_entrys, 1);

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Unhighlight the current entry */
							draw_checkbox_entry(cur, checkbox->cur_entrys, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Unhighlight the current entry */
							draw_checkbox_entry(cur, checkbox->cur_entrys, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == ' ') {
						/* Toggle checked */
						if (checkbox->entrys[checkbox->cur_entrys - 1].checked) checkbox->entrys[checkbox->cur_entrys - 1].checked = 0;
							else checkbox->entrys[checkbox->cur_entrys - 1].checked = 1;
						/* Redraw entry */
						draw_checkbox_entry(fields_cur, checkbox->cur_entrys, 1);
					}
						else
					if (ch == TAB) {
						/* Unhighlight the current entry */
						draw_checkbox_entry(fields_cur, checkbox->cur_entrys, 0);
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					switch (ch) {
						case SHIFT_TAB : {
															 /* Unhighlight the current entry */
															 draw_checkbox_entry(fields_cur, checkbox->cur_entrys, 0);
															 return(1);
														 }
						case UPARROW :
						case LEFTARROW : {
															 next_checkbox_entry(fields_cur, 0);
															 break;
														 }
						case DOWNARROW :
						case RIGHTARROW : {
																next_checkbox_entry(fields_cur, 1);
																break;
															}
					}
					/* Check user defined keys */
					if (check_user_def_char(ch, 1) && result) return(0);
				}
			}
		}

uchar form_c::load_listbox(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked

			/* Initialize variables */
			result = 0;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == TAB) {
						return(2);
					}
            else
          if (ch == CR) {
            /* Return string choosen */
            result = fields[fields_cur - 1].id;
            return(0);
          }
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					switch (ch) {
						case SHIFT_TAB : {
															 return(1);
														 }
            case UPARROW : {
                             next_listbox_entry(fields_cur, 0);
                             break;
                           }
            case DOWNARROW : {
                               next_listbox_entry(fields_cur, 1);
                               break;
                             }
            case LEFTARROW : {
                               shift_listbox_entry(fields_cur, 0);
                               break;
                             }
            case RIGHTARROW : {
                                shift_listbox_entry(fields_cur, 1);
                                break;
                              }
          }
					/* Check user defined keys */
					if (check_user_def_char(ch, 1) && result) return(0);
				}
			}
    }

uchar form_c::load_radiobutton(void)
		{
			uchar ch;
			ushort i;  						// General counter
			ushort cur;						// current field before mouse events checked
			field_radiobutton_t FAR * radiobutton;  // Pointer to radiobutton information

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to radiobutton pointer */
			radiobutton = (field_radiobutton_t FAR *) fields[fields_cur - 1].data;

			/* Highlight the current entry */
			draw_radiobutton_entry(fields_cur, radiobutton->cur_entrys, 1);

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					cur = fields_cur;
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Unhighlight the current entry */
							draw_radiobutton_entry(cur, radiobutton->cur_entrys, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Unhighlight the current entry */
							draw_radiobutton_entry(cur, radiobutton->cur_entrys, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					if (ch == TAB) {
						/* Unhighlight the current entry */
						draw_radiobutton_entry(fields_cur, radiobutton->cur_entrys, 0);
						return(2);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 0) && result) return(0);
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					switch (ch) {
						case SHIFT_TAB : {
															 /* Unhighlight the current entry */
															 draw_radiobutton_entry(fields_cur, radiobutton->cur_entrys, 0);
															 return(1);
														 }
						case UPARROW :
						case LEFTARROW : {
															 next_radiobutton_entry(fields_cur, 0);
															 break;
														 }
						case DOWNARROW :
						case RIGHTARROW : {
																next_radiobutton_entry(fields_cur, 1);
																break;
															}
					}
					/* Check user defined keys */
					if (check_user_def_char(ch, 1) && result) return(0);
				}
			}
		}

uchar form_c::load_string(void)
		{
			uchar ch;								// Character read from keyboard
			ushort i;  							// General counter
			ushort cur;							// current field before mouse events checked
			field_string_t FAR * string;	// Pointer to string information
			uchar highlighted = 1;	// True when string is highlighted
			uchar view_pos;					// Position in view window
			uchar view_str_pos;			// Position at front of view window
			uchar str_len;					// Length of string being edited

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			string = (field_string_t FAR *) fields[fields_cur - 1].data;

			/* Save current length of string */
			str_len = strlen(string->str);

			/* See if we need to display the visible part since string is so long */
			if (str_len > string->view_len) {
				view_pos = string->view_len;
				view_str_pos = str_len - string->view_len + 1;
				/* Display lots part of string that will fit in view */
				win->goto_xy(string->entry_x, string->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputctrlch(17);
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(&string->str[view_str_pos - 1]);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputs(" \b");
			}
				else
			{
				/* Display highlighted string */
				view_pos = 1;
				view_str_pos = 1;
				win->goto_xy(string->entry_x, string->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputch(' ');
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(string->str);
			}

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(string->str)) break;
																/* See if we need to remove the arrow */
																if ((strlen(string->str) - 1) == string->view_len) {
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->goto_xy(string->entry_x, string->entry_y);
																	win->wputch(' ');
																	win->goto_xy(string->entry_x + string->view_len, string->entry_y);
																}

																/* If current string length is greater than view point
																	 we will need to redraw string 										   */
																if (str_len > string->view_len) {
																	/* Trim a character off of the string */
																	str_len--;
																	view_str_pos--;
																	string->str[str_len] = 0;
																	/* Redisplay viewable portion */
																	win->goto_xy(string->entry_x, string->entry_y);
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	/* See if we need to display an arrow or space before string */
																	if (str_len > string->view_len) win->wputctrlch(17);
																		else win->wputch(' ');
																	/* Display string */
																	win->wputs(&string->str[view_str_pos - 1]);
																}
																	else
																{
																	/* Trim a character off of the string */
																	str_len--;
																	string->str[str_len] = 0;
																	/* Backspace over deleted character */
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->wputs("\b \b");
																	/* If string was highlighted than we need to redisplay it unhighlighted */
																	if (highlighted) {
																		win->goto_xy(string->entry_x, string->entry_y);
																		win->wputch(' ');
																		win->wputs(string->str);
																	}
																}
																/* Turn off highlighted string */
																highlighted = 0;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													str_len = 0;
													view_pos = 1;
													view_str_pos = 1;
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													strcpy(string->str, "");
													win->goto_xy(string->entry_x, string->entry_y);
													win->width_printf(2 + string->view_len, " ");
													/* Reset cursor position */
													win->goto_xy(string->entry_x + 1, string->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (str_len < string->max_len) {
													/* See if adding this character will make string longer than view */
													if ((str_len + 1) > string->view_len) {
														/* Add character */
														strcatch(string->str, ch);
														str_len++;
														view_str_pos++;
														/* Redisplay viewable portion */
														win->goto_xy(string->entry_x, string->entry_y);
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputctrlch(17);
														win->wputs(&string->str[view_str_pos - 1]);
													}
														else
													{
														/* Add character */
														strcatch(string->str, ch);
														str_len++;
														view_pos++;
														/* Display character */
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputch(ch);
													}
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_control_string(void)
		{
			uchar ch;								// Character read from keyboard
			ushort i;  							// General counter
			ushort cur;							// current field before mouse events checked
			field_control_string_t FAR * control_string;	// Pointer to control_string information
			uchar highlighted = 1;	// True when control_string is highlighted
			uchar view_pos;					// Position in view window
			uchar view_str_pos;			// Position at front of view window
			uchar str_len;					// Length of control_string being edited

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to control_string pointer */
			control_string = (field_control_string_t FAR *) fields[fields_cur - 1].data;

			/* Save current length of control_string */
			str_len = strlen(control_string->str);

			/* See if we need to display the visible part since control_string is so long */
			if (str_len > control_string->view_len) {
				view_pos = control_string->view_len;
				view_str_pos = str_len - control_string->view_len + 1;
				/* Display lots part of control_string that will fit in view */
				win->goto_xy(control_string->entry_x, control_string->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputctrlch(17);
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(&control_string->str[view_str_pos - 1]);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputs(" \b");
			}
				else
			{
				/* Display highlighted control_string */
				view_pos = 1;
				view_str_pos = 1;
				win->goto_xy(control_string->entry_x, control_string->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputch(' ');
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(control_string->str);
			}

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(control_string->str)) break;
																/* See if we need to remove the arrow */
																if ((strlen(control_string->str) - 1) == control_string->view_len) {
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->goto_xy(control_string->entry_x, control_string->entry_y);
																	win->wputch(' ');
																	win->goto_xy(control_string->entry_x + control_string->view_len, control_string->entry_y);
																}

																/* If current control_string length is greater than view point
																	 we will need to redraw control_string 										   */
																if (str_len > control_string->view_len) {
																	/* Trim a character off of the control_string */
																	str_len--;
																	view_str_pos--;
																	control_string->str[str_len] = 0;
																	/* Redisplay viewable portion */
																	win->goto_xy(control_string->entry_x, control_string->entry_y);
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	/* See if we need to display an arrow or space before control_string */
																	if (str_len > control_string->view_len) win->wputctrlch(17);
																		else win->wputch(' ');
																	/* Display control_string */
																	win->wputs(&control_string->str[view_str_pos - 1]);
																}
																	else
																{
																	/* Trim a character off of the control_string */
																	str_len--;
																	control_string->str[str_len] = 0;
																	/* Backspace over deleted character */
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->wputs("\b \b");
																	/* If control_string was highlighted than we need to redisplay it unhighlighted */
																	if (highlighted) {
																		win->goto_xy(control_string->entry_x, control_string->entry_y);
																		win->wputch(' ');
																		win->wputs(control_string->str);
																	}
																}
																/* Turn off highlighted control_string */
																highlighted = 0;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* See if character appears in control string */
												if (strchr((char *) control_string->ctrl_str, (int) ch) == NULL) break;
												/* If control_string is highlighted than erase the existing control_string */
												if (highlighted) {
													/* Redisplay blank control_string */
													str_len = 0;
													view_pos = 1;
													view_str_pos = 1;
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													strcpy(control_string->str, "");
													win->goto_xy(control_string->entry_x, control_string->entry_y);
													win->width_printf(2 + control_string->view_len, " ");
													/* Reset cursor position */
													win->goto_xy(control_string->entry_x + 1, control_string->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (str_len < control_string->max_len) {
													/* See if adding this character will make control_string longer than view */
													if ((str_len + 1) > control_string->view_len) {
														/* Add character */
														strcatch(control_string->str, ch);
														str_len++;
														view_str_pos++;
														/* Redisplay viewable portion */
														win->goto_xy(control_string->entry_x, control_string->entry_y);
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputctrlch(17);
														win->wputs(&control_string->str[view_str_pos - 1]);
													}
														else
													{
														/* Add character */
														strcatch(control_string->str, ch);
														str_len++;
														view_pos++;
														/* Display character */
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputch(ch);
													}
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}


uchar form_c::load_hhmmss(void)
		{
      uchar ch;                           // Character read from keyboard
      ushort i;                           // General counter
      ushort cur;                         // current field before mouse events checked
			field_hhmmss_t FAR * field_hhmmss;	// Pointer to field_hhmmss information
      uchar highlighted = 1;              // True when field_hhmmss is highlighted
      uchar str_len;                      // Length of field_hhmmss being edited
      char temp_str[9];                   // Holds string version of time

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to field_hhmmss pointer */
			field_hhmmss = (field_hhmmss_t FAR *) fields[fields_cur - 1].data;

			/* Convert time to string format */
			time_to_str(*field_hhmmss->t, temp_str);

			/* Save current length of field_hhmmss */
			str_len = strlen(temp_str);

			/* Display highlighted field_hhmmss */
			win->goto_xy(field_hhmmss->entry_x, field_hhmmss->entry_y);
			win->text_color(string_norm_fcolor);
			win->text_background(string_norm_bcolor);
			win->wputch(' ');
			win->text_color(string_high_fcolor);
			win->text_background(string_high_bcolor);
			win->wputs(temp_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Convert time string to number time */
							*field_hhmmss->t = str_to_time(temp_str);
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Convert time string to number time */
							*field_hhmmss->t = str_to_time(temp_str);
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
												 /* Convert time string to number time */
												 *field_hhmmss->t = str_to_time(temp_str);
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!str_len) break;
																/* If field_hhmmss was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->goto_xy(field_hhmmss->entry_x, field_hhmmss->entry_y);
																	win->wputch(' ');
																	win->wputs(temp_str);
																}
																/* Turn off highlighted field_hhmmss */
																highlighted = 0;
																if ((str_len == 3) || (str_len == 6)) str_len--;
																/* Erase character */
																win->goto_xy(field_hhmmss->entry_x + str_len, field_hhmmss->entry_y);
																win->text_color(string_norm_fcolor);
																win->text_background(string_norm_bcolor);
																win->wputs(" \b");
																/* Trim a character off of the field_hhmmss */
																temp_str[str_len - 1] = '0';
																str_len--;
																if ((str_len == 3) || (str_len == 6)) str_len--;
																/* Backspace over deleted character */
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* If field_hhmmss is highlighted than erase the existing field_hhmmss */
												if (highlighted) {
													/* Redisplay blank field_hhmmss */
													str_len = 0;
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													strcpy(temp_str, "00:00:00");
													win->goto_xy(field_hhmmss->entry_x, field_hhmmss->entry_y);
													win->wputs("   :  :  ");
													/* Reset cursor position */
													win->goto_xy(field_hhmmss->entry_x + 1, field_hhmmss->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if ((str_len < 8) && (isdigit(ch))) {
													if ((str_len == 2) || (str_len == 5)) str_len++;
													/* Display character */
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													win->goto_xy(field_hhmmss->entry_x + str_len + 1, field_hhmmss->entry_y);
													win->wputch(ch);
													/* Add character */
													str_len++;
													temp_str[str_len - 1] = ch;
													if ((str_len == 2) || (str_len == 5)) str_len++;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
						/* Convert time string to number time */
						*field_hhmmss->t = str_to_time(temp_str);
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_mmddyyyy(void)
		{
      uchar ch;                               // Character read from keyboard
      ushort i;                               // General counter
      ushort cur;                             // current field before mouse events checked
			field_mmddyyyy_t FAR * field_mmddyyyy;	// Pointer to field_mmddyyyy information
      uchar highlighted = 1;                  // True when field_mmddyyyy is highlighted
      uchar str_len;                          // Length of field_mmddyyyy being edited
      char temp_str[11];                      // Holds string version of time

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to field_mmddyyyy pointer */
			field_mmddyyyy = (field_mmddyyyy_t FAR *) fields[fields_cur - 1].data;

			/* Convert time to string format */
			date_to_str(*field_mmddyyyy->d, temp_str);

			/* Save current length of field_mmddyyyy */
			str_len = strlen(temp_str);

			/* Display highlighted field_mmddyyyy */
			win->goto_xy(field_mmddyyyy->entry_x, field_mmddyyyy->entry_y);
			win->text_color(string_norm_fcolor);
			win->text_background(string_norm_bcolor);
			win->wputch(' ');
			win->text_color(string_high_fcolor);
			win->text_background(string_high_bcolor);
			win->wputs(temp_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Convert date string to number date */
							*field_mmddyyyy->d = str_to_date(temp_str);
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Convert date string to number date */
							*field_mmddyyyy->d = str_to_date(temp_str);
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
												 /* Convert date string to number date */
												 *field_mmddyyyy->d = str_to_date(temp_str);
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!str_len) break;
																/* If field_mmddyyyy was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->goto_xy(field_mmddyyyy->entry_x, field_mmddyyyy->entry_y);
																	win->wputch(' ');
																	win->wputs(temp_str);
																}
																/* Turn off highlighted field_mmddyyyy */
																highlighted = 0;
																if ((str_len == 3) || (str_len == 6)) str_len--;
																/* Erase character */
																win->goto_xy(field_mmddyyyy->entry_x + str_len, field_mmddyyyy->entry_y);
																win->text_color(string_norm_fcolor);
																win->text_background(string_norm_bcolor);
																win->wputch(' ');
																/* Trim a character off of the field_mmddyyyy */
																temp_str[str_len - 1] = '0';
																str_len--;
																if ((str_len == 3) || (str_len == 6)) str_len--;
																/* Backspace over deleted character */
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* If field_mmddyyyy is highlighted than erase the existing field_mmddyyyy */
												if (highlighted) {
													/* Redisplay blank field_mmddyyyy */
													str_len = 0;
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													strcpy(temp_str, "00/00/0000");
													win->goto_xy(field_mmddyyyy->entry_x, field_mmddyyyy->entry_y);
													win->wputs("   /  /    ");
													/* Reset cursor position */
													win->goto_xy(field_mmddyyyy->entry_x + 1, field_mmddyyyy->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if ((str_len < 10) && (isdigit(ch))) {
													if ((str_len == 2) || (str_len == 5)) str_len++;
													/* Display character */
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													win->goto_xy(field_mmddyyyy->entry_x + str_len + 1, field_mmddyyyy->entry_y);
													win->wputch(ch);
													/* Add character */
													str_len++;
													temp_str[str_len - 1] = ch;
													if ((str_len == 2) || (str_len == 5)) str_len++;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
						/* Convert date string to number date */
						*field_mmddyyyy->d = str_to_date(temp_str);
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}


uchar form_c::load_4d_addr(void)
		{
			uchar ch;								// Character read from keyboard
			ushort i;  							// General counter
			ushort cur;							// current field before mouse events checked
			field_faddr_t FAR * field_faddr;	// Pointer to field_faddr information
			uchar highlighted = 1;	// True when field_faddr is highlighted
			uchar view_pos;					// Position in view window
			uchar view_str_pos;			// Position at front of view window
			uchar str_len;					// Length of field_faddr being edited
      char temp_str[24];     // Holds string representation of address
			uchar field;
      uchar field_cnt[4];
			uchar colon;
			uchar slash;
			uchar period;
      char field_str[4][6];
			uchar j;								// General counter
			uchar good_ch;					// Used to see if we should display the character pressed

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to field_faddr pointer */
			field_faddr = (field_faddr_t FAR *) fields[fields_cur - 1].data;

			/* Save current length of field_faddr */
			sprintf(temp_str, "%u:%u/%u.%u", field_faddr->addr->zone, field_faddr->addr->net, field_faddr->addr->node, field_faddr->addr->point);
			str_len = strlen(temp_str);

			field = 3;
			sprintf(field_str[0], "%u", field_faddr->addr->zone);
			sprintf(field_str[1], "%u", field_faddr->addr->net);
			sprintf(field_str[2], "%u", field_faddr->addr->node);
			sprintf(field_str[3], "%u", field_faddr->addr->point);
			for (j = 0; j < 4; j++) field_cnt[j] = strlen(field_str[j]);
			colon = slash = period = 1;

			/* See if we need to display the visible part since field_faddr is so long */
			if (str_len > field_faddr->view_len) {
				view_pos = field_faddr->view_len;
				view_str_pos = str_len - field_faddr->view_len + 1;
				/* Display lots part of field_faddr that will fit in view */
				win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputctrlch(17);
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(&temp_str[view_str_pos - 1]);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputs(" \b");
			}
				else
			{
				/* Display highlighted field_faddr */
				view_pos = 1;
				view_str_pos = 1;
				win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputch(' ');
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(temp_str);
			}

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Convert string version of address back to number format */
							field_faddr->addr->zone = atous(field_str[0]);
							field_faddr->addr->net = atous(field_str[1]);
							field_faddr->addr->node = atous(field_str[2]);
							field_faddr->addr->point = atous(field_str[3]);
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Convert string version of address back to number format */
							field_faddr->addr->zone = atous(field_str[0]);
							field_faddr->addr->net = atous(field_str[1]);
							field_faddr->addr->node = atous(field_str[2]);
							field_faddr->addr->point = atous(field_str[3]);
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
												 /* Convert string version of address back to number format */
												 field_faddr->addr->zone = atous(field_str[0]);
												 field_faddr->addr->net = atous(field_str[1]);
												 field_faddr->addr->node = atous(field_str[2]);
												 field_faddr->addr->point = atous(field_str[3]);
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																if (strlen(temp_str)) {
																	/* See if we need to remove the arrow */
																	if ((strlen(temp_str) - 1) == field_faddr->view_len) {
																		win->text_color(string_norm_fcolor);
																		win->text_background(string_norm_bcolor);
																		win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
																		win->wputch(' ');
																		win->goto_xy(field_faddr->entry_x + field_faddr->view_len, field_faddr->entry_y);
																	}

																	/* Trim off a character */
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
																		field_str[field][strlen(field_str[field]) - 1] = 0;
																	}
																	str_len--;
																	temp_str[str_len] = 0;
																	/* Redisplay address string */
																	if ((str_len + 1) > field_faddr->view_len) {
																		view_str_pos--;
																		/* Redisplay viewable portion */
																		win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
																		win->text_color(string_norm_fcolor);
																		win->text_background(string_norm_bcolor);
																		/* See if we need to display an arrow or space before field_faddr */
																		if (str_len > field_faddr->view_len) win->wputctrlch(17);
																			else win->wputch(' ');
																		/* Display field_faddr */
																		win->wputs(&temp_str[view_str_pos - 1]);
																	}
																		else
																	{
																		/* Backspace over deleted character */
																		win->text_color(string_norm_fcolor);
																		win->text_background(string_norm_bcolor);
																		win->wputs("\b \b");
																		/* If field_faddr was highlighted than we need to redisplay it unhighlighted */
																		if (highlighted) {
																			win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
																			win->wputch(' ');
																			win->wputs(temp_str);
																		}
																	}
																	/* Turn off highlighted string */
																	highlighted = 0;
																}
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* If field_faddr is highlighted than erase the existing field_faddr */
												if (highlighted) {
													/* Redisplay blank field_faddr */
													str_len = 0;
													view_pos = 1;
													view_str_pos = 1;
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													strcpy(temp_str, "");
													field = 0;
													for (j = 0; j < 4; j++) field_cnt[j] = 0;
													strcpy(field_str[0], "");
													strcpy(field_str[1], "");
													strcpy(field_str[2], "");
													strcpy(field_str[3], "");
													colon = slash = period = 0;
													win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
													win->width_printf(2 + field_faddr->view_len, " ");
													/* Reset cursor position */
													win->goto_xy(field_faddr->entry_x + 1, field_faddr->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}

												if ((isdigit(ch)) || (ch == ':') || (ch == '/') || (ch == '.')) {
													good_ch = 0;

													if (isdigit(ch)) {
														if(field_cnt[field] < 5) {
															strcatch(temp_str, ch);
															strcatch(field_str[field], ch);
															field_cnt[field]++;
															str_len++;
															good_ch = 1;
														}
													}
														else
													if (ch == ':') {
														if (!colon) {
															colon = 1;
															field++;
															strcatch(temp_str, ':');
															str_len++;
															good_ch = 1;
														}
													}
														else
													if (ch == '/') {
														if ((!slash) && (colon)) {
															slash = 1;
															field++;
															strcatch(temp_str, '/');
															str_len++;
															good_ch = 1;
														}
													}
														else
													if (ch == '.') {
														if ((!period) && (slash) && (colon)) {
															period = 1;
															field++;
															strcatch(temp_str, '.');
															str_len++;
															good_ch = 1;
														}
													}

													/* Display character */
													if (good_ch) {
														/* See if adding this character will make field_faddr longer than view */
														if (str_len > field_faddr->view_len) {
															view_str_pos++;
															/* Redisplay viewable portion */
															win->goto_xy(field_faddr->entry_x, field_faddr->entry_y);
															win->text_color(string_norm_fcolor);
															win->text_background(string_norm_bcolor);
															win->wputctrlch(17);
															win->wputs(&temp_str[view_str_pos - 1]);
														}
															else
														{
															/* Add character */
															view_pos++;
															/* Display character */
															win->text_color(string_norm_fcolor);
															win->text_background(string_norm_bcolor);
															win->wputch(ch);
														}
													}
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
						/* Convert string version of address back to number format */
						field_faddr->addr->zone = atous(field_str[0]);
						field_faddr->addr->net = atous(field_str[1]);
						field_faddr->addr->node = atous(field_str[2]);
						field_faddr->addr->point = atous(field_str[3]);
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_byte(void)
		{
      uchar ch;                       // Character read from keyboard
      ushort cur;                     // current field before mouse events checked
			field_byte_t FAR * field_byte;	// Pointer to byte information
      uchar highlighted = 1;          // True when string is highlighted
      char num_str[4];                // String that contains number
      ushort i;                       // General counter

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			field_byte = (field_byte_t FAR *) fields[fields_cur - 1].data;

			/* Convert number to a string */
			sprintf(num_str, "%u", *field_byte->num);

			/* Display highlighted string */
			win->goto_xy(field_byte->entry_x, field_byte->entry_y);
			win->text_color(number_norm_fcolor);
			win->text_background(number_norm_bcolor);
			win->wputch(' ');
			win->text_color(number_high_fcolor);
			win->text_background(number_high_bcolor);
			win->wputs(num_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(num_str)) break;
																/* Trim a character off of the string */
																num_str[strlen(num_str) - 1] = 0;
																/* Backspace over deleted character */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->wputs("\b \b");
																/* If string was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->goto_xy(field_byte->entry_x, field_byte->entry_y);
																	win->wputch(' ');
																	win->wputs(num_str);
																}
																/* Turn off highlighted string */
																highlighted = 0;
																/* Update actual number in memory */
																*field_byte->num = (uchar) atous(num_str);
																/* Make sure number is valid */
																if (*field_byte->num < field_byte->min_num) *field_byte->num = field_byte->min_num;
																if (*field_byte->num > field_byte->max_num) *field_byte->num = field_byte->max_num;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Make sure character is a number */
												if (!isdigit(ch)) break;
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													strcpy(num_str, "");
													win->goto_xy(field_byte->entry_x, field_byte->entry_y);
													win->width_printf(2 + field_byte->entry_width, " ");
													/* Reset cursor position */
													win->goto_xy(field_byte->entry_x + 1, field_byte->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (strlen(num_str) < field_byte->entry_width) {
													/* See if the number will be too big if added */
													strcatch(num_str, ch);
													if (atous(num_str) > field_byte->max_num) {
														/* Number would be too big.  Trim off character */
														num_str[strlen(num_str) - 1] = 0;
														break;
													}
													/* Display character */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													win->wputch(ch);

													/* Update number */
													*field_byte->num = (uchar) atous(num_str);
													/* Make sure number is valid */
													if (*field_byte->num < field_byte->min_num) *field_byte->num = field_byte->min_num;
													if (*field_byte->num > field_byte->max_num) *field_byte->num = field_byte->max_num;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_ushort(void)
		{
			uchar ch;								// Character read from keyboard
			ushort cur;							// current field before mouse events checked
			field_ushort_t FAR * field_ushort;	// Pointer to byte information
			uchar highlighted = 1;	// True when string is highlighted
      char num_str[6];       // String that contains number
			ushort i;								// General counter

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			field_ushort = (field_ushort_t FAR *) fields[fields_cur - 1].data;

			/* Convert number to a string */
			sprintf(num_str, "%u", *field_ushort->num);

			/* Display highlighted string */
			win->goto_xy(field_ushort->entry_x, field_ushort->entry_y);
			win->text_color(number_norm_fcolor);
			win->text_background(number_norm_bcolor);
			win->wputch(' ');
			win->text_color(number_high_fcolor);
			win->text_background(number_high_bcolor);
			win->wputs(num_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(num_str)) break;
																/* Trim a character off of the string */
																num_str[strlen(num_str) - 1] = 0;
																/* Backspace over deleted character */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->wputs("\b \b");
																/* If string was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->goto_xy(field_ushort->entry_x, field_ushort->entry_y);
																	win->wputch(' ');
																	win->wputs(num_str);
																}
																/* Turn off highlighted string */
																highlighted = 0;
																/* Update actual number in memory */
																*field_ushort->num = atous(num_str);
																/* Make sure number is valid */
																if (*field_ushort->num < field_ushort->min_num) *field_ushort->num = field_ushort->min_num;
																if (*field_ushort->num > field_ushort->max_num) *field_ushort->num = field_ushort->max_num;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Make sure character is a number */
												if (!isdigit(ch)) break;
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													strcpy(num_str, "");
													win->goto_xy(field_ushort->entry_x, field_ushort->entry_y);
													win->width_printf(2 + field_ushort->entry_width, " ");
													/* Reset cursor position */
													win->goto_xy(field_ushort->entry_x + 1, field_ushort->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (strlen(num_str) < field_ushort->entry_width) {
													/* See if the number will be too big if added */
													strcatch(num_str, ch);
													if (atoul(num_str) > field_ushort->max_num) {
														/* Number would be too big.  Trim off character */
														num_str[strlen(num_str) - 1] = 0;
														break;
													}
													/* Display character */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													win->wputch(ch);

													/* Update number */
													*field_ushort->num = atous(num_str);
													/* Make sure number is valid */
													if (*field_ushort->num < field_ushort->min_num) *field_ushort->num = field_ushort->min_num;
													if (*field_ushort->num > field_ushort->max_num) *field_ushort->num = field_ushort->max_num;

												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_ulong(void)
		{
			uchar ch;								// Character read from keyboard
			ushort cur;							// current field before mouse events checked
			field_ulong_t FAR * field_ulong;	// Pointer to byte information
			uchar highlighted = 1;	// True when string is highlighted
      char num_str[11];        // String that contains number
			ushort i;								// General counter

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			field_ulong = (field_ulong_t FAR *) fields[fields_cur - 1].data;

			/* Convert number to a string */
			sprintf(num_str, "%lu", *field_ulong->num);

			/* Display highlighted string */
			win->goto_xy(field_ulong->entry_x, field_ulong->entry_y);
			win->text_color(number_norm_fcolor);
			win->text_background(number_norm_bcolor);
			win->wputch(' ');
			win->text_color(number_high_fcolor);
			win->text_background(number_high_bcolor);
			win->wputs(num_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(num_str)) break;
																/* Trim a character off of the string */
																num_str[strlen(num_str) - 1] = 0;
																/* Backspace over deleted character */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->wputs("\b \b");
																/* If string was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->goto_xy(field_ulong->entry_x, field_ulong->entry_y);
																	win->wputch(' ');
																	win->wputs(num_str);
																}
																/* Turn off highlighted string */
																highlighted = 0;
																/* Update actual number in memory */
																*field_ulong->num = atoul(num_str);
																/* Make sure number is valid */
																if (*field_ulong->num < field_ulong->min_num) *field_ulong->num = field_ulong->min_num;
																if (*field_ulong->num > field_ulong->max_num) *field_ulong->num = field_ulong->max_num;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Make sure character is a number */
												if (!isdigit(ch)) break;
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													strcpy(num_str, "");
													win->goto_xy(field_ulong->entry_x, field_ulong->entry_y);
													win->width_printf(2 + field_ulong->entry_width, " ");
													/* Reset cursor position */
													win->goto_xy(field_ulong->entry_x + 1, field_ulong->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (strlen(num_str) < field_ulong->entry_width) {
													/* See if the number will be too big if added */
													strcatch(num_str, ch);
													if (atous(num_str) > field_ulong->max_num) {
														/* Number would be too big.  Trim off character */
														num_str[strlen(num_str) - 1] = 0;
														break;
													}
													/* Display character */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													win->wputch(ch);

													/* Update number */
													*field_ulong->num = atoul(num_str);

													/* Make sure number is valid */
													if (*field_ulong->num < field_ulong->min_num) *field_ulong->num = field_ulong->min_num;
													if (*field_ulong->num > field_ulong->max_num) *field_ulong->num = field_ulong->max_num;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
          if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_char(void)
		{
			uchar ch;								// Character read from keyboard
			ushort cur;							// current field before mouse events checked
			field_char_t FAR * field_char;	// Pointer to byte information
			uchar highlighted = 1;	// True when string is highlighted
      char num_str[5];       // String that contains number
			ushort i;								// General counter
			uchar negative = 0;			// True if number is currently negative

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			field_char = (field_char_t FAR *) fields[fields_cur - 1].data;

			/* Convert number to a string */
			sprintf(num_str, "%d", *field_char->num);
			if (*field_char->num < 0) negative = 1;

			/* Display highlighted string */
			win->goto_xy(field_char->entry_x, field_char->entry_y);
			win->text_color(number_norm_fcolor);
			win->text_background(number_norm_bcolor);
			win->wputch(' ');
			win->text_color(number_high_fcolor);
			win->text_background(number_high_bcolor);
			win->wputs(num_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(num_str)) break;
																/* Trim a character off of the string */
																num_str[strlen(num_str) - 1] = 0;
																/* Backspace over deleted character */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->wputs("\b \b");
																/* If string was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->goto_xy(field_char->entry_x, field_char->entry_y);
																	win->wputch(' ');
																	win->wputs(num_str);
																}
																/* Turn off highlighted string */
																highlighted = 0;
																/* Update actual number in memory */
																*field_char->num = (uchar) atoi(num_str);
																/* Make sure number is valid */
																if (*field_char->num < field_char->min_num) *field_char->num = field_char->min_num;
																if (*field_char->num > field_char->max_num) *field_char->num = field_char->max_num;
																/* Check for negative number */
																if (*field_char->num < 0) negative = 1;
																	else negative = 0;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Make sure character is a number */
												if (!isdigit(ch) && (ch != '-')) break;
												if ((ch == '-') && strlen(num_str) && !highlighted) break;
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													strcpy(num_str, "");
													win->goto_xy(field_char->entry_x, field_char->entry_y);
													win->width_printf(2 + field_char->entry_width, " ");
													/* Reset cursor position */
													win->goto_xy(field_char->entry_x + 1, field_char->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (strlen(num_str) < (field_char->entry_width + negative)) {
													/* See if the number will be too big if added */
													strcatch(num_str, ch);
													if (atoi(num_str) > field_char->max_num) {
														/* Number would be too big.  Trim off character */
														num_str[strlen(num_str) - 1] = 0;
														break;
													}
													/* Display character */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													win->wputch(ch);

													/* Update number */
													*field_char->num = (uchar) atoi(num_str);
													/* Make sure number is valid */
													if (*field_char->num < field_char->min_num) *field_char->num = field_char->min_num;
													if (*field_char->num > field_char->max_num) *field_char->num = field_char->max_num;
													/* Check for negative number */
													if (*field_char->num < 0) negative = 1;
														else negative = 0;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_int(void)
		{
			uchar ch;								// Character read from keyboard
			ushort cur;							// current field before mouse events checked
			field_int_t FAR * field_int;	// Pointer to byte information
			uchar highlighted = 1;	// True when string is highlighted
      char num_str[7];       // String that contains number
			ushort i;								// General counter
			uchar negative = 0;			// True if number is currently negative

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			field_int = (field_int_t FAR *) fields[fields_cur - 1].data;

			/* Convert number to a string */
			sprintf(num_str, "%d", *field_int->num);
			if (*field_int->num < 0) negative = 1;

			/* Display highlighted string */
			win->goto_xy(field_int->entry_x, field_int->entry_y);
			win->text_color(number_norm_fcolor);
			win->text_background(number_norm_bcolor);
			win->wputch(' ');
			win->text_color(number_high_fcolor);
			win->text_background(number_high_bcolor);
			win->wputs(num_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(num_str)) break;
																/* Trim a character off of the string */
																num_str[strlen(num_str) - 1] = 0;
																/* Backspace over deleted character */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->wputs("\b \b");
																/* If string was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->goto_xy(field_int->entry_x, field_int->entry_y);
																	win->wputch(' ');
																	win->wputs(num_str);
																}
																/* Turn off highlighted string */
																highlighted = 0;
																/* Update actual number in memory */
																*field_int->num = (uchar) atoi(num_str);
																/* Make sure number is valid */
																if (*field_int->num < field_int->min_num) *field_int->num = field_int->min_num;
																if (*field_int->num > field_int->max_num) *field_int->num = field_int->max_num;
																/* Check for negative number */
																if (*field_int->num < 0) negative = 1;
																	else negative = 0;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Make sure character is a number */
												if (!isdigit(ch) && (ch != '-')) break;
												if ((ch == '-') && strlen(num_str) && !highlighted) break;
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													strcpy(num_str, "");
													win->goto_xy(field_int->entry_x, field_int->entry_y);
													win->width_printf(2 + field_int->entry_width, " ");
													/* Reset cursor position */
													win->goto_xy(field_int->entry_x + 1, field_int->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (strlen(num_str) < (field_int->entry_width + negative)) {
													/* See if the number will be too big if added */
													strcatch(num_str, ch);
													if (atoi(num_str) > field_int->max_num) {
														/* Number would be too big.  Trim off character */
														num_str[strlen(num_str) - 1] = 0;
														break;
													}
													/* Display character */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													win->wputch(ch);

													/* Update number */
													*field_int->num = (uchar) atoi(num_str);
													/* Make sure number is valid */
													if (*field_int->num < field_int->min_num) *field_int->num = field_int->min_num;
													if (*field_int->num > field_int->max_num) *field_int->num = field_int->max_num;
													/* Check for negative number */
													if (*field_int->num < 0) negative = 1;
														else negative = 0;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_long(void)
		{
			uchar ch;								// Character read from keyboard
			ushort cur;							// current field before mouse events checked
			field_long_t FAR * field_long;	// Pointer to byte information
			uchar highlighted = 1;	// True when string is highlighted
      char num_str[12];      // String that contains number
			ushort i;								// General counter
			uchar negative = 0;			// True if number is currently negative

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			field_long = (field_long_t FAR *) fields[fields_cur - 1].data;

			/* Convert number to a string */
			sprintf(num_str, "%d", *field_long->num);
			if (*field_long->num < 0) negative = 1;

			/* Display highlighted string */
			win->goto_xy(field_long->entry_x, field_long->entry_y);
			win->text_color(number_norm_fcolor);
			win->text_background(number_norm_bcolor);
			win->wputch(' ');
			win->text_color(number_high_fcolor);
			win->text_background(number_high_bcolor);
			win->wputs(num_str);

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(num_str)) break;
																/* Trim a character off of the string */
																num_str[strlen(num_str) - 1] = 0;
																/* Backspace over deleted character */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->wputs("\b \b");
																/* If string was highlighted than we need to redisplay it unhighlighted */
																if (highlighted) {
																	win->goto_xy(field_long->entry_x, field_long->entry_y);
																	win->wputch(' ');
																	win->wputs(num_str);
																}
																/* Turn off highlighted string */
																highlighted = 0;
																/* Update actual number in memory */
																*field_long->num = (uchar) atol(num_str);
																/* Make sure number is valid */
																if (*field_long->num < field_long->min_num) *field_long->num = field_long->min_num;
																if (*field_long->num > field_long->max_num) *field_long->num = field_long->max_num;
																/* Check for negative number */
																if (*field_long->num < 0) negative = 1;
																	else negative = 0;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Make sure character is a number */
												if (!isdigit(ch) && (ch != '-')) break;
												if ((ch == '-') && strlen(num_str) && !highlighted) break;
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													strcpy(num_str, "");
													win->goto_xy(field_long->entry_x, field_long->entry_y);
													win->width_printf(2 + field_long->entry_width, " ");
													/* Reset cursor position */
													win->goto_xy(field_long->entry_x + 1, field_long->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (strlen(num_str) < (field_long->entry_width + negative)) {
													/* See if the number will be too big if added */
													strcatch(num_str, ch);
													if (atol(num_str) > field_long->max_num) {
														/* Number would be too big.  Trim off character */
														num_str[strlen(num_str) - 1] = 0;
														break;
													}
													/* Display character */
													win->text_color(number_norm_fcolor);
													win->text_background(number_norm_bcolor);
													win->wputch(ch);

													/* Update number */
													*field_long->num = (uchar) atol(num_str);
													/* Make sure number is valid */
													if (*field_long->num < field_long->min_num) *field_long->num = field_long->min_num;
													if (*field_long->num > field_long->max_num) *field_long->num = field_long->max_num;
													/* Check for negative number */
													if (*field_long->num < 0) negative = 1;
														else negative = 0;
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}


uchar form_c::load_upcase_string(void)
		{
			uchar ch;								// Character read from keyboard
			ushort i;  							// General counter
			ushort cur;							// current field before mouse events checked
			field_string_t FAR * string;	// Pointer to string information
			uchar highlighted = 1;	// True when string is highlighted
			uchar view_pos;					// Position in view window
			uchar view_str_pos;			// Position at front of view window
			uchar str_len;					// Length of string being edited

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			string = (field_string_t FAR *) fields[fields_cur - 1].data;

			/* Save current length of string */
			str_len = strlen(string->str);

			/* See if we need to display the visible part since string is so long */
			if (str_len > string->view_len) {
				view_pos = string->view_len;
				view_str_pos = str_len - string->view_len + 1;
				/* Display lots part of string that will fit in view */
				win->goto_xy(string->entry_x, string->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputctrlch(17);
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(&string->str[view_str_pos - 1]);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputs(" \b");
			}
				else
			{
				/* Display highlighted string */
				view_pos = 1;
				view_str_pos = 1;
				win->goto_xy(string->entry_x, string->entry_y);
				win->text_color(string_norm_fcolor);
				win->text_background(string_norm_bcolor);
				win->wputch(' ');
				win->text_color(string_high_fcolor);
				win->text_background(string_high_bcolor);
				win->wputs(string->str);
			}

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																/* See if there are any characters to delete */
																if (!strlen(string->str)) break;
																/* See if we need to remove the arrow */
																if ((strlen(string->str) - 1) == string->view_len) {
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->goto_xy(string->entry_x, string->entry_y);
																	win->wputch(' ');
																	win->goto_xy(string->entry_x + string->view_len, string->entry_y);
																}

																/* If current string length is greater than view point
																	 we will need to redraw string 										   */
																if (str_len > string->view_len) {
																	/* Trim a character off of the string */
																	str_len--;
																	view_str_pos--;
																	string->str[str_len] = 0;
																	/* Redisplay viewable portion */
																	win->goto_xy(string->entry_x, string->entry_y);
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	/* See if we need to display an arrow or space before string */
																	if (str_len > string->view_len) win->wputctrlch(17);
																		else win->wputch(' ');
																	/* Display string */
																	win->wputs(&string->str[view_str_pos - 1]);
																}
																	else
																{
																	/* Trim a character off of the string */
																	str_len--;
																	string->str[str_len] = 0;
																	/* Backspace over deleted character */
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->wputs("\b \b");
																	/* If string was highlighted than we need to redisplay it unhighlighted */
																	if (highlighted) {
																		win->goto_xy(string->entry_x, string->entry_y);
																		win->wputch(' ');
																		win->wputs(string->str);
																	}
																}
																/* Turn off highlighted string */
																highlighted = 0;
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Convert character to upcase */
												ch = toupper(ch);
												/* If string is highlighted than erase the existing string */
												if (highlighted) {
													/* Redisplay blank string */
													str_len = 0;
													view_pos = 1;
													view_str_pos = 1;
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													strcpy(string->str, "");
													win->goto_xy(string->entry_x, string->entry_y);
													win->width_printf(2 + string->view_len, " ");
													/* Reset cursor position */
													win->goto_xy(string->entry_x + 1, string->entry_y);
													/* Toggle highlighted */
													highlighted = 0;
												}
												/* See if there is enough room for another character */
												if (str_len < string->max_len) {
													/* See if adding this character will make string longer than view */
													if ((str_len + 1) > string->view_len) {
														/* Add character */
														strcatch(string->str, ch);
														str_len++;
														view_str_pos++;
														/* Redisplay viewable portion */
														win->goto_xy(string->entry_x, string->entry_y);
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputctrlch(17);
														win->wputs(&string->str[view_str_pos - 1]);
													}
														else
													{
														/* Add character */
														strcatch(string->str, ch);
														str_len++;
														view_pos++;
														/* Display character */
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputch(ch);
													}
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

uchar form_c::load_format_string(void)
		{
			uchar ch;								// Character read from keyboard
			ushort i;  							// General counter
			ushort cur;							// current field before mouse events checked
			field_frmt_string_t FAR * frmt_string;	// Pointer to format string information
			uchar highlighted = 1;	// True if string is highlighted

			/* Initialize variables */
			result = 0;

			/* Convert void data pointer to string pointer */
			frmt_string = (field_frmt_string_t FAR *) fields[fields_cur - 1].data;

			/* Store current field in case they click on another one with the mouse */
			cur = fields_cur;

			/* Display string highlighted */
			win->text_color(string_norm_fcolor);
			win->text_background(string_norm_bcolor);
			win->goto_xy(frmt_string->entry_x, frmt_string->entry_y);
			win->wputch(' ');
			win->text_color(string_high_fcolor);
			win->text_background(string_high_bcolor);
			win->wputs(frmt_string->str);
			win->text_color(string_norm_fcolor);
			win->text_background(string_norm_bcolor);
			win->wputch(' ');

			/* Loop and get characters and then process */
			for (;;) {
				/* Check mouse and keyboard for input */
				for (;;) {
					/* Check Keyboard */
          if (KBHIT()) {
            ch = GETCH();
						break;
					}
					/* Check mouse */
					i = field_clicked();
					if (i) {
						/* See if a button was clicked */
						if (result) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							return(0);
						}
						/* See if the field clicked is different from this field */
						if (i != cur) {
							/* Redraw field */
              draw_field(cur, 0, 1, 0);
							/* Return another field chosen */
							return(4);
						}
					}
					/* Call time slice function */
					timeslice();
				}
				/* Check for defined characters first then enter */
				if (ch != 0) {		// Not an extended character
					switch (ch) {
						case CR :
						case TAB : {
                         draw_field(cur, 0, 1, 0);
												 return(2);
											 }
						case ESC : {
												 break;
											 }
						case BACK_SPACE : {
																if (frmt_string->curr_pos > frmt_string->min_pos) {
																	/* Find next position to the left */
																	frmt_string->curr_pos--;
																	while(((frmt_string->ctrl_str[frmt_string->curr_pos - 1] != '#') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'A') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'a')) && (frmt_string->curr_pos > frmt_string->min_pos)) {
																		frmt_string->curr_pos--;
																	}
																	/* Goto the input position */
																	win->goto_xy(frmt_string->entry_x+frmt_string->curr_pos, frmt_string->entry_y);
																	/* Erase character in string */
																	frmt_string->str[frmt_string->curr_pos - 1] = ' ';
																	/* Erase the character */
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->wputs(" \b");
																	/* Redraw string if it was highlighted */
																	if (highlighted) {
																		win->goto_xy(frmt_string->entry_x + 1, frmt_string->entry_y);
																		win->text_color(string_norm_fcolor);
																		win->text_background(string_norm_bcolor);
																		win->wputs(frmt_string->str);
																		highlighted = 0;
																	}
																}
																break;
															}
						default : {
												/* Check user defined keys */
												if (check_user_def_char(ch, 0) && result) return(0);
												/* Blank string if highlighted and redisplay */
												if (highlighted) {

													strcpy(frmt_string->str, "");
													/* Fill string with correct characters from ctrl_str */
													strpad(frmt_string->str, frmt_string->max_len, ' ');
													for (i = 0; i < frmt_string->max_len; i++) {
														if ((frmt_string->ctrl_str[i] == '#') || (frmt_string->ctrl_str[i] == 'A') || (frmt_string->ctrl_str[i] == 'a')) {
															if (isalpha(frmt_string->ctrl_str[i])) {
																if (frmt_string->ctrl_str[i] == 'A') frmt_string->str[i] = toupper(frmt_string->str[i]);
																		else frmt_string->str[i] = tolower(frmt_string->str[i]);
															}
														}
															else frmt_string->str[i] = frmt_string->ctrl_str[i];
													}
													frmt_string->curr_pos = frmt_string->min_pos;
													/* Redraw string */
													win->goto_xy(frmt_string->entry_x + 1, frmt_string->entry_y);
													win->text_color(string_norm_fcolor);
													win->text_background(string_norm_bcolor);
													win->wputs(frmt_string->str);
													highlighted = 0;
												}

												if (frmt_string->curr_pos <= frmt_string->max_pos) {
													if (((frmt_string->ctrl_str[frmt_string->curr_pos - 1] == '#') && (isdigit(ch))) || ((frmt_string->ctrl_str[frmt_string->curr_pos - 1] == 'A') && (isalpha(ch))) || ((frmt_string->ctrl_str[frmt_string->curr_pos - 1] == 'a') && (isalpha(ch))) ) {
														/* Change case of the character if it supposed to be */
														if(isalpha(ch)) {
															if (frmt_string->ctrl_str[frmt_string->curr_pos - 1] == 'A') ch = toupper(ch);
																else ch = tolower(ch);
														}
														/* print character */
														win->goto_xy(frmt_string->entry_x+frmt_string->curr_pos, frmt_string->entry_y);
														win->text_color(string_norm_fcolor);
														win->text_background(string_norm_bcolor);
														win->wputch(ch);
														/* Update string */
														frmt_string->str[frmt_string->curr_pos - 1] = ch;
														/* Find next curr_pos */
														if (frmt_string->curr_pos < frmt_string->max_pos) {
															frmt_string->curr_pos++;
															while(((frmt_string->ctrl_str[frmt_string->curr_pos - 1] != '#') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'A') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'a')) && (frmt_string->curr_pos <= frmt_string->max_pos)) {
																frmt_string->curr_pos++;
															}
														}
															else frmt_string->curr_pos = (frmt_string->max_pos + 1);
														/* Goto new position */
														win->goto_xy(frmt_string->entry_x+frmt_string->curr_pos, frmt_string->entry_y);
													}
												}
												break;
											}
					}
				}
					else
				/* Check for defined extended characters than arrow keys */
				{
          ch = GETCH();
					/* Check for shift tab */
					if (ch == SHIFT_TAB) {
            draw_field(cur, 0, 1, 0);
						return(1);
					}
						else
					{
						/* Check user defined keys */
						if (check_user_def_char(ch, 1) && result) return(0);
					}
				}
			}
		}

