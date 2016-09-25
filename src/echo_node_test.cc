#include <string>

#include "node.h"
#include "echo_node.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;

TEST(NodeFactory, EchoNode) {
    lua_State * L = luaL_newstate();

    const char * lua_src = "echo_node = {message='Hello from lua'}";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    lua_getglobal(L, "echo_node");
    auto attrib = Attribute(L);

    wfcore_global_init();
    auto echo_node = NodeFactory::NewNode("echo", attrib);
    ASSERT_NE(echo_node, nullptr);
    echo_node->exec(nullptr);

    auto dummy_node = NodeFactory::NewNode("dummy", attrib);
    ASSERT_EQ(dummy_node, nullptr);
}

TEST(NodeFactory, InvalidNode) {
    lua_State * L = luaL_newstate();

    const char * lua_src = "dummy = {}";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    lua_getglobal(L, "dummy");
    auto attrib = Attribute(L);

    auto dummy_node = NodeFactory::NewNode("dummy", attrib);
    ASSERT_EQ(dummy_node, nullptr);
}
