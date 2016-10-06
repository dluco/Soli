/*
 * soli-view-frame.h
 * This file is part of soli
 *
 * Copyright (C) 2010 - Ignacio Casal Quinteiro
 *
 * soli is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * soli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with soli. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLI_VIEW_FRAME_H
#define SOLI_VIEW_FRAME_H

#include <gtk/gtk.h>
#include "soli-document.h"
#include "soli-view.h"
#include "soli-view-centering.h"

G_BEGIN_DECLS

#define SOLI_TYPE_VIEW_FRAME (soli_view_frame_get_type ())
G_DECLARE_FINAL_TYPE (SoliViewFrame, soli_view_frame, SOLI, VIEW_FRAME, GtkOverlay)

SoliViewFrame	*soli_view_frame_new			(void);

SoliViewCentering
		*soli_view_frame_get_view_centering	(SoliViewFrame *frame);

SoliView	*soli_view_frame_get_view		(SoliViewFrame *frame);

void		 soli_view_frame_popup_search		(SoliViewFrame *frame);

void		 soli_view_frame_popup_goto_line	(SoliViewFrame *frame);

void		 soli_view_frame_clear_search		(SoliViewFrame *frame);

G_END_DECLS

#endif /* SOLI_VIEW_FRAME_H */
