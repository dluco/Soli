/*
 * soli-message-bus.h
 * This file is part of soli
 *
 * Copyright (C) 2008-2010 - Jesse van den Kieboom
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

#ifndef SOLI_MESSAGE_BUS_H
#define SOLI_MESSAGE_BUS_H

#include <glib-object.h>
#include "soli-message.h"

G_BEGIN_DECLS

#define SOLI_TYPE_MESSAGE_BUS			(soli_message_bus_get_type ())
#define SOLI_MESSAGE_BUS(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_MESSAGE_BUS, SoliMessageBus))
#define SOLI_MESSAGE_BUS_CONST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SOLI_TYPE_MESSAGE_BUS, SoliMessageBus const))
#define SOLI_MESSAGE_BUS_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), SOLI_TYPE_MESSAGE_BUS, SoliMessageBusClass))
#define SOLI_IS_MESSAGE_BUS(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SOLI_TYPE_MESSAGE_BUS))
#define SOLI_IS_MESSAGE_BUS_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SOLI_TYPE_MESSAGE_BUS))
#define SOLI_MESSAGE_BUS_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), SOLI_TYPE_MESSAGE_BUS, SoliMessageBusClass))

typedef struct _SoliMessageBus		SoliMessageBus;
typedef struct _SoliMessageBusClass	SoliMessageBusClass;
typedef struct _SoliMessageBusPrivate	SoliMessageBusPrivate;

struct _SoliMessageBus
{
	GObject parent;

	SoliMessageBusPrivate *priv;
};

struct _SoliMessageBusClass
{
	GObjectClass parent_class;

	void (*dispatch)      (SoliMessageBus  *bus,
	                       SoliMessage     *message);
	void (*registered)    (SoliMessageBus  *bus,
	                       const gchar      *object_path,
	                       const gchar      *method);
	void (*unregistered)  (SoliMessageBus  *bus,
	                       const gchar      *object_path,
	                       const gchar      *method);
};

typedef void (* SoliMessageCallback)   (SoliMessageBus  *bus,
                                         SoliMessage     *message,
                                         gpointer          user_data);

typedef void (* SoliMessageBusForeach) (gchar const      *object_path,
                                         gchar const      *method,
                                         gpointer          user_data);

GType             soli_message_bus_get_type           (void) G_GNUC_CONST;

SoliMessageBus  *soli_message_bus_get_default        (void);
SoliMessageBus  *soli_message_bus_new                (void);

GType             soli_message_bus_lookup             (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method);

void              soli_message_bus_register           (SoliMessageBus        *bus,
                                                        GType                   message_type,
                                                        const gchar            *object_path,
                                                        const gchar            *method);

void              soli_message_bus_unregister         (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method);

void              soli_message_bus_unregister_all     (SoliMessageBus        *bus,
                                                        const gchar            *object_path);

gboolean          soli_message_bus_is_registered      (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method);

void              soli_message_bus_foreach            (SoliMessageBus        *bus,
                                                        SoliMessageBusForeach  func,
                                                        gpointer                user_data);

guint             soli_message_bus_connect            (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method,
                                                        SoliMessageCallback    callback,
                                                        gpointer                user_data,
                                                        GDestroyNotify          destroy_data);

void              soli_message_bus_disconnect         (SoliMessageBus        *bus,
                                                        guint                   id);

void              soli_message_bus_disconnect_by_func (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method,
                                                        SoliMessageCallback    callback,
                                                        gpointer                user_data);

void              soli_message_bus_block              (SoliMessageBus        *bus,
                                                        guint                   id);
void              soli_message_bus_block_by_func      (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method,
                                                        SoliMessageCallback    callback,
                                                        gpointer                user_data);

void              soli_message_bus_unblock            (SoliMessageBus        *bus,
                                                        guint                   id);
void              soli_message_bus_unblock_by_func    (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method,
                                                        SoliMessageCallback    callback,
                                                        gpointer                user_data);

void              soli_message_bus_send_message       (SoliMessageBus        *bus,
                                                        SoliMessage           *message);
void              soli_message_bus_send_message_sync  (SoliMessageBus        *bus,
                                                        SoliMessage           *message);

void              soli_message_bus_send               (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method,
                                                        const gchar            *first_property,
                                                        ...) G_GNUC_NULL_TERMINATED;

SoliMessage     *soli_message_bus_send_sync          (SoliMessageBus        *bus,
                                                        const gchar            *object_path,
                                                        const gchar            *method,
                                                        const gchar            *first_property,
                                                        ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* SOLI_MESSAGE_BUS_H */

/* ex:set ts=8 noet: */
