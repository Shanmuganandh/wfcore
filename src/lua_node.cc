#include <iostream>
#include <string>
#include "node.h"
#include "lua_node.h"


using namespace std;

namespace wfcore {

    LuaNode::LuaNode(Attribute &a) {
        this->lua_chunk = a.getLuaChunk("chunk");
    }

    LuaNode::~LuaNode() {
        this->lua_chunk = vector<char>();
    }

    int LuaNode::exec(Context * ctx) {
        ctx->execLuaChunk(this->lua_chunk);

        return 0;
    }

}
