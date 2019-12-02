#include <mutex>

#include <openssl/bn.h>
#include <openssl/rsa.h>

#include "vamk_random.h"
#include "vamk_rsa.h"

#define RSA_KEY_SIZE 2048

using std::unique_ptr;
using std::vector;
using std::mutex;
using std::lock_guard;

using BN_ptr = unique_ptr<BIGNUM, decltype(&::BN_free)>;
using RSA_ptr = unique_ptr<RSA, decltype(&::RSA_free)>;

namespace vamk {
struct Rsa::RsaImpl {
  RsaImpl();

  void generateKeyPair();
  vector<char> getPublicKey();
  vector<char> decrypt(vector<char>);

  mutex rsa_mutex;
  RSA_ptr rsa;
  BN_ptr bn;
};

// Rsa implementation
Rsa::RsaImpl::RsaImpl() : rsa(RSA_new(), ::RSA_free), bn(BN_new(), ::BN_free) {}

void Rsa::RsaImpl::generateKeyPair() {
  // seed the random number generator
  vamk::seedRandom();

  // set modulus
  BN_set_word(bn.get(), RSA_F4);

  // generate key
  RSA_generate_key_ex(rsa.get(), RSA_KEY_SIZE, bn.get(), NULL);
}

vector<char> Rsa::RsaImpl::getPublicKey() {
  const lock_guard<mutex> lock(rsa_mutex);

  // get public exponent
#if (OPENSSL_VERSION_NUMBER < 0x010100000)
  BIGNUM *n = rsa->n;
#else
  BIGNUM *n;
  RSA_get0_key(rsa.get(), (const BIGNUM **)&n, NULL, NULL);
#endif

  // copy public exponent into vector<byte>
  vector<char> key(BN_num_bytes(n));
  BN_bn2bin(n, (unsigned char *)&key[0]);
  return key;
}

vector<char> Rsa::RsaImpl::decrypt(vector<char> data) {
  const lock_guard<mutex> lock(rsa_mutex);

  // allocate buffer
  vector<char> text(RSA_size(rsa.get()));

  // decrypt
  ssize_t size = RSA_private_decrypt(
      data.size(), (const unsigned char *)&data[0], (unsigned char *)&text[0],
      rsa.get(), RSA_PKCS1_OAEP_PADDING);

  // resize buffer to correct size
  text.resize(size);

  return text;
}

// Rsa forward
Rsa::Rsa() : _pimpl(new RsaImpl) {}
Rsa::~Rsa() {}
void Rsa::generateKeyPair() { _pimpl->generateKeyPair(); }
vector<char> Rsa::getPublicKey() { return _pimpl->getPublicKey(); }
vector<char> Rsa::decrypt(vector<char> data) { return _pimpl->decrypt(data); }
}
