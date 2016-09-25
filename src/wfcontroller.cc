#include <iostream>
#include "wfcontroller.h"


namespace wfcore {

int wfhandler(Context &ctx, string head_node) {
    ctx.pushNodeToPipeline(head_node);

    auto next_node = ctx.nextNode();

    while (!next_node.empty()) {
        auto n = Ns::globalNs.getNode(next_node);

        if (n)
            n->exec(&ctx);
        
        next_node = ctx.nextNode();
    }

    return 0;
}

}
