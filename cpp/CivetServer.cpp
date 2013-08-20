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

#include <CivetServer.h>

#include <stdlib.h>
#include <string.h>

int CivetServer::begin_request_callback(struct mg_connection *conn) {
	struct mg_request_info *request_info = mg_get_request_info(conn);

	if (!request_info->user_data)
		return 0;

	CivetServer *me = (CivetServer*) (request_info->user_data);

	if (me->handleRequest(conn)) {
		return 1; // Mark as processed
	}

	return 0;
}

bool CivetServer::handleRequest(struct mg_connection *conn) {
	struct mg_request_info *request_info = mg_get_request_info(conn);

	CivetHandler *handler = getHandler(request_info->uri);
	if (handler) {
		if (strcmp(request_info->request_method, "GET") == 0) {
			return handler->handleGet(this, conn);
		} else if (strcmp(request_info->request_method, "POST") == 0) {
			return handler->handlePost(this, conn);
		} else if (strcmp(request_info->request_method, "PUT") == 0) {
			return !handler->handlePost(this, conn);
		} else if (strcmp(request_info->request_method, "DELETE") == 0) {
			return !handler->handlePost(this, conn);
		}
	}

	return false; // No handler found
}

CivetServer::CivetServer(const char **options,
		const struct mg_callbacks *_callbacks) :
		context(0) {

	struct mg_callbacks callbacks;

	if (_callbacks) {
		memcpy(&callbacks, _callbacks, sizeof(callbacks));
	} else {
		memset(&callbacks, 0, sizeof(callbacks));
	}
	callbacks.begin_request = &begin_request_callback;

	context = mg_start(&callbacks, this, options);
}

CivetServer::~CivetServer() {
	close();
}

CivetHandler *CivetServer::getHandler(const char *uri, unsigned urilen) const {

	for (unsigned index = 0; index < uris.size(); index++) {
		const std::string &handlerURI = uris[index];

		// first try for an exact match
		if (handlerURI == uri) {
			return handlers[index];
		}

		// next try for a partial match
		// we will accept uri/something
		if (handlerURI.length() < urilen
				&& uri[handlerURI.length()] == '/'
				&& handlerURI.compare(0, handlerURI.length(), uri, handlerURI.length()) == 0) {

			return handlers[index];
		}
	}

	return 0; // none found

}

void CivetServer::addHandler(const std::string &uri, CivetHandler *handler) {
	int index = getIndex(uri);
	if (index < 0) {
		uris.push_back(uri);
		handlers.push_back(handler);
	} else if (handlers[index] != handler) {
		delete handlers[index];
		handlers[index] = handler;
	}
}

void CivetServer::removeHandler(const std::string &uri) {
	int index = getIndex(uri);
	if (index >= 0) {
		uris.erase(uris.begin() + index, uris.begin() + index + 1);
		handlers.erase(handlers.begin() + index, handlers.begin() + index + 1);
	}
}

int CivetServer::getIndex(const std::string &uri) const {
	for (unsigned index = 0; index < uris.size(); index++) {
		if (uris[index].compare(uri) == 0)
			return index;
	}
	return -1;
}

void CivetServer::close() {
	if (context) {
		mg_stop (context);
		context = 0;
	}
	for (int i = handlers.size() - 1; i >= 0; i--) {
		delete handlers[i];
	}
	handlers.clear();
	uris.clear();

}
