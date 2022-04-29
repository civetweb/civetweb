/*
 * Copyright (c) 2018 the CivetWeb developers
 * MIT License
 */

/* Simple client demo. */
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "civetweb.h"


int
main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Requires server name as argument\n");
		return EXIT_FAILURE;
	}

	/* Init libcivetweb. */
	mg_init_library(MG_FEATURES_TLS);

	if (mg_check_feature(MG_FEATURES_TLS) != MG_FEATURES_TLS) {
		fprintf(stderr, "TLS is not available\n");
		return EXIT_FAILURE;
	}

	/* Connect client */
	char errbuf[256] = {0};
	struct mg_client_options opt = {0};
	opt.host = argv[1];       /* Host name from command line */
	opt.port = 443;           /* Default HTTPS port */
	opt.client_cert = NULL;   /* Client certificate, if required */
	opt.server_cert = NULL;   /* Server certificate to verify */
	opt.host_name = opt.host; /* Host name for SNI */
	struct mg_connection *cli =
	    mg_connect_client_secure(&opt, errbuf, sizeof(errbuf));

	/* Check return value: */
	if (cli == NULL) {
		fprintf(stderr, "Cannot connect client: %s\n", errbuf);
		return EXIT_FAILURE;
	}

	printf("cli: %p\n", cli);

	mg_printf(cli, "GET / HTTP/1.1\r\n");
	mg_printf(cli, "Host: %s\r\n", opt.host);
	mg_printf(cli, "Connection: close\r\n\r\n");

	int ret = mg_get_response(cli, errbuf, sizeof(errbuf), 10000);
	if (ret < 0) {
		fprintf(stderr, "Download failed: %s\n", errbuf);
		mg_close_connection(cli);
		return EXIT_FAILURE;
	}

	const struct mg_response_info *ri = mg_get_response_info(cli);
	if (ri == NULL) {
		fprintf(stderr, "mg_response_info failed\n");
		mg_close_connection(cli);
		return EXIT_FAILURE;
	}

	printf("Status: %i %s\n", ri->status_code, ri->status_text);
	printf("HTTP Version: %s\n", ri->http_version);
	printf("Content-Length: %lli\n", ri->content_length);
	printf("Headers:\n");
	int is_chunked = 0;
	for (int i = 0; i < ri->num_headers; i++) {
		printf("  %s: %s\n",
		       ri->http_headers[i].name,
		       ri->http_headers[i].value);
		if (!strcasecmp(ri->http_headers[i].name, "Transfer-Encoding")
		    && !strcasecmp(ri->http_headers[i].value, "chunked")) {
			is_chunked = 1;
		}
	}

	long long cont = ri->content_length;
	if (cont > 0) {
		/* Read regular content */
		printf("Content:\n");
		while (cont > 0) {
			char buf[1024];
			int ret = mg_read(cli, buf, sizeof(buf));
			if (ret <= 0) {
				printf("read error\n");
				break;
			}
			cont -= ret;
			fwrite(buf, 1, ret, stdout);
		}
	} else {
		/* Read chunked content (or content without content length) */
		char buf[1024];
		for (;;) {
			int ret = mg_read(cli, buf, sizeof(buf));
			if (ret <= 0)
				break;
			fwrite(buf, 1, ret, stdout);
		}
	}

	mg_close_connection(cli);
	return EXIT_SUCCESS;
}
