/* Filename : MENU.CPP                                                      *
 * Description : Pull down menu module for TI/2                             *
 * Target : MENU.OBJ -> TI2MENU.LIB or TI2.DLL                              *
 * Portability : OS/2, DOS																									*
 * Creation date : 10/25/94                                                 *
 * Last modified : 06/08/95                                                 *
 * Copyright (c) Jonathan Tew and Revolutionary Software 1995								*/

/******************************
 * Predefined header includes *
 ******************************/

#if defined(__OS2__) || defined(__EMX__)
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
#include "ti2.h"
#include "internal.h"

#ifdef __TURBOC__
#include <alloc.h>
#endif

#ifdef _USERTL2_
#include "..\rtl2\rtl2.h"
#else
#define CR                13
#define ESC 							27
#define UPARROW           72  // Extended character
#define DOWNARROW         80  // Extended character
#define LEFTARROW         75  // Extended character
#define RIGHTARROW        77  // Extended character
#endif

#define SUBMENU(cur) submenus[submenus_idx[cur - 1].pos]
#define MENUCMD(cur) menucmds[menucmds_idx[cur - 1].pos]
#define LEFT_RIGHT 1
#define NO_LEFT_RIGHT 0

/********************
 * Menu Global Data *
 ********************/

/******************
 * Menu Functions *
 ******************/

void _export menu_set_default_colors(uchar hotkey_fcolor, uchar norm_fcolor, uchar norm_bcolor, uchar high_fcolor,
																			uchar high_bcolor, uchar inactive_fcolor, uchar inactive_bcolor,
																			uchar frame_fcolor, uchar frame_bcolor)
    {
      /* Copy default colors */
      ti2_menu_default_colors.hotkey_fcolor = hotkey_fcolor;
      ti2_menu_default_colors.norm_fcolor = norm_fcolor;
      ti2_menu_default_colors.norm_bcolor = norm_bcolor;
      ti2_menu_default_colors.high_fcolor = high_fcolor;
      ti2_menu_default_colors.high_bcolor = high_bcolor;
      ti2_menu_default_colors.inactive_fcolor = inactive_fcolor;
      ti2_menu_default_colors.inactive_bcolor = inactive_bcolor;
      ti2_menu_default_colors.frame_fcolor = frame_fcolor;
      ti2_menu_default_colors.frame_bcolor = frame_bcolor;
    }

/************************
 * Menu Class Functions *
 ************************/

void menu_c::def_create(uchar new_line)
    {
			/* Call create function with default colors */
      create(new_line, ti2_menu_default_colors.hotkey_fcolor, ti2_menu_default_colors.norm_fcolor,
             ti2_menu_default_colors.norm_bcolor, ti2_menu_default_colors.high_fcolor, ti2_menu_default_colors.high_bcolor,
             ti2_menu_default_colors.inactive_fcolor, ti2_menu_default_colors.inactive_bcolor,
             ti2_menu_default_colors.frame_fcolor, ti2_menu_default_colors.frame_bcolor);
    }

void menu_c::create(uchar new_line, uchar new_hotkey_fcolor, uchar new_norm_fcolor, uchar new_norm_bcolor,
										uchar new_high_fcolor, uchar new_high_bcolor, uchar new_inactive_fcolor, uchar new_inactive_bcolor,
										uchar new_frame_fcolor, uchar new_frame_bcolor)
		{
			/* Copy and initialize parameters in menu_t */
			line = new_line;
			submenu_cur = 0;
			submenu_max = 0;
			pulldown = 0;
			reloading = 0;
			result = 0;
			hotkey_fcolor = new_hotkey_fcolor;
			norm_fcolor = new_norm_fcolor;
			norm_bcolor = new_norm_bcolor;
      high_fcolor = new_high_fcolor;
      high_bcolor = new_high_bcolor;
			inactive_fcolor = new_inactive_fcolor;
			inactive_bcolor = new_inactive_bcolor;
			hotkey_fcolor = new_hotkey_fcolor;
			frame_fcolor = new_frame_fcolor;
			frame_bcolor = new_frame_bcolor;
			menucmds_max = 0;
			submenus_max = 0;
			menucmds_idx_max = 0;
			submenus_idx_max = 0;
			/* Initialize all array pointers to NULL */
			submenu = NULL;
			menucmds = NULL;
			menucmds_idx = NULL;
			submenus = NULL;
			submenus_idx = NULL;
		}

void menu_c::destroy(void)
		{
			 int i;

			 /* Free menucmd array for each submenu */
			 for (i = 0; i < submenus_max; i++) {
				 FARFREE(submenus[i].menucmd);
				 FARFREE(submenus[i].title);
         /* Destroy hidden window if it hasn't been already */
         if (submenus[i].win_created) submenus[i].win.destroy();
			 }

			 /* Free menucmd title for functions and submenus */
			 for (i = 0; i < menucmds_max; i++) {
				 if (menucmds[i].type != 1) FREE(menucmds[i].title);
			 }

			 /* Free submenu array for submenus off of menu bar */
			 FARFREE(submenu);

			 /* Free all submenu and menucmd arrays and indexes */
			 FARFREE(submenus);
			 FARFREE(menucmds);
			 FARFREE(submenus_idx);
			 FARFREE(menucmds_idx);
		}

