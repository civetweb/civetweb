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


enum {
	FORM_DISPOSITION_SKIP = 0x0,
	FORM_DISPOSITION_GET = 0x1,
	FORM_DISPOSITION_STORE = 0x2,
	FORM_DISPOSITION_READ = 0x4,
	FORM_DISPOSITION_ABORT = 0x10
};


struct mg_form_data_handler {
	int (*field_found)(const char *key,
	                   size_t keylen,
	                   const char *filename,
	                   int *disposition,
	                   void *user_data);
	void *user_data;
};


static int
url_encoded_field_found(const char *key,
                        size_t keylen,
                        const char *filename,
                        int *disposition,
                        struct mg_form_data_handler *fdh)
{
	/* Call callback */
	char key_dec[1024];
	int ret =
	    mg_url_decode(key, (size_t)keylen, key_dec, (int)sizeof(key_dec), 1);
	if ((ret < sizeof(key_dec)) && (ret >= 0)) {
		return fdh->field_found(
		    key, keylen, filename, disposition, fdh->user_data);
	}
	return FORM_DISPOSITION_SKIP;
}


void
url_encoded_field_get(const char *key,
                      size_t keylen,
                      const char *filename,
                      const char *value,
                      size_t valuelen,
                      struct mg_form_data_handler *fdh)
{
	char key_dec[1024];
	char *value_dec = mg_malloc(valuelen + 1);
	if (!value_dec) {
		/* TODO: oom */
		return;
	}

	mg_url_decode(key, (size_t)keylen, key_dec, (int)sizeof(key_dec), 1);
	mg_url_decode(value, (size_t)valuelen, value_dec, (int)valuelen + 1, 1);


	/* TODO: Form decode */
	fdh->field_get(
	    key, keylen, filename, value, strlen(value_dec), fdh->user_data);
}


int
mg_handle_form_data(struct mg_connection *conn,
                    struct mg_form_data_handler *fdh)
{
	const char *content_type;
	const char *boundary;
	int disposition;

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
		const char *data;

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

			url_encoded_field_found(
			    data, (size_t)keylen, NULL, &disposition, fdh);

			val++;
			next = strchr(val, '&');
			if (next) {
				vallen = next - val;
				next++;
			} else {
				vallen = strlen(val);
				next = val + vallen;
			}

			if (disposition == FORM_DISPOSITION_GET) {
				/* Call callback */
				url_encoded_field_get(
				    data, (size_t)keylen, val, (size_t)vallen, fdh);
			}

			/* Proceed to next entry */
			data = next;
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
		char buf[1024];
		int buf_fill = 0;

		for (;;) {
			/* TODO(high): Handle (text) fields with data > sizeof(buf). */
			const char *val;
			const char *next;
			ptrdiff_t keylen, vallen;
			int used;

			if (buf_fill < (sizeof(buf) - 1)) {

				int r =
				    mg_read(conn, buf + buf_fill, sizeof(buf) - 1 - buf_fill);
				if (r < 0) {
					/* read error */
					return 0;
				}
				buf_fill += r;
				buf[buf_fill] = 0;
				if (buf_fill < 1) {
					break;
				}
			}

			val = strchr(buf, '=');

			if (!val) {
				break;
			}
			keylen = val - buf;
			val++;

			next = strchr(val, '&');
			if (next) {
				vallen = next - val;
				next++;
			} else {
				vallen = strlen(val);
				next = val + vallen;
			}

			/* Call callback */
			fdh->field_found(
			    buf, (size_t)keylen, NULL, &disposition, fdh->user_data);

			/* Proceed to next entry */
			used = next - buf;
			memmove(buf, buf + used, sizeof(buf) - used);
			buf_fill -= used;
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