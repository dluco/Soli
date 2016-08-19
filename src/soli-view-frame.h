/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * soli-view-frame.h
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

#ifndef _SOLI_VIEW_FRAME_H_
#define _SOLI_VIEW_FRAME_H_

#include <gtk/gtk.h>
#include "soli-view.h"

G_BEGIN_DECLS

#define SOLI_TYPE_VIEW_FRAME             (soli_view_frame_get_type ())
#define SOLI_VIEW_FRAME(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_VIEW_FRAME, SoliViewFrame))
#define SOLI_VIEW_FRAME_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_VIEW_FRAME, SoliViewFrameClass))
#define SOLI_IS_VIEW_FRAME(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_VIEW_FRAME))
#define SOLI_IS_VIEW_FRAME_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_VIEW_FRAME))
#define SOLI_VIEW_FRAME_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_VIEW_FRAME, SoliViewFrameClass))

typedef struct _SoliViewFrameClass SoliViewFrameClass;
typedef struct _SoliViewFrame SoliViewFrame;
typedef struct _SoliViewFramePrivate SoliViewFramePrivate;


struct _SoliViewFrameClass
{
	GtkOverlayClass parent_class;
};

struct _SoliViewFrame
{
	GtkOverlay parent_instance;

	SoliViewFramePrivate *priv;
};

GType soli_view_frame_get_type (void) G_GNUC_CONST;

SoliViewFrame *soli_view_frame_new (void);

SoliView *soli_view_frame_get_view (SoliViewFrame *frame);

G_END_DECLS

#endif /* _SOLI_VIEW_FRAME_H_ */

