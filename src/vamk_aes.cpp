#include <openssl/evp.h>
#include <stdio.h>

#include "vamk_aes.h"

using std::unique_ptr;
using std::vector;

using EVP_ptr = unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;

namespace vamk
{
struct Aes::AesImpl
{
	AesImpl();
	vector<char> decrypt(vector<char> data, vector<char> iv);
	vector<char> encrypt(vector<char> data, vector<char> iv);

	vector<char> key;
};

Aes::AesImpl::AesImpl() {}

vector<char> Aes::AesImpl::encrypt(vector<char> data, vector<char> iv) {
	auto evp = EVP_ptr(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

	int len, success;
	int ciphertext_len;
	auto result = vector<char>(data.size() + 16);

	success = EVP_EncryptInit_ex(evp.get(), EVP_aes_128_cbc(), NULL, (const unsigned char *)&key[0], (const unsigned char *)&iv[0]);
	if(!static_cast<bool>(success))
		return vector<char>{};

	success = EVP_EncryptUpdate(evp.get(), (unsigned char *)&result[0], &len, (const unsigned char *)&data[0], data.size());
	if(!static_cast<bool>(success))
		return vector<char>{};
	ciphertext_len = len;

	success = EVP_EncryptFinal_ex(evp.get(), (unsigned char *)&result[0] + len, &len);
	if(!static_cast<bool>(success))
		return vector<char>{};
	ciphertext_len += len;

	result.resize(ciphertext_len);
	return result;
}

vector<char> Aes::AesImpl::decrypt(vector<char> data, vector<char> iv) {
	auto evp = EVP_ptr(EVP_CIPHER_CTX_new(), EVP_CIPHER_CTX_free);

	int len, success;
	int ciphertext_len;
	auto result = vector<char>(data.size());

	success = EVP_DecryptInit_ex(evp.get(), EVP_aes_128_cbc(), NULL, (const unsigned char *)&key[0], (const unsigned char *)&iv[0]);
	if(!static_cast<bool>(success))
		return vector<char>{};

	success = EVP_DecryptUpdate(evp.get(), (unsigned char *)&result[0], &len, (const unsigned char *)&data[0], data.size());
	if(!static_cast<bool>(success))
		return vector<char>{};
	ciphertext_len = len;

	success = EVP_DecryptFinal_ex(evp.get(), (unsigned char *)&result[0] + len, &len);
	if(!static_cast<bool>(success))
		return vector<char>{};
	ciphertext_len += len;

	result.resize(ciphertext_len);
	return result;
}

// Aes forward
Aes::Aes() : _pimpl(new AesImpl) {}
Aes::~Aes() {}
vector<char> Aes::decrypt(vector<char> data, vector<char> iv)
{
	return _pimpl->decrypt(data, iv);
}
vector<char> Aes::encrypt(vector<char> data, vector<char> iv)
{
	return _pimpl->encrypt(data, iv);
}
void Aes::setKey(vector<char> key)
{
	_pimpl->key = key;
}
}