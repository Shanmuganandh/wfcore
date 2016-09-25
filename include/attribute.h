#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

extern "C" {
    #include "lua.h"
    #include "lauxlib.h"
    #include "lualib.h"
}

#include <memory>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

namespace wfcore {

class LuaStackTopIsNotTableException: public exception {
    const char * what() const throw() {
        return "Attribute Class expects luaStack top to be of type table";
    }

};

class Attribute {
    public:
        Attribute() = delete;

        /*
        * Attribute object could be initialized with a lua_State, expects a
        * LUA_TABLE object at the top of the stack.
        */
        Attribute(lua_State* lstate);

        ~Attribute();

        // Setters
        void pushAttributeError(string err_msg);
        
        // Getters
        tuple<int, bool> getInt(string key);
        tuple<float, bool> getFloat(string key);
        string getString(string key);
        bool getBool(string key);
        vector<char> getLuaChunk(string key);
        
        // Errors while initializing a Node instance with an Attribute object.
        vector<string> getAttributeErrors();

    private:
        lua_State* lstate;
        vector<string> attr_err_msgs;
};

}

#endif

