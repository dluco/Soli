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

#include "soli-message-bus.h"

#include <string.h>
#include <stdarg.h>
#include <gobject/gvaluecollector.h>

/**
 * SoliMessageCallback:
 * @bus: the #SoliMessageBus on which the message was sent
 * @message: the #SoliMessage which was sent
 * @user_data: the supplied user data when connecting the callback
 *
 * Callback signature used for connecting callback functions to be called
 * when a message is received (see soli_message_bus_connect()).
 *
 */

/**
 * SECTION:soli-message-bus
 * @short_description: internal message communication bus
 * @include: soli/soli-message-bus.h
 *
 * soli has a communication bus very similar to DBus. Its primary use is to
 * allow easy communication between plugins, but it can also be used to expose
 * soli functionality to external applications by providing DBus bindings for
 * the internal soli message bus.
 *
 * There are two different communication busses available. The default bus
 * (see soli_message_bus_get_default()) is an application wide communication
 * bus. In addition, each #SoliWindow has a separate, private bus
 * (see soli_window_get_message_bus()). This makes it easier for plugins to
 * communicate to other plugins in the same window.
 *
 * The concept of the message bus is very simple. You can register a message
 * type on the bus, specified as a Method at a specific Object Path with a
 * certain set of Method Arguments. You can then connect callback functions
 * for this message type on the bus. Whenever a message with the Object Path
 * and Method for which callbacks are connected is sent over the bus, the
 * callbacks are called. There is no distinction between Methods and Signals
 * (signals are simply messages where sender and receiver have switched places).
 *
 * <example>
 * <title>Registering a message type</title>
 * <programlisting>
 * SoliMessageBus *bus = soli_message_bus_get_default ();
 *
 * // Register 'method' at '/plugins/example' with one required
 * // string argument 'arg1'
 * soli_message_bus_register (bus, EXAMPLE_TYPE_METHOD_MESSAGE,
 *                             "/plugins/example", "method");
 * </programlisting>
 * </example>
 * <example>
 * <title>Connecting a callback</title>
 * <programlisting>
 * static void
 * example_method_cb (SoliMessageBus *bus,
 *                    SoliMessage    *message,
 *                    gpointer         user_data)
 * {
 * 	gchar *arg1 = NULL;
 *
 * 	soli_message_get (message, "arg1", &arg1, NULL);
 * 	g_message ("Evoked /plugins/example.method with: %s", arg1);
 * 	g_free (arg1);
 * }
 *
 * SoliMessageBus *bus = soli_message_bus_get_default ();
 *
 * guint id = soli_message_bus_connect (bus,
 *                                       "/plugins/example", "method",
 *                                       example_method_cb,
 *                                       NULL,
 *                                       NULL);
 *
 * </programlisting>
 * </example>
 * <example>
 * <title>Sending a message</title>
 * <programlisting>
 * SoliMessageBus *bus = soli_message_bus_get_default ();
 *
 * soli_message_bus_send (bus,
 *                         "/plugins/example", "method",
 *                         "arg1", "Hello World",
 *                         NULL);
 * </programlisting>
 * </example>
 *
 * Since: 2.25.3
 *
 */

typedef struct
{
	gchar *object_path;
	gchar *method;

	gchar *identifier;
} MessageIdentifier;

typedef struct
{
	MessageIdentifier *identifier;

	GList *listeners;
} Message;

typedef struct
{
	guint id;
	gboolean blocked;

	GDestroyNotify destroy_data;
	SoliMessageCallback callback;
	gpointer user_data;
} Listener;

typedef struct
{
	Message *message;
	GList *listener;
} IdMap;

struct _SoliMessageBusPrivate
{
	GHashTable *messages;
	GHashTable *idmap;

	GList *message_queue;
	guint idle_id;

	guint next_id;

	GHashTable *types; /* mapping from identifier to SoliMessageType */
};

/* signals */
enum
{
	DISPATCH,
	REGISTERED,
	UNREGISTERED,
	LAST_SIGNAL
};

static guint message_bus_signals[LAST_SIGNAL];

static void soli_message_bus_dispatch_real (SoliMessageBus *bus,
                                             SoliMessage    *message);

G_DEFINE_TYPE_WITH_PRIVATE (SoliMessageBus, soli_message_bus, G_TYPE_OBJECT)

