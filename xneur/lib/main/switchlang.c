/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  Copyright (C) 2006-2016 XNeur Team
 *
 */

#include <X11/XKBlib.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "xneur.h"

#include "window.h"
#include "keymap.h"

#include "types.h"
#include "utils.h"
#include "log.h"

#include "switchlang.h"

extern struct _window *main_window;

int get_curr_keyboard_group(void)
{
	XkbStateRec xkbState;
	XkbGetState(main_window->display, XkbUseCoreKbd, &xkbState);
	int group = xkbState.group;
	//XFree(xkbState);
	return group;
}

// TODO: use build system configuration mechanism
#define GSETTINGS_HACK 1

#if GSETTINGS_HACK
static char gsettings_command[1024];
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

void set_keyboard_group(int layout_group)
{
	XkbLockGroup(main_window->display, XkbUseCoreKbd, layout_group);

#if GSETTINGS_HACK
	snprintf(gsettings_command, ARRAY_SIZE(gsettings_command),
			"gsettings set org.gnome.desktop.input-sources current %d",
			layout_group);
	log_message (DEBUG, gsettings_command);
	if (system(gsettings_command)) {};
#endif
}

void set_next_keyboard_group(struct _xneur_handle *handle)
{
	int new_layout_group = get_curr_keyboard_group() + 1;
	if (new_layout_group == handle->total_languages)
		new_layout_group = 0;
	XkbLockGroup(main_window->display, XkbUseCoreKbd, new_layout_group);

#if GSETTINGS_HACK
	snprintf(gsettings_command, ARRAY_SIZE(gsettings_command),
			"gsettings set org.gnome.desktop.input-sources current %d",
			new_layout_group);
	log_message (DEBUG, gsettings_command);
	if (system(gsettings_command)) {};
#endif
}

void set_prev_keyboard_group(struct _xneur_handle *handle)
{
	int new_layout_group = get_curr_keyboard_group() - 1;
	if (new_layout_group < 0)
		new_layout_group = handle->total_languages - 1;
	XkbLockGroup(main_window->display, XkbUseCoreKbd, new_layout_group);

#if GSETTINGS_HACK
	snprintf(gsettings_command, ARRAY_SIZE(gsettings_command),
			"gsettings set org.gnome.desktop.input-sources current %d",
			new_layout_group);
	log_message (DEBUG, gsettings_command);
	if (system(gsettings_command)) {};
#endif
}
