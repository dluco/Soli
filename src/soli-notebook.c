/*
 * soli-notebook.c
 * This file is part of soli
 *
 * Copyright (C) 2005 - Paolo Maggi
 * Copyright (C) 2015 - Sébastien Wilmet
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
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/* This file is a modified version of the epiphany file ephy-notebook.c
 * Here the relevant copyright:
 *
 *  Copyright (C) 2002 Christophe Fergeau
 *  Copyright (C) 2003 Marco Pesenti Gritti
 *  Copyright (C) 2003, 2004 Christian Persch
 */

#include "soli-notebook.h"
#include "soli-tab-label.h"

#define SOLI_NOTEBOOK_GROUP_NAME "SoliNotebookGroup"

/* The DND targets defined in SoliView start at 100.
 * Those defined in GtkSourceView start at 200.
 */
#define TARGET_TAB 150

struct _SoliNotebookPrivate
{
	/* History of focused pages. The first element contains the most recent
	 * one.
	 */
	GList *focused_pages;

	guint ignore_focused_page_update : 1;
};

G_DEFINE_TYPE_WITH_PRIVATE (SoliNotebook, soli_notebook, GTK_TYPE_NOTEBOOK)

enum
{
	TAB_CLOSE_REQUEST,
	SHOW_POPUP_MENU,
	CHANGE_TO_PAGE,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

static void
soli_notebook_finalize (GObject *object)
{
	SoliNotebook *notebook = SOLI_NOTEBOOK (object);

	g_list_free (notebook->priv->focused_pages);

	G_OBJECT_CLASS (soli_notebook_parent_class)->finalize (object);
}

static void
soli_notebook_grab_focus (GtkWidget *widget)
{
	GtkNotebook *notebook = GTK_NOTEBOOK (widget);
	gint current_page;
	GtkWidget *tab;

	current_page = gtk_notebook_get_current_page (notebook);
	tab = gtk_notebook_get_nth_page (notebook, current_page);

	if (tab != NULL)
	{
		gtk_widget_grab_focus (tab);
	}
}

static gint
find_tab_num_at_pos (GtkNotebook *notebook,
                     gint         screen_x,
                     gint         screen_y)
{
	GtkPositionType tab_pos;
	GtkAllocation tab_allocation;
	gint page_num;

	tab_pos = gtk_notebook_get_tab_pos (notebook);

	for (page_num = 0; ; page_num++)
	{
		GtkWidget *page;
		GtkWidget *tab_label;
		gint max_x, max_y, x_root, y_root;

		page = gtk_notebook_get_nth_page (notebook, page_num);

		if (page == NULL)
		{
			break;
		}

		tab_label = gtk_notebook_get_tab_label (notebook, page);
		g_return_val_if_fail (tab_label != NULL, -1);

		if (!gtk_widget_get_mapped (tab_label))
		{
			continue;
		}

		gdk_window_get_origin (gtk_widget_get_window (tab_label), &x_root, &y_root);

		gtk_widget_get_allocation (tab_label, &tab_allocation);
		max_x = x_root + tab_allocation.x + tab_allocation.width;
		max_y = y_root + tab_allocation.y + tab_allocation.height;

		if ((tab_pos == GTK_POS_TOP || tab_pos == GTK_POS_BOTTOM) &&
		    screen_x <= max_x)
		{
			return page_num;
		}

		if ((tab_pos == GTK_POS_LEFT || tab_pos == GTK_POS_RIGHT) &&
		    screen_y <= max_y)
		{
			return page_num;
		}
	}

	return -1;
}

static gboolean
soli_notebook_button_press_event (GtkWidget      *widget,
				   GdkEventButton *event)
{
	GtkNotebook *notebook = GTK_NOTEBOOK (widget);

	if (event->type == GDK_BUTTON_PRESS &&
	    event->button == GDK_BUTTON_SECONDARY &&
	    (event->state & gtk_accelerator_get_default_mod_mask ()) == 0)
	{
		gint tab_clicked;

		tab_clicked = find_tab_num_at_pos (notebook, event->x_root, event->y_root);
		if (tab_clicked >= 0)
		{
			GtkWidget *tab;

			tab = gtk_notebook_get_nth_page (notebook, tab_clicked);

			g_signal_emit (G_OBJECT (widget), signals[SHOW_POPUP_MENU], 0, event, tab);

			return GDK_EVENT_STOP;
		}
	}

	return GTK_WIDGET_CLASS (soli_notebook_parent_class)->button_press_event (widget, event);
}

/*
 * We need to override this because when we don't show the tabs, like in
 * fullscreen we need to have wrap around too
 */
static gboolean
soli_notebook_change_current_page (GtkNotebook *notebook,
				    gint         offset)
{
	gint current;

	current = gtk_notebook_get_current_page (notebook);

	if (current != -1)
	{
		gint target;
		gboolean wrap_around;

		target = current + offset;

		g_object_get (gtk_widget_get_settings (GTK_WIDGET (notebook)),
			      "gtk-keynav-wrap-around", &wrap_around,
			      NULL);

		if (wrap_around)
		{
			if (target < 0)
			{
				target = gtk_notebook_get_n_pages (notebook) - 1;
			}
			else if (target >= gtk_notebook_get_n_pages (notebook))
			{
				target = 0;
			}
		}

		gtk_notebook_set_current_page (notebook, target);
	}
	else
	{
		gtk_widget_error_bell (GTK_WIDGET (notebook));
	}

	return TRUE;
}

static void
soli_notebook_switch_page (GtkNotebook *notebook,
                            GtkWidget   *page,
                            guint        page_num)
{
	SoliNotebookPrivate *priv = SOLI_NOTEBOOK (notebook)->priv;

	GTK_NOTEBOOK_CLASS (soli_notebook_parent_class)->switch_page (notebook, page, page_num);

	if (!priv->ignore_focused_page_update)
	{
		gint page_num;

		/* Get again page_num and page, the signal handler may have
		 * changed them.
		 */
		page_num = gtk_notebook_get_current_page (notebook);
		if (page_num != -1)
		{
			GtkWidget *page = gtk_notebook_get_nth_page (notebook, page_num);
			g_assert (page != NULL);

			/* Remove the old page, we dont want to grow unnecessarily
			 * the list.
			 */
			priv->focused_pages = g_list_remove (priv->focused_pages, page);

			priv->focused_pages = g_list_prepend (priv->focused_pages, page);
		}
	}

	/* give focus to the tab */
	gtk_widget_grab_focus (page);
}

static void
close_button_clicked_cb (SoliTabLabel *tab_label,
			 SoliNotebook *notebook)
{
	SoliTab *tab;

	tab = soli_tab_label_get_tab (tab_label);
	g_signal_emit (notebook, signals[TAB_CLOSE_REQUEST], 0, tab);
}

static void
switch_to_last_focused_page (SoliNotebook *notebook,
			     SoliTab      *tab)
{
	if (notebook->priv->focused_pages != NULL)
	{
		GList *node;
		GtkWidget *page;
		gint page_num;

		node = notebook->priv->focused_pages;
		page = GTK_WIDGET (node->data);

		page_num = gtk_notebook_page_num (GTK_NOTEBOOK (notebook), page);
		g_return_if_fail (page_num != -1);

		gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), page_num);
	}
}