void menu_c::add_submenu(ushort new_id, uchar new_title_pos, char *new_title, uchar new_hotkey)
		{
			/* Allocate another submenu in array */
			submenus_max++;
			if ((submenus = (submenu_t FAR *) FARREALLOC(submenus, sizeof(submenu_t) * submenus_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_submenu() error : Unable to allocate memory for submenu.\n");
				exit(1);
			}

			/* Update submenu index */
			if (new_id > submenus_idx_max) {
				submenus_idx_max = new_id;
				if ((submenus_idx = (submenus_idx_t FAR *) FARREALLOC(submenus_idx, sizeof(submenus_idx_t) * submenus_idx_max)) == NULL) {
					/* Error allocating memory */
					printf("\nmenu_c::add_submenu() error : Unable to reallocate memory for submenu index.\n");
					exit(1);
				}
			}
			submenus_idx[new_id - 1].id = new_id;
			submenus_idx[new_id - 1].pos = submenus_max - 1;

			/* Update submenu */
			submenus[submenus_max - 1].parent_id = 0;
			submenus[submenus_max - 1].id = new_id;
			submenus[submenus_max - 1].title_pos = new_title_pos;
      if ((submenus[submenus_max - 1].title = (char *) malloc(sizeof(char) * (strlen(new_title) + 1))) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_submenu() error : Unable to allocate memory for submenu title.\n");
				exit(1);
			}
			strcpy(submenus[submenus_max - 1].title, new_title);
      submenus[submenus_max - 1].win_created = 0;
			submenus[submenus_max - 1].hotkey = new_hotkey;
			submenus[submenus_max - 1].longest_menucmd_name = 0;
			submenus[submenus_max - 1].active = 1;
			submenus[submenus_max - 1].menucmd_cur = 0;
			submenus[submenus_max - 1].menucmd_max = 0;
			submenus[submenus_max - 1].menucmd = NULL;

			/* Add submenu to the list of submenus directly off of the menu bar */
			if (submenu_cur < 1) submenu_cur = 1;
			submenu_max++;
			if ((submenu = (ushort FAR *) FARREALLOC(submenu, sizeof(ushort) * submenu_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_submenu() error : Unable to reallocate submenus array.\n");
				exit(1);
			}
			submenu[submenu_max - 1] = new_id;
		}

void menu_c::add_menucmd_func(ushort parent_submenu_id, ushort new_id, char *new_title, uchar new_hotkey)
		{
			uchar len;		// Length of new menu command when displayed
			uchar i;			// General counter

			/* Allocate another menucmd in array */
			menucmds_max++;
			if ((menucmds = (menucmd_t FAR *) FARREALLOC(menucmds, sizeof(menucmd_t) * menucmds_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_func() error : Unable to reallocate menucmds array.\n");
				exit(1);
			}

			/* Update menucmd index */
			if (new_id > menucmds_idx_max) {
				menucmds_idx_max = new_id;
				if ((menucmds_idx = (menucmds_idx_t FAR *) FARREALLOC(menucmds_idx, sizeof(menucmds_idx_t) * menucmds_idx_max)) == NULL) {
					/* Error allocating memory */
					printf("\nmenu_c::add_menucmd_func() error : Unable to reallocate menucmds index.\n");
					exit(1);
				}
			}
			menucmds_idx[new_id - 1].id = new_id;
			menucmds_idx[new_id - 1].pos = menucmds_max - 1;

			/* Initialize new menucmd variables */
			menucmds[menucmds_max - 1].id = new_id;
      if ((menucmds[menucmds_max - 1].title = (char *) malloc(sizeof(char) * (strlen(new_title) + 1))) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_func() error : Unable to allocate memory for title.\n");
				exit(1);
			}
			strcpy(menucmds[menucmds_max - 1].title, new_title);
			menucmds[menucmds_max - 1].hotkey = new_hotkey;
			menucmds[menucmds_max - 1].active = 1;
			menucmds[menucmds_max - 1].type = 0;

			/* Add submenu to the list of menucmds directly off of the menu bar */
      if (SUBMENU(parent_submenu_id).menucmd_cur < 1) SUBMENU(parent_submenu_id).menucmd_cur = 1;
      SUBMENU(parent_submenu_id).menucmd_max++;
      if ((SUBMENU(parent_submenu_id).menucmd = (ushort FAR *) FARREALLOC(SUBMENU(parent_submenu_id).menucmd, sizeof(ushort) * SUBMENU(parent_submenu_id).menucmd_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_func() error : Unable to reallocate submenus's menucmds.\n");
				exit(1);
			}
      SUBMENU(parent_submenu_id).menucmd[SUBMENU(parent_submenu_id).menucmd_max - 1] = new_id;

			/* Update longest menu command length */
			len = 0;  // Real length of string when ~ characters aren't counted
			for (i = 0; i < strlen(new_title); i++) {
				if (new_title[i] != '~') len++;
			}
      if (len > SUBMENU(parent_submenu_id).longest_menucmd_name) SUBMENU(parent_submenu_id).longest_menucmd_name = len;
		}

void menu_c::add_menucmd_separator(ushort parent_submenu_id, ushort new_id)
		{
			/* Allocate another menucmd in array */
			menucmds_max++;
			if ((menucmds = (menucmd_t FAR *) FARREALLOC(menucmds, sizeof(menucmd_t) * menucmds_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_separator() error : Unable to reallocate menucmds array.\n");
				exit(1);
			}

			/* Update menucmd index */
			if (new_id > menucmds_idx_max) {
				menucmds_idx_max = new_id;
				if ((menucmds_idx = (menucmds_idx_t FAR *) FARREALLOC(menucmds_idx, sizeof(menucmds_idx_t) * menucmds_idx_max)) == NULL) {
					printf("\nmenu_c::add_menucmd_separator() error : Unable to reallocate menucmds index.\n");
					exit(1);
				}
			}
			menucmds_idx[new_id - 1].id = new_id;
			menucmds_idx[new_id - 1].pos = menucmds_max - 1;

			/* Update submenu */
			menucmds[menucmds_max - 1].id = new_id;
			menucmds[menucmds_max - 1].active = 1;
			menucmds[menucmds_max - 1].type = 1;
			menucmds[menucmds_max - 1].title = NULL;

			/* Add submenu to the list of menucmds directly off of the menu bar */
      if (SUBMENU(parent_submenu_id).menucmd_cur < 1) SUBMENU(parent_submenu_id).menucmd_cur = 1;
      SUBMENU(parent_submenu_id).menucmd_max++;
      if ((SUBMENU(parent_submenu_id).menucmd = (ushort FAR *) FARREALLOC(SUBMENU(parent_submenu_id).menucmd, sizeof(ushort) * SUBMENU(parent_submenu_id).menucmd_max)) == NULL) {
				printf("\nmenu_c::add_menucmd_separator() error : Unable to reallocate submenu's menucmds.\n");
				exit(1);
			}
      SUBMENU(parent_submenu_id).menucmd[SUBMENU(parent_submenu_id).menucmd_max - 1] = new_id;
		}

void menu_c::add_menucmd_submenu(ushort parent_submenu_id, ushort new_id, ushort new_submenu_id, char *new_title, uchar new_hotkey)
		{
			uchar len;		// Length of new menu command when displayed
			uchar i;			// General counter

			/* Allocate another menucmd in array */
			menucmds_max++;
			if ((menucmds = (menucmd_t FAR *) FARREALLOC(menucmds, sizeof(menucmd_t) * menucmds_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_submenu() error : Unable to reallocate menucmd array.\n");
				exit(1);
			}

			/* Update menucmd index */
			if (new_id > menucmds_idx_max) {
				menucmds_idx_max = new_id;
				if ((menucmds_idx = (menucmds_idx_t FAR *) FARREALLOC(menucmds_idx, sizeof(menucmds_idx_t) * menucmds_idx_max)) == NULL) {
					/* Error Allocating Memory */
					printf("\nmenu_c::add_menucmd_submenu() error : Unable to reallocate menucmd index.\n");
					exit(1);
				}
			}

			menucmds_idx[new_id - 1].id = new_id;
			menucmds_idx[new_id - 1].pos = menucmds_max - 1;

			/* Update menucmd */
			menucmds[menucmds_max - 1].id = new_id;
      if ((menucmds[menucmds_max - 1].title = (char *) malloc(sizeof(char) * (strlen(new_title) + 1))) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_submenu() error : Unable to allocate memory for title.\n");
				exit(1);
			}
			strcpy(menucmds[menucmds_max - 1].title, new_title);
			menucmds[menucmds_max - 1].hotkey = new_hotkey;
			menucmds[menucmds_max - 1].active = 1;
			menucmds[menucmds_max - 1].type = 2;
			menucmds[menucmds_max - 1].submenu = new_submenu_id;

			/* Add menucmd to the submenu */
      SUBMENU(parent_submenu_id).menucmd_max++;
      if ((SUBMENU(parent_submenu_id).menucmd = (ushort FAR *) FARREALLOC(SUBMENU(parent_submenu_id).menucmd, sizeof(ushort) * SUBMENU(parent_submenu_id).menucmd_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_submenu() error : Unable to reallocate submenu's menucmds.\n");
				exit(1);
			}
      SUBMENU(parent_submenu_id).menucmd[SUBMENU(parent_submenu_id).menucmd_max - 1] = new_id;

			/* Update longest menu command length */
			len = 0;  // Real length of string when ~ characters aren't counted
			for (i = 0; i < strlen(new_title); i++) {
				if (new_title[i] != '~') len++;
			}
			len += 2;  // Add to more characters because menu command is a submenu
      if (len > SUBMENU(parent_submenu_id).longest_menucmd_name) SUBMENU(parent_submenu_id).longest_menucmd_name = len;

			/* Allocate another submenu in array */
			submenus_max++;
			if ((submenus = (submenu_t FAR *) FARREALLOC(submenus, sizeof(submenu_t) * submenus_max)) == NULL) {
				/* Error allocating memory */
				printf("\nmenu_c::add_menucmd_submenu() error : Unable to reallocate submenu array.\n");
				exit(1);
			}

			/* Update submenu index */
			if (new_submenu_id > submenus_idx_max) {
				submenus_idx_max = new_submenu_id;
				if ((submenus_idx = (submenus_idx_t FAR *) FARREALLOC(submenus_idx, sizeof(submenus_idx_t) * submenus_idx_max)) == NULL) {
					/* Error allocating memory */
					printf("\nmenu_c::add_menucmd_submenu() error : Unable to reallocate submenu index.\n");
					exit(1);
				}
			}
			submenus_idx[new_submenu_id - 1].id = new_submenu_id;
			submenus_idx[new_submenu_id - 1].pos = submenus_max - 1;

			/* Update submenu */
			submenus[submenus_max - 1].parent_id = parent_submenu_id;
			submenus[submenus_max - 1].id = new_submenu_id;
			submenus[submenus_max - 1].title = NULL;
      submenus[submenus_max - 1].win_created = 0;
			submenus[submenus_max - 1].hotkey = new_hotkey;
			submenus[submenus_max - 1].longest_menucmd_name = 0;
			submenus[submenus_max - 1].active = 1;
			submenus[submenus_max - 1].menucmd_cur = 0;
			submenus[submenus_max - 1].menucmd_max = 0;
			submenus[submenus_max - 1].menucmd = NULL;
		}

void menu_c::bar_redraw(void)
		{
			int i;
			int j;
      submenu_t FAR *temp_submenu;    /* Whenever we need to display part of a submenu like the title we retrieve it from the *
                                       * submenus array and store it here                                               */

			/* Draw menu bar */
			title_bar.goto_xy(1, 1);
			title_bar.text_color(norm_fcolor);
			title_bar.text_background(norm_bcolor);
			title_bar.clr_eol();

			/* Place submenu titles across menu bar */
			for (i = 0; i < submenu_max; i++) {
				/* Copy submenus information into temporary submenu */
        temp_submenu = &SUBMENU(submenu[i]);
        title_bar.goto_xy(temp_submenu->title_pos, 1);
				/* Set color for this submenu title */
				if ((i + 1) == submenu_cur) {
																			title_bar.text_color(high_fcolor);
																			title_bar.text_background(high_bcolor);
                                      for (j = 0; j < strlen(temp_submenu->title); j++) {
																				/* Check for the hotkey character symbol*/
                                        if(!(temp_submenu->title[j] == '~')) title_bar.wputch(temp_submenu->title[j]);
																			}
																		}
					else
        if (!temp_submenu->active) {
															 title_bar.text_color(inactive_fcolor);
															 title_bar.text_background(inactive_bcolor);
                               for (j = 0; j < strlen(temp_submenu->title); j++) {
																 /* Check for the hotkey character symbol*/
                                 if(!(temp_submenu->title[j] == '~')) title_bar.wputch(temp_submenu->title[j]);
															 }
														 }
					else
				{
					title_bar.text_color(norm_fcolor);
					title_bar.text_background(norm_bcolor);
          for (j = 0; j < strlen(temp_submenu->title); j++) {
						/* Check for the hotkey character */
            if((temp_submenu->title[j] == '~') && ((j+1) < strlen(temp_submenu->title))){
							title_bar.text_color(hotkey_fcolor);
							j++;
              title_bar.wputch(temp_submenu->title[j]);
							title_bar.text_color(norm_fcolor);
						}
							else
            title_bar.wputch(temp_submenu->title[j]);
					}
				}
			}
		}

void menu_c::next_submenu(uchar direction)
		{
			int i, j;
      submenu_t FAR *temp_submenu;      // Temporary submenu

			ushort last_submenu;

			/* Save last submenu so that we can see if we need a redraw */
			last_submenu = submenu_cur;

			if (!direction) { // Direction = Left
				/* Search from current submenu to the left */
				for(i = submenu_cur - 1; i > 0; i--) {
          temp_submenu = &SUBMENU(submenu[i - 1]);
          if((temp_submenu->active) && (last_submenu == submenu_cur)) {
						submenu_cur = i;
					}
				}
				/* No active submenus to the left so search from right to the submenu */
				for(i = submenu_max; i > submenu_cur; i--) {
          temp_submenu = &SUBMENU(submenu[i - 1]);
          if((temp_submenu->active) && (last_submenu == submenu_cur)) {
						submenu_cur = i;
					}
				}
			}
				else
			{ // Direction = Right
				/* Search from current submenu to the right */
				for(i = submenu_cur + 1; i < (submenu_max + 1); i++) {
          temp_submenu = &SUBMENU(submenu[i - 1]);
          if((temp_submenu->active) && (last_submenu == submenu_cur)){
						submenu_cur = i;
					}
				}
				/* No active submenus to the right so search from left to the submenu */
				for(i = 1; i < submenu_cur; i++) {
          temp_submenu = &SUBMENU(submenu[i - 1]);
          if((temp_submenu->active) && (last_submenu == submenu_cur)){
						submenu_cur = i;
					}
				}
			}

			/* Redraw if current submenu is not the same as the last submenu */
			if (last_submenu != submenu_cur) {
				/* Redraw last_submenu title */
				/* Copy submenus information into temporary submenu */
        temp_submenu = &SUBMENU(submenu[last_submenu - 1]);
        title_bar.goto_xy(temp_submenu->title_pos, 1);
				title_bar.text_color(norm_fcolor);
				title_bar.text_background(norm_bcolor);
        for (j = 0; j < strlen(temp_submenu->title); j++) {
					/* Check for the hotkey character */
          if((temp_submenu->title[j] == '~') && ((j+1) < strlen(temp_submenu->title))){
						title_bar.text_color(hotkey_fcolor);
						j++;
            title_bar.wputch(temp_submenu->title[j]);
						title_bar.text_color(norm_fcolor);
					}
						else
          title_bar.wputch(temp_submenu->title[j]);
				}
				/* Redraw current submenu */
				/* Copy submenus information into temporary submenu */
        temp_submenu = &SUBMENU(submenu[submenu_cur - 1]);
        title_bar.goto_xy(temp_submenu->title_pos, line);
				/* Set color for this submenu title */
				title_bar.text_color(high_fcolor);
				title_bar.text_background(high_bcolor);
        for (j = 0; j < strlen(temp_submenu->title); j++) {
					/* Check for the hotkey character symbol*/
          if(!(temp_submenu->title[j] == '~')) title_bar.wputch(temp_submenu->title[j]);
				}
			}
		}

void menu_c::goto_submenu(uchar submenu_num)
		{
			int j;
      submenu_t FAR *temp_submenu;      // Temporary submenu

			ushort last_submenu;

			/* Save last submenu so that we can see if we need a redraw */
			last_submenu = submenu_cur;
			submenu_cur = submenu_num;

			/* Redraw if current submenu is not the same as the last submenu */
			if (last_submenu != submenu_cur) {
				/* Redraw last_submenu title */
				/* Copy submenus information into temporary submenu */
        temp_submenu = &SUBMENU(submenu[last_submenu - 1]);
        title_bar.goto_xy(temp_submenu->title_pos, 1);
				title_bar.text_color(norm_fcolor);
				title_bar.text_background(norm_bcolor);
        for (j = 0; j < strlen(temp_submenu->title); j++) {
					/* Check for the hotkey character */
          if((temp_submenu->title[j] == '~') && ((j+1) < strlen(temp_submenu->title))){
						title_bar.text_color(hotkey_fcolor);
						j++;
            title_bar.wputch(temp_submenu->title[j]);
						title_bar.text_color(norm_fcolor);
					}
						else
          title_bar.wputch(temp_submenu->title[j]);
				}
				/* Redraw current submenu */
				/* Copy submenus information into temporary submenu */
        temp_submenu = &SUBMENU(submenu[submenu_cur - 1]);
        title_bar.goto_xy(temp_submenu->title_pos, line);
				/* Set color for this submenu title */
				title_bar.text_color(high_fcolor);
				title_bar.text_background(high_bcolor);
        for (j = 0; j < strlen(temp_submenu->title); j++) {
					/* Check for the hotkey character symbol*/
          if(!(temp_submenu->title[j] == '~')) title_bar.wputch(temp_submenu->title[j]);
				}
			}
		}

int menu_c::hotkey(uchar key)
		{
			int i;
			submenu_t temp_submenu;      // Temporary submenu
			ushort last_submenu;

			/* Save current submenu number and compare to see if we need a redraw at end */
			last_submenu = submenu_cur;

			/* Search through menu and see if key matches that submenu's hotkey  */
			for(i = 1; i <= submenu_max; i++) {
        temp_submenu = SUBMENU(submenu[i - 1]);
				if((toupper(temp_submenu.hotkey) == toupper(key)) && (temp_submenu.active)) {
					submenu_cur = i;
					/* Only return true if the hotkey pressed was not the hotkey of the current submenu */
					if(submenu_cur != last_submenu) return(1);
						return(0);
				}
			}
			return(0);
		}

void menu_c::submenu_redraw_menucmds(submenu_t FAR *curr_submenu)
		{
			int i, j;
      menucmd_t FAR *menucmd;

			for(i = 0; i < curr_submenu->menucmd_max; i++) {
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);
				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
                      if (menucmd->active) {
												// Highlighted command
												if (curr_submenu->menucmd_cur == (i+1)) {
													curr_submenu->win.text_color(high_fcolor);
													curr_submenu->win.text_background(high_bcolor);
													/* Make high bar go across the whole window */
													curr_submenu->win.goto_xy(1, i+1);
													curr_submenu->win.clr_eol();
													curr_submenu->win.goto_xy(2, i+1);
													/* Print string */
                          for (j = 0; j < strlen(menucmd->title); j++) {
														/* Check for the hotkey character symbol*/
                            if(!(menucmd->title[j] == '~')) curr_submenu->win.nd_putch(menucmd->title[j]);
													}
												}
													else
												{ // Normal command
													curr_submenu->win.text_color(norm_fcolor);
													curr_submenu->win.text_background(norm_bcolor);
													/* Make high bar go across the whole window */
													curr_submenu->win.goto_xy(1, i+1);
													curr_submenu->win.clr_eol();
													curr_submenu->win.goto_xy(2, i+1);
                          for(j = 0; j < strlen(menucmd->title); j++) {
                            if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
															curr_submenu->win.text_color(hotkey_fcolor);
															j++;
                              curr_submenu->win.nd_putch(menucmd->title[j]);
															curr_submenu->win.text_color(norm_fcolor);
														}
															else
                            curr_submenu->win.nd_putch(menucmd->title[j]);
													}
												}
											}
												else
											{  // inactive command
												curr_submenu->win.text_color(inactive_fcolor);
												curr_submenu->win.text_background(inactive_bcolor);
												/* Make high bar go across the whole window */
												curr_submenu->win.goto_xy(1, i+1);
												curr_submenu->win.clr_eol();
												curr_submenu->win.goto_xy(2, i+1);
                        for (j = 0; j < strlen(menucmd->title); j++) {
													/* Check for the hotkey character symbol*/
                          if(!(menucmd->title[j] == '~')) curr_submenu->win.nd_putch(menucmd->title[j]);
												}
											}
											break;
									 }
					case 1 : {  // Seperator
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 for (j = 0; j < curr_submenu->longest_menucmd_name+2; j++) curr_submenu->win.nd_putch('Ä');
										 break;
									 }
					case 2 : {  // Sub menu
                      if (menucmd->active) {
												// Highlighted command
												if (curr_submenu->menucmd_cur == (i+1)) {
													curr_submenu->win.text_color(high_fcolor);
													curr_submenu->win.text_background(high_bcolor);
													curr_submenu->win.goto_xy(1, i+1);
													curr_submenu->win.clr_eol();
													curr_submenu->win.goto_xy(2, i+1);
                          for (j = 0; j < strlen(menucmd->title); j++) {
														/* Check for the hotkey character symbol*/
                            if(!(menucmd->title[j] == '~')) curr_submenu->win.nd_putch(menucmd->title[j]);
													}
													curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
													curr_submenu->win.nd_putch(16);
												}
													else
												{ // Normal command
													curr_submenu->win.text_color(norm_fcolor);
													curr_submenu->win.text_background(norm_bcolor);
													curr_submenu->win.goto_xy(1, i+1);
													curr_submenu->win.clr_eol();
													curr_submenu->win.goto_xy(2, i+1);
                          for(j = 0; j < strlen(menucmd->title); j++) {
                            if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
															curr_submenu->win.text_color(hotkey_fcolor);
															j++;
                              curr_submenu->win.nd_putch(menucmd->title[j]);
															curr_submenu->win.text_color(norm_fcolor);
														}
															else
                            curr_submenu->win.nd_putch(menucmd->title[j]);
													}
													curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
													curr_submenu->win.nd_putch(16);
												}
											}
												else
											{  // inactive command
												curr_submenu->win.text_color(inactive_fcolor);
												curr_submenu->win.text_background(inactive_bcolor);
												curr_submenu->win.goto_xy(1, i+1);
												curr_submenu->win.clr_eol();
												curr_submenu->win.goto_xy(2, i+1);
                        for (j = 0; j < strlen(menucmd->title); j++) {
													/* Check for the hotkey character symbol*/
                          if(!(menucmd->title[j] == '~')) curr_submenu->win.nd_putch(menucmd->title[j]);
												}
												curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
												curr_submenu->win.nd_putch(16);
											}
											break;
									 }
				}
			}

			/* Display all the menu commands */
			curr_submenu->win.redisplay_text();
		}

void menu_c::next_menucmd(submenu_t FAR *curr_submenu, uchar direction)   // 0 = Up, 1 = Down
		{
			int i, j;
			int last_menucmd;

      menucmd_t FAR *menucmd;

			/* Save current menu command number and compare to see if we need a redraw at end */
			last_menucmd = curr_submenu->menucmd_cur;

			if(!direction) { // Search up
				/* Search from current menucmd up */
				for(i = curr_submenu->menucmd_cur - 1; i > 0; i--) {
          menucmd = &MENUCMD(curr_submenu->menucmd[i - 1]);
          if((menucmd->active) && (menucmd->type != 1) && (curr_submenu->menucmd_cur == last_menucmd)) { // Make sure menu command is active and not a seperator and a new command not found already
						curr_submenu->menucmd_cur = i;
					}
				}
				/* No active menucmds up so search from the bottom up */
				for(i = curr_submenu->menucmd_max; i > curr_submenu->menucmd_cur; i--) {
          menucmd = &MENUCMD(curr_submenu->menucmd[i - 1]);
          if((menucmd->active) && (menucmd->type != 1) && (curr_submenu->menucmd_cur == last_menucmd)) {
						curr_submenu->menucmd_cur = i;
					}
				}
			}
				else
			{ // Search down
				for(i = curr_submenu->menucmd_cur + 1; i < (curr_submenu->menucmd_max + 1); i++) {
          menucmd = &MENUCMD(curr_submenu->menucmd[i - 1]);
          if((menucmd->active) && (menucmd->type != 1) && (curr_submenu->menucmd_cur == last_menucmd)) { // Make sure menu command is active and not a seperator and a new command not found already
						curr_submenu->menucmd_cur = i;
					}
				}
				/* No active menucmds up so search from the bottom up */
				for(i = 1; i < curr_submenu->menucmd_cur; i++) {
          menucmd = &MENUCMD(curr_submenu->menucmd[i - 1]);
          if((menucmd->active) && (menucmd->type != 1) && (curr_submenu->menucmd_cur == last_menucmd)) {
						curr_submenu->menucmd_cur = i;
					}
				}
			}

			/* Redraw both menu commands if the new command wasn't the old one */
			if (last_menucmd != curr_submenu->menucmd_cur) {
				/* Redraw last menucmd */
				i = last_menucmd - 1;
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);

				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for(j = 0; j < strlen(menucmd->title); j++) {
                       if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
												 curr_submenu->win.text_color(hotkey_fcolor);
												 j++;
                         curr_submenu->win.wputch(menucmd->title[j]);
												 curr_submenu->win.text_color(norm_fcolor);
											 }
													else
                       curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 break;
									 }
					case 1 : {  // Seperator

											break;
									 }
					case 2 : {  // Sub menu
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for(j = 0; j < strlen(menucmd->title); j++) {
                       if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
												 curr_submenu->win.text_color(hotkey_fcolor);
												 j++;
                         curr_submenu->win.wputch(menucmd->title[j]);
												 curr_submenu->win.text_color(norm_fcolor);
											 }
												 else
                       curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
										 curr_submenu->win.wputch(16);
										 break;
									 }
				}
				/* Redraw new menucmd */
				i = curr_submenu->menucmd_cur - 1;
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);

				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
											curr_submenu->win.text_color(high_fcolor);
											curr_submenu->win.text_background(high_bcolor);
											/* Make high bar go across the whole window */
											curr_submenu->win.goto_xy(1, i+1);
											curr_submenu->win.clr_eol();
											curr_submenu->win.goto_xy(2, i+1);
											/* Print string */
                      for (j = 0; j < strlen(menucmd->title); j++) {
												/* Check for the hotkey character symbol*/
                        if(!(menucmd->title[j] == '~')) curr_submenu->win.wputch(menucmd->title[j]);
											}
											break;
									 }
					case 1 : {  // Seperator
											break;
									 }
					case 2 : {  // Sub menu
										 curr_submenu->win.text_color(high_fcolor);
										 curr_submenu->win.text_background(high_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for (j = 0; j < strlen(menucmd->title); j++) {
											 /* Check for the hotkey character symbol*/
                       if(!(menucmd->title[j] == '~')) curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
										 curr_submenu->win.wputch(16);
										 break;
									 }
				}
			}
		}

void menu_c::goto_menucmd(submenu_t FAR *curr_submenu, uchar menucmd_num)
		{
			int i, j;
			int last_menucmd;

      menucmd_t FAR *menucmd;

			/* Save current menu command number and compare to see if we need a redraw at end */
			last_menucmd = curr_submenu->menucmd_cur;
			curr_submenu->menucmd_cur = menucmd_num;

			/* Redraw both menu commands if the new command wasn't the old one */
			if (last_menucmd != curr_submenu->menucmd_cur) {
				/* Redraw last menucmd */
				i = last_menucmd - 1;
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);

				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for(j = 0; j < strlen(menucmd->title); j++) {
                       if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
												 curr_submenu->win.text_color(hotkey_fcolor);
												 j++;
                         curr_submenu->win.wputch(menucmd->title[j]);
												 curr_submenu->win.text_color(norm_fcolor);
											 }
													else
                       curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 break;
									 }
					case 1 : {  // Seperator

											break;
									 }
					case 2 : {  // Sub menu
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for(j = 0; j < strlen(menucmd->title); j++) {
                       if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
												 curr_submenu->win.text_color(hotkey_fcolor);
												 j++;
                         curr_submenu->win.wputch(menucmd->title[j]);
												 curr_submenu->win.text_color(norm_fcolor);
											 }
												 else
                       curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
										 curr_submenu->win.wputch(16);
										 break;
									 }
				}
				/* Redraw new menucmd */
				i = curr_submenu->menucmd_cur - 1;
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);

				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
											curr_submenu->win.text_color(high_fcolor);
											curr_submenu->win.text_background(high_bcolor);
											/* Make high bar go across the whole window */
											curr_submenu->win.goto_xy(1, i+1);
											curr_submenu->win.clr_eol();
											curr_submenu->win.goto_xy(2, i+1);
											/* Print string */
                      for (j = 0; j < strlen(menucmd->title); j++) {
												/* Check for the hotkey character symbol*/
                        if(!(menucmd->title[j] == '~')) curr_submenu->win.wputch(menucmd->title[j]);
											}
											break;
									 }
					case 1 : {  // Seperator
											break;
									 }
					case 2 : {  // Sub menu
										 curr_submenu->win.text_color(high_fcolor);
										 curr_submenu->win.text_background(high_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for (j = 0; j < strlen(menucmd->title); j++) {
											 /* Check for the hotkey character symbol*/
                       if(!(menucmd->title[j] == '~')) curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
										 curr_submenu->win.wputch(16);
										 break;
									 }
				}
			}
		}

int menu_c::submenu_hotkey(submenu_t FAR *curr_submenu, uchar key)
		{
			int i, j;
			uchar valid_hotkey = 0;
			ushort last_menucmd;

      menucmd_t FAR *menucmd;

			/* Save current menu command number and compare to see if we need a redraw at end */
			last_menucmd = curr_submenu->menucmd_cur;

			/* Search through all menu commands and see if key matches hotkey */
			for(i = 1; i <= curr_submenu->menucmd_max; i++) {
        menucmd = &MENUCMD(curr_submenu->menucmd[i - 1]);
        if((menucmd->active) && (menucmd->type != 1) && (!(valid_hotkey)) && (toupper(key) == toupper(menucmd->hotkey))) { // Make sure menu command is active and not a seperator and a new command not found already and hotkey and key match
					curr_submenu->menucmd_cur = i;
					valid_hotkey = 1;
				}
			}

			/* Redraw both menu commands if the new command wasn't the old one */
			if (valid_hotkey) {
				/* If hotkey pressed was the hotkey of the current menucmd don't redraw, but return successful */
				if (last_menucmd == curr_submenu->menucmd_cur) return(1);

				/* Redraw last menucmd */
				i = last_menucmd - 1;
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);

				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for(j = 0; j < strlen(menucmd->title); j++) {
                       if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
												 curr_submenu->win.text_color(hotkey_fcolor);
												 j++;
                         curr_submenu->win.wputch(menucmd->title[j]);
												 curr_submenu->win.text_color(norm_fcolor);
											 }
													else
                       curr_submenu->win.wputch(menucmd->title[j]);
											}
											break;
									 }
					case 1 : {  // Seperator

											break;
									 }
					case 2 : {  // Sub menu
										 curr_submenu->win.text_color(norm_fcolor);
										 curr_submenu->win.text_background(norm_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for(j = 0; j < strlen(menucmd->title); j++) {
                       if((menucmd->title[j] == '~') && ((j+1) < strlen(menucmd->title))){
												 curr_submenu->win.text_color(hotkey_fcolor);
												 j++;
                         curr_submenu->win.wputch(menucmd->title[j]);
												 curr_submenu->win.text_color(norm_fcolor);
											 }
												 else
                       curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
										 curr_submenu->win.wputch(16);
										 break;
									 }
				}
				/* Redraw new menucmd */
				i = curr_submenu->menucmd_cur - 1;
        menucmd = &MENUCMD(curr_submenu->menucmd[i]);

				curr_submenu->win.goto_xy(2, i+1);
        switch(menucmd->type) {
					case 0 : {  // Normal menu command
											curr_submenu->win.text_color(high_fcolor);
											curr_submenu->win.text_background(high_bcolor);
											/* Make high bar go across the whole window */
											curr_submenu->win.goto_xy(1, i+1);
											curr_submenu->win.clr_eol();
											curr_submenu->win.goto_xy(2, i+1);
											/* Print string */
                      for (j = 0; j < strlen(menucmd->title); j++) {
												/* Check for the hotkey character symbol*/
                        if(!(menucmd->title[j] == '~')) curr_submenu->win.wputch(menucmd->title[j]);
											}
											break;
									 }
					case 1 : {  // Seperator
											break;
									 }
					case 2 : {  // Sub menu
										 curr_submenu->win.text_color(high_fcolor);
										 curr_submenu->win.text_background(high_bcolor);
										 curr_submenu->win.goto_xy(1, i+1);
										 curr_submenu->win.clr_eol();
										 curr_submenu->win.goto_xy(2, i+1);
                     for (j = 0; j < strlen(menucmd->title); j++) {
											 /* Check for the hotkey character symbol*/
                       if(!(menucmd->title[j] == '~')) curr_submenu->win.wputch(menucmd->title[j]);
										 }
										 curr_submenu->win.goto_xy(curr_submenu->longest_menucmd_name + 1, i+1);
										 curr_submenu->win.wputch(16);
										 break;
									 }
				}
				return(1);    /* Return one so that menu_submenu knows that a hotkey was successfully pressed and it should execute *
											 * the menu command or submenu                                                                        */
			}
			return(0);  // Key wasn't a hotkey
		}

