#if defined(USE_GNUTLS) // USE_GNUTLS used with NO_SSL

#include <gnutls/gnutls.h>
#include <gnutls/x509.h>

typedef struct {
	gnutls_session_t sess;
} SSL;
typedef struct {
	gnutls_certificate_credentials_t cred;
	gnutls_priority_t prio;
} SSL_CTX;


/* public api */
CIVETWEB_API int gtls_sslctx_init(SSL_CTX *ctx, const char *crt);
CIVETWEB_API void gtls_sslctx_uninit(SSL_CTX *ctx);
CIVETWEB_API void gtls_ssl_close(SSL *ssl);
CIVETWEB_API int gtls_ssl_accept(SSL **ssl,
                                 SSL_CTX *ssl_ctx,
                                 int sock,
                                 struct mg_context *phys_ctx);
CIVETWEB_API int gtls_ssl_read(SSL *ssl, unsigned char *buf, size_t len);
CIVETWEB_API int gtls_ssl_write(SSL *ssl, const unsigned char *buf, size_t len);


CIVETWEB_API int
gtls_sslctx_init(SSL_CTX *ctx, const char *crt)
{
	int rc;

	if (ctx == NULL || crt == NULL) {
		return -1;
	}

	DEBUG_TRACE("%s", "Initializing GnuTLS SSL");

	rc = gnutls_certificate_allocate_credentials(&ctx->cred);
	if (rc != GNUTLS_E_SUCCESS) {
		DEBUG_TRACE("Failed to allocate credentials (%d): %s",
		            rc,
		            gnutls_strerror(rc));
		goto failed;
	}

	rc = gnutls_priority_init(&ctx->prio, NULL, NULL);
	if (rc != GNUTLS_E_SUCCESS) {
		DEBUG_TRACE("Failed to allocate priority cache (%d): %s",
		            rc,
		            gnutls_strerror(rc));
		goto failed;
	}

	rc = gnutls_certificate_set_x509_key_file2(ctx->cred,
	                                           crt,
	                                           crt,
	                                           GNUTLS_X509_FMT_PEM,
	                                           NULL,
	                                           GNUTLS_PKCS_PLAIN
	                                               | GNUTLS_PKCS_NULL_PASSWORD);
	if (rc != GNUTLS_E_SUCCESS) {
		DEBUG_TRACE("TLS parse crt/key file failed (%d): %s",
		            rc,
		            gnutls_strerror(rc));
		goto failed;
	}

	return 0;

failed:
	gtls_sslctx_uninit(ctx);

	return -1;
}


CIVETWEB_API void
gtls_sslctx_uninit(SSL_CTX *ctx)
{
	if (ctx != NULL) {
		gnutls_certificate_free_credentials(ctx->cred);
		gnutls_priority_deinit(ctx->prio);
		ctx->cred = NULL;
		ctx->prio = NULL;
	}
}


CIVETWEB_API int
gtls_ssl_accept(SSL **ssl,
                SSL_CTX *ssl_ctx,
                int sock,
                struct mg_context *phys_ctx)
{
	int rc;

	if (ssl == NULL || ssl_ctx == NULL) {
		return -1;
	}

	DEBUG_TRACE("TLS accept processing %p", ssl);

	*ssl = (SSL *)mg_calloc_ctx(1, sizeof(SSL), phys_ctx);
	if (*ssl == NULL) {
		DEBUG_TRACE("Failed to allocate memory for session %zu", sizeof(SSL));
		return -1;
	}

	rc = gnutls_init(&(*ssl)->sess, GNUTLS_SERVER);
	if (rc != GNUTLS_E_SUCCESS) {
		DEBUG_TRACE("Failed to initialize session (%d): %s",
		            rc,
		            gnutls_strerror(rc));
		goto failed;
	}

	rc = gnutls_priority_set((*ssl)->sess, ssl_ctx->prio);
	if (rc != GNUTLS_E_SUCCESS) {
		DEBUG_TRACE("TLS set priortities failed (%d): %s",
		            rc,
		            gnutls_strerror(rc));
		goto failed;
	}

	rc = gnutls_credentials_set((*ssl)->sess,
	                            GNUTLS_CRD_CERTIFICATE,
	                            ssl_ctx->cred);
	if (rc != GNUTLS_E_SUCCESS) {
		DEBUG_TRACE("TLS set credentials failed (%d): %s",
		            rc,
		            gnutls_strerror(rc));
		goto failed;
	}

	gnutls_certificate_send_x509_rdn_sequence((*ssl)->sess, 1);
	gnutls_certificate_server_set_request((*ssl)->sess, GNUTLS_CERT_IGNORE);
	gnutls_handshake_set_timeout((*ssl)->sess,
	                             GNUTLS_DEFAULT_HANDSHAKE_TIMEOUT);
	gnutls_transport_set_int((*ssl)->sess, sock);

	while ((rc = gnutls_handshake((*ssl)->sess)) != GNUTLS_E_SUCCESS) {
		if (gnutls_error_is_fatal(rc)) {
			if (rc == GNUTLS_E_FATAL_ALERT_RECEIVED) {
				DEBUG_TRACE("TLS fatal alert received: %s",
				            gnutls_alert_get_name(
				                gnutls_alert_get((*ssl)->sess)));
			} else {
				DEBUG_TRACE("TLS handshake failed (%d): %s",
				            rc,
				            gnutls_strerror(rc));
			}

			goto failed;
		}
	}

	DEBUG_TRACE("TLS connection %p accepted", *ssl);

	return 0;

failed:
	gnutls_deinit((*ssl)->sess);
	mg_free(*ssl);
	*ssl = NULL;

	return -1;
}


CIVETWEB_API void
gtls_ssl_close(SSL *ssl)
{
	int rc;

	if (ssl == NULL) {
		return;
	}

	while ((rc = gnutls_bye(ssl->sess, GNUTLS_SHUT_RDWR)) != GNUTLS_E_SUCCESS) {
		switch (rc) {
		case GNUTLS_E_AGAIN: /* fall through */
		case GNUTLS_E_INTERRUPTED:
			continue;
		default: /* should actually never happen */
			break;
		}
	}

	DEBUG_TRACE("TLS connection %p closed", ssl);
	gnutls_deinit(ssl->sess);
	mg_free(ssl);
}


CIVETWEB_API int
gtls_ssl_read(SSL *ssl, unsigned char *buf, size_t len)
{
	ssize_t rc;

	if (ssl == NULL) {
		return GNUTLS_E_INVALID_SESSION;
	}

	while ((rc = gnutls_record_recv(ssl->sess, buf, len)) < 0) {
		switch (rc) {
		case GNUTLS_E_AGAIN: /* fall through */
		case GNUTLS_E_INTERRUPTED:
			continue;
		default:
			break;
		}
	}
	/* DEBUG_TRACE("gnutls_record_recv: %d", rc); */
	return (int)rc;
}


CIVETWEB_API int
gtls_ssl_write(SSL *ssl, const unsigned char *buf, size_t len)
{
	ssize_t rc;

	if (ssl == NULL) {
		return GNUTLS_E_INVALID_SESSION;
	}

	while ((rc = gnutls_record_send(ssl->sess, buf, len)) < 0) {
		switch (rc) {
		case GNUTLS_E_AGAIN: /* fall through */
		case GNUTLS_E_INTERRUPTED:
			continue;
		default:
			break;
		}
	}
	/* DEBUG_TRACE("gnutls_record_send: %d", rc); */
	return (int)rc;
}

#endif /* USE_GNUTLS */