static MessageIdentifier *
message_identifier_new (const gchar *object_path,
                        const gchar *method)
{
	MessageIdentifier *ret;

	ret = g_slice_new (MessageIdentifier);

	ret->object_path = g_strdup (object_path);
	ret->method = g_strdup (method);

	ret->identifier = soli_message_type_identifier (object_path, method);

	return ret;
}

static void
message_identifier_free (MessageIdentifier *identifier)
{
	g_free (identifier->object_path);
	g_free (identifier->method);
	g_free (identifier->identifier);

	g_slice_free (MessageIdentifier, identifier);
}

static guint
message_identifier_hash (gconstpointer id)
{
	return g_str_hash (((MessageIdentifier *)id)->identifier);
}

static gboolean
message_identifier_equal (gconstpointer id1,
                          gconstpointer id2)
{
	return g_str_equal (((MessageIdentifier *)id1)->identifier,
	                    ((MessageIdentifier *)id2)->identifier);
}

static void
listener_free (Listener *listener)
{
	if (listener->destroy_data)
	{
		listener->destroy_data (listener->user_data);
	}

	g_slice_free (Listener, listener);
}

static void
message_free (Message *message)
{
	message_identifier_free (message->identifier);

	g_list_free_full (message->listeners, (GDestroyNotify) listener_free);
	g_slice_free (Message, message);
}

static void
message_queue_free (GList *queue)
{
	g_list_free_full (queue, g_object_unref);
}

static void
soli_message_bus_finalize (GObject *object)
{
	SoliMessageBus *bus = SOLI_MESSAGE_BUS (object);

	if (bus->priv->idle_id != 0)
	{
		g_source_remove (bus->priv->idle_id);
	}

	message_queue_free (bus->priv->message_queue);

	g_hash_table_destroy (bus->priv->messages);
	g_hash_table_destroy (bus->priv->idmap);
	g_hash_table_destroy (bus->priv->types);

	G_OBJECT_CLASS (soli_message_bus_parent_class)->finalize (object);
}

