/* dialog.h - general dialog tools
 * $Id: dialog.h,v 1.1 2002/02/19 03:32:28 bitman Exp $
 * Copyright (C) 2001 Ryan Phillips <bitman@users.sourceforge.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DIALOG_H
#define _DIALOG_H 1

#include "display.h"

/* Component types */
#define DIALOG_COMP_TITLE 0
#define DIALOG_COMP_HEADING 1
#define DIALOG_COMP_LABEL 2
#define DIALOG_COMP_OPTION 3

typedef struct dialogComponent {
	int type;
	int x, y;  /* Position on screen or in dialog? */
	unsigned char color;

	char * text;

	int id;  /* Unique identifier */
} dialogComponent;

typedef struct dialog {
	dialogComponent * components;  /* Static components */
	dialogComponent * options;     /* Selectable options */

	int componentcount;
	int optioncount;

	int curoption; /* Current option */

} dialog;

/* dialogInit(dialog)
 * Initialize empty dialog
 */
void dialogInit(dialog * dia);

/* dialogFree(dialog)
 * Free and empty a dialog
 */
void dialogFree(dialog * dia);

/* dialogComponentMake(type, x, y, color, text, id)
 * Generate a component from the given values
 * Memory is shared in the case of "text"
 */
dialogComponent dialogComponentMake(int type, int x, int y, unsigned char color, char * text, int id);

/* dialogAddComponent(dialog, component)
 * Add a component to a dialog
 * Component is copied completely
 */
void dialogAddComponent(dialog * dia, dialogComponent component);

/* dialogAddCompList(dialog, complist, size)
 * Add a list of components to a dialog
 */
void dialogAddCompList(dialog * dia, dialogComponent * complist, int size);

/* dialogDraw(dialog, display)
 * Draw a dialog to the given display
 */
void dialogDraw(displaymethod * mydisplay, dialog dia);

/* dialogGetCurOption(dialog)
 * Returns a pointer to the current option
 */
dialogComponent * dialogGetCurOption(dialog dia);

/* dialogNextOption(dialog)
 * dialogPrevOption(dialog)
 * Switches the currently selected option
 */
void dialogNextOption(dialog * dia);
void dialogPrevOption(dialog * dia);


/* Testing code */
void dialogTest(displaymethod * mydisplay);

#endif
