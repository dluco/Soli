/*
 * soli-message.h
 * This file is part of soli
 *
 * Copyright (C) 2008, 2010 - Jesse van den Kieboom
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
 * along with soli; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef SOLI_MESSAGE_H
#define SOLI_MESSAGE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SOLI_TYPE_MESSAGE			(soli_message_get_type ())
#define SOLI_MESSAGE(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_MESSAGE, SoliMessage))
#define SOLI_MESSAGE_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_MESSAGE, SoliMessage const))
#define SOLI_MESSAGE_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_MESSAGE, SoliMessageClass))
#define SOLI_IS_MESSAGE(obj)			(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_MESSAGE))
#define SOLI_IS_MESSAGE_CLASS(klass)		(G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_MESSAGE))
#define SOLI_MESSAGE_GET_CLASS(obj)		(G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_MESSAGE, SoliMessageClass))

typedef struct _SoliMessage        SoliMessage;
typedef struct _SoliMessageClass   SoliMessageClass;
typedef struct _SoliMessagePrivate SoliMessagePrivate;

struct _SoliMessage
{
	GObject parent;

	SoliMessagePrivate *priv;
};

struct _SoliMessageClass
{
	GObjectClass parent_class;
};

GType        soli_message_get_type             (void) G_GNUC_CONST;

const gchar *soli_message_get_object_path      (SoliMessage *message);
const gchar *soli_message_get_method           (SoliMessage *message);

gboolean     soli_message_type_has             (GType         gtype,
                                                 const gchar  *propname);

gboolean     soli_message_type_check           (GType         gtype,
                                                 const gchar  *propname,
                                                 GType         value_type);

gboolean     soli_message_has                  (SoliMessage *message,
                                                 const gchar  *propname);

gboolean     soli_message_is_valid_object_path (const gchar  *object_path);
gchar       *soli_message_type_identifier      (const gchar  *object_path,
                                                 const gchar  *method);

G_END_DECLS

#endif /* SOLI_MESSAGE_H */

/* ex:set ts=8 noet: */
