/* Filename : FORM1.CPP                                                     *
 * Description : Form module #1 for TI/2                                    *
 * Target : FORM1.OBJ -> TI2FORM.LIB or TI2.DLL                             *
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

/********************
 * Form Global Data *
 ********************/

/******************
 * Form Functions *
 ******************/

void form_c::button_click(ushort field_num)
		{
			field_button_t FAR * button; // Pointer to button field information

			/* Convert void data pointer to button pointer */
			button = (field_button_t FAR *) fields[field_num - 1].data;
			/* See if button is already down */
			if (button->down) return;
			/* Toggle button down */
			button->down = 1;
			/* Redraw button */
      draw_field(field_num, 1, 0, 0);
		}

void form_c::button_unclick(ushort field_num)
		{
			field_button_t FAR * button; // Pointer to button field information

			/* Convert void data pointer to button pointer */
			button = (field_button_t FAR *) fields[field_num - 1].data;
			/* See if button is already down */
			if (!button->down) return;
			/* Toggle button down */
			button->down = 0;
			/* Redraw button */
      draw_field(field_num, 0, 0, 0);
		}

void form_c::draw_field(ushort field_num, uchar button_down, uchar draw_data, uchar nondisplay)
		{
			uchar j, l;							// General counter
			field_ptr_t ptr;				// Pointers to fields information
      char temp_str[24];      // Used to convert fidonet address and others to string for display

			/* Draw field */
			switch(fields[field_num - 1].type) {
				case FIELD_BUTTON : {
															/* Convert void field pointer to button pointer */
															ptr.button = (field_button_t FAR *) fields[field_num - 1].data;
															/* Draw button in correct position */
															if (button_down) {
																/* Clear area where button is */
																win->text_color(button_shadow_fcolor);
																win->text_background(button_shadow_bcolor);
																win->goto_xy(ptr.button->x, ptr.button->y);
                                if (nondisplay) win->nd_putch(' ');
                                  else win->wputch(' ');
																win->goto_xy(ptr.button->x + 1, ptr.button->y + 1);
																j = strlen(ptr.button->title) + 2;
                                for (l = 0; l < j; l++) {
                                  if (nondisplay) win->nd_putch(' ');
                                    else win->wputch(' ');
                                }
																/* Set correct color for button */
																if (field_num == fields_cur) {
																	win->text_color(button_high_fcolor);
																	win->text_background(button_high_bcolor);
																}
																	else
																{
																	win->text_color(button_norm_fcolor);
																	win->text_background(button_norm_bcolor);
																}
																if (!fields[field_num - 1].active) {
																	win->text_color(button_inactive_fcolor);
																	win->text_background(button_inactive_bcolor);
																}
																/* Draw button */
																win->goto_xy(ptr.button->x + 1, ptr.button->y);
                                if (nondisplay) win->nd_printf(" %s ", ptr.button->title);
                                  else win->wprintf(" %s ", ptr.button->title);
																ptr.button->down = 1;
															}
																else
															{
																/* Set correct color for button */
																if (field_num == fields_cur) {
																	win->text_color(button_high_fcolor);
																	win->text_background(button_high_bcolor);
																}
																	else
																{
																	win->text_color(button_norm_fcolor);
																	win->text_background(button_norm_bcolor);
																}
																if (!fields[field_num - 1].active) {
																	win->text_color(button_inactive_fcolor);
																	win->text_background(button_inactive_bcolor);
																}
																/* Draw button */
																win->goto_xy(ptr.button->x, ptr.button->y);
                                if (nondisplay) win->nd_printf(" %s ", ptr.button->title);
                                  else win->wprintf(" %s ", ptr.button->title);
																/* Draw shadow */
																win->text_color(button_shadow_fcolor);
																win->text_background(button_shadow_bcolor);
																j = strlen(ptr.button->title) + 2;
                                if (nondisplay) win->nd_putch('Ü');
                                  else win->wputch('Ü');
																win->goto_xy(ptr.button->x + 1, ptr.button->y + 1);
                                for (l = 0; l < j; l++) {
																	if (nondisplay) win->nd_putch('ß');
                                    else win->wputch('ß');
                                }
																ptr.button->down = 0;
															}
															break;
														}
				case FIELD_BYTE : {
														/* Convert void field pointer to format string pointer */
														ptr.field_byte = (field_byte_t FAR *) fields[field_num - 1].data;
														/* Set correct color for field title */
														if (field_num == fields_cur) {
															win->text_color(field_high_fcolor);
															win->text_background(field_high_bcolor);
														}
															else
														{
															win->text_color(field_norm_fcolor);
															win->text_background(field_norm_bcolor);
														}
														/* Set color to inactive if need be */
														if (!fields[field_num - 1].active) {
															win->text_color(field_inactive_fcolor);
															win->text_background(field_inactive_bcolor);
														}
														/* Draw field title */
														win->goto_xy(ptr.field_byte->title_x, ptr.field_byte->title_y);
                            if (nondisplay) win->nd_puts(ptr.field_byte->title);
                              else win->wputs(ptr.field_byte->title);
														/* Draw data if need */
														if (draw_data) {
															/* Display byte */
															win->text_color(number_norm_fcolor);
															win->text_background(number_norm_bcolor);
															win->goto_xy(ptr.field_byte->entry_x, ptr.field_byte->entry_y);
                              if (nondisplay) {
																win->nd_putch(' ');
                                win->nd_width_printf(ptr.field_byte->entry_width, "%u", *ptr.field_byte->num);
                                win->nd_putch(' ');
                              }
                                else
                              {
                                win->wputch(' ');
                                win->width_printf(ptr.field_byte->entry_width, "%u", *ptr.field_byte->num);
                                win->wputch(' ');
                              }
                            }
														break;
													}
				case FIELD_USHORT : {
															/* Convert void field pointer to byte pointer */
															ptr.field_ushort = (field_ushort_t FAR *) fields[field_num - 1].data;
															/* Set correct color for field title */
															if (field_num == fields_cur) {
																win->text_color(field_high_fcolor);
																win->text_background(field_high_bcolor);
															}
																else
															{
																win->text_color(field_norm_fcolor);
																win->text_background(field_norm_bcolor);
															}
															/* Set color to inactive if need be */
															if (!fields[field_num - 1].active) {
																win->text_color(field_inactive_fcolor);
																win->text_background(field_inactive_bcolor);
																}
															/* Draw field title */
															win->goto_xy(ptr.field_ushort->title_x, ptr.field_ushort->title_y);
                              if (nondisplay) win->nd_puts(ptr.field_ushort->title);
                                else win->wputs(ptr.field_ushort->title);
															/* Draw data if need */
															if (draw_data) {
																/* Display byte */
																win->text_color(number_norm_fcolor);
																win->text_background(number_norm_bcolor);
																win->goto_xy(ptr.field_ushort->entry_x, ptr.field_ushort->entry_y);
                                if (nondisplay) {
                                  win->nd_putch(' ');
                                  win->nd_width_printf(ptr.field_ushort->entry_width, "%u", *ptr.field_ushort->num);
                                  win->nd_putch(' ');
                                }
                                  else
                                {
                                  win->wputch(' ');
                                  win->width_printf(ptr.field_ushort->entry_width, "%u", *ptr.field_ushort->num);
                                  win->wputch(' ');
                                }
                              }
															break;
														}
				case FIELD_ULONG : {
														 /* Convert void field pointer to ulong pointer */
														 ptr.field_ulong = (field_ulong_t FAR *) fields[field_num - 1].data;
														 /* Set correct color for field title */
														 if (field_num == fields_cur) {
															 win->text_color(field_high_fcolor);
															 win->text_background(field_high_bcolor);
														 }
															else
														 {
															 win->text_color(field_norm_fcolor);
															 win->text_background(field_norm_bcolor);
														 }
														 /* Set color to inactive if need be */
														 if (!fields[field_num - 1].active) {
															 win->text_color(field_inactive_fcolor);
															 win->text_background(field_inactive_bcolor);
														 }
														 /* Draw field title */
														 win->goto_xy(ptr.field_ulong->title_x, ptr.field_ulong->title_y);
                             if (nondisplay) win->nd_puts(ptr.field_ulong->title);
                               else win->wputs(ptr.field_ulong->title);
														 /* Draw data if need */
														 if (draw_data) {
															 /* Display byte */
															 win->text_color(number_norm_fcolor);
															 win->text_background(number_norm_bcolor);
															 win->goto_xy(ptr.field_ulong->entry_x, ptr.field_ulong->entry_y);
                               if (nondisplay) {
                                 win->wputch(' ');
                                 win->width_printf(ptr.field_ulong->entry_width, "%lu", *ptr.field_ulong->num);
                                 win->wputch(' ');
                               }
                                 else
                               {
																 win->nd_putch(' ');
                                 win->nd_width_printf(ptr.field_ulong->entry_width, "%lu", *ptr.field_ulong->num);
                                 win->nd_putch(' ');
                               }
                             }
														 break;
													 }
				case FIELD_CHAR : {
														/* Convert void field pointer to format string pointer */
														ptr.field_char = (field_char_t FAR *) fields[field_num - 1].data;
														/* Set correct color for field title */
														if (field_num == fields_cur) {
															win->text_color(field_high_fcolor);
															win->text_background(field_high_bcolor);
														}
															else
														{
															win->text_color(field_norm_fcolor);
															win->text_background(field_norm_bcolor);
														}
														/* Set color to inactive if need be */
														if (!fields[field_num - 1].active) {
															win->text_color(field_inactive_fcolor);
															win->text_background(field_inactive_bcolor);
														}
														/* Draw field title */
														win->goto_xy(ptr.field_char->title_x, ptr.field_char->title_y);
                            if (nondisplay) win->nd_puts(ptr.field_char->title);
                              else win->wputs(ptr.field_char->title);
														/* Draw data if need */
														if (draw_data) {
															/* Display byte */
															win->text_color(number_norm_fcolor);
															win->text_background(number_norm_bcolor);
															win->goto_xy(ptr.field_char->entry_x, ptr.field_char->entry_y);
                              if (nondisplay) {
																win->nd_putch(' ');
                                win->nd_width_printf(ptr.field_char->entry_width + ((ptr.field_char->min_num < 0) ? 1 : 0), "%d", *ptr.field_char->num);
                                win->nd_putch(' ');
                              }
                                else
                              {
                                win->wputch(' ');
                                win->width_printf(ptr.field_char->entry_width + ((ptr.field_char->min_num < 0) ? 1 : 0), "%d", *ptr.field_char->num);
                                win->wputch(' ');
                              }
														}
														break;
													}
				case FIELD_INT : {
													 /* Convert void field pointer to format string pointer */
													 ptr.field_int = (field_int_t FAR *) fields[field_num - 1].data;
													 /* Set correct color for field title */
													 if (field_num == fields_cur) {
														 win->text_color(field_high_fcolor);
														 win->text_background(field_high_bcolor);
													 }
														 else
													 {
														 win->text_color(field_norm_fcolor);
														 win->text_background(field_norm_bcolor);
													 }
													 /* Set color to inactive if need be */
													 if (!fields[field_num - 1].active) {
														 win->text_color(field_inactive_fcolor);
														 win->text_background(field_inactive_bcolor);
													 }
													 /* Draw field title */
													 win->goto_xy(ptr.field_int->title_x, ptr.field_int->title_y);
                           if (nondisplay) win->nd_puts(ptr.field_int->title);
                             else win->wputs(ptr.field_int->title);
													 /* Draw data if need */
													 if (draw_data) {
														 /* Display byte */
														 win->text_color(number_norm_fcolor);
														 win->text_background(number_norm_bcolor);
														 win->goto_xy(ptr.field_int->entry_x, ptr.field_int->entry_y);
                             if (nondisplay) {
                               win->nd_putch(' ');
                               win->nd_width_printf(ptr.field_int->entry_width + ((ptr.field_int->min_num < 0) ? 1 : 0), "%d", *ptr.field_int->num);
                               win->nd_putch(' ');
                             }
                               else
                             {
                               win->wputch(' ');
                               win->width_printf(ptr.field_int->entry_width + ((ptr.field_int->min_num < 0) ? 1 : 0), "%d", *ptr.field_int->num);
                               win->wputch(' ');
                             }
													 }
													 break;
												 }
				case FIELD_LONG : {
														/* Convert void field pointer to format string pointer */
														ptr.field_long = (field_long_t FAR *) fields[field_num - 1].data;
														/* Set correct color for field title */
														if (field_num == fields_cur) {
															win->text_color(field_high_fcolor);
															win->text_background(field_high_bcolor);
														}
															else
														{
															win->text_color(field_norm_fcolor);
															win->text_background(field_norm_bcolor);
														}
														/* Set color to inactive if need be */
														if (!fields[field_num - 1].active) {
															win->text_color(field_inactive_fcolor);
															win->text_background(field_inactive_bcolor);
														}
														/* Draw field title */
														win->goto_xy(ptr.field_long->title_x, ptr.field_long->title_y);
                            if (nondisplay) win->nd_puts(ptr.field_long->title);
                              else win->wputs(ptr.field_long->title);
														/* Draw data if need */
														if (draw_data) {
															/* Display byte */
															win->text_color(number_norm_fcolor);
															win->text_background(number_norm_bcolor);
															win->goto_xy(ptr.field_long->entry_x, ptr.field_long->entry_y);
                              if (nondisplay) {
                                win->nd_putch(' ');
                                win->nd_width_printf(ptr.field_long->entry_width + ((ptr.field_long->min_num < 0) ? 1 : 0), "%ld", *ptr.field_long->num);
                                win->nd_putch(' ');
                              }
                                else
                              {
																win->wputch(' ');
                                win->width_printf(ptr.field_long->entry_width + ((ptr.field_long->min_num < 0) ? 1 : 0), "%ld", *ptr.field_long->num);
                                win->wputch(' ');
                              }
														}
														break;
													}
				case FIELD_UPCASE_STRING :
				case FIELD_STRING : {
															/* Convert void field pointer to string pointer */
															ptr.string = (field_string_t FAR *) fields[field_num - 1].data;
															/* Set correct color for field title */
															if (field_num == fields_cur) {
																win->text_color(field_high_fcolor);
																win->text_background(field_high_bcolor);
															}
																else
															{
																win->text_color(field_norm_fcolor);
																win->text_background(field_norm_bcolor);
															}
															/* Set color to inactive if need be */
															if (!fields[field_num - 1].active) {
																win->text_color(field_inactive_fcolor);
																win->text_background(field_inactive_bcolor);
															}
															/* Draw field title */
															win->goto_xy(ptr.string->title_x, ptr.string->title_y);
                              if (nondisplay) win->nd_puts(ptr.string->title);
                                else win->wputs(ptr.string->title);
															/* Draw data if need */
															if (draw_data) {
																/* Display view area and viewable part of string */
																win->text_color(string_norm_fcolor);
																win->text_background(string_norm_bcolor);
																win->goto_xy(ptr.string->entry_x, ptr.string->entry_y);
																if (nondisplay) {
                                  win->nd_putch(' ');
                                  win->nd_width_puts(ptr.string->view_len, ptr.string->str);
                                  /* Display arrow after string if string length is greater than view */
                                  if (strlen(ptr.string->str) > ptr.string->view_len) win->nd_putctrlch(16);
                                    else win->nd_putch(' ');
                                }
                                  else
                                {
                                  win->wputch(' ');
                                  win->width_puts(ptr.string->view_len, ptr.string->str);
                                  /* Display arrow after string if string length is greater than view */
                                  if (strlen(ptr.string->str) > ptr.string->view_len) win->wputctrlch(16);
                                    else win->wputch(' ');
                                }
															}
															break;
														}
				case FIELD_CONTROL_STRING : {
																			/* Convert void field pointer to control_string pointer */
																			ptr.control_string = (field_control_string_t FAR *) fields[field_num - 1].data;
																			/* Set correct color for field title */
																			if (field_num == fields_cur) {
																				win->text_color(field_high_fcolor);
																				win->text_background(field_high_bcolor);
																			}
																				else
																			{
																				win->text_color(field_norm_fcolor);
																				win->text_background(field_norm_bcolor);
																			}
																			/* Set color to inactive if need be */
																			if (!fields[field_num - 1].active) {
																				win->text_color(field_inactive_fcolor);
																				win->text_background(field_inactive_bcolor);
																			}
																			/* Draw field title */
																			win->goto_xy(ptr.control_string->title_x, ptr.control_string->title_y);
                                      if (nondisplay) win->nd_puts(ptr.control_string->title);
                                        else win->wputs(ptr.control_string->title);
																			/* Draw data if need */
																			if (draw_data) {
																				/* Display view area and viewable part of control_string */
																				win->text_color(string_norm_fcolor);
																				win->text_background(string_norm_bcolor);
																				win->goto_xy(ptr.control_string->entry_x, ptr.control_string->entry_y);
                                        if (nondisplay) {
                                          win->nd_putch(' ');
                                          win->nd_width_puts(ptr.control_string->view_len, ptr.control_string->str);
                                          /* Display arrow after control_string if control_string length is greater than view */
                                          if (strlen(ptr.control_string->str) > ptr.control_string->view_len) win->nd_putctrlch(16);
                                            else win->nd_putch(' ');
                                        }
                                          else
																				{
                                          win->wputch(' ');
                                          win->width_puts(ptr.control_string->view_len, ptr.control_string->str);
                                          /* Display arrow after control_string if control_string length is greater than view */
                                          if (strlen(ptr.control_string->str) > ptr.control_string->view_len) win->wputctrlch(16);
                                            else win->wputch(' ');
                                        }
																			}
																			break;
																		}
				case FIELD_TOGGLE_UCHAR : {
																		/* Convert void field pointer to field_toggle_uchar pointer */
																		ptr.field_toggle_uchar = (field_toggle_uchar_t FAR *) fields[field_num - 1].data;
																		/* Set correct color for field title */
																		if (field_num == fields_cur) {
																			win->text_color(field_high_fcolor);
																			win->text_background(field_high_bcolor);
																		}
																			else
																		{
																			win->text_color(field_norm_fcolor);
																			win->text_background(field_norm_bcolor);
																		}
																		/* Set color to inactive if need be */
																		if (!fields[field_num - 1].active) {
																			win->text_color(field_inactive_fcolor);
																			win->text_background(field_inactive_bcolor);
																		}
																		/* Draw field title */
																		win->goto_xy(ptr.field_toggle_uchar->title_x, ptr.field_toggle_uchar->title_y);
                                    if (nondisplay) win->nd_puts(ptr.field_toggle_uchar->title);
                                      else win->wputs(ptr.field_toggle_uchar->title);
																		/* Draw data if need */
																		if (draw_data) {
																			/* Display view area and viewable part of field_toggle_uchar */
																			win->text_color(string_norm_fcolor);
																			win->text_background(string_norm_bcolor);
																			win->goto_xy(ptr.field_toggle_uchar->entry_x, ptr.field_toggle_uchar->entry_y);
                                      if (nondisplay) {
                                        win->nd_putch(' ');
                                        if (*ptr.field_toggle_uchar->t) win->nd_puts("Yes");
                                          else win->nd_puts("No ");
                                        win->nd_putch(' ');
                                      }
                                        else
                                      { 
                                        win->wputch(' ');
                                        if (*ptr.field_toggle_uchar->t) win->wputs("Yes");
                                          else win->wputs("No ");
                                        win->wputch(' ');
                                      }
																		}
																		break;
																	}
				case FIELD_TOGGLE_USHORT_BIT : {
																				 /* Convert void field pointer to field_toggle_uchar pointer */
																				 ptr.field_toggle_ushort_bit = (field_toggle_ushort_bit_t FAR *) fields[field_num - 1].data;
																				 /* Set correct color for field title */
																				 if (field_num == fields_cur) {
																					 win->text_color(field_high_fcolor);
																					 win->text_background(field_high_bcolor);
																				 }
																					 else
																				 {
																					 win->text_color(field_norm_fcolor);
																					 win->text_background(field_norm_bcolor);
																				 }
																				 /* Set color to inactive if need be */
																				 if (!fields[field_num - 1].active) {
																					 win->text_color(field_inactive_fcolor);
																					 win->text_background(field_inactive_bcolor);
																				 }
																				 /* Draw field title */
																				 win->goto_xy(ptr.field_toggle_ushort_bit->title_x, ptr.field_toggle_ushort_bit->title_y);
                                         if (nondisplay) win->nd_puts(ptr.field_toggle_ushort_bit->title);
                                           else win->wputs(ptr.field_toggle_ushort_bit->title);
																				 /* Draw data if need */
																				 if (draw_data) {
																					 /* Display view area and viewable part of field_toggle_ushort_bit */
																					 win->text_color(string_norm_fcolor);
																					 win->text_background(string_norm_bcolor);
																					 win->goto_xy(ptr.field_toggle_ushort_bit->entry_x, ptr.field_toggle_ushort_bit->entry_y);
                                           if (nondisplay) {
                                             win->nd_putch(' ');
                                             if (*ptr.field_toggle_ushort_bit->t & ptr.field_toggle_ushort_bit->bit) win->nd_puts("Yes");
                                               else win->nd_puts("No ");
                                             win->nd_putch(' ');
                                           }
                                             else
                                           {
																						 win->wputch(' ');
                                             if (*ptr.field_toggle_ushort_bit->t & ptr.field_toggle_ushort_bit->bit) win->wputs("Yes");
                                               else win->wputs("No ");
                                             win->wputch(' ');
                                           }
																				 }
																				 break;
																			 }
				case FIELD_TOGGLE_UCHAR_BIT : {
																				/* Convert void field pointer to field_toggle_uchar pointer */
																				ptr.field_toggle_uchar_bit = (field_toggle_uchar_bit_t FAR *) fields[field_num - 1].data;
																				/* Set correct color for field title */
																				if (field_num == fields_cur) {
																					win->text_color(field_high_fcolor);
																					win->text_background(field_high_bcolor);
																				}
																					else
																				{
																					win->text_color(field_norm_fcolor);
																					win->text_background(field_norm_bcolor);
																				}
																				/* Set color to inactive if need be */
																				if (!fields[field_num - 1].active) {
																					win->text_color(field_inactive_fcolor);
																					win->text_background(field_inactive_bcolor);
																				}
																				/* Draw field title */
																				win->goto_xy(ptr.field_toggle_uchar_bit->title_x, ptr.field_toggle_uchar_bit->title_y);
                                        if (nondisplay) win->nd_puts(ptr.field_toggle_uchar_bit->title);
                                          else win->wputs(ptr.field_toggle_uchar_bit->title);
																				/* Draw data if need */
																				if (draw_data) {
																					/* Display view area and viewable part of field_toggle_uchar_bit */
																					win->text_color(string_norm_fcolor);
																					win->text_background(string_norm_bcolor);
																					win->goto_xy(ptr.field_toggle_uchar_bit->entry_x, ptr.field_toggle_uchar_bit->entry_y);
																					if (nondisplay) {
                                            win->nd_putch(' ');
                                            if (*ptr.field_toggle_uchar_bit->t & ptr.field_toggle_uchar_bit->bit) win->nd_puts("Yes");
                                              else win->nd_puts("No ");
                                            win->nd_putch(' ');
                                          }
                                            else
                                          {
                                            win->wputch(' ');
                                            if (*ptr.field_toggle_uchar_bit->t & ptr.field_toggle_uchar_bit->bit) win->wputs("Yes");
                                              else win->wputs("No ");
                                            win->wputch(' ');
                                          }
																				}
																				break;
																			}
				case FIELD_TOGGLE_ULONG_BIT : {
																				/* Convert void field pointer to field_toggle_uchar pointer */
																				ptr.field_toggle_ulong_bit = (field_toggle_ulong_bit_t FAR *) fields[field_num - 1].data;
																				/* Set correct color for field title */
																				if (field_num == fields_cur) {
																					win->text_color(field_high_fcolor);
																					win->text_background(field_high_bcolor);
																				}
																					else
																				{
																					win->text_color(field_norm_fcolor);
																					win->text_background(field_norm_bcolor);
																				}
																				/* Set color to inactive if need be */
																				if (!fields[field_num - 1].active) {
																					win->text_color(field_inactive_fcolor);
																					win->text_background(field_inactive_bcolor);
																				}
																				/* Draw field title */
																				win->goto_xy(ptr.field_toggle_ulong_bit->title_x, ptr.field_toggle_ulong_bit->title_y);
																				if (nondisplay) win->nd_puts(ptr.field_toggle_ulong_bit->title);
                                          else win->wputs(ptr.field_toggle_ulong_bit->title);
																				/* Draw data if need */
																				if (draw_data) {
																					/* Display view area and viewable part of field_toggle_ulong_bit */
																					win->text_color(string_norm_fcolor);
																					win->text_background(string_norm_bcolor);
																					win->goto_xy(ptr.field_toggle_ulong_bit->entry_x, ptr.field_toggle_ulong_bit->entry_y);
                                          if (nondisplay) {
                                            win->nd_putch(' ');
                                            if (*ptr.field_toggle_ulong_bit->t & ptr.field_toggle_ulong_bit->bit) win->nd_puts("Yes");
                                              else win->nd_puts("No ");
                                            win->nd_putch(' ');
                                          }
                                            else
                                          {
                                            win->wputch(' ');
                                            if (*ptr.field_toggle_ulong_bit->t & ptr.field_toggle_ulong_bit->bit) win->wputs("Yes");
																							else win->wputs("No ");
                                            win->wputch(' ');
                                          }
																				}
																				break;
																			}
				case FIELD_HHMMSS : {
															/* Convert void field pointer to hhmmss pointer */
															ptr.field_hhmmss = (field_hhmmss_t FAR *) fields[field_num - 1].data;
															/* Set correct color for field title */
															if (field_num == fields_cur) {
																win->text_color(field_high_fcolor);
																win->text_background(field_high_bcolor);
															}
																else
															{
																win->text_color(field_norm_fcolor);
																win->text_background(field_norm_bcolor);
															}
															/* Set color to inactive if need be */
															if (!fields[field_num - 1].active) {
																win->text_color(field_inactive_fcolor);
																win->text_background(field_inactive_bcolor);
															}
															/* Draw field title */
															win->goto_xy(ptr.field_hhmmss->title_x, ptr.field_hhmmss->title_y);
                              if (nondisplay) win->nd_puts(ptr.field_hhmmss->title);
                                else win->wputs(ptr.field_hhmmss->title);
															/* Draw data if need */
															if (draw_data) {
																/* Display view area and viewable part of string */
																win->text_color(string_norm_fcolor);
																win->text_background(string_norm_bcolor);
																win->goto_xy(ptr.field_hhmmss->entry_x, ptr.field_hhmmss->entry_y);
																time_to_str(*ptr.field_hhmmss->t, temp_str);
                                if (nondisplay) {
																	win->nd_putch(' ');
                                  win->nd_puts(temp_str);
                                  win->nd_putch(' ');
                                }
                                  else
                                {
                                  win->wputch(' ');
                                  win->wputs(temp_str);
                                  win->wputch(' ');
                                }
															}
															break;
														}
				case FIELD_MMDDYYYY : {
																/* Convert void field pointer to mmddyyyy pointer */
																ptr.field_mmddyyyy = (field_mmddyyyy_t FAR *) fields[field_num - 1].data;
																/* Set correct color for field title */
																if (field_num == fields_cur) {
																	win->text_color(field_high_fcolor);
																	win->text_background(field_high_bcolor);
																}
																	else
																{
																	win->text_color(field_norm_fcolor);
																	win->text_background(field_norm_bcolor);
																}
																/* Set color to inactive if need be */
																if (!fields[field_num - 1].active) {
																	win->text_color(field_inactive_fcolor);
																	win->text_background(field_inactive_bcolor);
																}
																/* Draw field title */
																win->goto_xy(ptr.field_mmddyyyy->title_x, ptr.field_mmddyyyy->title_y);
                                if (nondisplay) win->nd_puts(ptr.field_mmddyyyy->title);
                                win->wputs(ptr.field_mmddyyyy->title);
																/* Draw data if need */
																if (draw_data) {
																	/* Display view area and viewable part of string */
																	win->text_color(string_norm_fcolor);
																	win->text_background(string_norm_bcolor);
																	win->goto_xy(ptr.field_mmddyyyy->entry_x, ptr.field_mmddyyyy->entry_y);
																	date_to_str(*ptr.field_mmddyyyy->d, temp_str);
                                  if (nondisplay) {
                                    win->nd_putch(' ');
                                    win->nd_puts(temp_str);
                                    win->nd_putch(' ');
                                  }
                                    else
                                  {
                                    win->wputch(' ');
                                    win->wputs(temp_str);
                                    win->wputch(' ');
                                  }
																}
																break;
															}
				case FIELD_4D_ADDR : {
															 /* Convert void field pointer to field_faddr pointer */
															 ptr.field_faddr = (field_faddr_t FAR *) fields[field_num - 1].data;
															 /* Set correct color for field title */
															 if (field_num == fields_cur) {
																 win->text_color(field_high_fcolor);
																 win->text_background(field_high_bcolor);
															 }
																 else
															 {
																 win->text_color(field_norm_fcolor);
																 win->text_background(field_norm_bcolor);
															 }
															 /* Set color to inactive if need be */
															 if (!fields[field_num - 1].active) {
																 win->text_color(field_inactive_fcolor);
																 win->text_background(field_inactive_bcolor);
															 }
															 /* Draw field title */
															 win->goto_xy(ptr.field_faddr->title_x, ptr.field_faddr->title_y);
                               if (nondisplay) win->nd_puts(ptr.field_faddr->title);
                                 else win->wputs(ptr.field_faddr->title);
															 /* Draw data if need */
															 if (draw_data) {
																 /* Convert address to string */
																 sprintf(temp_str, "%u:%u/%u.%u", ptr.field_faddr->addr->zone, ptr.field_faddr->addr->net, ptr.field_faddr->addr->node, ptr.field_faddr->addr->point);
																 /* Display view area and viewable part of field_faddr */
																 win->text_color(string_norm_fcolor);
																 win->text_background(string_norm_bcolor);
																 win->goto_xy(ptr.field_faddr->entry_x, ptr.field_faddr->entry_y);
                                 if (nondisplay) {
                                   win->nd_putch(' ');
                                   win->nd_width_puts(ptr.field_faddr->view_len, temp_str);
                                   /* Display arrow after field_faddr if field_faddr length is greater than view */
																	 if (strlen(temp_str) > ptr.field_faddr->view_len) win->nd_putctrlch(16);
                                     else win->nd_putch(' ');
                                 }
                                   else
                                 {
                                   win->wputch(' ');
                                   win->width_puts(ptr.field_faddr->view_len, temp_str);
                                   /* Display arrow after field_faddr if field_faddr length is greater than view */
                                   if (strlen(temp_str) > ptr.field_faddr->view_len) win->wputctrlch(16);
                                     else win->wputch(' ');
                                 }
															 }
															 break;
														 }
				case FIELD_FORMAT_STRING : {
																		 /* Convert void field pointer to format string pointer */
																		 ptr.frmt_string = (field_frmt_string_t FAR *) fields[field_num - 1].data;
																		 /* Set correct color for field title */
																		 if (field_num == fields_cur) {
																			 win->text_color(field_high_fcolor);
																			 win->text_background(field_high_bcolor);
																		 }
																			 else
																		 {
																			 win->text_color(field_norm_fcolor);
																			 win->text_background(field_norm_bcolor);
																		 }
																		 /* Set color to inactive if need be */
																		 if (!fields[field_num - 1].active) {
																			 win->text_color(field_inactive_fcolor);
																			 win->text_background(field_inactive_bcolor);
																		 }
																		 /* Draw field title */
																		 win->goto_xy(ptr.frmt_string->title_x, ptr.frmt_string->title_y);
                                     if (nondisplay) win->nd_puts(ptr.frmt_string->title);
                                       else win->wputs(ptr.frmt_string->title);
																		 /* Draw data if need */
																		 if (draw_data) {
																			 /* Display view area and viewable part of frmt_string */
																			 win->text_color(string_norm_fcolor);
																			 win->text_background(string_norm_bcolor);
																			 win->goto_xy(ptr.frmt_string->entry_x, ptr.frmt_string->entry_y);
                                       if (nondisplay) {
                                         win->nd_putch(' ');
                                         win->nd_puts(ptr.frmt_string->str);
                                         win->nd_putch(' ');
                                       }
                                         else
                                       {
                                         win->wputch(' ');
                                         win->wputs(ptr.frmt_string->str);
                                         win->wputch(' ');
                                       }
																		 }
																		 break;
																	 }
				case FIELD_CHECKBOX : {
																/* Convert void field pointer to checkbox pointer */
																ptr.checkbox = (field_checkbox_t FAR *) fields[field_num - 1].data;
																/* Set correct color for field title */
																if (field_num == fields_cur) {
																	win->text_color(field_high_fcolor);
																	win->text_background(field_high_bcolor);
																}
																	else
																{
																	win->text_color(field_norm_fcolor);
																	win->text_background(field_norm_bcolor);
																}
																/* Set color to inactive if need be */
																if (!fields[field_num - 1].active) {
																	win->text_color(field_inactive_fcolor);
																	win->text_background(field_inactive_bcolor);
																}
																/* Draw field title */
																win->goto_xy(ptr.checkbox->title_x, ptr.checkbox->title_y);
                                if (nondisplay) win->nd_puts(ptr.checkbox->title);
                                  else win->wputs(ptr.checkbox->title);
																/* Draw data if need */
																if (draw_data) {
																	/* Display box */
																	win->text_background(checkbox_box_bcolor);
                                  if (nondisplay) win->nd_clr_region(ptr.checkbox->boxtopx, ptr.checkbox->boxtopy, ptr.checkbox->boxbotx, ptr.checkbox->boxboty);
                                    else win->clr_region(ptr.checkbox->boxtopx, ptr.checkbox->boxtopy, ptr.checkbox->boxbotx, ptr.checkbox->boxboty);
																	/* Display all checkbox entries */
																	for (j = 0; j < ptr.checkbox->num_entrys; j++) {
																		/* Set correct color */
																		if (((j + 1) == ptr.checkbox->cur_entrys) && (field_num == fields_cur)) {
																			win->text_color(checkbox_high_fcolor);
																			win->text_background(checkbox_high_bcolor);
																		}
																			else
																		{
																			win->text_color(checkbox_norm_fcolor);
																			win->text_background(checkbox_norm_bcolor);
																		}
																		/* Display entry */
																		win->goto_xy(ptr.checkbox->boxtopx + ptr.checkbox->entrys[j].x - 1, ptr.checkbox->boxtopy + ptr.checkbox->entrys[j].y - 1);
                                    if (nondisplay) {
                                      if (ptr.checkbox->entrys[j].checked) win->nd_puts("[X] ");
                                        else win->nd_puts("[ ] ");
                                      win->nd_puts(ptr.checkbox->entrys[j].title);
                                    }
                                      else
                                    {
                                      if (ptr.checkbox->entrys[j].checked) win->wputs("[X] ");
                                        else win->wputs("[ ] ");
                                      win->wputs(ptr.checkbox->entrys[j].title);
																		}
																	}
																}
																break;
															}
				case FIELD_LISTBOX : {
															 /* Convert void field pointer to listbox pointer */
															 ptr.listbox = (field_listbox_t FAR *) fields[field_num - 1].data;
															 /* Set correct color for field title */
															 if (field_num == fields_cur) {
																 win->text_color(field_high_fcolor);
																 win->text_background(field_high_bcolor);
															 }
																 else
															 {
																 win->text_color(field_norm_fcolor);
																 win->text_background(field_norm_bcolor);
															 }
															 /* Set color to inactive if need be */
															 if (!fields[field_num - 1].active) {
																 win->text_color(field_inactive_fcolor);
																 win->text_background(field_inactive_bcolor);
															 }
															 /* Draw field title */
															 win->goto_xy(ptr.listbox->title_x, ptr.listbox->title_y);
                               if (nondisplay) win->nd_puts(ptr.listbox->title);
                                 else win->wputs(ptr.listbox->title);
															 /* Draw data if need */
															 if (draw_data) {
																 /* Display box */
																 win->text_background(listbox_box_bcolor);
                                 if (nondisplay) win->nd_clr_region(ptr.listbox->boxtopx, ptr.listbox->boxtopy, ptr.listbox->boxbotx, ptr.listbox->boxboty);
                                   else win->clr_region(ptr.listbox->boxtopx, ptr.listbox->boxtopy, ptr.listbox->boxbotx, ptr.listbox->boxboty);
																 /* Display viewable listbox entries */
																 for (j = (ptr.listbox->top_entrys - 1); j < min(ptr.listbox->num_entrys, (ushort) (ptr.listbox->boxboty - ptr.listbox->boxtopy + ptr.listbox->top_entrys)); j++) {
																	 /* Set correct color */
																	 if ((j + 1) == ptr.listbox->cur_entrys) {
																		 win->text_color(listbox_high_fcolor);
																		 win->text_background(listbox_high_bcolor);
																	 }
																		 else
																	 {
																		 win->text_color(listbox_norm_fcolor);
																		 win->text_background(listbox_norm_bcolor);
																	 }
																	 /* Display entry */
                                   win->goto_xy(ptr.listbox->boxtopx, ptr.listbox->boxtopy  + (j - ptr.listbox->top_entrys + 1));
                                   if (nondisplay) win->nd_width_puts((ptr.listbox->boxbotx - ptr.listbox->boxtopx + 1),ptr.listbox->entrys[j].str);
                                     else win->width_puts((ptr.listbox->boxbotx - ptr.listbox->boxtopx + 1),ptr.listbox->entrys[j].str);
																 }
                                 /* Draw vertical scroll bar */
                                 draw_listbox_vscroll(field_num, nondisplay);
                                 draw_listbox_hscroll(field_num, nondisplay);
															 }
															 break;
														 }
				case FIELD_RADIOBUTTON : {
																	 /* Convert void field pointer to radiobutton pointer */
																	 ptr.radiobutton = (field_radiobutton_t FAR *) fields[field_num - 1].data;
																	 /* Set correct color for field title */
																	 if (field_num == fields_cur) {
																		 win->text_color(field_high_fcolor);
																		 win->text_background(field_high_bcolor);
																	 }
																		 else
																	 {
																		 win->text_color(field_norm_fcolor);
																		 win->text_background(field_norm_bcolor);
																	 }
																	 /* Set color to inactive if need be */
																	 if (!fields[field_num - 1].active) {
																		 win->text_color(field_inactive_fcolor);
																		 win->text_background(field_inactive_bcolor);
																	 }
																	 /* Draw field title */
																	 win->goto_xy(ptr.radiobutton->title_x, ptr.radiobutton->title_y);
                                   if (nondisplay) win->nd_puts(ptr.radiobutton->title);
                                     else win->wputs(ptr.radiobutton->title);
																	 /* Draw data if need */
																	 if (draw_data) {
																		 /* Display box */
																		 win->text_background(radiobutton_box_bcolor);
																		 win->clr_region(ptr.radiobutton->boxtopx, ptr.radiobutton->boxtopy, ptr.radiobutton->boxbotx, ptr.radiobutton->boxboty);
																		 /* Display all radiobutton entries */
																		 for (j = 0; j < ptr.radiobutton->num_entrys; j++) {
																			 /* Set correct color */
																			 if (((j + 1) == ptr.radiobutton->cur_entrys) && (field_num == fields_cur)) {
																				 win->text_color(radiobutton_high_fcolor);
																				 win->text_background(radiobutton_high_bcolor);
																			 }
																				 else
																			 {
																				 win->text_color(radiobutton_norm_fcolor);
																				 win->text_background(radiobutton_norm_bcolor);
																			 }
																			 /* Display entry */
																			 win->goto_xy(ptr.radiobutton->boxtopx + ptr.radiobutton->entrys[j].x - 1, ptr.radiobutton->boxtopy + ptr.radiobutton->entrys[j].y - 1);
                                       if (nondisplay) {
                                         if ((j + 1) == ptr.radiobutton->cur_entrys) {
                                           win->nd_putch('(');
                                           win->nd_putctrlch(7);
                                           win->nd_puts(") ");
                                         }
                                           else win->nd_puts("( ) ");
                                         win->nd_puts(ptr.radiobutton->entrys[j].title);
                                       }
                                         else
                                       {
																				 if ((j + 1) == ptr.radiobutton->cur_entrys) {
                                           win->wputch('(');
                                           win->wputctrlch(7);
                                           win->wputs(") ");
                                         }
                                           else win->wputs("( ) ");
                                         win->wputs(ptr.radiobutton->entrys[j].title);
                                       }
																		 }
																	 }
																	 break;
																 }
			}
		}