static void
soli_message_bus_class_init (SoliMessageBusClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = soli_message_bus_finalize;

	klass->dispatch = soli_message_bus_dispatch_real;

	/**
	 * SoliMessageBus::dispatch:
	 * @bus: a #SoliMessageBus
	 * @message: the #SoliMessage to dispatch
	 *
	 * The "dispatch" signal is emitted when a message is to be dispatched.
	 * The message is dispatched in the default handler of this signal.
	 * Primary use of this signal is to customize the dispatch of a message
	 * (for instance to automatically dispatch all messages over DBus).
	 *
	 */
	message_bus_signals[DISPATCH] =
		g_signal_new ("dispatch",
		              G_OBJECT_CLASS_TYPE (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (SoliMessageBusClass, dispatch),
		              NULL, NULL, NULL,
		              G_TYPE_NONE,
		              1,
		              SOLI_TYPE_MESSAGE);

	/**
	 * SoliMessageBus::registered:
	 * @bus: a #SoliMessageBus
	 * @object_path: the registered object path.
	 * @method: the registered method
	 *
	 * The "registered" signal is emitted when a message has been registered
	 * on the bus.
	 *
	 */
	message_bus_signals[REGISTERED] =
		g_signal_new ("registered",
		              G_OBJECT_CLASS_TYPE (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (SoliMessageBusClass, registered),
		              NULL, NULL, NULL,
		              G_TYPE_NONE,
		              2,
		              G_TYPE_STRING,
		              G_TYPE_STRING);

	/**
	 * SoliMessageBus::unregistered:
	 * @bus: a #SoliMessageBus
	 * @object_path: the unregistered object path.
	 * @method: the unregistered method
	 *
	 * The "unregistered" signal is emitted when a message has been
	 * unregistered from the bus.
	 *
	 */
	message_bus_signals[UNREGISTERED] =
		g_signal_new ("unregistered",
		              G_OBJECT_CLASS_TYPE (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (SoliMessageBusClass, unregistered),
		              NULL, NULL, NULL,
		              G_TYPE_NONE,
		              2,
		              G_TYPE_STRING,
		              G_TYPE_STRING);
}

static Message *
message_new (SoliMessageBus *bus,
             const gchar     *object_path,
             const gchar     *method)
{
	Message *message = g_slice_new (Message);

	message->identifier = message_identifier_new (object_path, method);
	message->listeners = NULL;

	g_hash_table_insert (bus->priv->messages,
	                     message->identifier,
	                     message);

	return message;
}

static Message *
lookup_message (SoliMessageBus *bus,
                const gchar      *object_path,
                const gchar      *method,
                gboolean          create)
{
	MessageIdentifier *identifier;
	Message *message;

	identifier = message_identifier_new (object_path, method);
	message = g_hash_table_lookup (bus->priv->messages, identifier);
	message_identifier_free (identifier);

	if (!message && !create)
	{
		return NULL;
	}

	if (!message)
	{
		message = message_new (bus, object_path, method);
	}

	return message;
}

static guint
add_listener (SoliMessageBus      *bus,
              Message		   *message,
              SoliMessageCallback  callback,
              gpointer		    user_data,
              GDestroyNotify        destroy_data)
{
	Listener *listener;
	IdMap *idmap;

	listener = g_slice_new (Listener);
	listener->id = ++bus->priv->next_id;
	listener->callback = callback;
	listener->user_data = user_data;
	listener->blocked = FALSE;
	listener->destroy_data = destroy_data;

	message->listeners = g_list_append (message->listeners, listener);

	idmap = g_new (IdMap, 1);
	idmap->message = message;
	idmap->listener = g_list_last (message->listeners);

	g_hash_table_insert (bus->priv->idmap, GINT_TO_POINTER (listener->id), idmap);

	return listener->id;
}

static void
remove_listener (SoliMessageBus *bus,
                 Message         *message,
                 GList		 *listener)
{
	Listener *lst;

	lst = (Listener *)listener->data;

	/* remove from idmap */
	g_hash_table_remove (bus->priv->idmap, GINT_TO_POINTER (lst->id));
	listener_free (lst);

	/* remove from list of listeners */
	message->listeners = g_list_delete_link (message->listeners, listener);

	if (!message->listeners)
	{
		/* remove message because it does not have any listeners */
		g_hash_table_remove (bus->priv->messages, message->identifier);
	}
}

static void
block_listener (SoliMessageBus *bus,
                Message         *message,
                GList           *listener)
{
	Listener *lst;

	lst = listener->data;
	lst->blocked = TRUE;
}

static void
unblock_listener (SoliMessageBus *bus,
                  Message         *message,
                  GList           *listener)
{
	Listener *lst;

	lst = listener->data;
	lst->blocked = FALSE;
}

static void
dispatch_message_real (SoliMessageBus *bus,
                       Message         *msg,
                       SoliMessage    *message)
{
	GList *item;

	for (item = msg->listeners; item; item = item->next)
	{
		Listener *listener = (Listener *)item->data;

		if (!listener->blocked)
		{
			listener->callback (bus, message, listener->user_data);
		}
	}
}

static void
soli_message_bus_dispatch_real (SoliMessageBus *bus,
                                 SoliMessage    *message)
{
	const gchar *object_path;
	const gchar *method;
	Message *msg;

	object_path = soli_message_get_object_path (message);
	method = soli_message_get_method (message);

	g_return_if_fail (object_path != NULL);
	g_return_if_fail (method != NULL);

	msg = lookup_message (bus, object_path, method, FALSE);

	if (msg)
	{
		dispatch_message_real (bus, msg, message);
	}
}

static void
dispatch_message (SoliMessageBus *bus,
                  SoliMessage    *message)
{
	g_signal_emit (bus, message_bus_signals[DISPATCH], 0, message);
}

static gboolean
idle_dispatch (SoliMessageBus *bus)
{
	GList *list;
	GList *item;

	/* make sure to set idle_id to 0 first so that any new async messages
	   will be queued properly */
	bus->priv->idle_id = 0;

	/* reverse queue to get correct delivery order */
	list = g_list_reverse (bus->priv->message_queue);
	bus->priv->message_queue = NULL;

	for (item = list; item; item = item->next)
	{
		SoliMessage *msg = SOLI_MESSAGE (item->data);

		dispatch_message (bus, msg);
	}

	message_queue_free (list);
	return FALSE;
}

typedef void (*MatchCallback) (SoliMessageBus *, Message *, GList *);

static void
process_by_id (SoliMessageBus *bus,
               guint            id,
               MatchCallback    processor)
{
	IdMap *idmap;

	idmap = (IdMap *)g_hash_table_lookup (bus->priv->idmap, GINT_TO_POINTER (id));

	if (idmap == NULL)
	{
		g_warning ("No handler registered with id `%d'", id);
		return;
	}

	processor (bus, idmap->message, idmap->listener);
}

static void
process_by_match (SoliMessageBus      *bus,
                  const gchar          *object_path,
                  const gchar          *method,
                  SoliMessageCallback  callback,
                  gpointer              user_data,
                  MatchCallback         processor)
{
	Message *message;
	GList *item;

	message = lookup_message (bus, object_path, method, FALSE);

	if (!message)
	{
		g_warning ("No such handler registered for %s.%s", object_path, method);
		return;
	}

	for (item = message->listeners; item; item = item->next)
	{
		Listener *listener = (Listener *)item->data;

		if (listener->callback == callback &&
		    listener->user_data == user_data)
		{
			processor (bus, message, item);
			return;
		}
	}

	g_warning ("No such handler registered for %s.%s", object_path, method);
}

static void
free_type (gpointer data)
{
	g_slice_free (GType, data);
}

static void
soli_message_bus_init (SoliMessageBus *self)
{
	self->priv = soli_message_bus_get_instance_private (self);

	self->priv->messages = g_hash_table_new_full (message_identifier_hash,
	                                              message_identifier_equal,
	                                              NULL,
	                                              (GDestroyNotify) message_free);

	self->priv->idmap = g_hash_table_new_full (g_direct_hash,
	                                           g_direct_equal,
	                                           NULL,
	                                           (GDestroyNotify) g_free);

	self->priv->types = g_hash_table_new_full (message_identifier_hash,
	                                           message_identifier_equal,
	                                           (GDestroyNotify) message_identifier_free,
	                                           (GDestroyNotify) free_type);
}

/**
 * soli_message_bus_get_default:
 *
 * Get the default application #SoliMessageBus.
 *
 * Return value: (transfer none): the default #SoliMessageBus
 *
 */
SoliMessageBus *
soli_message_bus_get_default (void)
{
	static SoliMessageBus *default_bus = NULL;

	if (G_UNLIKELY (default_bus == NULL))
	{
		default_bus = g_object_new (SOLI_TYPE_MESSAGE_BUS, NULL);

		g_object_add_weak_pointer (G_OBJECT (default_bus),
		                           (gpointer) &default_bus);
	}

	return default_bus;
}

/**
 * soli_message_bus_new:
 *
 * Create a new message bus. Use soli_message_bus_get_default() to get the
 * default, application wide, message bus. Creating a new bus is useful for
 * associating a specific bus with for instance a #SoliWindow.
 *
 * Return value: a new #SoliMessageBus
 *
 */
SoliMessageBus *
soli_message_bus_new (void)
{
	return SOLI_MESSAGE_BUS (g_object_new (SOLI_TYPE_MESSAGE_BUS, NULL));
}

/**
 * soli_message_bus_lookup:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 *
 * Get the registered #SoliMessageType for @method at @object_path. The
 * returned #SoliMessageType is owned by the bus and should not be unreffed.
 *
 * Return value: the registered #SoliMessageType or %NULL if no message type
 *               is registered for @method at @object_path
 *
 */
GType
soli_message_bus_lookup (SoliMessageBus *bus,
                          const gchar	  *object_path,
                          const gchar	  *method)
{
	MessageIdentifier *identifier;
	GType *message_type;

	g_return_val_if_fail (SOLI_IS_MESSAGE_BUS (bus), G_TYPE_INVALID);
	g_return_val_if_fail (object_path != NULL, G_TYPE_INVALID);
	g_return_val_if_fail (method != NULL, G_TYPE_INVALID);

	identifier = message_identifier_new (object_path, method);
	message_type = g_hash_table_lookup (bus->priv->types, identifier);
	message_identifier_free (identifier);

	if (!message_type)
	{
		return G_TYPE_INVALID;
	}
	else
	{
		return *message_type;
	}
}

/**
 * soli_message_bus_register:
 * @bus: a #SoliMessageBus
 * @message_type: the message type
 * @object_path: the object path
 * @method: the method to register
 *
 * Register a message on the bus. A message must be registered on the bus before
 * it can be send. This function registers the type for @method at
 * @object_path.
 *
 * This function emits a #SoliMessageBus::registered signal.
 *
 */
void
soli_message_bus_register (SoliMessageBus *bus,
                            GType            message_type,
                            const gchar     *object_path,
                            const gchar	    *method)
{
	MessageIdentifier *identifier;
	GType *ntype;

	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (soli_message_is_valid_object_path (object_path));
	g_return_if_fail (g_type_is_a (message_type, SOLI_TYPE_MESSAGE));

	if (soli_message_bus_is_registered (bus, object_path, method))
	{
		g_warning ("Message type for '%s.%s' is already registered",
		           object_path,
		           method);
	}

	identifier = message_identifier_new (object_path, method);
	ntype = g_slice_new (GType);

	*ntype = message_type;

	g_hash_table_insert (bus->priv->types,
	                     identifier,
	                     ntype);

	g_signal_emit (bus,
	               message_bus_signals[REGISTERED],
	               0,
	               object_path,
	               method);
}

static void
soli_message_bus_unregister_real (SoliMessageBus  *bus,
                                   const gchar      *object_path,
                                   const gchar      *method,
                                   gboolean          remove_from_store)
{
	MessageIdentifier *identifier;

	identifier = message_identifier_new (object_path, method);

	if (!remove_from_store || g_hash_table_remove (bus->priv->types,
	                                               identifier))
	{
		g_signal_emit (bus,
		               message_bus_signals[UNREGISTERED],
		               0,
		               object_path,
		               method);
	}

	message_identifier_free (identifier);
}

/**
 * soli_message_bus_unregister:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 *
 * Unregisters a previously registered message type. This is especially useful
 * for plugins which should unregister message types when they are deactivated.
 *
 * This function emits the #SoliMessageBus::unregistered signal.
 *
 */
void
soli_message_bus_unregister (SoliMessageBus  *bus,
                              const gchar      *object_path,
                              const gchar      *method)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (object_path != NULL);
	g_return_if_fail (method != NULL);

	soli_message_bus_unregister_real (bus,
	                                   object_path,
	                                   method,
	                                   TRUE);
}

typedef struct
{
	SoliMessageBus *bus;
	const gchar *object_path;
} UnregisterInfo;

static gboolean
unregister_each (MessageIdentifier *identifier,
                 GType             *gtype,
                 UnregisterInfo    *info)
{
	if (g_strcmp0 (identifier->object_path, info->object_path) == 0)
	{
		soli_message_bus_unregister_real (info->bus,
		                                   identifier->object_path,
		                                   identifier->method,
		                                   FALSE);

		return TRUE;
	}

	return FALSE;
}

/**
 * soli_message_bus_unregister_all:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 *
 * Unregisters all message types for @object_path. This is especially useful for
 * plugins which should unregister message types when they are deactivated.
 *
 * This function emits the #SoliMessageBus::unregistered signal for all
 * unregistered message types.
 *
 */
void
soli_message_bus_unregister_all (SoliMessageBus *bus,
                                  const gchar     *object_path)
{
	UnregisterInfo info = {bus, object_path};

	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (object_path != NULL);

	g_hash_table_foreach_remove (bus->priv->types,
	                             (GHRFunc)unregister_each,
	                             &info);
}

/**
 * soli_message_bus_is_registered:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 *
 * Check whether a message type @method at @object_path is registered on the
 * bus.
 *
 * Return value: %TRUE if the @method at @object_path is a registered message
 *               type on the bus
 *
 */
gboolean
soli_message_bus_is_registered (SoliMessageBus  *bus,
                                 const gchar	  *object_path,
                                 const gchar      *method)
{
	MessageIdentifier *identifier;
	gboolean ret;

	g_return_val_if_fail (SOLI_IS_MESSAGE_BUS (bus), FALSE);
	g_return_val_if_fail (object_path != NULL, FALSE);
	g_return_val_if_fail (method != NULL, FALSE);

	identifier = message_identifier_new (object_path, method);
	ret = g_hash_table_lookup (bus->priv->types, identifier) != NULL;
	message_identifier_free (identifier);

	return ret;
}

typedef struct
{
	SoliMessageBusForeach func;
	gpointer user_data;
} ForeachInfo;

static void
foreach_type (MessageIdentifier *identifier,
              GType              *message_type,
              ForeachInfo       *info)
{
	info->func (identifier->object_path,
	            identifier->method,
	            info->user_data);
}

/**
 * soli_message_bus_foreach:
 * @bus: the #SoliMessageBus
 * @func: (scope call): the callback function
 * @user_data: the user data to supply to the callback function
 *
 * Calls @func for each message type registered on the bus
 *
 */
void
soli_message_bus_foreach (SoliMessageBus        *bus,
                           SoliMessageBusForeach  func,
                           gpointer		   user_data)
{
	ForeachInfo info = {func, user_data};

	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (func != NULL);

	g_hash_table_foreach (bus->priv->types, (GHFunc)foreach_type, &info);
}

/**
 * soli_message_bus_connect:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: function to be called when message @method at @object_path is sent
 * @user_data: (allow-none): user_data to use for the callback
 * @destroy_data: (allow-none): function to evoke with @user_data as argument when @user_data
 *                needs to be freed
 *
 * Connect a callback handler to be evoked when message @method at @object_path
 * is sent over the bus.
 *
 * Return value: the callback identifier
 *
 */
guint
soli_message_bus_connect (SoliMessageBus	*bus,
                           const gchar		*object_path,
                           const gchar		*method,
                           SoliMessageCallback  callback,
                           gpointer		 user_data,
                           GDestroyNotify	 destroy_data)
{
	Message *message;

	g_return_val_if_fail (SOLI_IS_MESSAGE_BUS (bus), 0);
	g_return_val_if_fail (object_path != NULL, 0);
	g_return_val_if_fail (method != NULL, 0);
	g_return_val_if_fail (callback != NULL, 0);

	/* lookup the message and create if it does not exist yet */
	message = lookup_message (bus, object_path, method, TRUE);

	return add_listener (bus, message, callback, user_data, destroy_data);
}

/**
 * soli_message_bus_disconnect:
 * @bus: a #SoliMessageBus
 * @id: the callback id as returned by soli_message_bus_connect()
 *
 * Disconnects a previously connected message callback.
 *
 */
void
soli_message_bus_disconnect (SoliMessageBus *bus,
                              guint            id)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));

	process_by_id (bus, id, remove_listener);
}

