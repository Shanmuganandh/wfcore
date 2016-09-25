#ifndef CURL_NODE_H
#define CURL_NODE_H

#include <memory>

extern "C" {
    #include <curl/curl.h>
}

using namespace std;


namespace wfcore {

class CurlNode : public Node {
    public:
        CurlNode(Attribute &a);
        ~CurlNode();
        int exec(Context *);

        static shared_ptr<Node> ConsNode(Attribute &a) {
            return dynamic_pointer_cast<Node>(make_shared<CurlNode>(a));
        }
        
    private:
        CURL *curl;
        string url;
};

}

#endif
