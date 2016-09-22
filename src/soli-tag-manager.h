#ifndef _SOLI_TAG_MANAGER_H_
#define _SOLI_TAG_MANAGER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define SOLI_TYPE_TAG_MANAGER (soli_tag_manager_get_type ())

G_DECLARE_FINAL_TYPE (SoliTagManager, soli_tag_manager, SOLI, TAG_MANAGER, GObject)

SoliTagManager	*soli_tag_manager_new			(void);

SoliTagManager	*soli_tag_manager_get_default	(void);

G_END_DECLS

#endif /* _SOLI_TAG_MANAGER_H_ */