void form_c::draw_listbox_vscroll(ushort field_num, uchar nondisplay)
		{
      field_listbox_t FAR * listbox;  // Pointer to listbox information
      uchar i;                        // General counter
      uchar bar_height;
      uchar bar_pos;

      /* Convert void field pointer to listbox pointer */
      listbox = (field_listbox_t FAR *) fields[field_num - 1].data;

      /* Make sure we have a vertical scroll bar */
      if (!listbox->vscroll) return;

      /* Calculate bar height */
      bar_height = listbox->boxboty - listbox->boxtopy + 1 - 2;

      bar_pos = (int)((float)bar_height * ((float)listbox->cur_entrys / (float) (listbox->num_entrys ? listbox->num_entrys : 1)));
			if (bar_pos < 1) bar_pos = 1;
      if (listbox->cur_entrys == 1) bar_pos = 1;

      /* Set scroll bar colors */
      win->text_color(listbox_scroll_fcolor);
      win->text_background(listbox_scroll_bcolor);

      /* Draw vertical scroll bar */
      win->goto_xy(listbox->boxbotx + 1, listbox->boxtopy);
      if (nondisplay) win->nd_putctrlch(24);
        else win->wputctrlch(24);
			for(i = 0;i < bar_height; i++) {
        win->goto_xy(listbox->boxbotx + 1, listbox->boxtopy + 1 + i);
				/* Draw normal dotted box, unless it is the current position then draw the square */
				if (bar_pos == (i+1)) {
					/* Bar position indicator */
          if (nondisplay) win->nd_putch('þ');
            else win->wputch('þ');
				}
					else
				{
					/* Bar */
          if (nondisplay) win->nd_putch('°');
            else win->wputch('°');
				}
			}
      win->goto_xy(listbox->boxbotx + 1, listbox->boxboty);
      if (nondisplay) win->nd_putctrlch(25);
        else win->wputctrlch(25);
    }