uchar menu_c::load_submenu(submenu_t FAR *curr_submenu, uchar x, uchar y, uchar leftright)   // Returns 0 = ESC, 1 = Left menu, 2 = Right menu
		{
			int inactive = 0;   				// True when they hit escape from submenu
			uchar ch;
			int return_code = 0;
      menucmd_t FAR *menucmd;
			uchar hotkey_pressed = 0;   // When equal to one a hotkey was pressed so it should be executed
			uchar submenus_deep = 0;		// Increases each time a submenu is loaded
			uchar submenu_num;			    // Number of submenu that is clicked on with mouse
			uchar menucmd_clicked;			// True if a menucmd was clicked

			submenu_load:

			/* Do screen preparation if we aren't reloading */
			if (!reloading) {
				/* Make sure that the current menu cmd highlight is valid */
				if ((curr_submenu->menucmd_cur < 1) && (curr_submenu->menucmd_max > 0)) curr_submenu->menucmd_cur = 1;
				if (curr_submenu->menucmd_cur > curr_submenu->menucmd_max) curr_submenu->menucmd_cur = curr_submenu->menucmd_max;

				/* Create window if this is the first time the submenu has been displayed.  If hidden than just unhide */
        if (!curr_submenu->win_created || submenus_deep) {
					curr_submenu->win.create(x, y, x+curr_submenu->longest_menucmd_name+1+2  , y+curr_submenu->menucmd_max+1,
																	 frame_fcolor, frame_bcolor, norm_fcolor, norm_bcolor, "", 0, 0,
																	 WIN_FRAME_SD, WIN_SHADOW_RIGHT, 0);
          curr_submenu->win_created = 1;
					submenu_redraw_menucmds(curr_submenu);
				}
					else
				{
					curr_submenu->win.unhide();
				}
			}

			while(!inactive) {
				if (reloading) {
          menucmd = &MENUCMD(curr_submenu->menucmd[curr_submenu->menucmd_cur - 1]);
          if (menucmd->type == 2) {
						ch = CR;
					}
						else
					{
						reloading = 0;
					}
				}
				if (!reloading) {
					/* Wait for either the mouse to be clicked or keyboard to be pressed */
					for (;;) {
						/* Check keyboard */
						if(hotkey_pressed) {
							ch = 13;   // Only get a character from keyboard if last character wasn't a hotkey
							break;
						}
              else
            if (KBHIT()) {
							ch = GETCH();
							break;
						}
              else
            /* Check mouse */
            if (mouse_read_event()) {
              menucmd_clicked = 0;
              switch (submenu_or_menucmd_clicked(curr_submenu, &submenu_num)) {
                case 1 : {
                           if (submenu_num) {
                             /* See if it is a different submenu than the one we are on */
                             if (submenu_num != submenu_cur) {
                               /* Close all submenus back to titlebar */
                               while (submenus_deep) {
                                 /* Destroy submenu */
                                 submenus_deep--;
                                 curr_submenu->win.destroy();
                                 curr_submenu->win_created = 0;
                                 curr_submenu = &SUBMENU(curr_submenu->parent_id);
                                 if (submenus_deep == 0) leftright = 1;
                               }
                               curr_submenu->win.hide();
                               goto_submenu(submenu_num);
                               return(4);
                             }
                           }
                           break;
                         }
                case 2 : {
                           break;
                         }
                case 3 : {
                           ch = CR;
                           menucmd_clicked = 1;
                           break;
                         }
              }
              if (menucmd_clicked) break;
            }
            /* Call Time slice function */
            timeslice();
          }
				}

				switch(ch) {
					case 0  : { // Extended key. Arrows etc.
											ch = GETCH();
											switch(ch) {
												case UPARROW : {
																				 next_menucmd(curr_submenu, 0);
																				 break;
																			 }
												case LEFTARROW : {
																					 if(leftright) {
																						 return_code = 1;
																						 inactive = 1;
																					 }
																					 break;
																				 }
												case RIGHTARROW : {
																						if (leftright) {
																							return_code = 2;
																							inactive = 1;
																						}
																						break;
																					}
												case DOWNARROW : {
																					 next_menucmd(curr_submenu, 1);
																					 break;
																				 }
											}
											break;
										}
					case CR : {
											hotkey_pressed = 0;
											if (curr_submenu->menucmd_max < 1) break;
                      menucmd = &MENUCMD(curr_submenu->menucmd[curr_submenu->menucmd_cur - 1]);
                      switch(menucmd->type) {
												case 0 : {
                                   result = menucmd->id;
																	 reloading = 1;
																	 inactive = 1;
																	 return_code = 3;
																	 break;
																 }
												case 2 : {
																	 x += 2;
																	 y += curr_submenu->menucmd_cur;
                                   curr_submenu = &SUBMENU(menucmd->submenu);
																	 leftright = 0;
																	 submenus_deep++;
																	 goto submenu_load;
																 }
											}
											break;
										}
					case ESC : {
											 /* See if this is a submenu off of a submenu */
											 if (submenus_deep) {
												 /* Hide submenu */
												 submenus_deep--;
												 curr_submenu->win.destroy();
                         curr_submenu->win_created = 0;
                         curr_submenu = &SUBMENU(curr_submenu->parent_id);
												 if (submenus_deep == 0) leftright = 1;
											 }
												 else
											 {
												 return_code = 0;
												 inactive = 1;
											 }
											 break;
										}
					default : {
											if(submenu_hotkey(curr_submenu, ch)) hotkey_pressed = 1;
											break;
										}
        }
			}

			/* If a menu command wasn't pressed close window */
			if (!reloading) {
				curr_submenu->win.hide();
			}
			return(return_code);
		}

