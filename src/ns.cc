#include <iostream>
#include "node.h"
#include "ns.h"

namespace wfcore {

// initialize global namespace object
string Ns::currentNs;
Ns Ns::globalNs;

void Ns::addNode(string node_name, shared_ptr<Node> node_obj) {
    mNodes[node_name] = node_obj;
}

void Ns::addEdge(string from_node, string to_node) {
    mEdges.insert(make_pair(from_node, to_node));
}

string Ns::nextNode(string node_name) {
    map<string, string>::iterator it = mEdges.find(node_name);
    if (it != mEdges.end()) {
        return it->second;
    }

    return string();
}

vector<string> Ns::nextNodes(string node_name) {
    vector<string> nodes;
    multimap<string, string>::iterator it;

    auto m = mEdges.equal_range(node_name);

    if (m.first != mEdges.end()) {
        for (it=m.first; it != m.second; it++) {
            nodes.push_back(it->second);
        }
    }

    return nodes;
}


shared_ptr<Node> Ns::getNode(string node_name) {
    map<string, shared_ptr<Node>>::iterator it = mNodes.find(node_name);
    if (it != mNodes.end()) {
        return it->second;
    }

    return nullptr;
}


void Ns::clearNs() {
    this->mEdges.clear();
    this->mNodes.clear();
}

}
