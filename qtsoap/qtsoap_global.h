#ifndef QTSOAP_GLOBAL_H
#define QTSOAP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTSOAP_LIBRARY)
#  define QTSOAPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTSOAPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTSOAP_GLOBAL_H
