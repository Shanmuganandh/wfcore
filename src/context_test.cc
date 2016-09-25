#include <string>
#include "context.h"
#include "attribute.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;

TEST(Context, TestJumpToFromLua) {
    Context ctx;
    ctx.execLuaExpr("jumpTo(ctl, 'dummy_node')");
    ASSERT_EQ(ctx.getJumpTo(), "dummy_node");
}

TEST(Context, TestExecLuaChunk) {
    // Preparing a lua chunk
    lua_State * L = luaL_newstate();
    luaL_openlibs(L);

    const char * lua_src = "a = 5; n={f=function () b = a + 3 end};";
    int s = luaL_dostring(L, lua_src);

    ASSERT_FALSE(s);

    lua_getglobal(L, "n");
    auto attrib = Attribute(L);
    auto lua_chunk = attrib.getLuaChunk("f");
    
    ASSERT_GT(lua_chunk.size(), 0);

    // exec lua chunk in a context
    Context ctx;
    ctx.execLuaExpr("a = 5;");
    s = ctx.execLuaChunk(lua_chunk);
    // chunk execution for successful
    ASSERT_EQ(s, 0);

    // checking if the lua chunk eval has set a global variable
    ctx.execLuaExpr("print(b);");

}
