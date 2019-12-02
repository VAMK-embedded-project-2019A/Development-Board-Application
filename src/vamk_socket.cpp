#include <unistd.h>

#include "vamk_socket.h"

namespace vamk {
Socket::Socket(int socket) : _socket(socket) {}

void Socket::close() {
  using ::close;
  close(_socket);
}
}