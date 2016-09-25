#include "node.h"
#include "ns.h"
#include "wflualib.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;

TEST(NodeFactory, LuaNode) {
    wfcore_global_init();

    lua_State * L = luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{
            name='proc1', 
            type='lua',
            chunk=
                function() 
                    a=10; b=15 
                end
        };
    )";
    int s = luaL_dostring(L, lua_src);
    if (s != LUA_OK) {
        cout << "LuaException: " << lua_tostring(L, -1) << endl;
    }
    ASSERT_FALSE(s);

    Context ctx;
    auto n1 = Ns::globalNs.getNode("proc1");
    ASSERT_NE(n1, nullptr);
    n1->exec(&ctx);
    ctx.execLuaExpr("print(a);print(b);");
}
