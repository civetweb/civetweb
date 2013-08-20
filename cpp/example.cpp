// Copyright (c) 2013 Thomas Davis
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE

// Simple example program on how to use Embedded C++ interface.

#include "CivetServer.h"

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

int main(int argc, char *argv[]) {

	const char * options[] = { "document_root", DOCUMENT_ROOT,
			"listening_ports", PORT, 0 };

	CivetServer server(options);

	server.addHandler(EXAMPLE_URI, new ExampleHandler());
	server.addHandler(EXIT_URI, new ExitHandler());

	printf("Browse files at http://localhost:%s/\n", PORT);
	printf("Run example at http://localhost:%s%s\n", PORT, EXIT_URI);
	printf("Exit at http://localhost:%s%s\n", PORT, EXIT_URI);

	while (!exitNow) {
		sleep(1);
	}

	printf("Bye!\n");

	return 0;
}