static void
drag_data_received_cb (GtkWidget        *widget,
		       GdkDragContext   *context,
		       gint              x,
		       gint              y,
		       GtkSelectionData *selection_data,
		       guint             info,
		       guint             timestamp)
{
	GtkWidget *notebook;
	GtkWidget *new_notebook;
	GtkWidget *page;

	if (info != TARGET_TAB)
	{
		return;
	}

	notebook = gtk_drag_get_source_widget (context);

	if (!GTK_IS_WIDGET (notebook))
	{
		return;
	}

	page = *(GtkWidget **) gtk_selection_data_get_data (selection_data);
	g_return_if_fail (page != NULL);

	/* We need to iterate and get the notebook of the target view
	 * because we can have several notebooks per window.
	 */
	new_notebook = gtk_widget_get_ancestor (widget, SOLI_TYPE_NOTEBOOK);
	g_return_if_fail (new_notebook != NULL);

	if (notebook != new_notebook)
	{
		soli_notebook_move_tab (SOLI_NOTEBOOK (notebook),
					 SOLI_NOTEBOOK (new_notebook),
					 SOLI_TAB (page),
					 0);
	}

	gtk_drag_finish (context, TRUE, TRUE, timestamp);
}

static void
soli_notebook_page_removed (GtkNotebook *notebook,
                             GtkWidget   *page,
                             guint        page_num)
{
	SoliNotebookPrivate *priv = SOLI_NOTEBOOK (notebook)->priv;
	gboolean current_page;

	/* The page removed was the current page. */
	current_page = (priv->focused_pages != NULL &&
			priv->focused_pages->data == page);

	priv->focused_pages = g_list_remove (priv->focused_pages, page);

	if (current_page)
	{
		switch_to_last_focused_page (SOLI_NOTEBOOK (notebook),
					     SOLI_TAB (page));
	}
}

