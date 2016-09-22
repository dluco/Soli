#ifndef _SOLI_UTILS_H_
#define _SOLI_UTILS_H_

#include <glib.h>

G_BEGIN_DECLS

#define GBOOLEAN_TO_POINTER(i) (GINT_TO_POINTER ((i) ? 2 : 1))
#define GPOINTER_TO_BOOLEAN(i) ((gboolean) ((GPOINTER_TO_INT(i) == 2) ? TRUE : FALSE))

G_END_DECLS

#endif /* _SOLI_UTILS_H_ */
