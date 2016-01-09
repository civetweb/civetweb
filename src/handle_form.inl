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

int
handle_form_data(struct mg_connection *conn, struct mg_form_data_handler *fdh)
{
	const char *content_type;
	const char *boundary;
	int has_body_data =
	    (conn->request_info.content_length > 0) || (conn->is_chunked);
	char *data;

	content_type = mg_get_header(conn, "Content-Type");
	if (content_type == NULL) {
		/* This request does not have a content type set */
		return 0;
	}

	if (!mg_strcasecmp(content_type, "APPLICATION/X-WWW-FORM-URLENCODED")) {
		/* Encoded in key/value pairs */

		if (has_body_data) {
			/* form data is in the request body data */

		} else {
			/* form data is in the query string */
		}
	}

	if (!mg_strncasecmp(content_type, "MULTIPART/FORM-DATA;", 20)) {
		/* Encoded in multipart body */

		if (!has_body_data) {
			/* Error: no form data */
			return 0;
		}
	}

	return 0;
}