static void
soli_notebook_page_added (GtkNotebook *notebook,
                           GtkWidget   *page,
                           guint        page_num)
{
	GtkWidget *tab_label;
	SoliView *view;

	g_return_if_fail (SOLI_IS_TAB (page));

	tab_label = gtk_notebook_get_tab_label (notebook, page);
	g_return_if_fail (SOLI_IS_TAB_LABEL (tab_label));

	/* For a DND from one notebook to another, the same tab_label can be
	 * used, so we need to connect the signal here.
	 * More precisely, the same tab_label is used when the drop zone is in
	 * the tab labels (not the SoliView), that is, when the DND is handled
	 * by the GtkNotebook implementation.
	 */
	g_signal_connect (tab_label,
	                  "close-clicked",
	                  G_CALLBACK (close_button_clicked_cb),
	                  notebook);

	view = soli_tab_get_view (SOLI_TAB (page));
	g_signal_connect (view,
			  "drag-data-received",
			  G_CALLBACK (drag_data_received_cb),
			  NULL);
}

static void
soli_notebook_remove (GtkContainer *container,
                       GtkWidget    *widget)
{
	GtkNotebook *notebook = GTK_NOTEBOOK (container);
	SoliNotebookPrivate *priv = SOLI_NOTEBOOK (container)->priv;
	GtkWidget *tab_label;
	SoliView *view;

	g_return_if_fail (SOLI_IS_TAB (widget));

	tab_label = gtk_notebook_get_tab_label (notebook, widget);
	g_return_if_fail (SOLI_IS_TAB_LABEL (tab_label));

	/* For a DND from one notebook to another, the same tab_label can be
	 * used, so we need to disconnect the signal.
	 */
	g_signal_handlers_disconnect_by_func (tab_label,
					      G_CALLBACK (close_button_clicked_cb),
					      notebook);

	view = soli_tab_get_view (SOLI_TAB (widget));
	g_signal_handlers_disconnect_by_func (view, drag_data_received_cb, NULL);

	/* This is where GtkNotebook will remove the page. By doing so, it
	 * will also switch to a new page, messing up our focus list. So we
	 * set a flag here to ignore the switch temporarily.
	 */
	priv->ignore_focused_page_update = TRUE;

	if (GTK_CONTAINER_CLASS (soli_notebook_parent_class)->remove != NULL)
	{
		GTK_CONTAINER_CLASS (soli_notebook_parent_class)->remove (container,
									   widget);
	}

	priv->ignore_focused_page_update = FALSE;
}

static gboolean
soli_notebook_change_to_page (SoliNotebook *notebook,
                               gint           page_num)
{
	gint n_pages;

	n_pages = gtk_notebook_get_n_pages (GTK_NOTEBOOK (notebook));

	if (page_num >= n_pages)
	{
		return FALSE;
	}

	gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook),
	                               page_num);

	return TRUE;
}