/**
 * soli_message_bus_disconnect_by_func:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: (scope call): the connected callback
 * @user_data: the user_data with which the callback was connected
 *
 * Disconnects a previously connected message callback by matching the
 * provided callback function and user_data. See also
 * soli_message_bus_disconnect().
 *
 */
void
soli_message_bus_disconnect_by_func (SoliMessageBus      *bus,
                                      const gchar	   *object_path,
                                      const gchar	   *method,
                                      SoliMessageCallback  callback,
                                      gpointer		    user_data)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));

	process_by_match (bus,
	                  object_path,
	                  method,
	                  callback,
	                  user_data,
	                  remove_listener);
}

/**
 * soli_message_bus_block:
 * @bus: a #SoliMessageBus
 * @id: the callback id
 *
 * Blocks evoking the callback specified by @id. Unblock the callback by
 * using soli_message_bus_unblock().
 *
 */
void
soli_message_bus_block (SoliMessageBus *bus,
                         guint		  id)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));

	process_by_id (bus, id, block_listener);
}

/**
 * soli_message_bus_block_by_func:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: (scope call): the callback to block
 * @user_data: the user_data with which the callback was connected
 *
 * Blocks evoking the callback that matches provided @callback and @user_data.
 * Unblock the callback using soli_message_bus_unblock_by_func().
 *
 */