uchar menu_c::submenu_clicked(void)
		{
			uchar sub_title_len;		// Length of the submenu title
			uchar i, j;	 						// General counter

			/* See if this event had a button clicked */
			if (mouse_left || mouse_right || mouse_center) {
				/* See if y coordinate of mouse event is valid to the titlebar */
				if (title_bar.text_y_relative(mouse_row)) {
					/* Check all submenu titles to see if they clicked on one */
					for (i = 0; i < submenu_max; i++) {
						/* Calculate length of submenu title */
						sub_title_len = 0;
            for (j = 0; j < strlen(SUBMENU(submenu[i]).title); j++) {
              if (SUBMENU(submenu[i]).title[j] != '~') sub_title_len++;
						}
						/* See if the x coordinate of the mouse event falls on top of the titlebar */
            if ((title_bar.text_x_relative(mouse_col) >= SUBMENU(submenu[i]).title_pos) && (title_bar.text_x_relative(mouse_col) <= (SUBMENU(submenu[i]).title_pos + sub_title_len - 1))) {
							/* They clicked on the menu */
							return(i + 1);
						}
					}
				}
			}
			/* No valid mouse clicks */
			return(0);
		}

uchar menu_c::submenu_or_menucmd_clicked(submenu_t FAR *curr_submenu, uchar *num)
		{
			uchar sub_title_len;		// Length of the submenu title
			uchar i, j;	 						// General counter

			/* See if this event had a button clicked */
			if (mouse_left || mouse_right || mouse_center) {
				/* See if y coordinate of mouse event is valid to the titlebar */
				if (title_bar.text_y_relative(mouse_row) && (mouse_left || mouse_right || mouse_center)) {
					/* Check all submenu titles to see if they clicked on one */
					for (i = 0; i < submenu_max; i++) {
						/* Calculate length of submenu title */
						sub_title_len = 0;
            for (j = 0; j < strlen(SUBMENU(submenu[i]).title); j++) {
              if (SUBMENU(submenu[i]).title[j] != '~') sub_title_len++;
						}
						/* See if the x coordinate of the mouse event falls on top of the titlebar */
            if ((title_bar.text_x_relative(mouse_col) >= SUBMENU(submenu[i]).title_pos) && (title_bar.text_x_relative(mouse_col) <= (SUBMENU(submenu[i]).title_pos + sub_title_len - 1))) {
							/* They clicked on the menu */
							*num = (i + 1);
							return(1);
						}
					}
				}
				/* See if x and y coordinates of mouse event are valid for this submenu's window */
				if (curr_submenu->win.text_x_relative(mouse_col) && curr_submenu->win.text_y_relative(mouse_row)) {
					/* See if we are sliding or clicking on the menu command */
					if ((mouse_left == BUTTON_DOWN) || (mouse_center == BUTTON_DOWN) || (mouse_right == BUTTON_DOWN)) {
						goto_menucmd(curr_submenu, curr_submenu->win.text_y_relative(mouse_row));
						return(2);
					}
						else
					{
						goto_menucmd(curr_submenu, curr_submenu->win.text_y_relative(mouse_row));
						return(3);
					}
				}
			}
			/* No valid mouse clicks */
			return(0);
		}

