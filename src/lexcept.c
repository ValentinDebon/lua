/*
** $Id: lexcept.c $
** Exception and control flow
** See Copyright Notice in lua.h
*/

#define lexcept_c
#define LUA_CORE

#include "lprefix.h"


#include "lua.h"

#include "lexcept.h"
#include "llimits.h"


static _Unwind_Ptr landingPad;

static void cleanup (_Unwind_Reason_Code reason,
                     struct _Unwind_Exception *exception) {
  UNUSED(reason);
  UNUSED(exception);
}

struct _Unwind_Exception *luaEH_setlandingpad (void) {
  /* it is imperative luaEH_setlandingpad stay in a separate translation
  ** unit, apart from luaD_rawrunprotected, else the compiler may optimize
  ** its conditional branches as it infers luaEH_setlandingpad should only
  ** return NULL values.
  */
  landingPad = __builtin_extend_pointer(__builtin_return_address(0));
  return NULL;
}

void luaEH_raise (struct _Unwind_Exception *exception) {
  exception->exception_class = LUA_EXCEPTION_CLASS;
  exception->exception_cleanup = cleanup;
  _Unwind_RaiseException(exception);
}

_Unwind_Reason_Code luaEH_personality_v0 (int version,
        _Unwind_Action actions, _Unwind_Exception_Class exception_class,
        struct _Unwind_Exception *exception, struct _Unwind_Context *context) {
  UNUSED(exception_class);

  if (version != 1)
    return _URC_FATAL_PHASE1_ERROR;

  if (actions & _UA_SEARCH_PHASE)
    return _URC_HANDLER_FOUND;

  if (actions & _UA_CLEANUP_PHASE) {
    _Unwind_SetGR(context,
                  __builtin_eh_return_data_regno(0),
                  __builtin_extend_pointer(exception));

    _Unwind_SetIP(context, landingPad);

    return _URC_INSTALL_CONTEXT;
  }

  return _URC_CONTINUE_UNWIND;
}
