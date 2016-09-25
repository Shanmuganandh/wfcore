#include "context.h"
#include "ns.h"
#include <iostream>

namespace wfcore {

static int set_jump_to(lua_State * l) {
    const char * val;
    string node_name;

    Context * ctx = (Context *) lua_touserdata(l, 1);
    val = lua_tostring(l, 2);
    if (val != NULL) {
        node_name.assign(val);
    }
    
    ctx->setJumpTo(node_name);
    
    return 0;
}

const string Context::var_prefix = "var::";

Context::Context() {
    // create a new lua state and open all libs
    this->lstate = luaL_newstate();
    luaL_openlibs(this->lstate);

    // registering the context object as global ctx in lua_state
    lua_pushlightuserdata(this->lstate, (void *) this);
    lua_setglobal(this->lstate, "ctl");
    lua_register(this->lstate, "jumpTo", set_jump_to);
}

Context::~Context() {
    lua_close(this->lstate);
}

// Control flow primitives
string Context::getJumpTo() {
    return this->jump_to_node;
}

void Context::pushNodeToPipeline(string node_name) {
    this->epipe.push_back(node_name);
}

string Context::nextNode() {
    if (this->epipe.size() == 0)
        return "";

    auto node_name = epipe[0];

    auto _nxtns = Ns::globalNs.nextNodes(node_name);
    this->epipe.insert(this->epipe.end(), _nxtns.begin(), _nxtns.end());
    this->epipe.pop_front();

    return node_name;
}

void Context::setJumpTo(string node_name) {
    this->jump_to_node = node_name;
}

void Context::breakWithError(string error_message) {
    cout << "Context error: " << error_message << endl;
    this->err_msg = error_message;
}

void Context::execLuaExpr(string lua_expr) {
    int s = luaL_dostring(this->lstate, lua_expr.c_str());

    if (s) {
        breakWithError(lua_tostring(this->lstate, -1));
        lua_pop(this->lstate, 1);
    }
}

int Context::execLuaChunk(vector<char> lua_chunk) {
    luaL_checkstack(this->lstate, 10, NULL);
    int s = luaL_loadbuffer(this->lstate, lua_chunk.data(), lua_chunk.size(), NULL);
    
    if (s != LUA_OK) {
        return s;
    }

    s = lua_pcall(this->lstate, 0, 0, 0);

    if (s != LUA_OK) {
        breakWithError(string(lua_tostring(this->lstate, -1)));
    }

    return s;
}

string Context::resolveArg(string val) {
    if (val.compare(0, 5, var_prefix) != 0) {
        // does not have var:: prefix
        return val;
    }

    string arg_val;
    const char * t;

    auto var_name = val.substr(5, string::npos);
    luaL_checkstack(this->lstate, 5, NULL);
    lua_getglobal(this->lstate, var_name.c_str());
    t = lua_tostring(lstate, -1);
    if (t != NULL) {
        arg_val.assign(t);
    }

    lua_pop(lstate, 1);
    return arg_val;
}

}