void form_c::draw_listbox_hscroll(ushort field_num, uchar nondisplay)
    {
      field_listbox_t FAR * listbox;  // Pointer to listbox information
      uchar i;                        // General counter
      uchar bar_width;
      uchar bar_pos;

			/* Convert void field pointer to listbox pointer */
      listbox = (field_listbox_t FAR *) fields[field_num - 1].data;

      /* Make sure we have a vertical scroll bar */
      if (!listbox->hscroll) return;

      /* Calculate bar height */
      bar_width = listbox->boxbotx - listbox->boxtopx + 1 - 2;

      bar_pos = (int)((float)bar_width * ((float)listbox->hscroll_pos / (float) ((listbox->longest_entry ? listbox->longest_entry : 1) - bar_width + 1)));
			if (bar_pos < 1) bar_pos = 1;
      if (listbox->hscroll_pos == 1) bar_pos = 1;

      /* Set scroll bar colors */
      win->text_color(listbox_scroll_fcolor);
      win->text_background(listbox_scroll_bcolor);

      /* Draw vertical scroll bar */
			win->goto_xy(listbox->boxtopx, listbox->boxboty + 1);
      if (nondisplay) win->nd_putctrlch(17);
        else win->wputctrlch(17);
      for(i = 0; i < bar_width; i++) {
        win->goto_xy(listbox->boxtopx + 1 + i, listbox->boxboty + 1);
				/* Draw normal dotted box, unless it is the current position then draw the square */
				if (bar_pos == (i+1)) {
					/* Bar position indicator */
          if (nondisplay) win->nd_putch('þ');
            else win->wputch('þ');
				}
					else
				{
					/* Bar */
          if (nondisplay) win->nd_putch('°');
          win->wputch('°');
				}
			}
			win->goto_xy(listbox->boxbotx, listbox->boxboty + 1);
			if (nondisplay) win->nd_putctrlch(16);
				else win->wputctrlch(16);
		}

ushort form_c::check_user_def_char(uchar ch, uchar ext)
		{
			ushort i;		// General counters

			/* Check all user defined keys for a match.  If one is found return the id */
			for (i = 0; i < chars_max; i++) {
				if ((chars[i].ch == ch) && (chars[i].extended == ext)) {
					result = chars[i].id;
					return(1);
				}
			}

			/* Unsuccessful */
			return(0);
		}

void form_c::next_field(uchar direction) // 0 = Up, 1 = Down
		{
			int i;
			ushort last_field;

			// Save last field for comparison to see if we need a redraw
			last_field = fields_cur;

			/* Search for next field */
			if (!direction) {  /* Search up */
				/* Search for current field up */
				for (i = fields_cur; i > 0; i--) {
					if ((fields[i-1].active) && (last_field == fields_cur)) {
						fields_cur = i;
					}
				}
				/* Search from bottom to current field */
				for (i = fields_max; i > fields_cur; i--) {
					if ((fields[i-1].active) && (last_field == fields_cur)) {
						fields_cur = i;
					}
				}
			}
				else
			{									 /* Search down */
				/* Search from current field to bottom */
				for (i = fields_cur + 1; i <= fields_max; i++) {
					if ((fields[i-1].active) && (last_field == fields_cur)) {
						fields_cur = i;
					}
				}
				/* Search for current field up */
				for (i = 1; i < fields_cur; i++) {
					if ((fields[i-1].active) && (last_field == fields_cur)) {
						fields_cur = i;
					}
				}
			}

			/* Compare for redraw */
			if (last_field != fields_cur) {
				/* Redraw last field first */
        draw_field(last_field, 0, 0, 0);
				/* Redraw new field */
        draw_field(fields_cur, 0, 0, 0);
			}
		}

void form_c::goto_field(ushort field_num, uchar button_down)
		{
			ushort last_field;

			// Save last field for comparison to see if we need a redraw
			last_field = fields_cur;
			fields_cur = field_num;

			/* Compare for redraw */
			if (last_field != fields_cur) {
				/* Redraw last field first */
        draw_field(last_field, 0, 0, 0);
				/* Redraw new field */
        draw_field(fields_cur, button_down, 0, 0);
			}
		}

void form_c::draw_listbox_entry(ushort field_num, ushort entry_num, uchar highlighted)
		{
			field_listbox_t FAR * listbox;  // Pointer to listbox information

      /* Convert void field pointer to listbox pointer */
      listbox = (field_listbox_t FAR *) fields[field_num - 1].data;
      /* Display entry */
      if ((entry_num >= listbox->top_entrys) && (entry_num <= min(listbox->num_entrys, (ushort) (listbox->cur_entrys + listbox->boxboty - listbox->boxtopy)))) {
        /* Set correct color */
				if (highlighted) {
          win->text_color(listbox_high_fcolor);
					win->text_background(listbox_high_bcolor);
        }
          else
        {
          win->text_color(listbox_norm_fcolor);
          win->text_background(listbox_norm_bcolor);
        }
        /* Display entry */
				win->goto_xy(listbox->boxtopx, listbox->boxtopy  + ((entry_num - 1) - listbox->top_entrys + 1));
        if (strlen(listbox->entrys[entry_num - 1].str) >= listbox->hscroll_pos) {
          win->width_printf((listbox->boxbotx - listbox->boxtopx + 1), &listbox->entrys[entry_num - 1].str[listbox->hscroll_pos - 1]);
        }
          else win->width_printf((listbox->boxbotx - listbox->boxtopx + 1), "");
      }
    }


void form_c::draw_checkbox_entry(ushort field_num, uchar entry_num, uchar highlight)
		{
			field_checkbox_t FAR * checkbox;		// Pointer to checkbox information

			/* Convert void data pointer to checkbox pointer */
			checkbox = (field_checkbox_t FAR *) fields[field_num - 1].data;

			/* Set correct color */
			if ((entry_num == checkbox->cur_entrys) && (field_num == fields_cur) && highlight) {
				win->text_color(checkbox_high_fcolor);
				win->text_background(checkbox_high_bcolor);
			}
				else
			{
				win->text_color(checkbox_norm_fcolor);
				win->text_background(checkbox_norm_bcolor);
			}
			/* Display entry */
			win->goto_xy(checkbox->boxtopx + checkbox->entrys[entry_num - 1].x - 1, checkbox->boxtopy + checkbox->entrys[entry_num - 1].y - 1);
			if (checkbox->entrys[entry_num - 1].checked) win->wputs("[X] ");
				else win->wputs("[ ] ");
			win->wputs(checkbox->entrys[entry_num - 1].title);
		}

void form_c::next_listbox_entry(ushort field_num, uchar direction) // 0 = Up, 1 = Down
		{
			field_listbox_t FAR * listbox;    // Pointer to listbox information

			/* Convert void data pointer to listbox pointer */
			listbox = (field_listbox_t FAR *) fields[field_num - 1].data;

			/* Search for next field */
			if (!direction) {  /* Search up */
        /* See if we are at the top */
        if (listbox->cur_entrys == 1) return;
        /* Unhighlight current entry */
        draw_listbox_entry(field_num, listbox->cur_entrys, 0);
        /* Scroll list if necessary */
        listbox->cur_entrys--;
        if (listbox->cur_entrys < listbox->top_entrys) {
          win->scroll_part_down(listbox->boxtopx, listbox->boxtopy, listbox->boxbotx, listbox->boxboty);
          listbox->top_entrys--;
        }
        /* Display new highlighted field */
				draw_listbox_entry(field_num, listbox->cur_entrys, 1);
			}
				else
			{									 /* Search down */
        /* See if we are at the top */
        if (listbox->cur_entrys == listbox->num_entrys) return;
        /* Unhighlight current entry */
				draw_listbox_entry(field_num, listbox->cur_entrys, 0);
        /* Scroll list if necessary */
        listbox->cur_entrys++;
        if (listbox->cur_entrys > (listbox->top_entrys + listbox->boxboty - listbox->boxtopy)) {
          win->scroll_part_up(listbox->boxtopx, listbox->boxtopy, listbox->boxbotx, listbox->boxboty);
          listbox->top_entrys++;
        }
        /* Display new highlighted field */
        draw_listbox_entry(field_num, listbox->cur_entrys, 1);
      }
      /* Draw vertical scroll bar */
			draw_listbox_vscroll(field_num, 0);
		}

void form_c::shift_listbox_entry(ushort field_num, uchar direction) // 0 = Left, 1 = Right
		{
			int i;
      field_listbox_t FAR * listbox;    // Pointer to listbox information
      uchar list_width;                 // Width of list
      uchar list_height;

      /* Convert void data pointer to listbox pointer */
      listbox = (field_listbox_t FAR *) fields[field_num - 1].data;

      /* Calculate listbox height */
      list_height = (listbox->boxboty - listbox->boxtopy + 1);

      /* Shift listbox left */
      if (!direction) {
        /* See if we are at the left already */
				if (listbox->hscroll_pos == 1) return;
        /* Scroll list left */
        listbox->hscroll_pos--;
        win->text_color(listbox_norm_fcolor);
        win->text_background(listbox_norm_bcolor);
        win->scroll_part_right(listbox->boxtopx, listbox->boxtopy, listbox->boxbotx, listbox->boxboty);
        /* Redraw the character in left hand column */
        for (i = (listbox->top_entrys - 1); i < (listbox->top_entrys + list_height - 1); i++) {
          /* Set correct color for this line */
          if ((i + 1) == (listbox->cur_entrys)) {
            win->text_color(listbox_high_fcolor);
            win->text_background(listbox_high_bcolor);
          }
            else
          {
            win->text_color(listbox_norm_fcolor);
            win->text_background(listbox_norm_bcolor);
          }
					/* Goto the correct position */
          win->goto_xy(listbox->boxtopx, listbox->boxtopy + i - listbox->top_entrys + 1);
          if (strlen(listbox->entrys[i].str) >= listbox->hscroll_pos) {
            win->wputch(listbox->entrys[i].str[listbox->hscroll_pos - 1]);
          }
            else win->wputch(' ');
        }
			}
				else
      {                  /* Shift listbox right */
        /* Calculate width of listbox */
        list_width = listbox->boxbotx - listbox->boxtopx + 1;
        /* See if we are at the right already */
        if ((listbox->hscroll_pos + list_width - 1) >= listbox->longest_entry) return;
        /* Scroll list right */
        listbox->hscroll_pos++;
        win->text_color(listbox_norm_fcolor);
        win->text_background(listbox_norm_bcolor);
				win->scroll_part_left(listbox->boxtopx, listbox->boxtopy, listbox->boxbotx, listbox->boxboty);
        /* Redraw the character in right hand column */
        for (i = (listbox->top_entrys - 1); i < (listbox->top_entrys + list_height - 1); i++) {
          /* Set correct color for this line */
          if ((i + 1) == (listbox->cur_entrys)) {
            win->text_color(listbox_high_fcolor);
            win->text_background(listbox_high_bcolor);
          }
            else
          {
            win->text_color(listbox_norm_fcolor);
            win->text_background(listbox_norm_bcolor);
          }
          /* Goto the correct position */
          win->goto_xy(listbox->boxbotx, listbox->boxtopy + i - listbox->top_entrys + 1);
          if (strlen(listbox->entrys[i].str) >= (listbox->hscroll_pos + list_width - 1)) {
            win->wputch(listbox->entrys[i].str[listbox->hscroll_pos - 1 + list_width - 1]);
          }
						else win->wputch(' ');
        }
      }
      /* Draw vertical scroll bar */
      draw_listbox_hscroll(field_num, 0);
    }


void form_c::goto_listbox_entry(ushort field_num, uchar entry_num)
		{
			uchar last_entry;
      field_listbox_t FAR * listbox;    // Pointer to listbox information

      /* Convert void data pointer to listbox pointer */
      listbox = (field_listbox_t FAR *) fields[field_num - 1].data;

			// Save last entry for comparison to see if we need a redraw
			last_entry = listbox->cur_entrys;
			listbox->cur_entrys = entry_num;

			/* Compare for redraw */
      if (last_entry != listbox->cur_entrys) {
				/* Redraw last entry first */
        draw_listbox_entry(field_num, last_entry, 0);
				/* Redraw new entry */
        draw_listbox_entry(field_num, listbox->cur_entrys, 1);
        /* Draw vertical scroll bar */
        draw_listbox_vscroll(field_num, 0);
			}
		}

void form_c::next_checkbox_entry(ushort field_num, uchar direction) // 0 = Up, 1 = Down
		{
			int i;
			uchar last_entry;
			field_checkbox_t FAR * checkbox;		// Pointer to checkbox information

			/* Convert void data pointer to checkbox pointer */
			checkbox = (field_checkbox_t FAR *) fields[field_num - 1].data;

			// Save last field for comparison to see if we need a redraw
			last_entry = checkbox->cur_entrys;

			/* Search for next field */
			if (!direction) {  /* Search up */
				/* Search for current field up */
				for (i = checkbox->cur_entrys; i > 0; i--) {
					if (last_entry == checkbox->cur_entrys) {
						checkbox->cur_entrys = i;
					}
				}
				/* Search from bottom to current field */
				for (i = checkbox->num_entrys; i > checkbox->cur_entrys; i--) {
					if (last_entry == checkbox->cur_entrys) {
						checkbox->cur_entrys = i;
					}
				}
			}
				else
			{									 /* Search down */
				/* Search from current field to bottom */
				for (i = checkbox->cur_entrys + 1; i <= checkbox->num_entrys; i++) {
					if (last_entry == checkbox->cur_entrys) {
						checkbox->cur_entrys = i;
					}
				}
				/* Search for current field up */
				for (i = 1; i < checkbox->cur_entrys; i++) {
					if (last_entry == checkbox->cur_entrys) {
						checkbox->cur_entrys = i;
					}
				}
			}

			/* Compare for redraw */
			if (last_entry != checkbox->cur_entrys) {
				/* Redraw last entry first */
				draw_checkbox_entry(field_num, last_entry, 1);
				/* Redraw new entry */
				draw_checkbox_entry(field_num, checkbox->cur_entrys, 1);
			}
		}

void form_c::goto_checkbox_entry(ushort field_num, uchar entry_num)
		{
			uchar last_entry;
			field_checkbox_t FAR * checkbox;		// Pointer to checkbox information

			/* Convert void data pointer to checkbox pointer */
			checkbox = (field_checkbox_t FAR *) fields[field_num - 1].data;

			// Save last entry for comparison to see if we need a redraw
			last_entry = checkbox->cur_entrys;
			checkbox->cur_entrys = entry_num;

			/* Compare for redraw */
			if (last_entry != checkbox->cur_entrys) {
				/* Redraw last entry first */
				draw_checkbox_entry(field_num, last_entry, 1);
				/* Redraw new entry */
				draw_checkbox_entry(field_num, checkbox->cur_entrys, 1);
			}
		}

void form_c::draw_radiobutton_entry(ushort field_num, uchar entry_num, uchar highlight)
		{
			field_radiobutton_t FAR * radiobutton;		// Pointer to radiobutton information

			/* Convert void data pointer to radiobutton pointer */
			radiobutton = (field_radiobutton_t FAR *) fields[field_num - 1].data;

			/* Set correct color */
			if ((entry_num == radiobutton->cur_entrys) && (field_num == fields_cur) && highlight) {
				win->text_color(radiobutton_high_fcolor);
				win->text_background(radiobutton_high_bcolor);
			}
				else
			{
				win->text_color(radiobutton_norm_fcolor);
				win->text_background(radiobutton_norm_bcolor);
			}
			/* Display entry */
			win->goto_xy(radiobutton->boxtopx + radiobutton->entrys[entry_num - 1].x - 1, radiobutton->boxtopy + radiobutton->entrys[entry_num - 1].y - 1);
			if (entry_num == radiobutton->cur_entrys) {
				win->wputch('(');
				win->wputctrlch(7);
				win->wputs(") ");
			}
				else win->wputs("( ) ");
			win->wputs(radiobutton->entrys[entry_num - 1].title);
		}

