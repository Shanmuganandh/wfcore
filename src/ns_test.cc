#include <string>
#include "ns.h"
#include "wflualib.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;


TEST(NameSpace, DefNs) {
    lua_State * L = luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Ns("test_ns1");
    )";

    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    ASSERT_EQ(Ns::currentNs, "test_ns1");
    ASSERT_FALSE(Ns::currentNs.empty());
    
    // Changing the namespace
    const char * lua_src1 = R"(
        Ns("test_ns2");
    )";
    s = luaL_dostring(L, lua_src1);
    ASSERT_FALSE(s);
    ASSERT_EQ(Ns::currentNs, "test_ns2");
    ASSERT_FALSE(Ns::currentNs.empty());
}

TEST(NameSpace, SetNsAndDefaultNs) {
    lua_State * L = luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Ns("test_ns1");
    )";

    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    ASSERT_EQ(Ns::currentNs, "test_ns1");
    ASSERT_FALSE(Ns::currentNs.empty());
    
    // switching to default Ns
    const char * lua_src1 = R"(
        DefaultNs();
    )";
    s = luaL_dostring(L, lua_src1);
    ASSERT_FALSE(s);
    ASSERT_TRUE(Ns::currentNs.empty());
}
