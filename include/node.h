#ifndef NODE_H
#define NODE_H

#include <vector>
#include <memory>
#include <string>
#include <map>
#include "attribute.h"
#include "context.h"

using namespace std;

namespace wfcore {

class Node {
    public:
        virtual int exec(Context *) = 0;
};

class NodeFactory {
    public:
        typedef shared_ptr<Node> (*ConsNode)(Attribute &);
        static void RegisterNodeType(const string &type, ConsNode cn);
        static void UnregisterNodeType(const string &type);

        // Method to create an Instance of a concrete Node sub-class
        // Takes a reference to Attribute object, since we want the object
        // construction errors to be captured in the Attribute object.
        static shared_ptr<Node> NewNode(const string &type, Attribute &a);

    private:
        typedef map<string, ConsNode> ConsNodeMap;
        static ConsNodeMap mConsNode;
};

void wfcore_global_init();

}
#endif