void form_c::next_radiobutton_entry(ushort field_num, uchar direction) // 0 = Up, 1 = Down
		{
			int i;
			uchar last_entry;
			field_radiobutton_t FAR * radiobutton;		// Pointer to radiobutton information

			/* Convert void data pointer to radiobutton pointer */
			radiobutton = (field_radiobutton_t FAR *) fields[field_num - 1].data;

			// Save last field for comparison to see if we need a redraw
			last_entry = radiobutton->cur_entrys;

			/* Search for next field */
			if (!direction) {  /* Search up */
				/* Search for current field up */
				for (i = radiobutton->cur_entrys; i > 0; i--) {
					if (last_entry == radiobutton->cur_entrys) {
						radiobutton->cur_entrys = i;
					}
				}
				/* Search from bottom to current field */
				for (i = radiobutton->num_entrys; i > radiobutton->cur_entrys; i--) {
					if (last_entry == radiobutton->cur_entrys) {
						radiobutton->cur_entrys = i;
					}
				}
			}
				else
			{									 /* Search down */
				/* Search from current field to bottom */
				for (i = radiobutton->cur_entrys + 1; i <= radiobutton->num_entrys; i++) {
					if (last_entry == radiobutton->cur_entrys) {
						radiobutton->cur_entrys = i;
					}
				}
				/* Search for current field up */
				for (i = 1; i < radiobutton->cur_entrys; i++) {
					if (last_entry == radiobutton->cur_entrys) {
						radiobutton->cur_entrys = i;
					}
				}
			}

			/* Compare for redraw */
			if (last_entry != radiobutton->cur_entrys) {
				/* Redraw last entry first */
				draw_radiobutton_entry(field_num, last_entry, 1);
				/* Redraw new entry */
				draw_radiobutton_entry(field_num, radiobutton->cur_entrys, 1);
			}
		}

void form_c::goto_radiobutton_entry(ushort field_num, uchar entry_num)
		{
			uchar last_entry;
			field_radiobutton_t FAR * radiobutton;		// Pointer to radiobutton information

			/* Convert void data pointer to radiobutton pointer */
			radiobutton = (field_radiobutton_t FAR *) fields[field_num - 1].data;

			// Save last entry for comparison to see if we need a redraw
			last_entry = radiobutton->cur_entrys;
			radiobutton->cur_entrys = entry_num;

			/* Compare for redraw */
			if (last_entry != radiobutton->cur_entrys) {
				/* Redraw last entry first */
				draw_radiobutton_entry(field_num, last_entry, 1);
				/* Redraw new entry */
				draw_radiobutton_entry(field_num, radiobutton->cur_entrys, 1);
			}
		}