ushort menu_c::load(void)
		{
			uchar inactive = 0;
			uchar ch;
			uchar return_code;
			uchar submenu_num;				/// Number of submenu clicked on with the mouse

			/* If this is the first time loading the menu do screen setup */
			if (!reloading) {
        title_bar.create(1, line, desktop.text_width(), line, BLACK, BLACK, LIGHTGRAY, BLACK, "", BLACK, BLACK, WIN_FRAME_NONE, WIN_SHADOW_NONE, 0);

				/* Check current submenu pointer and make sure its correct */
				if ((submenu_cur < 1) && (submenu_max > 0)) submenu_cur = 1;
				if (submenu_cur > submenu_max) submenu_cur = submenu_max;

				bar_redraw();
			}

			while (!inactive) {
				/* If menu->pulldown is TRUE than last command just shifted the menu left or right so load that submenu */
				if (pulldown || reloading) {
					ch = CR;
				}
					else
				{
					/* Loop until either a submenu is clicked on with the mouse or a character is pressed */
					for (;;) {
						/* Check keyboard */
            if (KBHIT()) {
							ch = GETCH(); /* Get user next key and process */
							break;
						}
              else
						/* Check mouse */
						if (mouse_read_event()) {
							submenu_num = submenu_clicked();
							if (submenu_num) {
								/* A submenu was clicked on */
								goto_submenu(submenu_num);
								ch = CR;
								break;
							}
						}
						/* Call Time slice function */
            timeslice();
					}
				}
				switch(ch) {
					case 0  : { // Extended key. Arrows etc.
											ch = GETCH();
											switch(ch) {
												case LEFTARROW : {
																					 next_submenu(0);
																					 break;
																				 }
												case RIGHTARROW : {
																						next_submenu(1);
																						break;
																					}
												case DOWNARROW : {
																					 /* This is like pressing CR so we fall through. */
																					 break;
																				 }
											}
                      if (ch != DOWNARROW) break;
										}
					case CR : {
											pulldown = 1; // Set menu is pulled down to TRUE

                      return_code = load_submenu(&SUBMENU(submenu[submenu_cur - 1]), SUBMENU(submenu[submenu_cur - 1]).title_pos, line + 1, LEFT_RIGHT);

											switch(return_code) {
												case 0 : {
																	 pulldown = 0;
																	 break;
																 }
												case 1 : { // Left arrow
																		next_submenu(0);
																		break;
																 }
												case 2 : { // Right arrow
																		next_submenu(1);
																		break;
																 }
												case 3 : {
																	 inactive = 1;
																	 break;
																 }
												case 4 : {
																	 /* Another submenu was clicked on with the mouse */
																	 pulldown = 1;
																	 break;
																 }
											}
											break;
										}
					case ESC : { // ESC pressed.  Close menu
											 inactive = 1;
											 result = 0;
											 break;
										 }
					default : {
											if(hotkey(ch)) {
												bar_redraw();
												pulldown = 1;
											}
											break;
										}
				}
			}

			/* If menu is not going to be reloading restore screen */
			if (!reloading) {
				title_bar.destroy();
			}

			/* Return menu command id or zero if menu is being closed */
			return(result);
		}

