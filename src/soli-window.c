/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-window.c
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 *
 * soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "soli-window.h"

struct _SoliWindowPrivate
{
	 /* To avoid warning:
	  * (g_type_class_add_private: assertion `private_size > 0' failed)
	  */
	 gchar dummy;
};


G_DEFINE_TYPE (SoliWindow, soli_window, GTK_TYPE_APPLICATION_WINDOW);

static void
soli_window_init (SoliWindow *soli_window)
{
	soli_window->priv = G_TYPE_INSTANCE_GET_PRIVATE (soli_window, SOLI_TYPE_WINDOW, SoliWindowPrivate);

	/* TODO: Add initialization code here */
}

static void
soli_window_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (soli_window_parent_class)->finalize (object);
}

static void
soli_window_class_init (SoliWindowClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (SoliWindowPrivate));

	object_class->finalize = soli_window_finalize;
}


