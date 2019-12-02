#ifndef VAMK_RSA
#define VAMK_RSA

#include <memory>
#include <vector>

namespace vamk {

class Rsa {
public:
  Rsa();
  Rsa(const Rsa &) = delete;
  Rsa operator=(const Rsa &) = delete;
  ~Rsa();

  void generateKeyPair();
  std::vector<char> getPublicKey();
  std::vector<char> decrypt(std::vector<char>);

private:
  // pImpl technique
  struct RsaImpl;
  std::unique_ptr<RsaImpl> _pimpl;
};
}

#endif