void menu_c::submenu_inactive(ushort submenu_id)
		{
			 int i;

			 /* Go through all the submenus find submenu to make inactive */
			 for (i = 0; i < submenus_max; i++) {
				 if (submenus[i].id == submenu_id) {
					 submenus[i].active = 0;
					 return;
				 }
			 }
		}

void menu_c::menucmd_inactive(ushort menucmd_id)
		{
			 int i;

			 /* Go through all the menucmds and find the one to make inactive */
			 for (i = 0; i < menucmds_max; i++) {
				 if (menucmds[i].id == menucmd_id) {
					 menucmds[i].active = 0;
					 return;
				 }
			 }
		}

void menu_c::submenu_active(ushort submenu_id)
		{
			 int i;

			 /* Go through all the submenus find submenu to make active */
			 for (i = 0; i < submenus_max; i++) {
				 if (submenus[i].id == submenu_id) {
					 submenus[i].active = 1;
					 return;
				 }
			 }
		}

void menu_c::menucmd_active(ushort menucmd_id)
		{
			 int i;

			 /* Go through all the menucmds and find the one to make active */
			 for (i = 0; i < menucmds_max; i++) {
				 if (menucmds[i].id == menucmd_id) {
					 menucmds[i].active = 1;
					 return;
				 }
			 }
		}

