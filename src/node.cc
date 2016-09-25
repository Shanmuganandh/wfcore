#include <string>
#include <memory>
#include <map>

#include "node.h"
#include "attribute.h"
#include "echo_node.h"
#include "curl_node.h"
#include "lua_node.h"

using namespace std;

namespace wfcore {

// initialize the static variable in NodeFactory
NodeFactory::ConsNodeMap NodeFactory::mConsNode;

void NodeFactory::RegisterNodeType(const string &type, ConsNode cn) {
    mConsNode[type] = cn;
}

void NodeFactory::UnregisterNodeType(const string &type) {
    mConsNode.erase(type);
}

shared_ptr<Node> NodeFactory::NewNode(const string &type, Attribute &a) {
    ConsNodeMap::iterator it = mConsNode.find(type);
    if (it != mConsNode.end()) {
        return (it->second)(a);
    }

    return nullptr;
}


void wfcore_global_init() {
    NodeFactory::RegisterNodeType("curl", CurlNode::ConsNode);
    NodeFactory::RegisterNodeType("echo", EchoNode::ConsNode);
    NodeFactory::RegisterNodeType("lua", LuaNode::ConsNode);
}

}
