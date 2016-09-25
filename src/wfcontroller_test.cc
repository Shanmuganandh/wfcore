#include <string>
#include "ns.h"
#include "context.h"
#include "wflualib.h"
#include "wfcontroller.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;


TEST(WFController, SimpleHandler) {
    wfcore_global_init();

    lua_State * L= luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{name='a', type='echo',
            message='Hi from a'};
        Node{name='b', type='echo', 
            after='a', 
            message='Hi from b'};
        Node{name='c', type='lua',
            after='b',
            chunk=
                function()
                    y = x + 10
                end
        };
        Node{name='d', type='lua',
            after='c',
            chunk=
                function()
                    print(y);
                end
        };
        Node{name='e', type='curl',
            after='d',
            url='var::url'
        };
    )";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    Context ctx;
    ctx.execLuaExpr("x=5");
    ctx.execLuaExpr("url='http://www.google.com'");

    auto handler_status = wfhandler(ctx, "a");
    ASSERT_EQ(handler_status, 0);
}

TEST(WFController, BranchesInPaths) {
    wfcore_global_init();
    Ns::globalNs.clearNs();

    lua_State * L= luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{name='a', type='echo',
            message='Hi from a'};
        Node{name='b', type='echo', 
            after='a', 
            message='Hi from b'};
        Node{name='c', type='echo',
            after='a',
            message='Hi from c after a'};
        Node{name='d', type='echo',
            after='b',
            message='Hi from d after b'};
    )";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    Context ctx;

    auto handler_status = wfhandler(ctx, "a");
    ASSERT_EQ(handler_status, 0);
}
