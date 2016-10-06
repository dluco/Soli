/*
 * soli-window.h
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANWINDOWILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOLI_WINDOW_H
#define SOLI_WINDOW_H

#include <gtksourceview/gtksource.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "soli-tab.h"
#include "soli-message-bus.h"

G_BEGIN_DECLS

typedef enum
{
	SOLI_WINDOW_STATE_NORMAL		= 0,
	SOLI_WINDOW_STATE_SAVING		= 1 << 1,
	SOLI_WINDOW_STATE_PRINTING		= 1 << 2,
	SOLI_WINDOW_STATE_LOADING		= 1 << 3,
	SOLI_WINDOW_STATE_ERROR		= 1 << 4
} SoliWindowState;

#define SOLI_TYPE_WINDOW              (soli_window_get_type())
#define SOLI_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST((obj), SOLI_TYPE_WINDOW, SoliWindow))
#define SOLI_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST((klass), SOLI_TYPE_WINDOW, SoliWindowClass))
#define SOLI_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE((obj), SOLI_TYPE_WINDOW))
#define SOLI_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_WINDOW))
#define SOLI_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS((obj), SOLI_TYPE_WINDOW, SoliWindowClass))

typedef struct _SoliWindow        SoliWindow;
typedef struct _SoliWindowClass   SoliWindowClass;
typedef struct _SoliWindowPrivate SoliWindowPrivate;

struct _SoliWindow
{
	GtkApplicationWindow window;

	/*< private > */
	SoliWindowPrivate *priv;
};

struct _SoliWindowClass
{
	GtkApplicationWindowClass parent_class;

	/* Signals */
	void	 (* tab_added)      	(SoliWindow *window,
					 SoliTab    *tab);
	void	 (* tab_removed)    	(SoliWindow *window,
					 SoliTab    *tab);
	void	 (* tabs_reordered) 	(SoliWindow *window);
	void	 (* active_tab_changed)	(SoliWindow *window,
				     	 SoliTab    *tab);
	void	 (* active_tab_state_changed)
					(SoliWindow *window);
};

/* Public methods */
GType 		 soli_window_get_type 			(void) G_GNUC_CONST;

SoliTab	*soli_window_create_tab		(SoliWindow         *window,
							 gboolean             jump_to);

SoliTab	*soli_window_create_tab_from_location	(SoliWindow             *window,
							 GFile                   *location,
							 const GtkSourceEncoding *encoding,
							 gint                     line_pos,
							 gint                     column_pos,
							 gboolean                 create,
							 gboolean                 jump_to);

SoliTab	*soli_window_create_tab_from_stream	(SoliWindow             *window,
							 GInputStream            *stream,
							 const GtkSourceEncoding *encoding,
							 gint                     line_pos,
							 gint                     column_pos,
							 gboolean                 jump_to);

void		 soli_window_close_tab			(SoliWindow         *window,
							 SoliTab            *tab);

void		 soli_window_close_all_tabs		(SoliWindow         *window);

void		 soli_window_close_tabs		(SoliWindow         *window,
							 const GList         *tabs);

SoliTab	*soli_window_get_active_tab		(SoliWindow         *window);

void		 soli_window_set_active_tab		(SoliWindow         *window,
							 SoliTab            *tab);

/* Helper functions */
SoliView	*soli_window_get_active_view		(SoliWindow         *window);
SoliDocument	*soli_window_get_active_document	(SoliWindow         *window);

/* Returns a newly allocated list with all the documents in the window */
GList		*soli_window_get_documents		(SoliWindow         *window);

/* Returns a newly allocated list with all the documents that need to be
   saved before closing the window */
GList		*soli_window_get_unsaved_documents 	(SoliWindow         *window);

/* Returns a newly allocated list with all the views in the window */
GList		*soli_window_get_views			(SoliWindow         *window);

GtkWindowGroup  *soli_window_get_group			(SoliWindow         *window);

GtkWidget	*soli_window_get_side_panel		(SoliWindow         *window);

GtkWidget	*soli_window_get_bottom_panel		(SoliWindow         *window);

GtkWidget	*soli_window_get_statusbar		(SoliWindow         *window);

SoliWindowState soli_window_get_state 		(SoliWindow         *window);

SoliTab        *soli_window_get_tab_from_location	(SoliWindow         *window,
							 GFile               *location);

/* Message bus */
SoliMessageBus	*soli_window_get_message_bus		(SoliWindow         *window);

/*
 * Non exported functions
 */
GtkWidget	*_soli_window_get_multi_notebook	(SoliWindow         *window);
GtkWidget	*_soli_window_get_notebook		(SoliWindow         *window);

GMenuModel	*_soli_window_get_hamburger_menu	(SoliWindow         *window);

SoliWindow	*_soli_window_move_tab_to_new_window	(SoliWindow         *window,
							 SoliTab            *tab);
void             _soli_window_move_tab_to_new_tab_group(SoliWindow         *window,
                                                         SoliTab            *tab);
gboolean	 _soli_window_is_removing_tabs		(SoliWindow         *window);

GFile		*_soli_window_get_default_location 	(SoliWindow         *window);

void		 _soli_window_set_default_location 	(SoliWindow         *window,
							 GFile               *location);

void		 _soli_window_fullscreen		(SoliWindow         *window);

void		 _soli_window_unfullscreen		(SoliWindow         *window);

gboolean	 _soli_window_is_fullscreen		(SoliWindow         *window);

GList		*_soli_window_get_all_tabs		(SoliWindow         *window);

GFile		*_soli_window_pop_last_closed_doc	(SoliWindow         *window);

G_END_DECLS

#endif  /* SOLI_WINDOW_H  */

/* ex:set ts=8 noet: */
