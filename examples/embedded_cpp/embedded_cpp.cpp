/*
 * Copyright (c) 2013 No Face Press, LLC
 * License http://opensource.org/licenses/mit-license.php MIT License
 */

// Simple example program on how to use Embedded C++ interface.

#include "CivetServer.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#define DOCUMENT_ROOT "."
#define PORT "8888"
#define EXAMPLE_URI "/example"
#define EXIT_URI "/exit"
bool exitNow = false;

class ExampleHandler: public CivetHandler {
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        mg_printf(conn, "<html><body>");
        mg_printf(conn, "<h2>This is example text!!!</h2>");
        mg_printf(conn, "<p>To exit <a href=\"%s\">click here</a></p>",
                EXIT_URI);
        mg_printf(conn, "</body></html>\n");
        return true;
    }
};

class ExitHandler: public CivetHandler {
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\n");
        mg_printf(conn, "Bye!\n");
        exitNow = true;
        return true;
    }
};

class AHandler: public CivetHandler {
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        mg_printf(conn, "<html><body>");
        mg_printf(conn, "<h2>This is the A handler!!!</h2>");
        mg_printf(conn, "</body></html>\n");
        return true;
    }
};

class ABHandler: public CivetHandler {
public:
    bool handleGet(CivetServer *server, struct mg_connection *conn) {
        mg_printf(conn, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
        mg_printf(conn, "<html><body>");
        mg_printf(conn, "<h2>This is the AB handler!!!</h2>");
        mg_printf(conn, "</body></html>\n");
        return true;
    }
};

int main(int argc, char *argv[]) {

    const char * options[] = { "document_root", DOCUMENT_ROOT,
            "listening_ports", PORT, 0 };

    CivetServer server(options);

    server.addHandler(EXAMPLE_URI, new ExampleHandler());
    server.addHandler(EXIT_URI, new ExitHandler());
    server.addHandler("/a", new AHandler());
    server.addHandler("/a/b", new ABHandler());  // going out of order with this to see if it will work.

    printf("Browse files at http://localhost:%s/\n", PORT);
    printf("Run example at http://localhost:%s%s\n", PORT, EXAMPLE_URI);
    printf("Exit at http://localhost:%s%s\n", PORT, EXIT_URI);

#define TEST1 "a=1 1&b=2%202&a=3+3"
    std::string rtn;
        std::string back;
    bool ok;
    ok = CivetServer::getParam(TEST1,strlen(TEST1),"a",rtn,0); 
    printf("TEST 1 %s,%d => %s    ok=%d\n", "a", 0, rtn.c_str(), ok?1:0);
    ok = CivetServer::getParam(TEST1,strlen(TEST1),"a",rtn,1); 
    printf("TEST 1 %s,%d => %s    ok=%d\n", "a", 1, rtn.c_str(), ok?1:0);
    ok = CivetServer::getParam(TEST1,strlen(TEST1),"a",rtn,2); 
    printf("TEST 1 %s,%d => %s    ok=%d\n", "a", 2, rtn.c_str(), ok?1:0);
    ok = CivetServer::getParam(TEST1,strlen(TEST1),"x",rtn,0); 
    printf("TEST 1 %s,%d => %s    ok=%d\n", "x", 0, rtn.c_str(), ok?1:0);
    ok = CivetServer::getParam(TEST1,strlen(TEST1),"b",rtn,0); 
    printf("TEST 1 %s,%d => %s    ok=%d\n", "b", 0, rtn.c_str(), ok?1:0);

    CivetServer::urlEncode(TEST1,strlen(TEST1),rtn); 
    CivetServer::urlDecode(rtn,back); 
    printf("[%s]\n[%s]\n[%s]\n", TEST1, rtn.c_str(), back.c_str());

    char b1[256], b2[256];
    mg_url_encode(TEST1, b1, 256);
    mg_url_decode(b1, 256, b2,256,1);
    printf("[%s]\n[%s]\n[%s]\n", TEST1, b1, b2);

    while (!exitNow) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    printf("Bye!\n");

    return 0;
}
