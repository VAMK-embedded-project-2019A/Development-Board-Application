#ifndef VAMK_RFCOMM
#define VAMK_RFCOMM

#include <memory>
#include <vector>

namespace vamk {
class Socket;
class RfcommClientSocket;

class RfcommServerSocket : public Socket {
public:
  // allocate the socket
  RfcommServerSocket();

  // delete implicit constructor
  RfcommServerSocket(const RfcommServerSocket &) = delete;
  RfcommServerSocket operator=(const RfcommServerSocket &) = delete;

  // bind, start listening
  void listen(int channel);

  // accept a client
  std::unique_ptr<RfcommClientSocket> accept();
};

class RfcommClientSocket : public Socket {
public:
  // delete implicit constructor
  RfcommClientSocket() = delete;
  RfcommClientSocket(const RfcommClientSocket &) = delete;
  RfcommServerSocket operator=(const RfcommClientSocket &) = delete;

  // read from stream
  int read(std::vector<char> &);

  // write to stream
  int write(const std::vector<char> &);

private:
  // allocate the socket
  RfcommClientSocket(int socket);

  // only server can construct client
  friend RfcommServerSocket;
};
}

#endif