ushort form_c::field_clicked(void)
		{
			ushort i;					// General counter
			uchar j;					// General counter
			field_ptr_t ptr;  // Pointers to field information

			/* Check all mouse events to see if they clicked on a submenu title */
			while (mouse_read_event()) {
				/* See if this event had a button clicked */
				if (mouse_left || mouse_right || mouse_center) {
					/* See if x and y coordinates of mouse event are valid for this submenu's window */
					if (win->text_x_relative(mouse_col) && win->text_y_relative(mouse_row)) {

						/* See if a button is depressed and should be released */
						if (fields[fields_cur - 1].type == FIELD_BUTTON) {
							ptr.button = (field_button_t FAR *) fields[fields_cur - 1].data;
							if (ptr.button->down) {
                if (!((win->text_x_relative(mouse_col) >= ptr.button->x) && (win->text_x_relative(mouse_col) <= (ptr.button->x + strlen(ptr.button->title) + 2)) && (win->text_y_relative(mouse_row) == ptr.button->y))) button_unclick(fields_cur);
                if ((win->text_x_relative(mouse_col) >= ptr.button->x) && (win->text_x_relative(mouse_col) <= (ptr.button->x + strlen(ptr.button->title) + 2)) && (win->text_y_relative(mouse_row) == ptr.button->y)) {
                  if (mouse_left == BUTTON_CLICKED) {
                    button_unclick(fields_cur);
                    result = fields[fields_cur - 1].id;
                    /* Return id of button pressed */
                    return(fields_cur);
                  }
								}
							}
						}
						/* See if we are sliding or clicking on a field */
            if ((mouse_left == BUTTON_DOWN) || (mouse_center == BUTTON_DOWN) || (mouse_right == BUTTON_DOWN)) {
							/* See if they clicked on a field */
							for (i = 0; i < fields_max; i++) {
								/* See if mouse event coordinates match the field */
								switch(fields[i].type) {
									case FIELD_BUTTON : {
																				/* Convert data void pointer to button pointer */
																				ptr.button = (field_button_t FAR *) fields[i].data;
																				/* See if event coordinates are on the button */
																				if ((win->text_x_relative(mouse_col) >= ptr.button->x) && (win->text_x_relative(mouse_col) <= (ptr.button->x + strlen(ptr.button->title) + 2)) && (win->text_y_relative(mouse_row) == ptr.button->y)) {
																					/* Change field and click button */
																					if ((i + 1) != fields_cur) goto_field(i + 1, 1);
																						else button_click(i + 1);
																					/* Return id of new button */
																					return(i + 1);
																				}
																				break;
																			}
                  case FIELD_LISTBOX : {
                                         /* Convert void data pointer to listbox pointer */
                                         ptr.listbox = (field_listbox_t FAR *) fields[i].data;
                                         /* See if event is on vertical scroll bar arrows */
                                         if (ptr.listbox->vscroll) {
                                           /* Top arrow clicked on */
                                           if ((win->text_x_relative(mouse_col) == (ptr.listbox->boxbotx + 1)) && (win->text_y_relative(mouse_row) == ptr.listbox->boxtopy)) {
                                             goto_field(i + 1, 1);
                                             next_listbox_entry(i + 1, 0);
                                             return(i + 1);
                                           }
                                             else
                                           if ((win->text_x_relative(mouse_col) == (ptr.listbox->boxbotx + 1)) && (win->text_y_relative(mouse_row) == ptr.listbox->boxboty)) {
                                             /* Bottom arrow clicked on */
                                             goto_field(i + 1, 1);
                                             next_listbox_entry(i + 1, 1);
                                             return(i + 1);
                                           }
                                         }
                                         /* See if event is on horizontal scroll bar arrows */
                                         if (ptr.listbox->hscroll) {
                                           /* Top arrow clicked on */
                                           if ((win->text_x_relative(mouse_col) == ptr.listbox->boxtopx) && (win->text_y_relative(mouse_row) == (ptr.listbox->boxboty + 1))) {
                                             goto_field(i + 1, 1);
                                             shift_listbox_entry(i + 1, 0);
                                             return(i + 1);
                                           }
                                             else
                                           if ((win->text_x_relative(mouse_col) == ptr.listbox->boxbotx) && (win->text_y_relative(mouse_row) == (ptr.listbox->boxboty + 1))) {
                                             /* Bottom arrow clicked on */
                                             goto_field(i + 1, 1);
                                             shift_listbox_entry(i + 1, 1);
                                             return(i + 1);
                                           }
                                         }
                                         break;
                                       }
								}
							}
						}
							else
						{
							/* See if they clicked on a field */
							for (i = 0; i < fields_max; i++) {
								/* See if mouse event coordinates match the field */
								switch(fields[i].type) {
									case FIELD_BUTTON : {
																				/* Convert data void pointer to button pointer */
																				ptr.button = (field_button_t FAR *) fields[i].data;
																				/* See if event coordinates are on the button */
																				if ((win->text_x_relative(mouse_col) >= ptr.button->x) && (win->text_x_relative(mouse_col) <= (ptr.button->x + strlen(ptr.button->title) + 1)) && (win->text_y_relative(mouse_row) == ptr.button->y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 1);
																					result = fields[i].id;
																					/* Click button */
																					button_click(fields_cur);
																					DELAY(100);
																					button_unclick(fields_cur);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				break;
																			}
									case FIELD_UPCASE_STRING :
									case FIELD_STRING : {
																				/* Convert data void pointer to string pointer */
																				ptr.string = (field_string_t FAR *) fields[i].data;
																				/* See if event coordinates are on the string field title */
																				if ((win->text_x_relative(mouse_col) >= ptr.string->title_x) && (win->text_x_relative(mouse_col) <= (ptr.string->title_x + strlen(ptr.string->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.string->title_y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 1);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				/* See if event coordinates are on the string entry field */
																				if ((win->text_x_relative(mouse_col) >= ptr.string->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.string->entry_x + ptr.string->view_len + 1)) && (win->text_y_relative(mouse_row) == ptr.string->entry_y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 0);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				break;
																			}
									case FIELD_CONTROL_STRING : {
																								/* Convert data void pointer to control_string pointer */
																								ptr.control_string = (field_control_string_t FAR *) fields[i].data;
																								/* See if event coordinates are on the control_string field title */
																								if ((win->text_x_relative(mouse_col) >= ptr.control_string->title_x) && (win->text_x_relative(mouse_col) <= (ptr.control_string->title_x + strlen(ptr.control_string->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.control_string->title_y)) {
																									/* Goto correct field */
																									goto_field(i + 1, 1);
																									/* Return result of new field */
																									return(i + 1);
																								}
																								/* See if event coordinates are on the control_string entry field */
																								if ((win->text_x_relative(mouse_col) >= ptr.control_string->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.control_string->entry_x + ptr.control_string->view_len + 1)) && (win->text_y_relative(mouse_row) == ptr.control_string->entry_y)) {
																									/* Goto correct field */
																									goto_field(i + 1, 0);
																									/* Return result of new field */
																									return(i + 1);
																								}
																								break;
																							}
									case FIELD_TOGGLE_UCHAR : {
																							/* Convert data void pointer to field_toggle_uchar pointer */
																							ptr.field_toggle_uchar = (field_toggle_uchar_t FAR *) fields[i].data;
																							/* See if event coordinates are on the field_toggle_uchar field title */
																							if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_uchar->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_uchar->title_x + strlen(ptr.field_toggle_uchar->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_uchar->title_y)) {
																								/* Toggle field */
																								if (*ptr.field_toggle_uchar->t) *ptr.field_toggle_uchar->t = 0;
																									else *ptr.field_toggle_uchar->t = 1;
																								/* Goto correct field */
																								goto_field(i + 1, 1);
                                                draw_field(i + 1, 0, 1, 0);
																								/* Return result of new field */
																								return(i + 1);
																							}
																							/* See if event coordinates are on the field_toggle_uchar entry field */
																							if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_uchar->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_uchar->entry_x + 4)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_uchar->entry_y)) {
																								/* Toggle field */
																								if (*ptr.field_toggle_uchar->t) *ptr.field_toggle_uchar->t = 0;
																									else *ptr.field_toggle_uchar->t = 1;
																								/* Goto correct field */
																								goto_field(i + 1, 0);
                                                draw_field(i + 1, 0, 1, 0);
																								/* Return result of new field */
																								return(i + 1);
																							}
																							break;
																						}
									case FIELD_TOGGLE_UCHAR_BIT : {
																									/* Convert data void pointer to field_toggle_uchar_bit pointer */
																									ptr.field_toggle_uchar_bit = (field_toggle_uchar_bit_t FAR *) fields[i].data;
																									/* See if event coordinates are on the field_toggle_uchar_bit field title */
																									if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_uchar_bit->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_uchar_bit->title_x + strlen(ptr.field_toggle_uchar_bit->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_uchar_bit->title_y)) {
																										/* Toggle field */
																										if (*ptr.field_toggle_uchar_bit->t & ptr.field_toggle_uchar_bit->bit) *ptr.field_toggle_uchar_bit->t &= ~ptr.field_toggle_uchar_bit->bit;
																											else *ptr.field_toggle_uchar_bit->t |= ptr.field_toggle_uchar_bit->bit;
																										/* Goto correct field */
																										goto_field(i + 1, 1);
                                                    draw_field(i + 1, 0, 1, 0);
																										/* Return result of new field */
																										return(i + 1);
																									}
																									/* See if event coordinates are on the field_toggle_uchar_bit entry field */
																									if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_uchar_bit->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_uchar_bit->entry_x + 4)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_uchar_bit->entry_y)) {
																										/* Toggle field */
																										if (*ptr.field_toggle_uchar_bit->t & ptr.field_toggle_uchar_bit->bit) *ptr.field_toggle_uchar_bit->t &= ~ptr.field_toggle_uchar_bit->bit;
																											else *ptr.field_toggle_uchar_bit->t |= ptr.field_toggle_uchar_bit->bit;
																										/* Goto correct field */
																										goto_field(i + 1, 0);
                                                    draw_field(i + 1, 0, 1, 0);
																										/* Return result of new field */
																										return(i + 1);
																									}
																									break;
																								}
									case FIELD_TOGGLE_USHORT_BIT : {
																									 /* Convert data void pointer to field_toggle_ushort_bit pointer */
																									 ptr.field_toggle_ushort_bit = (field_toggle_ushort_bit_t FAR *) fields[i].data;
																									 /* See if event coordinates are on the field_toggle_ushort_bit field title */
																									 if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_ushort_bit->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_ushort_bit->title_x + strlen(ptr.field_toggle_ushort_bit->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_ushort_bit->title_y)) {
																										 /* Toggle field */
																										 if (*ptr.field_toggle_ushort_bit->t & ptr.field_toggle_ushort_bit->bit) *ptr.field_toggle_ushort_bit->t &= ~ptr.field_toggle_ushort_bit->bit;
																											 else *ptr.field_toggle_ushort_bit->t |= ptr.field_toggle_ushort_bit->bit;
																										 /* Goto correct field */
																										 goto_field(i + 1, 1);
                                                     draw_field(i + 1, 0, 1, 0);
																										 /* Return result of new field */
																										 return(i + 1);
																									 }
																									 /* See if event coordinates are on the field_toggle_ushort_bit entry field */
																									 if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_ushort_bit->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_ushort_bit->entry_x + 4)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_ushort_bit->entry_y)) {
																										 /* Toggle field */
																										 if (*ptr.field_toggle_ushort_bit->t & ptr.field_toggle_ushort_bit->bit) *ptr.field_toggle_ushort_bit->t &= ~ptr.field_toggle_ushort_bit->bit;
																											 else *ptr.field_toggle_ushort_bit->t |= ptr.field_toggle_ushort_bit->bit;
																										 /* Goto correct field */
																										 goto_field(i + 1, 0);
                                                     draw_field(i + 1, 0, 1, 0);
																										 /* Return result of new field */
																										 return(i + 1);
																									 }
																									 break;
																								 }
									case FIELD_TOGGLE_ULONG_BIT : {
																									/* Convert data void pointer to field_toggle_ulong_bit pointer */
																									ptr.field_toggle_ulong_bit = (field_toggle_ulong_bit_t FAR *) fields[i].data;
																									/* See if event coordinates are on the field_toggle_ulong_bit field title */
																									if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_ulong_bit->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_ulong_bit->title_x + strlen(ptr.field_toggle_ulong_bit->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_ulong_bit->title_y)) {
																										/* Toggle field */
																										if (*ptr.field_toggle_ulong_bit->t & ptr.field_toggle_ulong_bit->bit) *ptr.field_toggle_ulong_bit->t &= ~ptr.field_toggle_ulong_bit->bit;
																											else *ptr.field_toggle_ulong_bit->t |= ptr.field_toggle_ulong_bit->bit;
																										/* Goto correct field */
																										goto_field(i + 1, 1);
                                                    draw_field(i + 1, 0, 1, 0);
																										/* Return result of new field */
																										return(i + 1);
																									}
																									/* See if event coordinates are on the field_toggle_ulong_bit entry field */
																									if ((win->text_x_relative(mouse_col) >= ptr.field_toggle_ulong_bit->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_toggle_ulong_bit->entry_x + 4)) && (win->text_y_relative(mouse_row) == ptr.field_toggle_ulong_bit->entry_y)) {
																										/* Toggle field */
																										if (*ptr.field_toggle_ulong_bit->t & ptr.field_toggle_ulong_bit->bit) *ptr.field_toggle_ulong_bit->t &= ~ptr.field_toggle_ulong_bit->bit;
																											else *ptr.field_toggle_ulong_bit->t |= ptr.field_toggle_ulong_bit->bit;
																										/* Goto correct field */
																										goto_field(i + 1, 0);
                                                    draw_field(i + 1, 0, 1, 0);
																										/* Return result of new field */
																										return(i + 1);
																									}
																									break;
																								}
									case FIELD_HHMMSS : {
																				/* Convert data void pointer to hhmmss pointer */
																				ptr.field_hhmmss = (field_hhmmss_t FAR *) fields[i].data;
																				/* See if event coordinates are on the string field title */
																				if ((win->text_x_relative(mouse_col) >= ptr.field_hhmmss->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_hhmmss->title_x + strlen(ptr.field_hhmmss->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_hhmmss->title_y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 1);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				/* See if event coordinates are on the string entry field */
																				if ((win->text_x_relative(mouse_col) >= ptr.field_hhmmss->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_hhmmss->entry_x + 9)) && (win->text_y_relative(mouse_row) == ptr.field_hhmmss->entry_y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 0);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				break;
																			}
									case FIELD_MMDDYYYY : {
																					/* Convert data void pointer to mmddyyyy pointer */
																					ptr.field_mmddyyyy = (field_mmddyyyy_t FAR *) fields[i].data;
																					/* See if event coordinates are on the string field title */
																					if ((win->text_x_relative(mouse_col) >= ptr.field_mmddyyyy->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_mmddyyyy->title_x + strlen(ptr.field_mmddyyyy->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_mmddyyyy->title_y)) {
																						/* Goto correct field */
																						goto_field(i + 1, 1);
																						/* Return result of new field */
																						return(i + 1);
																					}
																					/* See if event coordinates are on the string entry field */
																					if ((win->text_x_relative(mouse_col) >= ptr.field_mmddyyyy->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_mmddyyyy->entry_x + 11)) && (win->text_y_relative(mouse_row) == ptr.field_mmddyyyy->entry_y)) {
																						/* Goto correct field */
																						goto_field(i + 1, 0);
																						/* Return result of new field */
																						return(i + 1);
																					}
																					break;
																				}
									case FIELD_4D_ADDR : {
																				 /* Convert data void pointer to field_faddr pointer */
																				 ptr.field_faddr = (field_faddr_t FAR *) fields[i].data;
																				 /* See if event coordinates are on the field_faddr field title */
																				 if ((win->text_x_relative(mouse_col) >= ptr.field_faddr->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_faddr->title_x + strlen(ptr.field_faddr->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_faddr->title_y)) {
																					 /* Goto correct field */
																					 goto_field(i + 1, 1);
																					 /* Return result of new field */
																					 return(i + 1);
																				 }
																				 /* See if event coordinates are on the field_faddr entry field */
																				 if ((win->text_x_relative(mouse_col) >= ptr.field_faddr->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_faddr->entry_x + ptr.field_faddr->view_len + 1)) && (win->text_y_relative(mouse_row) == ptr.field_faddr->entry_y)) {
																					 /* Goto correct field */
																					 goto_field(i + 1, 0);
																					 /* Return result of new field */
																					 return(i + 1);
																				 }
																				 break;
																			 }
									case FIELD_FORMAT_STRING : {
																							 /* Convert data void pointer to string pointer */
																							 ptr.frmt_string = (field_frmt_string_t FAR *) fields[i].data;
																							 /* See if event coordinates are on the string field title */
																							 if ((win->text_x_relative(mouse_col) >= ptr.frmt_string->title_x) && (win->text_x_relative(mouse_col) <= (ptr.frmt_string->title_x + strlen(ptr.frmt_string->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.frmt_string->title_y)) {
																								 /* Goto correct field */
																								 goto_field(i + 1, 1);
																								 /* Return result of new field */
																								 return(i + 1);
																							 }
																							 /* See if event coordinates are on the string entry field */
																							 if ((win->text_x_relative(mouse_col) >= ptr.frmt_string->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.frmt_string->entry_x + ptr.frmt_string->max_len + 1)) && (win->text_y_relative(mouse_row) == ptr.frmt_string->entry_y)) {
																								 /* Goto correct field */
																								 goto_field(i + 1, 0);
																								 /* Return result of new field */
																								 return(i + 1);
																							 }
																							 break;
																						 }
									case FIELD_BYTE : {
																			/* Convert data void pointer to byte pointer */
																			ptr.field_byte = (field_byte_t FAR *) fields[i].data;
																			/* See if event coordinates are on the string field title */
																			if ((win->text_x_relative(mouse_col) >= ptr.field_byte->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_byte->title_x + strlen(ptr.field_byte->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_byte->title_y)) {
																				/* Goto correct field */
																				goto_field(i + 1, 1);
																				/* Return result of new field */
																				return(i + 1);
																			}
																			/* See if event coordinates are on the field_byte entry field */
																			if ((win->text_x_relative(mouse_col) >= ptr.field_byte->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_byte->entry_x + ptr.field_byte->entry_width + 1)) && (win->text_y_relative(mouse_row) == ptr.field_byte->entry_y)) {
																				/* Goto correct field */
																				goto_field(i + 1, 0);
																				/* Return result of new field */
																				return(i + 1);
																			}
																			break;
																		}
									case FIELD_USHORT : {
																				/* Convert data void pointer to byte pointer */
																				ptr.field_ushort = (field_ushort_t FAR *) fields[i].data;
																				/* See if event coordinates are on the string field title */
																				if ((win->text_x_relative(mouse_col) >= ptr.field_ushort->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_ushort->title_x + strlen(ptr.field_ushort->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_ushort->title_y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 1);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				/* See if event coordinates are on the field_ushort entry field */
																				if ((win->text_x_relative(mouse_col) >= ptr.field_ushort->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_ushort->entry_x + ptr.field_ushort->entry_width + 1)) && (win->text_y_relative(mouse_row) == ptr.field_ushort->entry_y)) {
																					/* Goto correct field */
																					goto_field(i + 1, 0);
																					/* Return result of new field */
																					return(i + 1);
																				}
																				break;
																			}
									case FIELD_ULONG : {
																			 /* Convert data void pointer to byte pointer */
																			 ptr.field_ulong = (field_ulong_t FAR *) fields[i].data;
																			 /* See if event coordinates are on the string field title */
																			 if ((win->text_x_relative(mouse_col) >= ptr.field_ulong->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_ulong->title_x + strlen(ptr.field_ulong->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_ulong->title_y)) {
																				 /* Goto correct field */
																				 goto_field(i + 1, 1);
																				 /* Return result of new field */
																				 return(i + 1);
																			 }
																			 /* See if event coordinates are on the field_ulong entry field */
																			 if ((win->text_x_relative(mouse_col) >= ptr.field_ulong->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_ulong->entry_x + ptr.field_ulong->entry_width + 1)) && (win->text_y_relative(mouse_row) == ptr.field_ulong->entry_y)) {
																				 /* Goto correct field */
																				 goto_field(i + 1, 0);
																				 /* Return result of new field */
																				 return(i + 1);
																			 }
																			 break;
																		 }
									case FIELD_CHAR : {
																			/* Convert data void pointer to byte pointer */
																			ptr.field_char = (field_char_t FAR *) fields[i].data;
																			/* See if event coordinates are on the string field title */
																			if ((win->text_x_relative(mouse_col) >= ptr.field_char->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_char->title_x + strlen(ptr.field_char->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_char->title_y)) {
																				/* Goto correct field */
																				goto_field(i + 1, 1);
																				/* Return result of new field */
																				return(i + 1);
																			}
																			/* See if event coordinates are on the field_char entry field */
																			if ((win->text_x_relative(mouse_col) >= ptr.field_char->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_char->entry_x + ptr.field_char->entry_width + ((ptr.field_char->min_num < 0) ? 1 : 0) + 2)) && (win->text_y_relative(mouse_row) == ptr.field_char->entry_y)) {
																				/* Goto correct field */
																				goto_field(i + 1, 0);
																				/* Return result of new field */
																				return(i + 1);
																			}
																			break;
																		}
									case FIELD_INT : {
																		 /* Convert data void pointer to byte pointer */
																		 ptr.field_int = (field_int_t FAR *) fields[i].data;
																		 /* See if event coordinates are on the string field title */
																		 if ((win->text_x_relative(mouse_col) >= ptr.field_int->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_int->title_x + strlen(ptr.field_int->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_int->title_y)) {
																			 /* Goto correct field */
																			 goto_field(i + 1, 1);
																			 /* Return result of new field */
																			 return(i + 1);
																		 }
																		 /* See if event coordinates are on the field_int entry field */
																		 if ((win->text_x_relative(mouse_col) >= ptr.field_int->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_int->entry_x + ptr.field_int->entry_width + ((ptr.field_int->min_num < 0) ? 1 : 0) + 2)) && (win->text_y_relative(mouse_row) == ptr.field_int->entry_y)) {
																			 /* Goto correct field */
																			 goto_field(i + 1, 0);
																			 /* Return result of new field */
																			 return(i + 1);
																		 }
																		 break;
																	 }
									case FIELD_LONG : {
																			/* Convert data void pointer to byte pointer */
																			ptr.field_long = (field_long_t FAR *) fields[i].data;
																			/* See if event coordinates are on the string field title */
																			if ((win->text_x_relative(mouse_col) >= ptr.field_long->title_x) && (win->text_x_relative(mouse_col) <= (ptr.field_long->title_x + strlen(ptr.field_long->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.field_long->title_y)) {
																				/* Goto correct field */
																				goto_field(i + 1, 1);
																				/* Return result of new field */
																				return(i + 1);
																			}
																			/* See if event coordinates are on the field_long entry field */
																			if ((win->text_x_relative(mouse_col) >= ptr.field_long->entry_x) && (win->text_x_relative(mouse_col) <= (ptr.field_long->entry_x + ptr.field_long->entry_width + ((ptr.field_long->min_num < 0) ? 1 : 0) + 2)) && (win->text_y_relative(mouse_row) == ptr.field_long->entry_y)) {
																				/* Goto correct field */
																				goto_field(i + 1, 0);
																				/* Return result of new field */
																				return(i + 1);
																			}
																			break;
																		}
                  case FIELD_CHECKBOX : {
																					/* Convert void data pointer to checkbox pointer */
																					ptr.checkbox = (field_checkbox_t FAR *) fields[i].data;
																					/* See if event coordinates are on the checkbox field title */
																					if ((win->text_x_relative(mouse_col) >= ptr.checkbox->title_x) && (win->text_x_relative(mouse_col) <= (ptr.checkbox->title_x + strlen(ptr.checkbox->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.checkbox->title_y)) {
																						/* Goto correct field */
																						goto_field(i + 1, 1);
																						/* Return result of new field */
																						return(i + 1);
																					}
																					/* See of event coordinates are on a checkbox entry */
																					for (j = 0; j < ptr.checkbox->num_entrys; j++) {
																						if ((win->text_x_relative(mouse_col) >= (ptr.checkbox->boxtopx + ptr.checkbox->entrys[j].x - 1)) && (win->text_x_relative(mouse_col) <= (ptr.checkbox->boxtopx + ptr.checkbox->entrys[j].x + strlen(ptr.checkbox->entrys[j].title) + 3)) && (win->text_y_relative(mouse_row) == (ptr.checkbox->boxtopy + ptr.checkbox->entrys[j].y - 1))) {
																							/* Toggle that fields check */
																							if (ptr.checkbox->entrys[j].checked) ptr.checkbox->entrys[j].checked = 0;
																								else ptr.checkbox->entrys[j].checked = 1;
																							/* Goto correct field */
																							goto_field(i + 1, 1);
																							/* Goto the correct field */
																							goto_checkbox_entry(i + 1, j + 1);
																							if (ptr.checkbox->cur_entrys == (j + 1)) draw_checkbox_entry(i + 1, j + 1, 1);
																							/* Return result of new field */
																							return(i + 1);
																						}
																					}
																					break;
																				}
									case FIELD_RADIOBUTTON : {
																						 /* Convert void data pointer to radiobutton pointer */
																						 ptr.radiobutton = (field_radiobutton_t FAR *) fields[i].data;
																						 /* See if event coordinates are on the radiobutton field title */
																						 if ((win->text_x_relative(mouse_col) >= ptr.radiobutton->title_x) && (win->text_x_relative(mouse_col) <= (ptr.radiobutton->title_x + strlen(ptr.radiobutton->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.radiobutton->title_y)) {
																							 /* Goto correct field */
																							 goto_field(i + 1, 1);
																							 /* Return result of new field */
																							 return(i + 1);
																						 }
																						 /* See of event coordinates are on a radiobutton entry */
																						 for (j = 0; j < ptr.radiobutton->num_entrys; j++) {
																							 if ((win->text_x_relative(mouse_col) >= (ptr.radiobutton->boxtopx + ptr.radiobutton->entrys[j].x - 1)) && (win->text_x_relative(mouse_col) <= (ptr.radiobutton->boxtopx + ptr.radiobutton->entrys[j].x + strlen(ptr.radiobutton->entrys[j].title) + 3)) && (win->text_y_relative(mouse_row) == (ptr.radiobutton->boxtopy + ptr.radiobutton->entrys[j].y - 1))) {
																								 /* Goto correct field */
																								 goto_field(i + 1, 1);
																								 /* Goto the correct field */
																								 goto_radiobutton_entry(i + 1, j + 1);
																								 if (ptr.radiobutton->cur_entrys == (j + 1)) draw_radiobutton_entry(i + 1, j + 1, 1);
																								 /* Return result of new field */
																								 return(i + 1);
																							 }
																						 }
																						 break;
																					 }
                  case FIELD_LISTBOX : {
                                         /* Convert void data pointer to listbox pointer */
                                         ptr.listbox = (field_listbox_t FAR *) fields[i].data;
                                         /* See if event coordinates are on the listbox field title */
                                         if ((win->text_x_relative(mouse_col) >= ptr.listbox->title_x) && (win->text_x_relative(mouse_col) <= (ptr.listbox->title_x + strlen(ptr.listbox->title) - 1)) && (win->text_y_relative(mouse_row) == ptr.listbox->title_y)) {
                                           /* Goto correct field */
                                           goto_field(i + 1, 1);
                                           /* Return result of new field */
                                           return(i + 1);
                                         }
                                         /* See if event is on vertical scroll bar arrows */
                                         if (ptr.listbox->vscroll) {
                                           /* Top arrow clicked on */
                                           if ((win->text_x_relative(mouse_col) == (ptr.listbox->boxbotx + 1)) && (win->text_y_relative(mouse_row) == ptr.listbox->boxtopy)) {
                                             goto_field(i + 1, 1);
                                             next_listbox_entry(i + 1, 0);
                                             return(i + 1);
                                           }
                                             else
                                           if ((win->text_x_relative(mouse_col) == (ptr.listbox->boxbotx + 1)) && (win->text_y_relative(mouse_row) == ptr.listbox->boxboty)) {
                                             /* Bottom arrow clicked on */
                                             goto_field(i + 1, 1);
                                             next_listbox_entry(i + 1, 1);
                                             return(i + 1);
                                           }
                                         }
                                         /* See if event is on horizontal scroll bar arrows */
                                         if (ptr.listbox->hscroll) {
                                           /* Top arrow clicked on */
                                           if ((win->text_x_relative(mouse_col) == ptr.listbox->boxtopx) && (win->text_y_relative(mouse_row) == (ptr.listbox->boxboty + 1))) {
                                             goto_field(i + 1, 1);
                                             shift_listbox_entry(i + 1, 0);
                                             return(i + 1);
                                           }
                                             else
                                           if ((win->text_x_relative(mouse_col) == ptr.listbox->boxbotx) && (win->text_y_relative(mouse_row) == (ptr.listbox->boxboty + 1))) {
                                             /* Bottom arrow clicked on */
                                             goto_field(i + 1, 1);
                                             shift_listbox_entry(i + 1, 1);
                                             return(i + 1);
                                           }
                                         }
                                         /* See of event coordinates are on a listbox entry */
                                         if ((win->text_x_relative(mouse_col) >= ptr.listbox->boxtopx) && (win->text_x_relative(mouse_col) <= ptr.listbox->boxbotx) && (win->text_y_relative(mouse_row) >= ptr.listbox->boxtopy) && (win->text_y_relative(mouse_row) <= ptr.listbox->boxboty)) {
																					 /* They clicked inside the listbox so highligh the field */
                                           goto_field(i + 1, 1);
                                           j = (win->text_y_relative(mouse_row) - ptr.listbox->boxtopy + ptr.listbox->top_entrys);
                                           if (j <= ptr.listbox->num_entrys) {
                                             /* See if they are clicking on the highlighted entry to choose it */
                                             if (j == ptr.listbox->cur_entrys) {
                                               result = fields[i].id;
																							 return(i + 1);
                                             }
                                               else goto_listbox_entry(i + 1, j);
                                           }
                                           /* Return result of new field */
                                           return(i + 1);
                                         }
                                         break;
                                       }


								}
							}
						}
					}
				}
			}
			/* No valid mouse clicks */
			return(0);
		}

void form_c::add_user_def_char(ushort new_id, uchar new_ch, uchar new_ext)
		{
			/* Check for invalid user definable keys */
			if (!new_ext) {
				switch(new_ch) {
					case CR :
					case ESC :
					case BACK_SPACE :
					case TAB :
					case ' ' : return;
				}
			}
				else
			{
				switch(new_ch) {
					case SHIFT_TAB :
					case UPARROW :
					case DOWNARROW :
					case LEFTARROW :
					case RIGHTARROW : return;
				}
			}

			/* Allocate another char */
			chars_max++;
			if ((chars = (form_char_t FAR *) FARREALLOC(chars, sizeof(form_char_t) * chars_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_user_def_char() error : Unable to reallocate chars.\n");
				exit(1);
			}
			/* Initialize generic field information */
			chars[chars_max - 1].id = new_id;
			chars[chars_max - 1].active = 1;
			chars[chars_max - 1].ch = new_ch;
			chars[chars_max - 1].extended = new_ext;
		}

void form_c::add_button(ushort new_id, uchar new_x, uchar new_y, char *new_title)
		{
			field_button_t FAR * button;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_button() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_BUTTON;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_button_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_button() error : Unable to allocate memory for button field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			button = (field_button_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((button->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_button() error : Unable to allocate memory for button title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			button->x = new_x;
			button->y = new_y;
			strcpy(button->title, new_title);
			button->down = 0;
		}

void form_c::add_byte(ushort new_id, uchar new_title_x, uchar new_title_y,
                      char *new_title, uchar new_entry_x, uchar new_entry_y,
											uchar new_min_num, uchar new_max_num)
		{
			field_byte_t FAR * field_byte;
      char max_num_str[4];            // String used to determine entry field length

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_byte() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_BYTE;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_byte_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_byte() error : Unable to allocate memory for byte field.\n");
				exit(1);
			}

			/* Convert void field pointer to byte pointer */
			field_byte = (field_byte_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_byte->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_byte() error : Unable to allocate memory for byte title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_byte->title_x = new_title_x;
			field_byte->title_y = new_title_y;
			strcpy(field_byte->title, new_title);
			field_byte->entry_x = new_entry_x;
			field_byte->entry_y = new_entry_y;
			field_byte->min_num = new_min_num;
			field_byte->max_num = new_max_num;
			field_byte->num = NULL;

			/* Calculate length of entry field */
			sprintf(max_num_str, "%u", field_byte->max_num);
			field_byte->entry_width = strlen(max_num_str);
		}

void form_c::add_ushort(ushort new_id, uchar new_title_x, uchar new_title_y,
                        char *new_title, uchar new_entry_x, uchar new_entry_y,
												ushort new_min_num, ushort new_max_num)
		{
			field_ushort_t FAR * field_ushort;
      char max_num_str[6];                // String used to determine entry field length

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_ushort() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_USHORT;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_ushort_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_byte() error : Unable to allocate memory for byte field.\n");
				exit(1);
			}

			/* Convert void field pointer to byte pointer */
			field_ushort = (field_ushort_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_ushort->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_ushort() error : Unable to allocate memory for ushort title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_ushort->title_x = new_title_x;
			field_ushort->title_y = new_title_y;
			strcpy(field_ushort->title, new_title);
			field_ushort->entry_x = new_entry_x;
			field_ushort->entry_y = new_entry_y;
			field_ushort->min_num = new_min_num;
			field_ushort->max_num = new_max_num;
			field_ushort->num = NULL;

			/* Calculate length of entry field */
			sprintf(max_num_str, "%u", field_ushort->max_num);
			field_ushort->entry_width = strlen(max_num_str);
		}

void form_c::add_ulong(ushort new_id, uchar new_title_x, uchar new_title_y,
                       char *new_title, uchar new_entry_x, uchar new_entry_y,
											 ulong new_min_num, ulong new_max_num)
		{
			field_ulong_t FAR * field_ulong;
      char max_num_str[11];             // String used to determine entry field length

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_ulong() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_ULONG;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_ulong_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_byte() error : Unable to allocate memory for byte field.\n");
				exit(1);
			}

			/* Convert void field pointer to byte pointer */
			field_ulong = (field_ulong_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_ulong->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_byte() error : Unable to allocate memory for byte title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_ulong->title_x = new_title_x;
			field_ulong->title_y = new_title_y;
			strcpy(field_ulong->title, new_title);
			field_ulong->entry_x = new_entry_x;
			field_ulong->entry_y = new_entry_y;
			field_ulong->min_num = new_min_num;
			field_ulong->max_num = new_max_num;
			field_ulong->num = NULL;

			/* Calculate length of entry field */
			sprintf(max_num_str, "%lu", field_ulong->max_num);
			field_ulong->entry_width = strlen(max_num_str);
		}

void form_c::add_char(ushort new_id, uchar new_title_x, uchar new_title_y,
                      char *new_title, uchar new_entry_x, uchar new_entry_y,
											char new_min_num, char new_max_num)
		{
			field_char_t FAR * field_char;
      char max_num_str[5];   // String used to determine entry field length

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_char() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_CHAR;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_char_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_char() error : Unable to allocate memory for char field.\n");
				exit(1);
			}

			/* Convert void field pointer to byte pointer */
			field_char = (field_char_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_char->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_char() error : Unable to allocate memory for char title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_char->title_x = new_title_x;
			field_char->title_y = new_title_y;
			strcpy(field_char->title, new_title);
			field_char->entry_x = new_entry_x;
			field_char->entry_y = new_entry_y;
			field_char->min_num = new_min_num;
			field_char->max_num = new_max_num;
			field_char->num = NULL;

			/* Calculate length of entry field */
			sprintf(max_num_str, "%d", field_char->max_num);
			field_char->entry_width = strlen(max_num_str);
			/* Correct for negative */
			if (field_char->max_num < 0) field_char->entry_width--;
		}

void form_c::add_int(ushort new_id, uchar new_title_x, uchar new_title_y,
                      char *new_title, uchar new_entry_x, uchar new_entry_y,
											int new_min_num, int new_max_num)
		{
			field_int_t FAR * field_int;
      char max_num_str[7];         // String used to determine entry field length

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_int() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_INT;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_int_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_int() error : Unable to allocate memory for int field.\n");
				exit(1);
			}

			/* Convert void field pointer to byte pointer */
			field_int = (field_int_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_int->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_int() error : Unable to allocate memory for int title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_int->title_x = new_title_x;
			field_int->title_y = new_title_y;
			strcpy(field_int->title, new_title);
			field_int->entry_x = new_entry_x;
			field_int->entry_y = new_entry_y;
			field_int->min_num = new_min_num;
			field_int->max_num = new_max_num;
			field_int->num = NULL;

			/* Calculate length of entry field */
			sprintf(max_num_str, "%d", field_int->max_num);
			field_int->entry_width = strlen(max_num_str);
			/* Correct for negative */
			if (field_int->max_num < 0) field_int->entry_width--;
		}

void form_c::add_long(ushort new_id, uchar new_title_x, uchar new_title_y,
                      char *new_title, uchar new_entry_x, uchar new_entry_y,
											long new_min_num, long new_max_num)
		{
			field_long_t FAR * field_long;
      char max_num_str[12];           // String used to determine entry field length

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_long() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_LONG;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_long_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_long() error : Unable to allocate memory for long field.\n");
				exit(1);
			}

			/* Convert void field pointer to byte pointer */
			field_long = (field_long_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_long->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_long() error : Unable to allocate memory for long title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_long->title_x = new_title_x;
			field_long->title_y = new_title_y;
			strcpy(field_long->title, new_title);
			field_long->entry_x = new_entry_x;
			field_long->entry_y = new_entry_y;
			field_long->min_num = new_min_num;
			field_long->max_num = new_max_num;
			field_long->num = NULL;

			/* Calculate length of entry field */
			sprintf(max_num_str, "%ld", field_long->max_num);
			field_long->entry_width = strlen(max_num_str);
			/* Correct for negative */
			if (field_long->max_num < 0) field_long->entry_width--;
		}


void form_c::add_checkbox(ushort new_id, uchar new_title_x, uchar new_title_y,
                          char *new_title, uchar new_boxtopx, uchar new_boxtopy,
													uchar new_boxbotx, uchar new_boxboty)
		{
			field_checkbox_t FAR * checkbox;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_checkbox() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_CHECKBOX;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_checkbox_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_checkbox() error : Unable to allocate memory for checkbox field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			checkbox = (field_checkbox_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((checkbox->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_checkbox() error : Unable to allocate memory for checkbox title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			checkbox->title_x = new_title_x;
			checkbox->title_y = new_title_y;
			strcpy(checkbox->title, new_title);
			checkbox->boxtopx = new_boxtopx;
			checkbox->boxtopy = new_boxtopy;
			checkbox->boxbotx = new_boxbotx;
			checkbox->boxboty = new_boxboty;
			checkbox->entrys = NULL;
			checkbox->num_entrys = 0;
			checkbox->cur_entrys = 1;
		}

void form_c::add_checkbox_entry(ushort checkbox_id, uchar new_id, uchar new_x,
                                uchar new_y, char *new_title, uchar new_checked)
		{
			field_checkbox_t FAR * checkbox; // Pointer to checkbox information
			ushort i;

			/* Search for checkbox with the correct id */
			for (i = 0; i < fields_max; i++) {
				/* Does this field have the right id */
				if (fields[i].id == checkbox_id) {
					/* Convert void data pointer to checkbox pointer */
					checkbox = (field_checkbox_t FAR *) fields[i].data;
					/* Allocate another checkbox entry */
					checkbox->num_entrys++;
					if ((checkbox->entrys = (field_checkbox_entry_t FAR *) FARREALLOC(checkbox->entrys, sizeof(field_checkbox_entry_t) * checkbox->num_entrys)) == NULL) {
						/* Error allocating memory */
						printf("\nform_c::add_checkbox_entry() error : Unable to allocate memory for checkbox entry.\n");
						exit(1);
					}
					/* Initialize checkbox entry */
					checkbox->entrys[checkbox->num_entrys - 1].id = new_id;
					checkbox->entrys[checkbox->num_entrys - 1].x = new_x;
					checkbox->entrys[checkbox->num_entrys - 1].y = new_y;
					checkbox->entrys[checkbox->num_entrys - 1].checked = new_checked;
					/* Allocate memory for entry title */
          if ((checkbox->entrys[checkbox->num_entrys - 1].title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
						/* Error allocating memory */
						printf("\nform_c::add_checkbox_entry() error : Unable to allocate memory for checkbox entry title.\n");
						exit(1);
					}
					/* Copy title */
					strcpy(checkbox->entrys[checkbox->num_entrys - 1].title, new_title);
					return;
				}
			}
		}

void form_c::add_listbox(ushort new_id, uchar new_title_x, uchar new_title_y,
                         char *new_title, uchar new_boxtopx, uchar new_boxtopy,
                         uchar new_boxbotx, uchar new_boxboty, uchar new_vscroll,
                         uchar new_hscroll)
		{
			field_listbox_t FAR * listbox;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_listbox() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_LISTBOX;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_listbox_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_listbox() error : Unable to allocate memory for listbox field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			listbox = (field_listbox_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((listbox->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_listbox() error : Unable to allocate memory for listbox title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			listbox->title_x = new_title_x;
			listbox->title_y = new_title_y;
			strcpy(listbox->title, new_title);
			listbox->boxtopx = new_boxtopx;
			listbox->boxtopy = new_boxtopy;
			listbox->boxbotx = new_boxbotx;
			listbox->boxboty = new_boxboty;
			listbox->entrys = NULL;
			listbox->num_entrys = 0;
			listbox->cur_entrys = 1;
			listbox->top_entrys = 1;
      listbox->vscroll = new_vscroll;
      listbox->hscroll = new_hscroll;
      listbox->longest_entry = 0;
      listbox->hscroll_pos = 1;
		}

void form_c::add_listbox_entry(ushort listbox_id, char *new_str, void FAR *new_spec_data, ushort new_spec_data_size)
		{
			field_listbox_t FAR * listbox; // Pointer to listbox information
			ushort i;

			/* Search for listbox with the correct id */
			for (i = 0; i < fields_max; i++) {
				/* Does this field have the right id */
				if (fields[i].id == listbox_id) {
					/* Convert void data pointer to listbox pointer */
					listbox = (field_listbox_t FAR *) fields[i].data;
					/* Allocate another listbox entry */
					listbox->num_entrys++;
					if ((listbox->entrys = (field_listbox_entry_t FAR *) FARREALLOC(listbox->entrys, sizeof(field_listbox_entry_t) * listbox->num_entrys)) == NULL) {
						/* Error allocating memory */
						printf("\nform_c::add_listbox_entry() error : Unable to allocate memory for listbox entry.\n");
						exit(1);
					}
					/* Allocate memory for entry title */
          if ((listbox->entrys[listbox->num_entrys - 1].str = (char *) malloc(strlen(new_str) + 1)) == NULL) {
						/* Error allocating memory */
						printf("\nform_c::add_listbox_entry() error : Unable to allocate memory for listbox entry.\n");
						exit(1);
          }
          if ((new_spec_data != NULL) && (new_spec_data_size)) {
            /* Allocate memory for entry specific data */
            if ((listbox->entrys[listbox->num_entrys - 1].spec_data = (uchar *) FARMALLOC(new_spec_data_size)) == NULL) {
              /* Error allocating memory */
              printf("\nform_c::add_listbox_entry() error : Unable to allocate memory for listbox entry.\n");
              exit(1);
            }
            /* Copy entry specific data */
            FARMEMCPY(listbox->entrys[listbox->num_entrys - 1].spec_data, new_spec_data, new_spec_data_size);
            listbox->entrys[listbox->num_entrys - 1].spec_data_size = new_spec_data_size;
          }
            else
          {
            /* No specific data to copy */
            listbox->entrys[listbox->num_entrys - 1].spec_data = NULL;
            listbox->entrys[listbox->num_entrys - 1].spec_data_size = 0;
          }
          /* Copy title */
					strcpy(listbox->entrys[listbox->num_entrys - 1].str, new_str);
          if (strlen(new_str) > listbox->longest_entry) listbox->longest_entry = strlen(new_str);
					return;
				}
			}
		}

void form_c::add_radiobutton(ushort new_id, uchar new_title_x, uchar new_title_y,
                             char *new_title, uchar new_boxtopx, uchar new_boxtopy,
														 uchar new_boxbotx, uchar new_boxboty)
		{
			field_radiobutton_t FAR * radiobutton;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_radiobutton() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_RADIOBUTTON;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_radiobutton_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_radiobutton() error : Unable to allocate memory for radiobutton field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			radiobutton = (field_radiobutton_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((radiobutton->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_radiobutton() error : Unable to allocate memory for radiobutton title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			radiobutton->title_x = new_title_x;
			radiobutton->title_y = new_title_y;
			strcpy(radiobutton->title, new_title);
			radiobutton->boxtopx = new_boxtopx;
			radiobutton->boxtopy = new_boxtopy;
			radiobutton->boxbotx = new_boxbotx;
			radiobutton->boxboty = new_boxboty;
			radiobutton->entrys = NULL;
			radiobutton->num_entrys = 0;
			radiobutton->cur_entrys = 1;
		}

void form_c::add_radiobutton_entry(ushort radiobutton_id, uchar new_id, uchar new_x,
                                   uchar new_y, char *new_title, uchar new_cur)
		{
			field_radiobutton_t FAR * radiobutton; // Pointer to radiobutton information
			ushort i;

			/* Search for radiobutton with the correct id */
			for (i = 0; i < fields_max; i++) {
				/* Does this field have the right id */
				if (fields[i].id == radiobutton_id) {
					/* Convert void data pointer to radiobutton pointer */
					radiobutton = (field_radiobutton_t FAR *) fields[i].data;
					/* Allocate another radiobutton entry */
					radiobutton->num_entrys++;
					if ((radiobutton->entrys = (field_radiobutton_entry_t FAR *) FARREALLOC(radiobutton->entrys, sizeof(field_radiobutton_entry_t) * radiobutton->num_entrys)) == NULL) {
						/* Error allocating memory */
						printf("\nform_c::add_radiobutton_entry() error : Unable to allocate memory for radiobutton entry.\n");
						exit(1);
					}
					/* Initialize radiobutton entry */
					radiobutton->entrys[radiobutton->num_entrys - 1].id = new_id;
					radiobutton->entrys[radiobutton->num_entrys - 1].x = new_x;
					radiobutton->entrys[radiobutton->num_entrys - 1].y = new_y;
					/* Allocate memory for entry title */
          if ((radiobutton->entrys[radiobutton->num_entrys - 1].title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
						/* Error allocating memory */
						printf("\nform_c::add_radiobutton_entry() error : Unable to allocate memory for radiobutton entry title.\n");
						exit(1);
					}
					/* Copy title */
					strcpy(radiobutton->entrys[radiobutton->num_entrys - 1].title, new_title);
					/* See if this is the new current entry */
					if (new_cur) radiobutton->cur_entrys = new_cur;
					return;
				}
			}
		}


void form_c::add_string(ushort new_id, uchar new_title_x, uchar new_title_y,
                        char *new_title, uchar new_entry_x, uchar new_entry_y,
												uchar new_view_len, uchar new_max_len)
		{
			field_string_t FAR * string;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_string() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_STRING;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_string_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_string() error : Unable to allocate memory for string field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			string = (field_string_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((string->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_string() error : Unable to allocate memory for string title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			string->title_x = new_title_x;
			string->title_y = new_title_y;
			strcpy(string->title, new_title);
			string->entry_x = new_entry_x;
			string->entry_y = new_entry_y;
			string->view_len = new_view_len;
			string->max_len = new_max_len;
			string->str = NULL;
		}

void form_c::add_control_string(ushort new_id, uchar new_title_x, uchar new_title_y,
                                char *new_title, char *new_ctrl_str, uchar new_entry_x, uchar new_entry_y,
																uchar new_view_len, uchar new_max_len)
		{
			field_control_string_t FAR * control_string;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_control_string() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_CONTROL_STRING;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_control_string_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_control_string() error : Unable to allocate memory for control_string field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			control_string = (field_control_string_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((control_string->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_control_string() error : Unable to allocate memory for control_string title.\n");
				exit(1);
			}

			/* Allocate memory for control string */
      if ((control_string->ctrl_str = (char *) malloc(strlen(new_ctrl_str) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_control_string() error : Unable to allocate memory for control string.\n");
				exit(1);
			}

			/* Initialize field specific data */
			control_string->title_x = new_title_x;
			control_string->title_y = new_title_y;
			strcpy(control_string->title, new_title);
			strcpy(control_string->ctrl_str, new_ctrl_str);
			control_string->entry_x = new_entry_x;
			control_string->entry_y = new_entry_y;
			control_string->view_len = new_view_len;
			control_string->max_len = new_max_len;
			control_string->str = NULL;
		}


void form_c::add_toggle_uchar(ushort new_id, uchar new_title_x, uchar new_title_y,
                              char *new_title, uchar new_entry_x, uchar new_entry_y)
		{
			field_toggle_uchar_t FAR * field_toggle_uchar;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_uchar() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_TOGGLE_UCHAR;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_toggle_uchar_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_uchar() error : Unable to allocate memory for toggle_uchar field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			field_toggle_uchar = (field_toggle_uchar_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_toggle_uchar->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_toggle_uchar() error : Unable to allocate memory for field_toggle_uchar title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_toggle_uchar->title_x = new_title_x;
			field_toggle_uchar->title_y = new_title_y;
			strcpy(field_toggle_uchar->title, new_title);
			field_toggle_uchar->entry_x = new_entry_x;
			field_toggle_uchar->entry_y = new_entry_y;
			field_toggle_uchar->t = NULL;
		}

void form_c::add_toggle_uchar_bit(ushort new_id, uchar new_title_x, uchar new_title_y,
                                  char *new_title, uchar new_entry_x, uchar new_entry_y,
																	uchar new_bit)
		{
			field_toggle_uchar_bit_t FAR * field_toggle_uchar_bit;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_uchar_bit() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_TOGGLE_UCHAR;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_toggle_uchar_bit_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_uchar_bit() error : Unable to allocate memory for toggle_uchar_bit field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			field_toggle_uchar_bit = (field_toggle_uchar_bit_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_toggle_uchar_bit->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_toggle_uchar_bit() error : Unable to allocate memory for field_toggle_uchar_bit title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_toggle_uchar_bit->title_x = new_title_x;
			field_toggle_uchar_bit->title_y = new_title_y;
			strcpy(field_toggle_uchar_bit->title, new_title);
			field_toggle_uchar_bit->entry_x = new_entry_x;
			field_toggle_uchar_bit->entry_y = new_entry_y;
			field_toggle_uchar_bit->t = NULL;
			field_toggle_uchar_bit->bit = new_bit;
		}

void form_c::add_toggle_ushort_bit(ushort new_id, uchar new_title_x, uchar new_title_y,
                                   char *new_title, uchar new_entry_x, uchar new_entry_y,
																	 ushort new_bit)
		{
			field_toggle_ushort_bit_t FAR * field_toggle_ushort_bit;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_ushort_bit() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_TOGGLE_UCHAR;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_toggle_ushort_bit_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_ushort_bit() error : Unable to allocate memory for toggle_ushort_bit field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			field_toggle_ushort_bit = (field_toggle_ushort_bit_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_toggle_ushort_bit->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_toggle_ushort_bit() error : Unable to allocate memory for field_toggle_ushort_bit title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_toggle_ushort_bit->title_x = new_title_x;
			field_toggle_ushort_bit->title_y = new_title_y;
			strcpy(field_toggle_ushort_bit->title, new_title);
			field_toggle_ushort_bit->entry_x = new_entry_x;
			field_toggle_ushort_bit->entry_y = new_entry_y;
			field_toggle_ushort_bit->t = NULL;
			field_toggle_ushort_bit->bit = new_bit;
		}

void form_c::add_toggle_ulong_bit(ushort new_id, uchar new_title_x, uchar new_title_y,
                                  char *new_title, uchar new_entry_x, uchar new_entry_y,
																	ulong new_bit)
		{
			field_toggle_ulong_bit_t FAR * field_toggle_ulong_bit;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_ulong_bit() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_TOGGLE_UCHAR;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_toggle_ulong_bit_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_toggle_ulong_bit() error : Unable to allocate memory for toggle_ulong_bit field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			field_toggle_ulong_bit = (field_toggle_ulong_bit_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_toggle_ulong_bit->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_toggle_ulong_bit() error : Unable to allocate memory for field_toggle_ulong_bit title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_toggle_ulong_bit->title_x = new_title_x;
			field_toggle_ulong_bit->title_y = new_title_y;
			strcpy(field_toggle_ulong_bit->title, new_title);
			field_toggle_ulong_bit->entry_x = new_entry_x;
			field_toggle_ulong_bit->entry_y = new_entry_y;
			field_toggle_ulong_bit->t = NULL;
			field_toggle_ulong_bit->bit = new_bit;
		}


void form_c::add_hhmmss(ushort new_id, uchar new_title_x, uchar new_title_y,
                        char *new_title, uchar new_entry_x, uchar new_entry_y)
		{
			field_hhmmss_t FAR * field_hhmmss;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_hhmmss() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_HHMMSS;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_hhmmss_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_hhmmss() error : Unable to allocate memory for hhmmss field.\n");
				exit(1);
			}

			/* Convert void field pointer to hhmmss pointer */
			field_hhmmss = (field_hhmmss_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_hhmmss->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_hhmmss() error : Unable to allocate memory for hhmmss title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_hhmmss->title_x = new_title_x;
			field_hhmmss->title_y = new_title_y;
			strcpy(field_hhmmss->title, new_title);
			field_hhmmss->entry_x = new_entry_x;
			field_hhmmss->entry_y = new_entry_y;
			field_hhmmss->t = NULL;
		}

void form_c::add_mmddyyyy(ushort new_id, uchar new_title_x, uchar new_title_y,
                          char *new_title, uchar new_entry_x, uchar new_entry_y)
		{
			field_mmddyyyy_t FAR * field_mmddyyyy;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_mmddyyyy() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_MMDDYYYY;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_mmddyyyy_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_mmddyyyy() error : Unable to allocate memory for hhmmss field.\n");
				exit(1);
			}

			/* Convert void field pointer to hhmmss pointer */
			field_mmddyyyy = (field_mmddyyyy_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_mmddyyyy->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_field_mmddyyyy() error : Unable to allocate memory for mmddyyyy title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			field_mmddyyyy->title_x = new_title_x;
			field_mmddyyyy->title_y = new_title_y;
			strcpy(field_mmddyyyy->title, new_title);
			field_mmddyyyy->entry_x = new_entry_x;
			field_mmddyyyy->entry_y = new_entry_y;
			field_mmddyyyy->d = NULL;
		}

void form_c::add_4d_addr(ushort new_id, uchar new_title_x, uchar new_title_y,
                         char *new_title, uchar new_entry_x, uchar new_entry_y,
												 uchar new_view_len)
		{
			field_faddr_t FAR * field_faddr;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_4d_addr() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_4D_ADDR;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_faddr_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_4d_addr() error : Unable to allocate memory for 4d address field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			field_faddr = (field_faddr_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((field_faddr->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_4d_addr() error : Unable to allocate memory for string title.\n");
				exit(1);
			}

			/* Make sure view length isn't greater than what it can be */
			if (new_view_len > 23) new_view_len = 23;

			/* Initialize field specific data */
			field_faddr->title_x = new_title_x;
			field_faddr->title_y = new_title_y;
			strcpy(field_faddr->title, new_title);
			field_faddr->entry_x = new_entry_x;
			field_faddr->entry_y = new_entry_y;
			field_faddr->view_len = new_view_len;
			field_faddr->addr = NULL;
		}

void form_c::add_upcase_string(ushort new_id, uchar new_title_x, uchar new_title_y,
                               char *new_title, uchar new_entry_x, uchar new_entry_y,
															 uchar new_view_len, uchar new_max_len)
		{
			field_string_t FAR * string;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_upcase_string() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_UPCASE_STRING;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_string_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_upcase_string() error : Unable to allocate memory for string field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			string = (field_string_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((string->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_upcase_string() error : Unable to allocate memory for string title.\n");
				exit(1);
			}

			/* Initialize field specific data */
			string->title_x = new_title_x;
			string->title_y = new_title_y;
			strcpy(string->title, new_title);
			string->entry_x = new_entry_x;
			string->entry_y = new_entry_y;
			string->view_len = new_view_len;
			string->max_len = new_max_len;
			string->str = NULL;
		}

void form_c::add_format_string(ushort new_id, uchar new_title_x, uchar new_title_y,
                               char *new_title, char *new_ctrl_str, uchar new_entry_x, uchar new_entry_y)
		{
			field_frmt_string_t FAR * frmt_string;

			/* Allocate another field */
			fields_max++;
			if ((fields = (form_field_t FAR *) FARREALLOC(fields, sizeof(form_field_t) * fields_max)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_format_string() error : Unable to reallocate fields.\n");
				exit(1);
			}
			/* Initialize generic field information */
			fields[fields_max - 1].id = new_id;
			fields[fields_max - 1].active = 1;
			fields[fields_max - 1].type = FIELD_FORMAT_STRING;

			/* Allocate memory for field specific data */
			if ((fields[fields_max - 1].data = (void FAR *) FARMALLOC(sizeof(field_frmt_string_t))) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_format_string() error : Unable to allocate memory for string field.\n");
				exit(1);
			}

			/* Convert void field pointer to button pointer */
			frmt_string = (field_frmt_string_t FAR *) fields[fields_max - 1].data;

			/* Allocate memory for title */
      if ((frmt_string->title = (char *) malloc(strlen(new_title) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_format_string() error : Unable to allocate memory for string title.\n");
				exit(1);
			}
			/* Allocate memory for control string */
      if ((frmt_string->ctrl_str = (char *) malloc(strlen(new_ctrl_str) + 1)) == NULL) {
				/* Error allocating memory */
				printf("\nform_c::add_format_string() error : Unable to allocate memory for control string.\n");
				exit(1);
			}

			/* Initialize field specific data */
			frmt_string->title_x = new_title_x;
			frmt_string->title_y = new_title_y;
			strcpy(frmt_string->title, new_title);
			strcpy(frmt_string->ctrl_str, new_ctrl_str);
			frmt_string->entry_x = new_entry_x;
			frmt_string->entry_y = new_entry_y;
			frmt_string->str = NULL;

			/* Find max length for control string */
			frmt_string->max_len = strlen(frmt_string->ctrl_str);
			/* Find minimum position for input */
			frmt_string->curr_pos = 1;
			while(((frmt_string->ctrl_str[frmt_string->curr_pos - 1] != '#') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'A') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'a')) && (frmt_string->curr_pos < frmt_string->max_len)) {
				frmt_string->curr_pos++;
			}
			frmt_string->min_pos = frmt_string->curr_pos;
			/* Find maximum position for input */
			frmt_string->curr_pos = frmt_string->max_len;
			while(((frmt_string->ctrl_str[frmt_string->curr_pos - 1] != '#') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'A') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'a')) && (frmt_string->curr_pos > 1)) {
				frmt_string->curr_pos--;
			}
			frmt_string->max_pos = frmt_string->curr_pos;
		}

void form_c::set_byte_var(ushort new_id, uchar *new_num, uchar edit)
		{
			ushort i;								// General counter
			field_byte_t FAR * field_byte;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_byte = (field_byte_t FAR *) fields[i].data;
					field_byte->num = new_num;
					if (!edit) *field_byte->num = field_byte->min_num;
					return;
				}
			}
		}

void form_c::set_toggle_uchar_var(ushort new_id, uchar *new_t, uchar edit)
		{
			ushort i;								// General counter
			field_toggle_uchar_t FAR * field_toggle_uchar;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_toggle_uchar = (field_toggle_uchar_t FAR *) fields[i].data;
					field_toggle_uchar->t = new_t;
					if (!edit) *field_toggle_uchar->t = 0;
					return;
				}
			}
		}

void form_c::set_toggle_uchar_bit_var(ushort new_id, uchar *new_t, uchar edit)
		{
			ushort i;								// General counter
			field_toggle_uchar_bit_t FAR * field_toggle_uchar_bit;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_toggle_uchar_bit = (field_toggle_uchar_bit_t FAR *) fields[i].data;
					field_toggle_uchar_bit->t = new_t;
					if (!edit) *field_toggle_uchar_bit->t &= ~field_toggle_uchar_bit->bit;
					return;
				}
			}
		}

void form_c::set_toggle_ushort_bit_var(ushort new_id, ushort *new_t, uchar edit)
		{
			ushort i;								// General counter
			field_toggle_ushort_bit_t FAR * field_toggle_ushort_bit;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_toggle_ushort_bit = (field_toggle_ushort_bit_t FAR *) fields[i].data;
					field_toggle_ushort_bit->t = new_t;
					if (!edit) *field_toggle_ushort_bit->t &= ~field_toggle_ushort_bit->bit;
					return;
				}
			}
		}

void form_c::set_toggle_ulong_bit_var(ushort new_id, ulong *new_t, uchar edit)
		{
			ushort i;								// General counter
			field_toggle_ulong_bit_t FAR * field_toggle_ulong_bit;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_toggle_ulong_bit = (field_toggle_ulong_bit_t FAR *) fields[i].data;
					field_toggle_ulong_bit->t = new_t;
					if (!edit) *field_toggle_ulong_bit->t &= ~field_toggle_ulong_bit->bit;
					return;
				}
			}
		}

void form_c::set_ushort_var(ushort new_id, ushort *new_num, uchar edit)
		{
			ushort i;								// General counter
			field_ushort_t FAR * field_ushort;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_ushort = (field_ushort_t FAR *) fields[i].data;
					field_ushort->num = new_num;
					if (!edit) *field_ushort->num = field_ushort->min_num;
					return;
				}
			}
		}

void form_c::set_ulong_var(ushort new_id, ulong *new_num, uchar edit)
		{
			ushort i;								// General counter
			field_ulong_t FAR * field_ulong;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_ulong = (field_ulong_t FAR *) fields[i].data;
					field_ulong->num = new_num;
					if (!edit) *field_ulong->num = field_ulong->min_num;
					return;
				}
			}
		}

void form_c::set_char_var(ushort new_id, char *new_num, uchar edit)
		{
			ushort i;								// General counter
			field_char_t FAR * field_char;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_char = (field_char_t FAR *) fields[i].data;
					field_char->num = new_num;
					if (!edit) *field_char->num = field_char->min_num;
					return;
				}
			}
		}


void form_c::set_int_var(ushort new_id, int *new_num, uchar edit)
		{
			ushort i;								// General counter
			field_int_t FAR * field_int;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_int = (field_int_t FAR *) fields[i].data;
					field_int->num = new_num;
					if (!edit) *field_int->num = field_int->min_num;
					return;
				}
			}
		}

void form_c::set_long_var(ushort new_id, long *new_num, uchar edit)
		{
			ushort i;								// General counter
			field_long_t FAR * field_long;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to byte pointer and set number pointer */
					field_long = (field_long_t FAR *) fields[i].data;
					field_long->num = new_num;
					if (!edit) *field_long->num = field_long->min_num;
					return;
				}
			}
		}

void form_c::set_string_var(ushort new_id, char *new_str, uchar edit)
		{
			ushort i;								// General counter
			field_string_t FAR * string;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to string pointer and set string pointer */
					string = (field_string_t FAR *) fields[i].data;
					string->str = new_str;
					if (!edit) strcpy(string->str, "");
					return;
				}
			}
		}

void form_c::set_control_string_var(ushort new_id, char *new_str, uchar edit)
		{
			ushort i;								// General counter
			field_control_string_t FAR * control_string;	// Pointer to control_string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to control_string pointer and set control_string pointer */
					control_string = (field_control_string_t FAR *) fields[i].data;
					control_string->str = new_str;
					if (!edit) strcpy(control_string->str, "");
					return;
				}
			}
		}

void form_c::set_hhmmss_var(ushort new_id, ulong *new_t, uchar edit)
		{
			ushort i;								// General counter
			field_hhmmss_t FAR * field_hhmmss;	// Pointer to field_hhmmss information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to field_hhmmss pointer and set field_hhmmss pointer */
					field_hhmmss = (field_hhmmss_t FAR *) fields[i].data;
					field_hhmmss->t = new_t;
					if (!edit) *field_hhmmss->t = 0;
					return;
				}
			}
		}

void form_c::set_mmddyyyy_var(ushort new_id, ulong *new_d, uchar edit)
		{
			ushort i;								// General counter
			field_mmddyyyy_t FAR * field_mmddyyyy;	// Pointer to field_mmddyyyy information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to field_mmddyyyy pointer and set field_mmddyyyy pointer */
					field_mmddyyyy = (field_mmddyyyy_t FAR *) fields[i].data;
					field_mmddyyyy->d = new_d;
					if (!edit) *field_mmddyyyy->d = 0;
					return;
				}
			}
		}

void form_c::set_4d_addr_var(ushort new_id, faddr_t *new_addr, uchar edit)
		{
			ushort i;								// General counter
			field_faddr_t FAR * field_faddr;	// Pointer to field_faddr information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to field_faddr pointer and set field_faddr pointer */
					field_faddr = (field_faddr_t FAR *) fields[i].data;
					field_faddr->addr = new_addr;
					if (!edit) {
						field_faddr->addr->zone = 0;
						field_faddr->addr->net = 0;
						field_faddr->addr->node = 0;
						field_faddr->addr->point = 0;
					}
					return;
				}
			}
		}

void form_c::set_upcase_string_var(ushort new_id, char *new_str, uchar edit)
		{
			ushort i;								// General counter
			field_string_t FAR * string;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to string pointer and set string pointer */
					string = (field_string_t FAR *) fields[i].data;
					string->str = new_str;
					if (!edit) strcpy(string->str, "");
					return;
				}
			}
		}

void form_c::set_format_string_var(ushort new_id, char *new_str, uchar edit)
		{
			ushort i;													// General counter
			field_frmt_string_t FAR * frmt_string;	// Pointer to string information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == new_id) {
					/* Found field.  Convert void pointer to string pointer and set string pointer */
					frmt_string = (field_frmt_string_t FAR *) fields[i].data;
					frmt_string->str = new_str;
					if (!edit) strcpy(frmt_string->str, "");
					/* Find first point to edit */
					frmt_string->curr_pos = strlen(frmt_string->str);
					/* If current position is one of the valid characters than increase by one */
					if ((frmt_string->ctrl_str[frmt_string->curr_pos - 1] == '#') || (frmt_string->ctrl_str[frmt_string->curr_pos - 1] == 'A') || (frmt_string->ctrl_str[frmt_string->curr_pos - 1] == 'a')) frmt_string->curr_pos++;
					/* Find a position if current position is valid */
					while(((frmt_string->ctrl_str[frmt_string->curr_pos - 1] != '#') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'A') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'a')) && (frmt_string->curr_pos < frmt_string->max_len)) {
						frmt_string->curr_pos++;
					}
					/* If current position is not a valid position and it is at the end seek for a current position to the left */
					if (frmt_string->curr_pos <= frmt_string->max_len) {
						while(((frmt_string->ctrl_str[frmt_string->curr_pos - 1] != '#') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'A') && (frmt_string->ctrl_str[frmt_string->curr_pos - 1] != 'a')) && (frmt_string->curr_pos > 1)) {
							frmt_string->curr_pos--;
						}
					}
					/* If current_position is at the end or still not a valid position increase by one */
					if ((frmt_string->curr_pos == frmt_string->max_pos)) frmt_string->curr_pos++;
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
					return;
				}
			}
		}

