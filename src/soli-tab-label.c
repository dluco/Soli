/*
 * soli-tab-label.c
 * This file is part of soli
 *
 * Copyright (C) 2010 - Paolo Borelli
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

#include "soli-tab-label.h"
#include "soli-tab-private.h"

struct _SoliTabLabel
{
	GtkBox parent_instance;

	SoliTab *tab;

	GtkWidget *spinner;
	GtkWidget *icon;
	GtkWidget *label;
	GtkWidget *close_button;
};

enum
{
	PROP_0,
	PROP_TAB,
	LAST_PROP
};

static GParamSpec *properties[LAST_PROP];

enum
{
	CLOSE_CLICKED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

G_DEFINE_TYPE (SoliTabLabel, soli_tab_label, GTK_TYPE_BOX)

static void
soli_tab_label_set_property (GObject      *object,
			      guint         prop_id,
			      const GValue *value,
			      GParamSpec   *pspec)
{
	SoliTabLabel *tab_label = SOLI_TAB_LABEL (object);

	switch (prop_id)
	{
		case PROP_TAB:
			g_return_if_fail (tab_label->tab == NULL);
			tab_label->tab = SOLI_TAB (g_value_get_object (value));
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_tab_label_get_property (GObject    *object,
			      guint       prop_id,
			      GValue     *value,
			      GParamSpec *pspec)
{
	SoliTabLabel *tab_label = SOLI_TAB_LABEL (object);

	switch (prop_id)
	{
		case PROP_TAB:
			g_value_set_object (value, tab_label->tab);
			break;

		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
close_button_clicked_cb (GtkWidget     *widget,
			 SoliTabLabel *tab_label)
{
	g_signal_emit (tab_label, signals[CLOSE_CLICKED], 0, NULL);
}

static void
sync_tooltip (SoliTab      *tab,
	      SoliTabLabel *tab_label)
{
	gchar *str;

	str = _soli_tab_get_tooltip (tab);
	g_return_if_fail (str != NULL);

	gtk_widget_set_tooltip_markup (GTK_WIDGET (tab_label), str);
	g_free (str);
}

static void
sync_name (SoliTab      *tab,
	   GParamSpec    *pspec,
	   SoliTabLabel *tab_label)
{
	gchar *str;

	g_return_if_fail (tab == tab_label->tab);

	str = _soli_tab_get_name (tab);
	g_return_if_fail (str != NULL);

	gtk_label_set_text (GTK_LABEL (tab_label->label), str);
	g_free (str);

	sync_tooltip (tab, tab_label);
}

static void
update_close_button_sensitivity (SoliTabLabel *tab_label)
{
	SoliTabState state = soli_tab_get_state (tab_label->tab);

	gtk_widget_set_sensitive (tab_label->close_button,
				  (state != SOLI_TAB_STATE_CLOSING) &&
				  (state != SOLI_TAB_STATE_SAVING)  &&
				  (state != SOLI_TAB_STATE_SHOWING_PRINT_PREVIEW) &&
				  (state != SOLI_TAB_STATE_PRINTING) &&
				  (state != SOLI_TAB_STATE_SAVING_ERROR));
}

static void
sync_state (SoliTab      *tab,
	    GParamSpec    *pspec,
	    SoliTabLabel *tab_label)
{
	SoliTabState state;

	g_return_if_fail (tab == tab_label->tab);

	update_close_button_sensitivity (tab_label);

	state = soli_tab_get_state (tab);

	if ((state == SOLI_TAB_STATE_LOADING) ||
	    (state == SOLI_TAB_STATE_SAVING) ||
	    (state == SOLI_TAB_STATE_REVERTING))
	{
		gtk_widget_hide (tab_label->icon);

		gtk_widget_show (tab_label->spinner);
		gtk_spinner_start (GTK_SPINNER (tab_label->spinner));
	}
	else
	{
		GdkPixbuf *pixbuf;

		pixbuf = _soli_tab_get_icon (tab);

		if (pixbuf != NULL)
		{
			gtk_image_set_from_pixbuf (GTK_IMAGE (tab_label->icon),
			                           pixbuf);

			g_clear_object (&pixbuf);

			gtk_widget_show (tab_label->icon);
		}
		else
		{
			gtk_widget_hide (tab_label->icon);
		}

		gtk_spinner_stop (GTK_SPINNER (tab_label->spinner));
		gtk_widget_hide (tab_label->spinner);
	}

	/* sync tip since encoding is known only after load/save end */
	sync_tooltip (tab, tab_label);
}

static void
soli_tab_label_constructed (GObject *object)
{
	SoliTabLabel *tab_label = SOLI_TAB_LABEL (object);

	if (tab_label->tab == NULL)
	{
		g_critical ("The tab label was not properly constructed");
		return;
	}

	sync_name (tab_label->tab, NULL, tab_label);
	sync_state (tab_label->tab, NULL, tab_label);

	g_signal_connect_object (tab_label->tab,
				 "notify::name",
				 G_CALLBACK (sync_name),
				 tab_label,
				 0);

	g_signal_connect_object (tab_label->tab,
				 "notify::state",
				 G_CALLBACK (sync_state),
				 tab_label,
				 0);

	G_OBJECT_CLASS (soli_tab_label_parent_class)->constructed (object);
}

static void
soli_tab_label_close_clicked (SoliTabLabel *tab_label)
{
}

static void
soli_tab_label_class_init (SoliTabLabelClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

	object_class->set_property = soli_tab_label_set_property;
	object_class->get_property = soli_tab_label_get_property;
	object_class->constructed = soli_tab_label_constructed;

	properties[PROP_TAB] =
		g_param_spec_object ("tab",
		                     "Tab",
		                     "The SoliTab",
		                     SOLI_TYPE_TAB,
		                     G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);

	g_object_class_install_properties (object_class, LAST_PROP, properties);

	signals[CLOSE_CLICKED] =
		g_signal_new_class_handler ("close-clicked",
		                            G_TYPE_FROM_CLASS (klass),
		                            G_SIGNAL_RUN_LAST,
		                            G_CALLBACK (soli_tab_label_close_clicked),
		                            NULL, NULL, NULL,
		                            G_TYPE_NONE,
		                            0);

	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/ca/dluco/soli/ui/soli-tab-label.ui");
	gtk_widget_class_bind_template_child (widget_class, SoliTabLabel, spinner);
	gtk_widget_class_bind_template_child (widget_class, SoliTabLabel, icon);
	gtk_widget_class_bind_template_child (widget_class, SoliTabLabel, label);
	gtk_widget_class_bind_template_child (widget_class, SoliTabLabel, close_button);
}

static void
soli_tab_label_init (SoliTabLabel *tab_label)
{
	gtk_widget_init_template (GTK_WIDGET (tab_label));

	g_signal_connect (tab_label->close_button,
	                  "clicked",
	                  G_CALLBACK (close_button_clicked_cb),
	                  tab_label);
}

SoliTab *
soli_tab_label_get_tab (SoliTabLabel *tab_label)
{
	g_return_val_if_fail (SOLI_IS_TAB_LABEL (tab_label), NULL);

	return tab_label->tab;
}

GtkWidget *
soli_tab_label_new (SoliTab *tab)
{
	return g_object_new (SOLI_TYPE_TAB_LABEL,
			     "tab", tab,
			     NULL);
}

/* ex:set ts=8 noet: */
