/* Copyright (c) 2014 the Civetweb developers
 * Copyright (c) 2004-2012 Sergey Lyubka
 * This file is a part of civetweb project, http://github.com/bel2125/civetweb
 */

#include "civetweb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int generate_content(struct mg_connection *conn)
{
	el html,head,body,table,td,tr,meta;
	int size;
	char * output;
	
	html=c(NULL,"html");
		head=c(html,"head");
			meta=c(head,"meta");
			sa(meta,"charset","utf-8");
		body=c(html,"body");
			t(body,"Hello world!");
			c(body,"br");
		table=c(body,"table");
		sa(table,"border","1");
			tr=c(table,"tr");
					td=c(tr,"td");
						t(td,"Cell 1");
					td=c(tr,"td");
						t(td,"Cell 2");
			tr=c(table,"tr");
					td=c(tr,"td");
						t(td,"Cell 3");
					td=c(tr,"td");
						t(td,"Cell 4");
			

	fxml_toString(html,&output,&size); 
	if (size==-1)
	{
		printf("Malloc failed!\n");
		return -1;
	}
	size+=15; /*+15 is for <!DOCTYPE html>*/
	
	mg_printf(conn,
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: %d\r\n"
	"\r\n"
	"<!DOCTYPE html>%s",size, output);
	
	/* This will free the memory. Pass NULL instead of output if
	 * you want to manually free it later.
	 */
	fxml_delete(html,&output); 
	return 1;
}

/* Main program: Set callbacks and start the server.  */
int main(void)
{
    /* Test server will use this port */
    const char * PORT = "8080";

    /* Startup options for the server */
    struct mg_context *ctx;
    const char *options[] = {
        "listening_ports", PORT,
        NULL};
    struct mg_callbacks callbacks;

    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.begin_request = generate_content;

    /* Display a welcome message */
    printf("XML generation demo.\n");
    printf("Open http://localhost:%s/ im your browser.\n\n", PORT);

    /* Start the server */
    ctx = mg_start(&callbacks, NULL, options);

    /* Wait until thr user hits "enter", then stop the server */
    getchar();
    mg_stop(ctx);

    return 0;
}

