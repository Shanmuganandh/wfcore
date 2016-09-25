#include <iostream>
#include <string>
#include "node.h"
#include "echo_node.h"

using namespace std;

namespace wfcore {

EchoNode::EchoNode(Attribute &a) {
    this->message.assign(a.getString("message"));
}

EchoNode::~EchoNode() {}

int EchoNode::exec(Context * ctx) {
    cout << "Echo: " << this->message << endl;
    return 0;
}

}