static void
soli_notebook_class_init (SoliNotebookClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	GtkContainerClass *container_class = GTK_CONTAINER_CLASS (klass);
	GtkNotebookClass *notebook_class = GTK_NOTEBOOK_CLASS (klass);
	GtkBindingSet *binding_set;
	gint i;

	object_class->finalize = soli_notebook_finalize;

	widget_class->grab_focus = soli_notebook_grab_focus;
	widget_class->button_press_event = soli_notebook_button_press_event;

	container_class->remove = soli_notebook_remove;

	notebook_class->change_current_page = soli_notebook_change_current_page;
	notebook_class->switch_page = soli_notebook_switch_page;
	notebook_class->page_removed = soli_notebook_page_removed;
	notebook_class->page_added = soli_notebook_page_added;

	klass->change_to_page = soli_notebook_change_to_page;

	signals[TAB_CLOSE_REQUEST] =
		g_signal_new ("tab-close-request",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (SoliNotebookClass, tab_close_request),
			      NULL, NULL, NULL,
			      G_TYPE_NONE,
			      1,
			      SOLI_TYPE_TAB);

	signals[SHOW_POPUP_MENU] =
		g_signal_new ("show-popup-menu",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_FIRST,
			      G_STRUCT_OFFSET (SoliNotebookClass, show_popup_menu),
			      NULL, NULL, NULL,
			      G_TYPE_NONE,
			      2,
			      GDK_TYPE_EVENT | G_SIGNAL_TYPE_STATIC_SCOPE,
			      SOLI_TYPE_TAB);

	signals[CHANGE_TO_PAGE] =
		g_signal_new ("change-to-page",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		              G_STRUCT_OFFSET (SoliNotebookClass, change_to_page),
		              NULL, NULL, NULL,
		              G_TYPE_BOOLEAN, 1,
		              G_TYPE_INT);

	binding_set = gtk_binding_set_by_class (klass);
	for (i = 1; i < 10; i++)
	{
		gtk_binding_entry_add_signal (binding_set,
		                              GDK_KEY_0 + i, GDK_MOD1_MASK,
		                              "change-to-page", 1,
		                              G_TYPE_INT, i - 1);
	}
}

/**
 * soli_notebook_new:
 *
 * Creates a new #SoliNotebook object.
 *
 * Returns: a new #SoliNotebook
 */
GtkWidget *
soli_notebook_new (void)
{
	return GTK_WIDGET (g_object_new (SOLI_TYPE_NOTEBOOK, NULL));
}

static void
soli_notebook_init (SoliNotebook *notebook)
{
	notebook->priv = soli_notebook_get_instance_private (notebook);

	gtk_notebook_set_scrollable (GTK_NOTEBOOK (notebook), TRUE);
	gtk_notebook_set_show_border (GTK_NOTEBOOK (notebook), FALSE);
	gtk_notebook_set_show_tabs (GTK_NOTEBOOK (notebook), TRUE);
	gtk_notebook_set_group_name (GTK_NOTEBOOK (notebook),
	                             SOLI_NOTEBOOK_GROUP_NAME);
	gtk_container_set_border_width (GTK_CONTAINER (notebook), 0);
}

/**
 * soli_notebook_add_tab:
 * @notebook: a #SoliNotebook
 * @tab: a #SoliTab
 * @position: the position where the @tab should be added
 * @jump_to: %TRUE to set the @tab as active
 *
 * Adds the specified @tab to the @notebook.
 */
