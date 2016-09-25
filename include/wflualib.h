#ifndef WFLUALIB_H
#define WFLUALIB_H

#include "node.h"

namespace wfcore {

static int def_node(lua_State * lstate);
static int def_ns(lua_State * lstate);
static int def_default_ns(lua_State * lstate);

int luaL_openwflib(lua_State * lstate);

}
#endif
