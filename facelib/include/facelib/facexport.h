#pragma once

#include <QtCore/QtGlobal>

#if defined(FACELIB)
#  define FACEXPORT Q_DECL_EXPORT
#else
#  define FACEXPORT Q_DECL_IMPORT
#endif
