/*
** $Id: lexcept.h $
** Exception runtime
** See Copyright Notice in lua.h
*/

#ifndef lexcept_h
#define lexcept_h


#include <unwind.h>

#include "lua.h"


#define LUA_EXCEPTION_CLASS 0x4c55410000000000 /* "LUA\0\0\0\0\0" */

LUAI_FUNC struct _Unwind_Exception *luaEH_setlandingpad (void);

LUAI_FUNC void luaEH_raise (struct _Unwind_Exception *exception);

LUAI_FUNC _Unwind_Reason_Code luaEH_personality_v0 (int version,
        _Unwind_Action actions, _Unwind_Exception_Class exception_class,
        struct _Unwind_Exception *exception, struct _Unwind_Context *context);

#endif

