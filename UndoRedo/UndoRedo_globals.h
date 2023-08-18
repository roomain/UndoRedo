#pragma once

#ifndef BUILD_STATIC
# if defined(UNDO_REDO_LIB)
#  define UNDO_REDO_EXPORT __declspec(dllexport)
# else
#  define UNDO_REDO_EXPORT __declspec(dllimport)
# endif
#else
# define UNDO_REDO_EXPORT
#endif