#include "uwsgi.h"
#include "ns.h"
#include "context.h"
#include "wflualib.h"
#include "wfcontroller.h"

using namespace std;
using namespace wfcore;

#ifdef __cplusplus
extern "C" {
#endif 
int mysym_function(struct wsgi_request *wsgi_req);
#ifdef __cplusplus
}
#endif

int mysym_function(struct wsgi_request *wsgi_req) {
    // read request variables
    if (uwsgi_parse_vars(wsgi_req)) {
            return -1;
    }

    wfcore_global_init();

    lua_State * L = luaL_newstate();
    luaL_openwflib(L);
    const char * lua_src = R"(
        Node{name='a', type='echo',
            message='Hi from a'};
        Node{name='b', type='echo', 
            after='a', 
            message='Hi from b'};
        Node{name='c', type='lua',
            after='b',
            chunk=
                function()
                    y = x + 10
                end
        };
        Node{name='d', type='lua',
            after='c',
            chunk=
                function()
                    print(y);
                end
        };
        Node{name='last', type='curl',
            after='d',
            url='https://news.ycombinator.com/'};
    )";
    int s = luaL_dostring(L, lua_src);

    Context ctx;
    ctx.execLuaExpr("x=5");

    auto handler_status = wfhandler(Ns::globalNs, ctx, "a");

    // get REMOTE_ADDR
    uint16_t vlen = 0;
    char *v = uwsgi_get_var(wsgi_req, (char *)"REMOTE_ADDR", 11, &vlen);

    // send status
    if (uwsgi_response_prepare_headers(wsgi_req, (char *)"200 OK", 6)) return -1;
    // send content_type
    if (uwsgi_response_add_content_type(wsgi_req, (char *)"text/plain", 10)) return -1;
    // send a custom header
    if (uwsgi_response_add_header(wsgi_req, (char *)"Foo", 3, (char *)"Bar", 3)) return -1;

    // send the body
    if (uwsgi_response_write_body_do(wsgi_req, v, vlen)) return -1;

    return UWSGI_OK;


}

/*
g++ -fPIC -shared -o wfwsgihandler.so `uwsgi --cflags` ./src/wf_wsgi_handler.cc -Iinclude -undefined dynamic_lookup
uwsgi --dlopen ./wfwsgihandler.so --symcall mysym_function --http-socket :9090 --http-socket-modifier1 18
*/
