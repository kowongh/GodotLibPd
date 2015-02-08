#ifndef STREAM_PEER_OPEN_SSL_H
#define STREAM_PEER_OPEN_SSL_H

#ifdef OPENSSL_ENABLED


#include <stdio.h> // If you don't know what this is for stop reading now.
#include "io/stream_peer_ssl.h"
#include "globals.h"
#include "os/file_access.h"
#include "curl_hostcheck.h"

#include <openssl/bio.h> // BIO objects for I/O
#include <openssl/ssl.h> // SSL and SSL_CTX for SSL connections
#include <openssl/err.h> // Error reporting
#include <openssl/x509v3.h>

class StreamPeerOpenSSL : public StreamPeerSSL {
private:
	static int _bio_create( BIO *b );
	static int _bio_destroy( BIO *b );
	static int _bio_read( BIO *b, char *buf, int len );
	static int _bio_write( BIO *b, const char *buf, int len );
	static long _bio_ctrl( BIO *b, int cmd, long num, void *ptr );
	static int _bio_gets( BIO *b, char *buf, int len );
	static int _bio_puts( BIO *b, const char *str );

	static BIO_METHOD _bio_method;

	static bool _match_host_name(const char *name, const char *hostname);
	static Error _match_common_name(const char *hostname, const X509 *server_cert);
	static Error _match_subject_alternative_name(const char *hostname, const X509 *server_cert);


	static int _cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg);


	Status status;
	String hostname;
	int max_cert_chain_depth;
	SSL_CTX* ctx;
	SSL* ssl;
	BIO* bio;
	bool connected;
	int flags;
	bool use_blocking;
	bool validate_certs;
	bool validate_hostname;

	Ref<StreamPeer> base;

	static StreamPeerSSL* _create_func();
	void _print_error(int err);

	static Vector<X509*> certs;

protected:
	static void _bind_methods();
public:


	virtual Error accept(Ref<StreamPeer> p_base);
	virtual Error connect(Ref<StreamPeer> p_base,bool p_validate_certs=false,const String& p_for_hostname=String());
	virtual Status get_status() const;

	virtual void disconnect();

	virtual Error put_data(const uint8_t* p_data,int p_bytes);
	virtual Error put_partial_data(const uint8_t* p_data,int p_bytes, int &r_sent);

	virtual Error get_data(uint8_t* p_buffer, int p_bytes);
	virtual Error get_partial_data(uint8_t* p_buffer, int p_bytes,int &r_received);

	static void initialize_ssl();
	static void finalize_ssl();

	StreamPeerOpenSSL();
	~StreamPeerOpenSSL();
};

#endif
#endif // STREAM_PEER_SSL_H