void _export form_set_default_colors(uchar button_norm_fcolor,          uchar button_norm_bcolor,
                                     uchar button_high_fcolor,          uchar button_high_bcolor,
                                     uchar button_inactive_fcolor,      uchar button_inactive_bcolor,
                                     uchar button_shadow_fcolor,        uchar button_shadow_bcolor,
                                     uchar field_norm_fcolor,           uchar field_norm_bcolor,
                                     uchar field_high_fcolor,           uchar field_high_bcolor,
                                     uchar field_inactive_fcolor,       uchar field_inactive_bcolor,
                                     uchar string_norm_fcolor,          uchar string_norm_bcolor,
                                     uchar string_high_fcolor,          uchar string_high_bcolor,
                                     uchar string_inactive_fcolor,      uchar string_inactive_bcolor,
                                     uchar number_norm_fcolor,          uchar number_norm_bcolor,
                                     uchar number_high_fcolor,          uchar number_high_bcolor,
                                     uchar number_inactive_fcolor,      uchar number_inactive_bcolor,
                                     uchar checkbox_norm_fcolor,        uchar checkbox_norm_bcolor,
                                     uchar checkbox_high_fcolor,        uchar checkbox_high_bcolor,
                                     uchar checkbox_inactive_fcolor,    uchar checkbox_inactive_bcolor,
                                     uchar checkbox_box_bcolor,         uchar radiobutton_norm_fcolor,
                                     uchar radiobutton_norm_bcolor,     uchar radiobutton_high_fcolor,
                                     uchar radiobutton_high_bcolor,     uchar radiobutton_inactive_fcolor,
                                     uchar radiobutton_inactive_bcolor, uchar radiobutton_box_bcolor,
                                     uchar listbox_norm_fcolor,         uchar listbox_norm_bcolor,
                                     uchar listbox_high_fcolor,         uchar listbox_high_bcolor,
                                     uchar listbox_bcolor,              uchar listbox_scroll_fcolor,
                                     uchar listbox_scroll_bcolor)
    {
			/* Copy new default colors */
      ti2_form_default_colors.button_norm_fcolor = button_norm_fcolor;
      ti2_form_default_colors.button_norm_bcolor = button_norm_bcolor;
      ti2_form_default_colors.button_high_fcolor = button_high_fcolor;
      ti2_form_default_colors.button_high_bcolor = button_high_bcolor;
			ti2_form_default_colors.button_inactive_fcolor = button_inactive_fcolor;
      ti2_form_default_colors.button_inactive_bcolor = button_inactive_bcolor;
      ti2_form_default_colors.button_shadow_fcolor = button_shadow_fcolor;
      ti2_form_default_colors.button_shadow_bcolor = button_shadow_bcolor;
      ti2_form_default_colors.field_norm_fcolor = field_norm_fcolor;
      ti2_form_default_colors.field_norm_bcolor = field_norm_bcolor;
			ti2_form_default_colors.field_high_fcolor = field_high_fcolor;
      ti2_form_default_colors.field_high_bcolor = field_high_bcolor;
      ti2_form_default_colors.field_inactive_fcolor = field_inactive_fcolor;
      ti2_form_default_colors.field_inactive_bcolor = field_inactive_bcolor;
      ti2_form_default_colors.string_norm_fcolor = string_norm_fcolor;
      ti2_form_default_colors.string_norm_bcolor = string_norm_bcolor;
      ti2_form_default_colors.string_high_fcolor = string_high_fcolor;
      ti2_form_default_colors.string_high_bcolor = string_high_bcolor;
      ti2_form_default_colors.string_inactive_fcolor = string_inactive_fcolor;
			ti2_form_default_colors.string_inactive_bcolor = string_inactive_bcolor;
      ti2_form_default_colors.number_norm_fcolor = number_norm_fcolor;
      ti2_form_default_colors.number_norm_bcolor = number_norm_bcolor;
			ti2_form_default_colors.number_high_fcolor = number_high_fcolor;
      ti2_form_default_colors.number_high_bcolor = number_high_bcolor;
      ti2_form_default_colors.number_inactive_fcolor = number_inactive_fcolor;
      ti2_form_default_colors.number_inactive_bcolor = number_inactive_bcolor;
      ti2_form_default_colors.checkbox_norm_fcolor = checkbox_norm_fcolor;
      ti2_form_default_colors.checkbox_norm_bcolor = checkbox_norm_bcolor;
			ti2_form_default_colors.checkbox_high_fcolor = checkbox_high_fcolor;
      ti2_form_default_colors.checkbox_high_bcolor = checkbox_high_bcolor;
      ti2_form_default_colors.checkbox_inactive_fcolor = checkbox_inactive_fcolor;
      ti2_form_default_colors.checkbox_inactive_bcolor = checkbox_inactive_bcolor;
      ti2_form_default_colors.checkbox_box_bcolor = checkbox_box_bcolor;
      ti2_form_default_colors.radiobutton_norm_fcolor = radiobutton_norm_fcolor;
      ti2_form_default_colors.radiobutton_norm_bcolor = radiobutton_norm_bcolor;
      ti2_form_default_colors.radiobutton_high_fcolor = radiobutton_high_fcolor;
      ti2_form_default_colors.radiobutton_high_bcolor = radiobutton_high_bcolor;
      ti2_form_default_colors.radiobutton_inactive_fcolor = radiobutton_inactive_fcolor;
      ti2_form_default_colors.radiobutton_inactive_bcolor = radiobutton_inactive_bcolor;
      ti2_form_default_colors.radiobutton_box_bcolor = radiobutton_box_bcolor;
      ti2_form_default_colors.listbox_norm_fcolor   = listbox_norm_fcolor;
      ti2_form_default_colors.listbox_norm_bcolor   = listbox_norm_bcolor;
      ti2_form_default_colors.listbox_high_fcolor   = listbox_high_fcolor;
      ti2_form_default_colors.listbox_high_bcolor   = listbox_high_bcolor;
      ti2_form_default_colors.listbox_bcolor        = listbox_bcolor;
      ti2_form_default_colors.listbox_scroll_fcolor = listbox_scroll_fcolor;
      ti2_form_default_colors.listbox_scroll_bcolor = listbox_scroll_bcolor;
    }

