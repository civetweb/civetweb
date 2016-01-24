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


/**********************/
/* proposed interface */

enum {
	FORM_DISPOSITION_SKIP = 0x0,
	FORM_DISPOSITION_GET = 0x1,
	FORM_DISPOSITION_STORE = 0x2,
	FORM_DISPOSITION_READ = 0x3,
	FORM_DISPOSITION_ABORT = 0x10
};


struct mg_form_data_handler {
	int (*field_found)(const char *key,
	                   const char *filename,
	                   char *path,
	                   size_t pathlen,
	                   void *user_data);
	int (*field_get)(const char *key,
	                 const char *filename,
	                 const char *value,
	                 size_t valuelen,
	                 void *user_data);
	void *user_data;
};


int mg_handle_form_data(struct mg_connection *conn,
                        struct mg_form_data_handler *fdh);

/* end of interface */
/********************/

static int
url_encoded_field_found(const char *key,
                        size_t key_len,
                        const char *filename,
                        size_t filename_len,
                        char *path,
                        size_t path_len,
                        struct mg_form_data_handler *fdh)
{
	char key_dec[1024];
	char filename_dec[1024];
	int key_dec_len;
	int filename_dec_len;

	key_dec_len =
	    mg_url_decode(key, (int)key_len, key_dec, (int)sizeof(key_dec), 1);

	if (((size_t)key_dec_len >= (size_t)sizeof(key_dec)) || (key_dec_len < 0)) {
		return FORM_DISPOSITION_SKIP;
	}

	if (filename) {
		filename_dec_len = mg_url_decode(filename,
		                                 (int)filename_len,
		                                 filename_dec,
		                                 (int)sizeof(filename_dec),
		                                 1);

		if (((size_t)filename_dec_len >= (size_t)sizeof(filename_dec))
		    || (filename_dec_len < 0)) {
			return FORM_DISPOSITION_SKIP;
		}
	} else {
		filename_dec[0] = 0;
	}

	return fdh->field_found(
	    key_dec, filename_dec, path, path_len, fdh->user_data);
}


static int
url_encoded_field_get(const char *key,
                      size_t key_len,
                      const char *filename,
                      size_t filename_len,
                      const char *value,
                      size_t value_len,
                      struct mg_form_data_handler *fdh)
{
	char key_dec[1024];
	char filename_dec[1024];

	char *value_dec = mg_malloc(value_len + 1);
	int value_dec_len;

	if (!value_dec) {
		/* TODO: oom */
		return FORM_DISPOSITION_ABORT;
	}

	mg_url_decode(key, (int)key_len, key_dec, (int)sizeof(key_dec), 1);

	if (filename) {
		mg_url_decode(filename,
		              (int)filename_len,
		              filename_dec,
		              (int)sizeof(filename_dec),
		              1);
	} else {
		filename_dec[0] = 0;
	}

	value_dec_len =
	    mg_url_decode(value, (int)value_len, value_dec, (int)value_len + 1, 1);

	return fdh->field_get(key_dec,
	                      filename_dec,
	                      value_dec,
	                      (size_t)value_dec_len,
	                      fdh->user_data);
}


int
mg_handle_form_data(struct mg_connection *conn,
                    struct mg_form_data_handler *fdh)
{
	const char *content_type;
	char path[512];
	char buf[1024];
	int disposition;
	int buf_fill = 0;

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
		/* The entire data has already been loaded, so there is no nead to
		 * call mg_read. We just need to split the query string into key-value
		 * pairs. */
		data = conn->request_info.query_string;
		if (!data) {
			/* No query string. */
			return 0;
		}

