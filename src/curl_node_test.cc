#include <string>

#include "node.h"
#include "curl_node.h"
#include "ns.h"
#include "wflualib.h"
#include "gtest/gtest.h"

using namespace std;
using namespace wfcore;

TEST(NodeFactory, CurlNode) {
    wfcore_global_init();

    lua_State * L = luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{
            name='a', type='curl', 
            url='https://news.ycombinator.com/'
        }
    )";
    int s = luaL_dostring(L, lua_src);
    ASSERT_FALSE(s);

    auto n1 = Ns::globalNs.getNode("a");
    ASSERT_NE(n1, nullptr);
    n1->exec(nullptr);
}
