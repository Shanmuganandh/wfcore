#ifndef NS_H
#define NS_H

#include <map>
#include <string>
#include <memory>
#include "node.h"

using namespace std;

namespace wfcore {

class Ns {
    public:
        static string currentNs;
        static Ns globalNs;

        Ns() {};
        ~Ns() {};
        void addNode(string, shared_ptr<Node>);
        void addEdge(string, string);
        shared_ptr<Node> getNode(string);
        shared_ptr<Node> resolveNode(string);
        string nextNode(string);
        vector<string> nextNodes(string);

        void clearNs();

        static void setCurrentNs(string ns_name) {
            currentNs = ns_name;
        }

        static void switchToDefaultNs() { 
            currentNs.clear(); 
        }

    private:
        map<string, shared_ptr<Node>> mNodes; 
        multimap<string, string> mEdges;
};

}

#endif
