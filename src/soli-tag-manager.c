#include "soli-tag-manager.h"

#include <glib.h>

struct _SoliTagManager
{
	GObject parent_instance;
};

G_DEFINE_TYPE (SoliTagManager, soli_tag_manager, G_TYPE_OBJECT)

static void
soli_tag_manager_class_init (SoliTagManagerClass *klass)
{
//	GObjectClass *object_class = G_OBJECT_CLASS (klass);
}

static void
soli_tag_manager_init (SoliTagManager *manager)
{

}

SoliTagManager *
soli_tag_manager_new (void)
{
	return g_object_new (SOLI_TYPE_TAG_MANAGER, NULL);
}

SoliTagManager *
soli_tag_manager_get_default (void)
{
	static SoliTagManager *instance;

	if (instance == NULL)
	{
		instance = soli_tag_manager_new ();
		g_object_add_weak_pointer (G_OBJECT (instance),
								   (gpointer) &instance);
	}

	return instance;
}
