/*
 * soli-status-menu-button.c
 * This file is part of soli
 *
 * Copyright (C) 2008 - Jesse van den Kieboom
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

#include "soli-status-menu-button.h"

struct _SoliStatusMenuButton
{
	GtkMenuButton parent_instance;

	GtkWidget *label;
};

typedef struct
{
	GtkCssProvider *css;
} SoliStatusMenuButtonClassPrivate;

enum
{
	PROP_0,
	PROP_LABEL,
	LAST_PROP
};

G_DEFINE_TYPE_WITH_CODE (SoliStatusMenuButton,
			 soli_status_menu_button,
			 GTK_TYPE_MENU_BUTTON,
			 g_type_add_class_private (g_define_type_id, sizeof (SoliStatusMenuButtonClassPrivate)))

static void
soli_status_menu_button_get_property (GObject    *object,
				       guint       prop_id,
				       GValue     *value,
				       GParamSpec *pspec)
{
	SoliStatusMenuButton *obj = SOLI_STATUS_MENU_BUTTON (object);

	switch (prop_id)
	{
		case PROP_LABEL:
			g_value_set_string (value, soli_status_menu_button_get_label (obj));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_status_menu_button_set_property (GObject      *object,
				       guint         prop_id,
				       const GValue *value,
				       GParamSpec   *pspec)
{
	SoliStatusMenuButton *obj = SOLI_STATUS_MENU_BUTTON (object);

	switch (prop_id)
	{
		case PROP_LABEL:
			soli_status_menu_button_set_label (obj, g_value_get_string (value));
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
soli_status_menu_button_class_init (SoliStatusMenuButtonClass *klass)
{
	static const gchar style[] =
		"* {\n"
		  "padding: 1px 8px 2px 4px;\n"
		  "border: 0;\n"
		  "outline-width: 0;\n"
		"}";

	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);
	SoliStatusMenuButtonClassPrivate *class_priv;

	object_class->get_property = soli_status_menu_button_get_property;
	object_class->set_property = soli_status_menu_button_set_property;

	g_object_class_override_property (object_class, PROP_LABEL, "label");

	/* Bind class to template */
	gtk_widget_class_set_template_from_resource (widget_class,
	                                             "/ca/dluco/soli/ui/soli-status-menu-button.ui");
	gtk_widget_class_bind_template_child_internal (widget_class, SoliStatusMenuButton, label);

	/* Store the CSS provider in the class private data so it is shared among all instances */
	class_priv = G_TYPE_CLASS_GET_PRIVATE (klass, SOLI_TYPE_STATUS_MENU_BUTTON, SoliStatusMenuButtonClassPrivate);
	class_priv->css = gtk_css_provider_new ();
	gtk_css_provider_load_from_data (class_priv->css, style, -1, NULL);
}

static void
soli_status_menu_button_init (SoliStatusMenuButton *self)
{
	GtkStyleContext *context;
	SoliStatusMenuButtonClassPrivate *class_priv;

	gtk_widget_init_template (GTK_WIDGET (self));

	/* make it as small as possible */
	context = gtk_widget_get_style_context (GTK_WIDGET (self));
	class_priv = G_TYPE_CLASS_GET_PRIVATE (G_TYPE_INSTANCE_GET_CLASS (self, SOLI_TYPE_STATUS_MENU_BUTTON, SoliStatusMenuButtonClass),
	                                       SOLI_TYPE_STATUS_MENU_BUTTON,
	                                       SoliStatusMenuButtonClassPrivate);
	gtk_style_context_add_provider (context,
	                                GTK_STYLE_PROVIDER (class_priv->css),
	                                GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

/**
 * soli_status_menu_button_new:
 * @label: (allow-none):
 */
GtkWidget *
soli_status_menu_button_new (void)
{
	return g_object_new (SOLI_TYPE_STATUS_MENU_BUTTON, NULL);
}

/* we cannot rely on gtk_button_set_label since it manually replaces
 * the internal child instead of just setting the property :( */

void
soli_status_menu_button_set_label (SoliStatusMenuButton *button,
                                    const gchar           *label)
{
	g_return_if_fail (SOLI_IS_STATUS_MENU_BUTTON (button));

	gtk_label_set_markup (GTK_LABEL (button->label), label);
}

const gchar *
soli_status_menu_button_get_label (SoliStatusMenuButton *button)
{
	g_return_val_if_fail (SOLI_IS_STATUS_MENU_BUTTON (button), NULL);

	return gtk_label_get_label (GTK_LABEL (button->label));
}

/* ex:set ts=8 noet: */
