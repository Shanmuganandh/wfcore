#include <string>
#include "node.h"
#include "ns.h"
#include "wflualib.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;


TEST(WFKernelLuaLib, DefNode) {
    wfcore_global_init();

    lua_State * L= luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{
            name='a', 
            type='echo', 
            message='Hi from a'}; 
        Node{
            name='b', 
            type='echo', 
            message='Hi from b'}
    )";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    auto n1 = Ns::globalNs.getNode("a");
    ASSERT_NE(n1, nullptr);
    n1->exec(nullptr);

    auto n2 = Ns::globalNs.getNode("b");
    ASSERT_NE(n2, nullptr);
    n2->exec(nullptr);
    
    auto non_exisited_node_instance = Ns::globalNs.getNode("c");
    ASSERT_EQ(non_exisited_node_instance, nullptr);

}

TEST(WFKernelLuaLib, DefNodeDependentNode) {
    wfcore_global_init();

    lua_State * L= luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{name='a', type='echo',
            message='Hi from a'};
        Node{name='b', type='echo', 
            after='a', 
            message='Hi from b'}
    )";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    auto next_node = Ns::globalNs.nextNode("a");
    ASSERT_EQ(next_node, "b");
}
