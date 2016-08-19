/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-view-frame.c
 * Copyright (C) 2016 David Luco <dluco11@gmail.com>
 *
 * Soli is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Soli is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "soli-view-frame.h"
#include "soli-view.h"

struct _SoliViewFramePrivate
{
	GtkScrolledWindow *scrolled_window;
	SoliView *view;
};


G_DEFINE_TYPE_WITH_PRIVATE (SoliViewFrame, soli_view_frame, GTK_TYPE_OVERLAY);

static void
soli_view_frame_init (SoliViewFrame *frame)
{
	frame->priv = soli_view_frame_get_instance_private (frame);
	
	gtk_widget_init_template (GTK_WIDGET (frame));
}

static void
soli_view_frame_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */

	G_OBJECT_CLASS (soli_view_frame_parent_class)->finalize (object);
}

static void
soli_view_frame_class_init (SoliViewFrameClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->finalize = soli_view_frame_finalize;
	
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/org/gnome/soli/soli-view-frame.ui");

	gtk_widget_class_bind_template_child_private (widget_class, SoliViewFrame, view);
	gtk_widget_class_bind_template_child_private (widget_class, SoliViewFrame, scrolled_window);
}

SoliViewFrame *
soli_view_frame_new (void)
{
	return g_object_new (SOLI_TYPE_VIEW_FRAME, NULL);
}

SoliView *
soli_view_frame_get_view (SoliViewFrame *frame)
{
	g_return_val_if_fail (SOLI_IS_VIEW_FRAME (frame), NULL);
	
	return frame->priv->view;
}


