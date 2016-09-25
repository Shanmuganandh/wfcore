#include <iostream>
#include <fstream>
#include <string>

#include "attribute.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;

TEST(AttributeConstruction, AccessingValidKeysFromLuaTable) {
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    const char * lua_src = "color = {r=10, g=20, b=30}";
    int s = luaL_dostring(L, lua_src);

    ASSERT_FALSE(s);

    lua_getglobal(L, "color");

    auto attrib = Attribute(L);
    auto red_int_tuple = attrib.getInt("r");
    int red_int = get<0>(red_int_tuple);
    ASSERT_EQ(red_int, 10);
    ASSERT_TRUE(attrib.getString("r").compare("10") == 0);
}


TEST(AttributeConstruction, AccessingInvalidKeysFromLuaTable) {
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    const char * lua_src = "color = {r=10, g=20, b=30}";
    int s = luaL_dostring(L, lua_src);

    ASSERT_FALSE(s);

    lua_getglobal(L, "color");

    auto attrib = Attribute(L);
    auto x_int_tuple = attrib.getInt("x");
    int x_int_status = get<1>(x_int_tuple);
    ASSERT_EQ(x_int_status, false);

    auto y_str = attrib.getString("y");
    ASSERT_TRUE(y_str.empty());

    auto z_bool = attrib.getBool("z");
    ASSERT_FALSE(z_bool);

    auto g_int_tuple = attrib.getInt("g");
    int g_int = get<0>(g_int_tuple);
    ASSERT_EQ(g_int, 20);
}

TEST(AttributeGet, GetLuaChunk) {
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    const char * lua_src = "n={f=function (x) return x * x end}";
    int s = luaL_dostring(L, lua_src);

    ASSERT_FALSE(s);

    lua_getglobal(L, "n");
    auto attrib = Attribute(L);
    auto lua_chunk = attrib.getLuaChunk("f");
    
    ASSERT_GT(lua_chunk.size(), 0);

    // evaluating a lua chunk
    luaL_checkstack(L, 10, NULL);
    int load_status = luaL_loadbuffer(L, lua_chunk.data(), lua_chunk.size(), NULL);
    ASSERT_EQ(load_status, 0);
    lua_pushinteger(L, 5);
    int call_status = lua_pcall(L, 1, 1, 0);
    ASSERT_EQ(call_status, 0);
    if (call_status != 0)
        cout << "error in calling chunk: " << lua_tostring(L, -1) <<   endl;
    int square = lua_tointeger(L, -1);
    ASSERT_EQ(square, 25);
}


TEST(AttributeGet, GetLuaChunkReferringUpValue) {
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    const char * lua_src = "a = 5; n={f=function (x) return a + x end};";
    int s = luaL_dostring(L, lua_src);

    ASSERT_FALSE(s);

    lua_getglobal(L, "n");
    auto attrib = Attribute(L);
    auto lua_chunk = attrib.getLuaChunk("f");
    
    ASSERT_GT(lua_chunk.size(), 0);

    // evaluating a lua chunk
    luaL_checkstack(L, 10, NULL);
    int load_status = luaL_loadbuffer(L, lua_chunk.data(), lua_chunk.size(), NULL);
    ASSERT_EQ(load_status, 0);
    lua_pushinteger(L, 5);
    int call_status = lua_pcall(L, 1, 1, 0);
    ASSERT_EQ(call_status, 0);
    if (call_status != 0)
        cout << "error in calling chunk: " << lua_tostring(L, -1) <<   endl;
    int square = lua_tointeger(L, -1);
    ASSERT_EQ(square, 10);
}