		/* Split data in a=1&b=xy&c=3&c=4 ... */
		while (*data) {
			const char *val = strchr(data, '=');
			const char *next;
			ptrdiff_t keylen, vallen;

			if (!val) {
				break;
			}
			keylen = val - data;

			/* In every "field_found" callback we ask what to do with the
			 * data ("disposition"). This could be:
			 * FORM_DISPOSITION_SKIP (0) ... ignore the value if this field
			 * FORM_DISPOSITION_GET (1) ... read the data and call the get
			 *                              callback function
			 * FORM_DISPOSITION_STORE (2) ... store the data in a file
			 *                                TODO: get a filename
			 * FORM_DISPOSITION_READ (3) ... let the user read the data
			 *                               (for parsing long data on the fly)
			 * FORM_DISPOSITION_ABORT (flag) ... stop parsing
			 */
			memset(path, 0, sizeof(path));
			disposition = url_encoded_field_found(
			    data, (size_t)keylen, NULL, 0, path, sizeof(path) - 1, fdh);

			val++;
			next = strchr(val, '&');
			if (next) {
				vallen = next - val;
				next++;
			} else {
				vallen = (ptrdiff_t)strlen(val);
				next = val + vallen;
			}

			if (disposition == FORM_DISPOSITION_GET) {
				/* Call callback */
				url_encoded_field_get(
				    data, (size_t)keylen, NULL, 0, val, (size_t)vallen, fdh);
			}
			if (disposition == FORM_DISPOSITION_STORE) {
				/* Store the content to a file */
				FILE *f = fopen(path, "wb");
				if (f != NULL) {
					size_t n = (size_t)fwrite(val, 1, (size_t)vallen, f);
					if ((n != (size_t)vallen) || (ferror(f))) {
						mg_cry(conn,
						       "%s: Cannot write file %s",
						       __func__,
						       path);
					}
					fclose(f);
				} else {
					mg_cry(conn, "%s: Cannot create file %s", __func__, path);
				}
			}
			if (disposition == FORM_DISPOSITION_READ) {
				/* The idea of "disposition=read" is to let the API user read
				 * data chunk by chunk and to some data processing on the fly.
				 * This should avoid the need to store data in the server:
				 * It should neither be stored in memory, like
				 * "disposition=get" does, nor in a file like
				 * "disposition=store".
				 * However, for a "GET" request this does not make any much
				 * sense, since the data is already stored in memory, as it is
				 * part of the query string.
				 */
				/* TODO, or not TODO, that is the question */
			}
			if ((disposition & FORM_DISPOSITION_ABORT)
			    == FORM_DISPOSITION_ABORT) {
				/* Stop parsing the request */
				break;
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
		int all_data_read = 0;

		/* Read body data and split it in a=1&b&c=3&c=4 ... */
		/* The encoding is like in the "GET" case above, but here we read data
		 * on the fly */
		for (;;) {
			/* TODO(high): Handle (text) fields with data > sizeof(buf). */
			const char *val;
			const char *next;
			ptrdiff_t keylen, vallen;
			ptrdiff_t used;
			FILE *fstore = NULL;
			int end_of_key_value_pair_found = 0;

			if ((size_t)buf_fill < (sizeof(buf) - 1)) {

				size_t to_read = sizeof(buf) - 1 - (size_t)buf_fill;
				int r = mg_read(conn, buf + (size_t)buf_fill, to_read);
				if (r < 0) {
					/* read error */
					return 0;
				}
				if (r != (int)to_read) {
					/* TODO: Create a function to get "all_data_read" from
					 * the conn object. Add data is read if the Content-Length
					 * has been reached, or if chunked encoding is used and
					 * the end marker has been read, or if the connection has
					 * been closed. */
					all_data_read = 1;
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

			/* Call callback */
			memset(path, 0, sizeof(path));
			disposition = url_encoded_field_found(
			    buf, (size_t)keylen, NULL, 0, path, sizeof(path) - 1, fdh);

			if ((disposition & FORM_DISPOSITION_ABORT)
			    == FORM_DISPOSITION_ABORT) {
				/* Stop parsing the request */
				break;
			}

			if (disposition == FORM_DISPOSITION_STORE) {
				fstore = fopen(path, "wb");
				if (!fstore) {
					mg_cry(conn, "%s: Cannot create file %s", __func__, path);
				}
			}

			/* Loop to read values larger than sizeof(buf)-keylen-2 */
			do {
				next = strchr(val, '&');
				if (next) {
					vallen = next - val;
					next++;
					end_of_key_value_pair_found = 1;
				} else {
					vallen = (ptrdiff_t)strlen(val);
					next = val + vallen;
				}

				if (fstore) {
					size_t n = (size_t)fwrite(val, 1, (size_t)vallen, fstore);
					if ((n != (size_t)vallen) || (ferror(fstore))) {
						mg_cry(conn,
						       "%s: Cannot write file %s",
						       __func__,
						       path);
						fclose(fstore);
						fstore = NULL;
					}
				}
				if (disposition == FORM_DISPOSITION_GET) {
					if (!end_of_key_value_pair_found && !all_data_read) {
						/* TODO: check for an easy way to get longer data */
						mg_cry(conn,
						       "%s: Data too long for callback",
						       __func__);
						return 0;
					}
					/* Call callback */
					url_encoded_field_get(
					    buf, (size_t)keylen, NULL, 0, val, (size_t)vallen, fdh);
				}

				if (!end_of_key_value_pair_found) {
					/* TODO: read more data */
					break;
				}

			} while (!end_of_key_value_pair_found);

			if (fstore) {
				fclose(fstore);
			}


			/* Proceed to next entry */
			used = next - buf;
			memmove(buf, buf + (size_t)used, sizeof(buf) - (size_t)used);
			buf_fill -= used;
		}

		return 0;
	}

	if (!mg_strncasecmp(content_type, "MULTIPART/FORM-DATA;", 20)) {
		/* The form data is in the request body data, encoded as multipart
		 * content (see https://www.ietf.org/rfc/rfc1867.txt,
		 * https://www.ietf.org/rfc/rfc2388.txt). */
		const char *boundary;
		size_t bl;
		int r;
		struct mg_request_info part_header;
		char *hbuf, *hend, *fbeg, *fend, *nbeg, *nend;
		const char *content_disp;

		memset(&part_header, 0, sizeof(part_header));

		/* There has to be a BOUNDARY definition in the Content-Type header */
		if (mg_strncasecmp(content_type + 21, "BOUNDARY=", 9)) {
			/* Malformed request */
			return 0;
		}

		boundary = content_type + 30;
		bl = strlen(boundary);

		r = mg_read(conn,
		            buf + (size_t)buf_fill,
		            sizeof(buf) - 1 - (size_t)buf_fill);
		if (r < 0) {
			/* read error */
			return 0;
		}
		buf_fill += r;
		buf[buf_fill] = 0;
		if (buf_fill < 1) {
			/* No data */
			return 0;
		}

		if (buf[0] != '-' || buf[1] != '-') {
			/* Malformed request */
			return 0;
		}
		if (strncmp(buf + 2, boundary, bl)) {
			/* Malformed request */
			return 0;
		}
		if (buf[bl + 2] != '\r' || buf[bl + 3] != '\n') {
			/* Malformed request */
			return 0;
		}

		/* Next, we need to get the part header: Read until \r\n\r\n */
		hbuf = buf + bl + 4;
		hend = strstr(hbuf, "\r\n\r\n");
		if (!hend) {
			/* Malformed request */
			return 0;
		}
		parse_http_headers(&hbuf, &part_header);
		if ((hend + 2) != hbuf) {
			/* Malformed request */
			return 0;
		}

		/* According to the RFC, every part has to have a header field like:
		 * Content-Disposition: form-data; name="..." */
		content_disp = get_header(&part_header, "Content-Disposition");
		if (!content_disp) {
			/* Malformed request */
			return 0;
		}

		/* Get the mandatory name="..." part of the Content-Disposition
		 * header. */
		nbeg = strstr(content_disp, "name=\"");
		if (!nbeg) {
			/* Malformed request */
			return 0;
		}
		nbeg += 6;
		nend = strchr(nbeg, '\"');
		if (!nend) {
			/* Malformed request */
			return 0;
		}

		/* Get the optional filename="..." part of the Content-Disposition
		 * header. */
		fbeg = strstr(content_disp, "filename=\"");
		if (fbeg) {
			fbeg += 10;
			fend = strchr(fbeg, '\"');
			if (!fend) {
				/* Malformed request (the filename field is optional, but if it
				 * exists, it needs to be terminated correctly). */
				return 0;
			}

			/* TODO: check Content-Type */
			/* Content-Type: application/octet-stream */

		} else {
			fend = fbeg;
		}

		memset(path, 0, sizeof(path));
		disposition = url_encoded_field_found(nbeg,
		                                      (size_t)(nend - nbeg),
		                                      fbeg,
		                                      (size_t)(fend - fbeg),
		                                      path,
		                                      sizeof(path) - 1,
		                                      fdh);

		do {
			const char *next = strstr(hbuf, "--");
			while (next && (strncmp(next + 2, boundary, bl))) {
				/* found "--" not followed by boundary: look for next "--" */
				next = strstr(next + 1, "--");
			}

			if (disposition == FORM_DISPOSITION_GET) {
				if (!next) {
					/* TODO: check for an easy way to get longer data */
					mg_cry(conn, "%s: Data too long for callback", __func__);
					return 0;
				}

				/* Call callback */
				url_encoded_field_get(nbeg,
				                      (size_t)(nend - nbeg),
				                      fbeg,
				                      (size_t)(fend - fbeg),
				                      hend,
				                      (size_t)(next - hend),
				                      fdh);
			}
			if (disposition == FORM_DISPOSITION_STORE) {
				/* Store the content to a file */
				FILE *f = fopen(path, "wb");
				if (f != NULL) {

					/* TODO: store from part_header to next boundary */
					fclose(f);
				} else {
					mg_cry(conn, "%s: Cannot create file %s", __func__, path);
				}
			}
			if ((disposition & FORM_DISPOSITION_ABORT)
			    == FORM_DISPOSITION_ABORT) {
				/* Stop parsing the request */
				return 0;
			}
		} while (0 /* TODO */);

		/* TODO: handle multipart request */
		return 0;
	}

	/* Unknown Content-Type */
	return 0;
}