void
soli_message_bus_block_by_func (SoliMessageBus      *bus,
                                 const gchar	      *object_path,
                                 const gchar	      *method,
                                 SoliMessageCallback  callback,
                                 gpointer	       user_data)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));

	process_by_match (bus,
	                  object_path,
	                  method,
	                  callback,
	                  user_data,
	                  block_listener);
}

/**
 * soli_message_bus_unblock:
 * @bus: a #SoliMessageBus
 * @id: the callback id
 *
 * Unblocks the callback specified by @id.
 *
 */
void
soli_message_bus_unblock (SoliMessageBus *bus,
                           guint	    id)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));

	process_by_id (bus, id, unblock_listener);
}

/**
 * soli_message_bus_unblock_by_func:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 * @callback: (scope call): the callback to block
 * @user_data: the user_data with which the callback was connected
 *
 * Unblocks the callback that matches provided @callback and @user_data.
 *
 */
void
soli_message_bus_unblock_by_func (SoliMessageBus      *bus,
                                   const gchar	        *object_path,
                                   const gchar	        *method,
                                   SoliMessageCallback  callback,
                                   gpointer	         user_data)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));

	process_by_match (bus,
	                  object_path,
	                  method,
	                  callback,
	                  user_data,
	                  unblock_listener);
}

