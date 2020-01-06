#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>
#include <openssl/ssl.h>

//! This class provides interfaces to communicate with an HTTPS server.
class HttpsClient
{
public:
	HttpsClient() = delete;
	HttpsClient(const HttpsClient &) = delete;
	HttpsClient(const std::string &ip, uint16_t port);
	~HttpsClient();

	//! Start the connection.
	/*!
	  Call serverConnect() and sslConnect().
	  \return True if there is no error, false otherwise.
	*/
	bool connect();
	
	// TODO: why the send request only works the first time
	// Temporary workaround: new object each request
	
	//! Send \p request_str to the server.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool sendRequest(const std::string &request_str);
	//! Receive all from the server (blocking).
	/*!
	  \return The server's response. An empty string on error.
	*/
	std::string receiveResponse(); // blocking
	//! Print out the SSL certificate of the server.
	void printCerts();

private:
	//! Print out the SSL connection error.
	void printConnectionError();
	//! Print out the error when trying to send or receive.
	void printSendReceiveError(const int err) const;
	//! Initiate a connection on a socket.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool serverConnect();
	//! Establish a TLS connection.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool sslConnect();

	SSL *_ssl{nullptr};
	SSL_CTX *_ctx{nullptr};
	int _sockfd;

	std::string _ip;
	uint16_t _port;
};

#endif // HTTPSCLIENT_H
