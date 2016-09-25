#ifndef CONTEXT_H
#define CONTEXT_H

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}
#include <string>
#include <vector>
#include <deque>

using namespace std;

namespace wfcore {

class Context {
    public:
        Context();
        ~Context();

        // Control flow primitives
        void pushNodeToPipeline(string);
        string nextNode();
        string getJumpTo();
        void setJumpTo(string);
        void breakWithError(string);

        // Exec lua expression, result of the expression ignored
        void execLuaExpr(string);
        int execLuaChunk(vector<char>);

        // misc
        string resolveArg(string);

        // TODO:
        // eval lua expression, expect a value in return
        // return type of the lua expression has to be thought through.
        
    private:
        lua_State * lstate;
        string jump_to_node;
        string err_msg;
        deque<string> epipe;
        static const string var_prefix; 
};

}

#endif
