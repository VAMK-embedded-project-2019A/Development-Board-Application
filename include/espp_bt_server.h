#ifndef ESPP_BT_SERVER
#define ESPP_BT_SERVER

#include <memory>

namespace vamk {
class RfcommServerSocket;
class Sdp;
}

namespace espp {

class BtClient;

class BtServer {
public:
  BtServer();
  BtServer(const BtServer &) = delete;
  BtServer operator=(const BtServer &) = delete;
  ~BtServer();

  void start();
  void stop();
  std::unique_ptr<BtClient> accept();

private:
  void _init();
  void _deinit();

  std::unique_ptr<vamk::Sdp> _sdp;
  std::shared_ptr<vamk::Rsa> _rsa;
  std::unique_ptr<vamk::RfcommServerSocket> _server_socket;
};
}

#endif