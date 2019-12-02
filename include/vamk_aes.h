#ifndef VAMK_AES
#define VAMK_AES

#include <memory>
#include <vector>

namespace vamk {

class Aes {
public:
  Aes();
  Aes(const Aes &) = delete;
  Aes operator=(const Aes &) = delete;
  ~Aes();

  std::vector<char> decrypt(std::vector<char> data, std::vector<char> iv);
  std::vector<char> encrypt(std::vector<char> data, std::vector<char> iv);
  void setKey(std::vector<char> key);

private:
  // pImpl technique
  struct AesImpl;
  std::unique_ptr<AesImpl> _pimpl;
};
}

#endif