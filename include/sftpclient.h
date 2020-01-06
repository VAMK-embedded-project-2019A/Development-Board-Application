#ifndef SFTPCLIENT_H
#define SFTPCLIENT_H

#include <string>
#include <curl/curl.h>
#undef DISABLE_SSH_AGENT

size_t writeCallback(void *buffer, size_t size, size_t nmemb, void *stream);

//! This class provides interfaces to communicate with an SFTP server.
class SftpClient
{
public:
	SftpClient() = delete;
	SftpClient(const SftpClient &) = delete;
	SftpClient(const std::string &ip, const std::string &username);
	~SftpClient();

	//! Set the file path containing the passphrase for the private key file.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool setPassphraseFilePath(const std::string &passphrase_file_path);
	//! Set the file path for the known hosts file.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool setKnownHostsFilePath(const std::string &known_hosts_file_path);
	//! Set the file path for the public key file.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool setPublicKeyFilePath(const std::string &public_key_file_path);
	//! Set the file path for the private key file.
	/*!
	  \return True if there is no error, false otherwise.
	*/
	bool setPrivateKeyFilePath(const std::string &private_key_file_path);

	//! Get a file from the server.
	/*!
	  \param server_file_path path of the file on the server, should be in format "/directory/../file_name"
	  \param save_file_path_const path to save the file on the system. If empty, "./file_name" will be used
	  \return True if there is no error, false otherwise.
	*/
	bool getFile(const std::string &server_file_path, const std::string &save_file_path_const = std::string{});

private:
	CURL *_curl{nullptr};

	std::string _ip;
	std::string _username;
	std::string _passphrase;
	std::string _known_hosts_path;
	std::string _public_key_path;
	std::string _private_key_path;
};

#endif // SFTPCLIENT_H
