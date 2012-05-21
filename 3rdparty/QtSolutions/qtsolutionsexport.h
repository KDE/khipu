
#ifndef qmml_EXPORT_H
#define qmml_EXPORT_H


/* needed for KDE_EXPORT macros */
#include <kdemacros.h>
#ifndef QTSOLUTIONS_EXPORT
# ifdef MAKE_QTSOLUTIONS_LIB
#  define QTSOLUTIONS_EXPORT KDE_EXPORT
# else
#  define QTSOLUTIONS_EXPORT KDE_IMPORT
# endif
#endif

#endif



