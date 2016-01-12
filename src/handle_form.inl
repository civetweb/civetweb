/* Copyright (c) 2016 the Civetweb developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/********************/
/* EXPERIMENTAL !!! */
/********************/
#ifdef USE_EXPERIMENTAL

void
mirror_body___dev_helper(struct mg_connection *conn)
{
	/* TODO: remove this function when handle_form_data is completed. */
	char buf[256];
	int r;
	mg_printf(conn, "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\n\r\n");

	do {
		r = mg_read(conn, buf, sizeof(buf));
		mg_write(conn, buf, r);
	} while (r > 0);
}


struct mg_form_data_handler {
	int (*field_found)(const char *key,
	                   size_t keylen,
	                   const char *value,
	                   size_t vallen,
	                   void *user_data);
	int (*file_found)(const char *key,
	                  size_t keylen,
	                  const char *filename,
	                  int *disposition,
	                  void *user_data);
	void *user_data;
};


int
mg_handle_form_data(struct mg_connection *conn,
                    struct mg_form_data_handler *fdh)
{
	const char *content_type;
	const char *boundary;
	const char *data;
	int has_body_data =
	    (conn->request_info.content_length > 0) || (conn->is_chunked);

	/* There are three ways to encode data from a HTML form:
	 * 1) method: GET (default)
	 *    The form data is in the HTTP query string.
	 * 2) method: POST, enctype: "application/x-www-form-urlencoded"
	 *    The form data is in the request body.
	 *    The body is url encoded (the default encoding for POST).
	 * 3) method: POST, enctype: "multipart/form-data".
	 *    The form data is in the request body of a multipart message.
	 *    This is the typical way to handle file upload from a form.
	 */

	if (!has_body_data) {
		if (strcmp(conn->request_info.request_method, "GET")) {
			/* No body data, but not a GET request.
			 * This is not a valid form request. */
			return 0;
		}

		/* GET request: form data is in the query string. */
		data = conn->request_info.query_string;
		if (!data) {
			/* No query string. */
			return 0;
		}

		/* Split data in a=1&b&c=3&c=4 ... */
		while (*data) {
			const char *val = strchr(data, '=');
			const char *next;
			ptrdiff_t keylen, vallen;

			if (!val) {
				break;
			}
			keylen = val - data;
			val++;
			next = strchr(val, '&');
			if (next) {
				vallen = next - val;
				next++;
			} else {
				vallen = strlen(val);
			}

			/* Call callback */
			fdh->field_found(
			    data, (size_t)keylen, val, (size_t)vallen, fdh->user_data);

			/* Proceed to next entry */
			data = val + vallen;
		}

		return 0;
	}

	content_type = mg_get_header(conn, "Content-Type");

	if (!content_type
	    || !mg_strcasecmp(content_type, "APPLICATION/X-WWW-FORM-URLENCODED")) {
		/* The form data is in the request body data, encoded in key/value
		 * pairs. */

		/* Read body data and split it in a=1&b&c=3&c=4 ... */
		/* The encoding is like in the "GET" case above, but here we read data
		 * on the fly */
		char buf[/*10*/ 24];
		int buf_fill = 0;

		memset(buf, 0, sizeof(buf));
		data = buf;

		for (;;) {

			const char *val;
			const char *next;
			ptrdiff_t keylen, vallen;

			if (!*data) {
				buf_fill = mg_read(conn, buf, sizeof(buf) - 1);
				if (buf_fill <= 0) {
					/* No more data available */
					break;
				}
				buf[buf_fill] = 0;

				data = buf;
			}

			val = strchr(data, '=');

			if (!val) {
				size_t used = data - buf;
				char *tgt = buf + sizeof(buf) - 1 - used;

				/* Drop used data (used = data - buf) */
				memmove(buf, data, used);
				buf_fill -= used;
				buf_fill += mg_read(conn, tgt, used);
				buf[sizeof(buf) - 1] = 0;
				data = buf;

				val = strchr(data, '=');
				if (!val) {
					break;
				}
			}
			keylen = val - data;
			val++;
			
            next = strchr(val, '&');
            if (!next) {
               /* TODO: could need to add data to the buffer to get the next & */
            }

			if (next) {
				vallen = next - val;
				next++;
			} else {
				vallen = strlen(val);
				next = val + vallen;
			}

			/* Call callback */
			fdh->field_found(
			    data, (size_t)keylen, val, (size_t)vallen, fdh->user_data);

			/* Proceed to next entry */
			data = next;	
		}

		return 0;
	}

	mirror_body___dev_helper(conn);

	if (!mg_strncasecmp(content_type, "MULTIPART/FORM-DATA;", 20)) {
		/* The form data is in the request body data, encoded as multipart
		 * content. */

		/* There has to be a BOUNDARY definition in the Content-Type header */
		if (!mg_strncasecmp(content_type + 20, "BOUNDARY=", 9)) {
			/* Malformed request */
			return 0;
		}

		/* TODO: handle multipart request */
		return 0;
	}

	/* Unknown Content-Type */
	return 0;
}

#endif
