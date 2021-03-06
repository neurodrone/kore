/*
 * Copyright (c) 2013 Joris Vink <joris@coders.se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __H_HTTP_H
#define __H_HTTP_H

#define HTTP_HEADER_MAX_LEN	4096
#define HTTP_URI_LEN		2000
#define HTTP_USERAGENT_LEN	256
#define HTTP_REQ_HEADER_MAX	25
#define HTTP_MAX_QUERY_ARGS	10

struct http_header {
	char			*header;
	char			*value;

	TAILQ_ENTRY(http_header)	list;
};

struct http_arg {
	char			*name;
	char			*value;

	TAILQ_ENTRY(http_arg)	list;
};

#define HTTP_METHOD_GET		0
#define HTTP_METHOD_POST	1

#define HTTP_REQUEST_COMPLETE	0x01
#define HTTP_REQUEST_DELETE	0x02

struct http_request {
	u_int8_t		method;
	u_int8_t		flags;
	int			status;
	u_int64_t		start;
	u_int64_t		end;
	char			host[KORE_DOMAINNAME_LEN];
	char			path[HTTP_URI_LEN];
	char			*agent;
	struct connection	*owner;
	struct spdy_stream	*stream;
	struct kore_buf		*post_data;
	void			*hdlr_extra;

	TAILQ_HEAD(, http_header)	req_headers;
	TAILQ_HEAD(, http_header)	resp_headers;
	TAILQ_HEAD(, http_arg)		arguments;
	TAILQ_ENTRY(http_request)	list;
};

extern int	http_request_count;

void		http_init(void);
void		http_process(void);
time_t		http_date_to_time(char *);
void		http_request_free(struct http_request *);
int		http_response(struct http_request *, int,
		    u_int8_t *, u_int32_t);
int		http_request_header_get(struct http_request *, char *, char **);
void		http_response_header_add(struct http_request *, char *, char *);
int		http_request_new(struct connection *, struct spdy_stream *,
		    char *, char *, char *, struct http_request **);

int		http_argument_urldecode(char *);
int		http_header_recv(struct netbuf *);
int		http_generic_404(struct http_request *);
char		*http_post_data_text(struct http_request *);
int		http_populate_arguments(struct http_request *);
void		http_argument_multiple_free(struct http_arg *);
int		http_argument_lookup(struct http_request *,
		    const char *, char **);
int		http_argument_multiple_lookup(struct http_request *,
		    struct http_arg *);

void		kore_accesslog(struct http_request *);

enum http_status_code {
	HTTP_STATUS_CONTINUE			= 100,
	HTTP_STATUS_SWITCHING_PROTOCOLS		= 101,
	HTTP_STATUS_OK				= 200,
	HTTP_STATUS_CREATED			= 201,
	HTTP_STATUS_ACCEPTED			= 202,
	HTTP_STATUS_NON_AUTHORITATIVE		= 203,
	HTTP_STATUS_NO_CONTENT			= 204,
	HTTP_STATUS_RESET_CONTENT		= 205,
	HTTP_STATUS_PARTIAL_CONTENT		= 206,
	HTTP_STATUS_MULTIPLE_CHOICES		= 300,
	HTTP_STATUS_MOVED_PERMANENTLY		= 301,
	HTTP_STATUS_FOUND			= 302,
	HTTP_STATUS_SEE_OTHER			= 303,
	HTTP_STATUS_NOT_MODIFIED		= 304,
	HTTP_STATUS_USE_PROXY			= 305,
	HTTP_STATUS_TEMPORARY_REDIRECT		= 307,
	HTTP_STATUS_BAD_REQUEST			= 400,
	HTTP_STATUS_UNAUTHORIZED		= 401,
	HTTP_STATUS_PAYMENT_REQUIRED		= 402,
	HTTP_STATUS_FORBIDDEN			= 403,
	HTTP_STATUS_NOT_FOUND			= 404,
	HTTP_STATUS_METHOD_NOT_ALLOWED		= 405,
	HTTP_STATUS_NOT_ACCEPTABLE		= 406,
	HTTP_STATUS_PROXY_AUTH_REQUIRED		= 407,
	HTTP_STATUS_REQUEST_TIMEOUT		= 408,
	HTTP_STATUS_CONFLICT			= 409,
	HTTP_STATUS_GONE			= 410,
	HTTP_STATUS_LENGTH_REQUIRED		= 411,
	HTTP_STATUS_PRECONDITION_FAILED		= 412,
	HTTP_STATUS_REQUEST_ENTITY_TOO_LARGE	= 413,
	HTTP_STATUS_REQUEST_URI_TOO_LARGE	= 414,
	HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE	= 415,
	HTTP_STATUS_REQUEST_RANGE_INVALID	= 416,
	HTTP_STATUS_EXPECTATION_FAILED		= 417,
	HTTP_STATUS_INTERNAL_ERROR		= 500,
	HTTP_STATUS_NOT_IMPLEMENTED		= 501,
	HTTP_STATUS_BAD_GATEWAY			= 502,
	HTTP_STATUS_SERVICE_UNAVAILABLE		= 503,
	HTTP_STATUS_GATEWAY_TIMEOUT		= 504,
	HTTP_STATUS_BAD_VERSION			= 505
};

#endif /* !__H_HTTP_H */
