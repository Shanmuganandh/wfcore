#include <iostream>
#include <string>
#include "node.h"
#include "curl_node.h"

using namespace std;

namespace wfcore {

    CurlNode::CurlNode(Attribute &a) {
        // curl_global_init(CURL_GLOBAL_DEFAULT);
        this->curl = curl_easy_init();
        this->url = a.getString("url");
    }

    CurlNode::~CurlNode() {
        curl_easy_cleanup(this->curl);
        // curl_global_cleanup();
    }

    int CurlNode::exec(Context * ctx) {
        CURLcode res;
        
        if (this->curl) {
            this->url = ctx->resolveArg(this->url);
            curl_easy_setopt(this->curl, CURLOPT_URL, this->url.c_str());
            curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(this->curl, CURLOPT_SSL_VERIFYHOST, 0L);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
                cerr << "curl_easy_perform(); failed "
                    << curl_easy_strerror(res)
                    << endl;

            return 0;
        }

        return -1;
    }

}
