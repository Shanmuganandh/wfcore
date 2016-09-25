#include <iostream>
#include "wflualib.h"
#include "ns.h"

using namespace std;

namespace wfcore {

static int def_node(lua_State * lstate) {
    Attribute a = Attribute(lstate);
    string node_type = a.getString("type");
    string node_name = a.getString("name");

    if (!Ns::currentNs.empty()) {
        node_name.insert(0, Ns::currentNs);
    }

    Ns::globalNs.addNode(node_name, NodeFactory::NewNode(node_type, a));

    string dependent_on = a.getString("after");

    if (!dependent_on.empty()) {
            Ns::globalNs.addEdge(dependent_on, node_name);
    }

    return 0;
}

static int def_ns(lua_State * lstate) {
    const char * val;
    string ns_name;

    val = lua_tostring(lstate, -1);
    if (val != NULL) {
        ns_name.assign(val);
    }

    if (!ns_name.empty()) {
        Ns::setCurrentNs(ns_name);
    }
    return 0;
}

static int def_default_ns(lua_State * lstate) {
    Ns::switchToDefaultNs();
    return 0;
}

int luaL_openwflib(lua_State * lstate) {
    lua_register(lstate, "Node", def_node);
    lua_register(lstate, "Ns", def_ns);
    lua_register(lstate, "DefaultNs", def_default_ns);
    return 0;
}

}