static void
send_message_real (SoliMessageBus *bus,
                   SoliMessage    *message)
{
	bus->priv->message_queue = g_list_prepend (bus->priv->message_queue,
	                                           g_object_ref (message));

	if (bus->priv->idle_id == 0)
	{
		bus->priv->idle_id = g_idle_add_full (G_PRIORITY_HIGH,
		                                      (GSourceFunc)idle_dispatch,
		                                      bus,
		                                      NULL);
	}
}

/**
 * soli_message_bus_send_message:
 * @bus: a #SoliMessageBus
 * @message: the message to send
 *
 * This sends the provided @message asynchronously over the bus. To send
 * a message synchronously, use soli_message_bus_send_message_sync(). The
 * convenience function soli_message_bus_send() can be used to easily send
 * a message without constructing the message object explicitly first.
 *
 */
void
soli_message_bus_send_message (SoliMessageBus *bus,
                                SoliMessage    *message)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (SOLI_IS_MESSAGE (message));

	send_message_real (bus, message);
}

/**
 * soli_message_bus_send_message_sync:
 * @bus: a #SoliMessageBus
 * @message: the message to send
 *
 * This sends the provided @message synchronously over the bus. To send
 * a message asynchronously, use soli_message_bus_send_message(). The
 * convenience function soli_message_bus_send_sync() can be used to easily send
 * a message without constructing the message object explicitly first.
 *
 */
