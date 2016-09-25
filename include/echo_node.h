#ifndef ECHO_NODE_H
#define ECHO_NODE_H

#include <memory>
#include <string>

using namespace std;

namespace wfcore {

class EchoNode : public Node {
    public:
        EchoNode(Attribute &a);
        ~EchoNode();
        int exec(Context *);

        static shared_ptr<Node> ConsNode(Attribute &a) {
            return dynamic_pointer_cast<Node>(make_shared<EchoNode>(a));
        }

    private:
        string message;
};

}

#endif
