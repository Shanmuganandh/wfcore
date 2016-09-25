#include <tuple>
#include <string>
#include <vector>
#include <iostream>

#include "attribute.h"

using namespace std;

namespace wfcore {

// lua_chunk_writer func
static int lua_chunk_writer(lua_State * l, const void * p, size_t sz, void * u) {
    auto buff = (vector<char> *) u;

    buff->reserve(sz);
    buff->insert(buff->end(), (char *) p, (char *)p + sz);

    return 0;
}


Attribute::Attribute(lua_State * lstate) {
    if (lstate == nullptr || !lua_istable(lstate, -1))
        throw LuaStackTopIsNotTableException();
    this->lstate = lstate;

}

tuple<int, bool> Attribute::getInt(string key) {
    int val, err;
    lua_checkstack(lstate, 5);
    lua_pushstring(lstate, key.c_str());
    lua_gettable(lstate, -2);
    val = (int) lua_tointegerx(lstate, -1, &err);
    lua_pop(lstate, 1);

    if (err)
        return make_tuple(val, true);
    else
        return make_tuple(0, false);
}

tuple<float, bool> Attribute::getFloat(string key) {
    float val;
    int err;
    lua_checkstack(lstate, 5);
    lua_pushstring(lstate, key.c_str());
    lua_gettable(lstate, -2);
    val = (float) lua_tonumberx(lstate, -1, &err);
    lua_pop(lstate, 1);

    if (err)
        return make_tuple(val, true);
    else
        return make_tuple(0, false);
}

string Attribute::getString(string key) {
    const char * val;
    string rval;
    
    lua_checkstack(lstate, 5);
    lua_pushstring(lstate, key.c_str());
    lua_gettable(lstate, -2);
    val = lua_tostring(lstate, -1);
    if (val != NULL) {
        rval.assign(val);
    }

    lua_pop(lstate, 1);
    return rval;
}

bool Attribute::getBool(string key) {
    int val;

    lua_checkstack(lstate, 5);
    lua_pushstring(lstate, key.c_str());
    lua_gettable(lstate, -2);
    val = lua_toboolean(lstate, -1);
    lua_pop(lstate, 1);

    if (val)
        return true;
    else
        return false;
}

vector<char> Attribute::getLuaChunk(string key) {
    vector<char> lua_chunk;

    lua_checkstack(lstate, 5);
    lua_pushstring(lstate, key.c_str());
    lua_gettable(lstate, -2);

    int is_lua_check = lua_isfunction(lstate, -1);
    if (is_lua_check) {
        int op_status = lua_dump(lstate, lua_chunk_writer, &lua_chunk);
    }

    lua_pop(lstate, 1);
    return lua_chunk;

}

// Error setter and getter
void Attribute::pushAttributeError(string err_msg) {
    attr_err_msgs.push_back(err_msg);
}

vector<string> Attribute::getAttributeErrors() {
    return attr_err_msgs;
}

Attribute::~Attribute() {
    attr_err_msgs.clear();
}

}