/************************
 * Form Class Functions *
 ************************/

void form_c::default_colors(void)
    {
			/* Copy default colors to colors for this form */
      button_norm_fcolor          = ti2_form_default_colors.button_norm_fcolor;
      button_norm_bcolor          = ti2_form_default_colors.button_norm_bcolor;
      button_high_fcolor          = ti2_form_default_colors.button_high_fcolor;
      button_high_bcolor          = ti2_form_default_colors.button_high_bcolor;
      button_inactive_fcolor      = ti2_form_default_colors.button_inactive_fcolor;
      button_inactive_bcolor      = ti2_form_default_colors.button_inactive_bcolor;
      button_shadow_fcolor        = ti2_form_default_colors.button_shadow_fcolor;
      button_shadow_bcolor        = ti2_form_default_colors.button_shadow_bcolor;
      field_norm_fcolor           = ti2_form_default_colors.field_norm_fcolor;
      field_norm_bcolor           = ti2_form_default_colors.field_norm_bcolor;
			field_high_fcolor           = ti2_form_default_colors.field_high_fcolor;
			field_high_bcolor           = ti2_form_default_colors.field_high_bcolor;
      field_inactive_fcolor       = ti2_form_default_colors.field_inactive_fcolor;
      field_inactive_bcolor       = ti2_form_default_colors.field_inactive_bcolor;
      string_norm_fcolor          = ti2_form_default_colors.string_norm_fcolor;
			string_norm_bcolor          = ti2_form_default_colors.string_norm_bcolor;
			string_high_fcolor          = ti2_form_default_colors.string_high_fcolor;
      string_high_bcolor          = ti2_form_default_colors.string_high_bcolor;
      string_inactive_fcolor      = ti2_form_default_colors.string_inactive_fcolor;
			string_inactive_bcolor      = ti2_form_default_colors.string_inactive_bcolor;
      number_norm_fcolor          = ti2_form_default_colors.number_norm_fcolor;
      number_norm_bcolor          = ti2_form_default_colors.number_norm_bcolor;
      number_high_fcolor          = ti2_form_default_colors.number_high_fcolor;
      number_high_bcolor          = ti2_form_default_colors.number_high_bcolor;
      number_inactive_fcolor      = ti2_form_default_colors.number_inactive_fcolor;
      number_inactive_bcolor      = ti2_form_default_colors.number_inactive_bcolor;
      checkbox_norm_fcolor        = ti2_form_default_colors.checkbox_norm_fcolor;
      checkbox_norm_bcolor        = ti2_form_default_colors.checkbox_norm_bcolor;
      checkbox_high_fcolor        = ti2_form_default_colors.checkbox_high_fcolor;
      checkbox_high_bcolor        = ti2_form_default_colors.checkbox_high_bcolor;
			checkbox_inactive_fcolor    = ti2_form_default_colors.checkbox_inactive_fcolor;
			checkbox_inactive_bcolor    = ti2_form_default_colors.checkbox_inactive_bcolor;
      checkbox_box_bcolor         = ti2_form_default_colors.checkbox_box_bcolor;
			radiobutton_norm_fcolor     = ti2_form_default_colors.radiobutton_norm_fcolor;
			radiobutton_norm_bcolor     = ti2_form_default_colors.radiobutton_norm_bcolor;
			radiobutton_high_fcolor     = ti2_form_default_colors.radiobutton_high_fcolor;
			radiobutton_high_bcolor     = ti2_form_default_colors.radiobutton_high_bcolor;
			radiobutton_inactive_fcolor = ti2_form_default_colors.radiobutton_inactive_fcolor;
			radiobutton_inactive_bcolor = ti2_form_default_colors.radiobutton_inactive_bcolor;
			radiobutton_box_bcolor      = ti2_form_default_colors.radiobutton_box_bcolor;
      listbox_norm_fcolor         = ti2_form_default_colors.listbox_norm_fcolor;
      listbox_norm_bcolor         = ti2_form_default_colors.listbox_norm_bcolor;
      listbox_high_fcolor         = ti2_form_default_colors.listbox_high_fcolor;
      listbox_high_bcolor         = ti2_form_default_colors.listbox_high_bcolor;
			listbox_box_bcolor          = ti2_form_default_colors.listbox_bcolor;
      listbox_scroll_fcolor       = ti2_form_default_colors.listbox_scroll_fcolor;
      listbox_scroll_bcolor       = ti2_form_default_colors.listbox_scroll_bcolor;
    }

