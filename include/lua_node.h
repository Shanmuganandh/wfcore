#ifndef LUA_NODE_H
#define LUA_NODE_H

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#include <memory>
#include <string>

using namespace std;

namespace wfcore {

class LuaNode : public Node {
    public:
        LuaNode(Attribute &a);
        ~LuaNode();
        int exec(Context *);

        static shared_ptr<Node> ConsNode(Attribute &a) {
            return dynamic_pointer_cast<Node>(make_shared<LuaNode>(a));
        }

    private:
        vector<char> lua_chunk;
};

}


#endif
