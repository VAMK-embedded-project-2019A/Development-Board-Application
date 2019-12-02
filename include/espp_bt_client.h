#ifndef ESPP_BT_SOCKET
#define ESPP_BT_SOCKET

#include <memory>
#include <vector>

namespace vamk {
class Rsa;
class Aes;
class RfcommClientSocket;
}

namespace espp {

class BtServer;

class BtClient {
public:
  BtClient() = delete;
  BtClient(const BtClient &) = delete;
  BtClient operator=(const BtClient &) = delete;
  ~BtClient();

  bool exchangeKey();
  int read(std::vector<char> &);
  int write(const std::vector<char> &);

private:
  BtClient(std::shared_ptr<vamk::Rsa> , std::unique_ptr<vamk::RfcommClientSocket> &);
  std::shared_ptr<vamk::Rsa> _rsa;
  std::unique_ptr<vamk::RfcommClientSocket> _socket;
  std::unique_ptr<vamk::Aes> _aes;

  friend BtServer;
};
}

#endif