void form_c::create(win_c FAR *new_win)
		{
			 /* Initialize all variables */
			 fields_cur = 1;
			 fields_max = 0;
			 chars_max	= 0;
			 fields     = NULL;
			 chars   		= NULL;
			 win			  = new_win;
		}

void form_c::button_colors(uchar new_button_norm_fcolor, uchar new_button_norm_bcolor,
													 uchar new_button_high_fcolor, uchar new_button_high_bcolor,
													 uchar new_button_inactive_fcolor, uchar new_button_inactive_bcolor,
													 uchar new_button_shadow_fcolor, uchar new_button_shadow_bcolor)
		{
			/* Initialize all button color variables */
			button_norm_fcolor     = new_button_norm_fcolor;
			button_norm_bcolor     = new_button_norm_bcolor;
			button_high_fcolor     = new_button_high_fcolor;
			button_high_bcolor     = new_button_high_bcolor;
			button_inactive_fcolor = new_button_inactive_fcolor;
			button_inactive_bcolor = new_button_inactive_bcolor;
			button_shadow_fcolor   = new_button_shadow_fcolor;
			button_shadow_bcolor   = new_button_shadow_bcolor;
		}

void form_c::field_colors(uchar new_field_norm_fcolor, uchar new_field_norm_bcolor,
													uchar new_field_high_fcolor, uchar new_field_high_bcolor,
													uchar new_field_inactive_fcolor, uchar new_field_inactive_bcolor)
		{
			/* Initialize all field color variables */
			field_norm_fcolor     = new_field_norm_fcolor;
			field_norm_bcolor     = new_field_norm_bcolor;
			field_high_fcolor     = new_field_high_fcolor;
			field_high_bcolor     = new_field_high_bcolor;
			field_inactive_fcolor = new_field_inactive_fcolor;
			field_inactive_bcolor = new_field_inactive_bcolor;
		}

void form_c::string_colors(uchar new_string_norm_fcolor, uchar new_string_norm_bcolor,
													 uchar new_string_high_fcolor, uchar new_string_high_bcolor,
													 uchar new_string_inactive_fcolor, uchar new_string_inactive_bcolor)
		{
			/* Initialize all string color variables */
			string_norm_fcolor     = new_string_norm_fcolor;
			string_norm_bcolor     = new_string_norm_bcolor;
			string_high_fcolor     = new_string_high_fcolor;
			string_high_bcolor     = new_string_high_bcolor;
			string_inactive_fcolor = new_string_inactive_fcolor;
			string_inactive_bcolor = new_string_inactive_bcolor;
		}

void form_c::number_colors(uchar new_number_norm_fcolor, uchar new_number_norm_bcolor,
													 uchar new_number_high_fcolor, uchar new_number_high_bcolor,
													 uchar new_number_inactive_fcolor, uchar new_number_inactive_bcolor)
		{
			/* Initialize all number color variables */
			number_norm_fcolor     = new_number_norm_fcolor;
			number_norm_bcolor     = new_number_norm_bcolor;
			number_high_fcolor     = new_number_high_fcolor;
			number_high_bcolor     = new_number_high_bcolor;
			number_inactive_fcolor = new_number_inactive_fcolor;
			number_inactive_bcolor = new_number_inactive_bcolor;
		}

void form_c::checkbox_colors(uchar new_checkbox_norm_fcolor, uchar new_checkbox_norm_bcolor,
													 uchar new_checkbox_high_fcolor, uchar new_checkbox_high_bcolor,
													 uchar new_checkbox_inactive_fcolor, uchar new_checkbox_inactive_bcolor,
													 uchar new_checkbox_box_bcolor)
		{
			/* Initialize all checkbox color variables */
			checkbox_norm_fcolor     = new_checkbox_norm_fcolor;
			checkbox_norm_bcolor     = new_checkbox_norm_bcolor;
			checkbox_high_fcolor     = new_checkbox_high_fcolor;
			checkbox_high_bcolor     = new_checkbox_high_bcolor;
			checkbox_inactive_fcolor = new_checkbox_inactive_fcolor;
			checkbox_inactive_bcolor = new_checkbox_inactive_bcolor;
			checkbox_box_bcolor			 = new_checkbox_box_bcolor;
		}

void form_c::radiobutton_colors(uchar new_radiobutton_norm_fcolor, uchar new_radiobutton_norm_bcolor,
																uchar new_radiobutton_high_fcolor, uchar new_radiobutton_high_bcolor,
																uchar new_radiobutton_inactive_fcolor, uchar new_radiobutton_inactive_bcolor,
																uchar new_radiobutton_box_bcolor)
		{
			/* Initialize all radiobutton color variables */
			radiobutton_norm_fcolor     = new_radiobutton_norm_fcolor;
			radiobutton_norm_bcolor     = new_radiobutton_norm_bcolor;
			radiobutton_high_fcolor     = new_radiobutton_high_fcolor;
			radiobutton_high_bcolor     = new_radiobutton_high_bcolor;
			radiobutton_inactive_fcolor = new_radiobutton_inactive_fcolor;
			radiobutton_inactive_bcolor = new_radiobutton_inactive_bcolor;
			radiobutton_box_bcolor			= new_radiobutton_box_bcolor;
		}

void form_c::listbox_colors(uchar new_listbox_norm_fcolor, uchar new_listbox_norm_bcolor,
														uchar new_listbox_high_fcolor, uchar new_listbox_high_bcolor,
                            uchar new_listbox_box_bcolor, uchar new_listbox_scroll_fcolor,
                            uchar new_listbox_scroll_bcolor)
		{
			/* Initialize all listbox color variables */
			listbox_norm_fcolor = new_listbox_norm_fcolor;
			listbox_norm_bcolor = new_listbox_norm_bcolor;
			listbox_high_fcolor = new_listbox_high_fcolor;
			listbox_high_bcolor = new_listbox_high_bcolor;
			listbox_box_bcolor	= new_listbox_box_bcolor;
      listbox_scroll_fcolor = new_listbox_scroll_fcolor;
      listbox_scroll_bcolor = new_listbox_scroll_bcolor;
		}


uchar form_c::checkbox_entry_status(ushort field_id, uchar entry_id)
		{
			ushort i;		// General counter
			uchar j;    // General counter
			field_checkbox_t FAR * checkbox;		// Pointer to checkbox information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == field_id) {
					/* Found field.  Convert void pointer to checkbox pointer*/
					checkbox = (field_checkbox_t FAR *) fields[i].data;
					/* Search for entry with correct id */
					for (j = 0; j < checkbox->num_entrys; j++) {
						if (checkbox->entrys[j].id == entry_id) return(checkbox->entrys[j].checked);
					}
					/* Should get here unless search is unsuccessful */
					return(0);
				}
			}
			/* Should get here unless search is unsuccessful */
			return(0);
		}

void form_c::get_listbox_entry(ushort listbox_id, char *new_str, void FAR *new_spec_data)
    {
      field_listbox_t FAR *listbox;   // Pointer to listbox information
      ushort i;                       // General counter

      /* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
        if (fields[i].id == listbox_id) {
          /* Found field.  Convert void pointer to listbox pointer*/
          listbox = (field_listbox_t FAR *) fields[i].data;
					/* If pointer isn't NULL than return that information */
          if (new_str != NULL) {
            strcpy(new_str, listbox->entrys[listbox->cur_entrys - 1].str);
          }
					if (new_spec_data != NULL) {
            FARMEMCPY(new_spec_data, listbox->entrys[listbox->cur_entrys - 1].spec_data, listbox->entrys[listbox->cur_entrys - 1].spec_data_size);
          }
          return;
				}
      }
    }

uchar form_c::radiobutton_status(ushort field_id)
		{
			ushort i;		// General counter
			field_radiobutton_t FAR * radiobutton;		// Pointer to radiobutton information

			/* Search for field with the correct id */
			for (i = 0; i < fields_max; i++) {
				if (fields[i].id == field_id) {
					/* Found field.  Convert void pointer to radiobutton pointer*/
					radiobutton = (field_radiobutton_t FAR *) fields[i].data;
					/* Return the id of the highlighted entry */
					return(radiobutton->entrys[radiobutton->cur_entrys - 1].id);
				}
			}
			/* Should get here unless search is unsuccessful */
			return(0);
		}

void form_c::destroy(void)
		{
			ushort i;														// General counter
			uchar j;														// General counter
			field_ptr_t ptr;										// Pointers to fields information

			/* Free all memory for fields */
			for (i = 0; i < fields_max; i++) {
				switch(fields[i].type) {
					case FIELD_BUTTON : {
																/* Convert void field pointer to button pointer */
																ptr.button = (field_button_t FAR *) fields[i].data;
																/* Free memory for title */
																FREE(ptr.button->title);
																break;
															}
					case FIELD_BYTE : {
															/* Convert void filed pointer to byte pointer */
															ptr.field_byte = (field_byte_t FAR *) fields[i].data;
															/* Free memory for title */
															FREE(ptr.field_byte->title);
															break;
														}
					case FIELD_USHORT : {
																/* Convert void filed pointer to ushort pointer */
																ptr.field_ushort = (field_ushort_t FAR *) fields[i].data;
																/* Free memory for title */
																FREE(ptr.field_ushort->title);
																break;
															}
					case FIELD_ULONG : {
															 /* Convert void filed pointer to ulong pointer */
															 ptr.field_ulong = (field_ulong_t FAR *) fields[i].data;
															 /* Free memory for title */
															 FREE(ptr.field_ulong->title);
															 break;
														 }
					case FIELD_CHAR : {
															/* Convert void filed pointer to byte pointer */
															ptr.field_char = (field_char_t FAR *) fields[i].data;
															/* Free memory for title */
															FREE(ptr.field_char->title);
															break;
														}
					case FIELD_INT : {
														 /* Convert void filed pointer to byte pointer */
														 ptr.field_int = (field_int_t FAR *) fields[i].data;
														 /* Free memory for title */
														 FREE(ptr.field_int->title);
														 break;
													 }
					case FIELD_LONG : {
															/* Convert void filed pointer to byte pointer */
															ptr.field_long = (field_long_t FAR *) fields[i].data;
															/* Free memory for title */
															FREE(ptr.field_long->title);
															break;
														}
					case FIELD_UPCASE_STRING :
					case FIELD_STRING : {
																/* Convert void field pointer to string pointer */
																ptr.string = (field_string_t FAR *) fields[i].data;
																/* Free memory for string title */
																FREE(ptr.string->title);
																break;
															}
					case FIELD_CONTROL_STRING : {
																				/* Convert void field pointer to control_string pointer */
																				ptr.control_string = (field_control_string_t FAR *) fields[i].data;
																				/* Free memory for control_string title */
																				FREE(ptr.control_string->title);
																				/* Free memory for control string */
																				FREE(ptr.control_string->ctrl_str);
																				break;
																			}
					case FIELD_TOGGLE_UCHAR : {
																			/* Convert void field pointer to string pointer */
																			ptr.field_toggle_uchar = (field_toggle_uchar_t FAR *) fields[i].data;
																			/* Free memory for field_toggle_uchar title */
																			FREE(ptr.field_toggle_uchar->title);
																			break;
																		}
					case FIELD_TOGGLE_UCHAR_BIT : {
																					/* Convert void field pointer to string pointer */
																					ptr.field_toggle_uchar_bit = (field_toggle_uchar_bit_t FAR *) fields[i].data;
																					/* Free memory for field_toggle_uchar_bit title */
																					FREE(ptr.field_toggle_uchar_bit->title);
																					break;
																				}
					case FIELD_TOGGLE_USHORT_BIT : {
																					 /* Convert void field pointer to string pointer */
																					 ptr.field_toggle_ushort_bit = (field_toggle_ushort_bit_t FAR *) fields[i].data;
																					 /* Free memory for field_toggle_ushort_bit title */
																					 FREE(ptr.field_toggle_ushort_bit->title);
																					 break;
																				 }
					case FIELD_TOGGLE_ULONG_BIT : {
																					/* Convert void field pointer to string pointer */
																					ptr.field_toggle_ulong_bit = (field_toggle_ulong_bit_t FAR *) fields[i].data;
																					/* Free memory for field_toggle_ulong_bit title */
																					FREE(ptr.field_toggle_ulong_bit->title);
																					break;
																				}
					case FIELD_HHMMSS : {
																/* Convert void field pointer to hhmmss pointer */
																ptr.field_hhmmss = (field_hhmmss_t FAR *) fields[i].data;
																/* Free memory for string title */
																FREE(ptr.field_hhmmss->title);
																break;
															}
					case FIELD_MMDDYYYY : {
																	/* Convert void field pointer to hhmmss pointer */
																	ptr.field_mmddyyyy = (field_mmddyyyy_t FAR *) fields[i].data;
																	/* Free memory for string title */
																	FREE(ptr.field_mmddyyyy->title);
																	break;
																}
					case FIELD_4D_ADDR : {
																 /* Convert void field pointer to string pointer */
																 ptr.field_faddr = (field_faddr_t FAR *) fields[i].data;
																 /* Free memory for string title */
																 FREE(ptr.field_faddr->title);
																 break;
															 }
					case FIELD_FORMAT_STRING : {
																			 /* Convert void field pointer to format string pointer */
																			 ptr.frmt_string = (field_frmt_string_t FAR *) fields[i].data;
																			 /* Free memory for title and control string */
																			 FREE(ptr.frmt_string->title);
																			 FREE(ptr.frmt_string->ctrl_str);
																			 break;
																		 }
					case FIELD_CHECKBOX : {
																	/* Convert void field pointer to checkbox pointer */
																	ptr.checkbox = (field_checkbox_t FAR *) fields[i].data;
																	/* Free memory used by entries */
																	for (j = 0; j < ptr.checkbox->num_entrys; j++) {
																		FREE(ptr.checkbox->entrys[j].title);
																	}
																	/* Free array of entries */
																	FARFREE(ptr.checkbox->entrys);
																	/* Free title */
																	FREE(ptr.checkbox->title);
																	break;
																}
					case FIELD_LISTBOX : {
																 /* Convert void field pointer to listbox pointer */
																 ptr.listbox = (field_listbox_t FAR *) fields[i].data;
																 /* Free memory used by entries */
																 for (j = 0; j < ptr.listbox->num_entrys; j++) {
																	 FREE(ptr.listbox->entrys[j].str);
																 }
																 /* Free array of entries */
																 FARFREE(ptr.listbox->entrys);
																 /* Free title */
																 FREE(ptr.listbox->title);
																 break;
															 }
					case FIELD_RADIOBUTTON : {
																		 /* Convert void field pointer to radiobutton pointer */
																		 ptr.radiobutton = (field_radiobutton_t FAR *) fields[i].data;
																		 /* Free memory used by entries */
																		 for (j = 0; j < ptr.radiobutton->num_entrys; j++) {
																			 FREE(ptr.radiobutton->entrys[j].title);
																		 }
																		 /* Free array of entries */
																		 FARFREE(ptr.radiobutton->entrys);
																		 /* Free title */
																		 FREE(ptr.radiobutton->title);
																		 break;
																	 }
				}
				/* Free field specific data */
				FARFREE(fields[i].data);
			}

			/* Free fields array */
			FARFREE(fields);

			/* Reset variables */
			fields_cur = fields_max = 0;
		}

void form_c::display(void)
		{
			ushort i;								// General counter

			/* Display all fields */
			for (i = 0; i < fields_max; i++) {
				draw_field(i + 1, 0, 1, 1);
			}

			win->redisplay_text();
		}