void
soli_notebook_add_tab (SoliNotebook *notebook,
		        SoliTab      *tab,
		        gint           position,
		        gboolean       jump_to)
{
	GtkWidget *tab_label;
	SoliView *view;
	GtkTargetList *target_list;

	g_return_if_fail (SOLI_IS_NOTEBOOK (notebook));
	g_return_if_fail (SOLI_IS_TAB (tab));

	tab_label = soli_tab_label_new (tab);

	gtk_notebook_insert_page (GTK_NOTEBOOK (notebook),
				  GTK_WIDGET (tab),
				  tab_label,
				  position);

	gtk_notebook_set_tab_reorderable (GTK_NOTEBOOK (notebook),
	                                  GTK_WIDGET (tab),
	                                  TRUE);

	gtk_notebook_set_tab_detachable (GTK_NOTEBOOK (notebook),
	                                 GTK_WIDGET (tab),
	                                 TRUE);

	gtk_container_child_set (GTK_CONTAINER (notebook),
				 GTK_WIDGET (tab),
				 "tab-expand", TRUE,
				 NULL);

	/* Drag and drop support: move a tab to another notebook, with the drop
	 * zone in the SoliView. The drop zone in the tab labels is already
	 * implemented by GtkNotebook.
	 */
	view = soli_tab_get_view (tab);
	target_list = gtk_drag_dest_get_target_list (GTK_WIDGET (view));

	if (target_list != NULL)
	{
		gtk_target_list_add (target_list,
		                     gdk_atom_intern_static_string ("GTK_NOTEBOOK_TAB"),
		                     GTK_TARGET_SAME_APP,
		                     TARGET_TAB);
	}

	/* The signal handler may have reordered the tabs */
	position = gtk_notebook_page_num (GTK_NOTEBOOK (notebook),
					  GTK_WIDGET (tab));

	if (jump_to)
	{
		gtk_notebook_set_current_page (GTK_NOTEBOOK (notebook), position);
		gtk_widget_grab_focus (GTK_WIDGET (tab));
	}
}

/**
 * soli_notebook_move_tab:
 * @src: a #SoliNotebook
 * @dest: a #SoliNotebook
 * @tab: a #SoliTab
 * @dest_position: the position for @tab
 *
 * Moves @tab from @src to @dest.
 * If @dest_position is greater than or equal to the number of tabs
 * of the destination nootebook or negative, tab will be moved to the
 * end of the tabs.
 */
void
soli_notebook_move_tab (SoliNotebook *src,
                         SoliNotebook *dest,
                         SoliTab      *tab,
                         gint           dest_position)
{
	g_return_if_fail (SOLI_IS_NOTEBOOK (src));
	g_return_if_fail (SOLI_IS_NOTEBOOK (dest));
	g_return_if_fail (src != dest);
	g_return_if_fail (SOLI_IS_TAB (tab));

	/* Make sure the tab isn't destroyed while we move it. */
	g_object_ref (tab);

	/* Make sure the @src notebook isn't destroyed during the tab
	 * detachment, to prevent a crash in gtk_notebook_detach_tab(). In fact,
	 * if @tab is the last tab of @src, and if @src is not the last notebook
	 * of the SoliMultiNotebook, then @src will be destroyed when
	 * gtk_container_remove() is called by gtk_notebook_detach_tab().
	 */
	g_object_ref (src);
	gtk_notebook_detach_tab (GTK_NOTEBOOK (src), GTK_WIDGET (tab));
	g_object_unref (src);

	soli_notebook_add_tab (dest, tab, dest_position, TRUE);

	g_object_unref (tab);
}

/**
 * soli_notebook_remove_all_tabs:
 * @notebook: a #SoliNotebook
 *
 * Removes all #SoliTab from @notebook.
 */
void
soli_notebook_remove_all_tabs (SoliNotebook *notebook)
{
	GList *tabs;
	GList *t;

	g_return_if_fail (SOLI_IS_NOTEBOOK (notebook));

	g_list_free (notebook->priv->focused_pages);
	notebook->priv->focused_pages = NULL;

	/* Remove tabs in reverse order since it is faster
	 * due to how GtkNotebook works.
	 */
	tabs = gtk_container_get_children (GTK_CONTAINER (notebook));
	for (t = g_list_last (tabs); t != NULL; t = t->prev)
	{
		GtkWidget *tab = t->data;
		gtk_container_remove (GTK_CONTAINER (notebook), tab);
	}

	g_list_free (tabs);
}

/* ex:set ts=8 noet: */