void
soli_message_bus_send_message_sync (SoliMessageBus *bus,
                                     SoliMessage    *message)
{
	g_return_if_fail (SOLI_IS_MESSAGE_BUS (bus));
	g_return_if_fail (SOLI_IS_MESSAGE (message));

	dispatch_message (bus, message);
}

static SoliMessage *
create_message (SoliMessageBus *bus,
                const gchar     *object_path,
                const gchar     *method,
                const gchar     *first_property,
                va_list          var_args)
{
	GType message_type;
	SoliMessage *msg;

	message_type = soli_message_bus_lookup (bus, object_path, method);

	if (message_type == G_TYPE_INVALID)
	{
		g_warning ("Could not find message type for '%s.%s'",
		           object_path,
		           method);

		return NULL;
	}

	msg = SOLI_MESSAGE (g_object_new_valist (message_type,
	                                          first_property,
	                                          var_args));

	if (msg)
	{
		g_object_set (msg,
		              "object_path",
		              object_path,
		              "method",
		              method,
		              NULL);
	}

	return msg;
}

/**
 * soli_message_bus_send:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 * @first_property: the first property
 * @...: NULL terminated list of key/value pairs
 *
 * This provides a convenient way to quickly send a message @method at
 * @object_path asynchronously over the bus. The variable argument list
 * specifies key (string) value pairs used to construct the message arguments.
 * To send a message synchronously use soli_message_bus_send_sync().
 */
void
soli_message_bus_send (SoliMessageBus *bus,
                        const gchar     *object_path,
                        const gchar     *method,
                        const gchar     *first_property,
                        ...)
{
	va_list var_args;
	SoliMessage *message;

	va_start (var_args, first_property);

	message = create_message (bus,
	                          object_path,
	                          method,
	                          first_property,
	                          var_args);

	if (message)
	{
		send_message_real (bus, message);
		g_object_unref (message);
	}
	else
	{
		g_warning ("Could not instantiate message");
	}

	va_end (var_args);
}

/**
 * soli_message_bus_send_sync:
 * @bus: a #SoliMessageBus
 * @object_path: the object path
 * @method: the method
 * @first_property: the first property
 * @...: (allow-none): %NULL terminated list of key/value pairs
 *
 * This provides a convenient way to quickly send a message @method at
 * @object_path synchronously over the bus. The variable argument list
 * specifies key (string) value pairs used to construct the message
 * arguments. To send a message asynchronously use soli_message_bus_send().
 *
 * Return value: (allow-none) (transfer full): the constructed #SoliMessage.
 *               The caller owns a reference to the #SoliMessage and should
 *               call g_object_unref() when it is no longer needed.
 */
SoliMessage *
soli_message_bus_send_sync (SoliMessageBus *bus,
                             const gchar     *object_path,
                             const gchar     *method,
                             const gchar     *first_property,
                             ...)
{
	va_list var_args;
	SoliMessage *message;

	va_start (var_args, first_property);
	message = create_message (bus,
	                          object_path,
	                          method,
	                          first_property,
	                          var_args);

	if (message)
	{
		dispatch_message (bus, message);
	}

	va_end (var_args);

	return message;
}

/* ex:set ts=8 noet: */
