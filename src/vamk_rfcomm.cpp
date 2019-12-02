extern "C" {
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
}

#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "vamk_socket.h"

#include "vamk_rfcomm.h"

#define SZ_SIZE 2

using std::cout;
using std::endl;

namespace vamk {
// server socket
RfcommServerSocket::RfcommServerSocket()
    : Socket(socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM)) {}

void RfcommServerSocket::listen(int channel) {
  // server address struct
  struct sockaddr_rc addr;

  // bind socket to local bluetooth adapter
  bdaddr_t any = {0, 0, 0, 0, 0, 0};
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_bdaddr = any;
  addr.rc_channel = (uint8_t)channel;
  bind(_socket, (struct sockaddr *)&addr, sizeof(addr));

  // put socket into listening mode
  using ::listen;
  listen(_socket, 1);
}

// accept one connection
std::unique_ptr<RfcommClientSocket> RfcommServerSocket::accept() {
  // server address struct
  struct sockaddr_rc addr;
  socklen_t opt = sizeof(addr);

  // accept one client
  using ::accept;
  int client = accept(_socket, (struct sockaddr *)&addr, &opt);
  return std::unique_ptr<RfcommClientSocket>(new RfcommClientSocket(client));
}

// client socket
RfcommClientSocket::RfcommClientSocket(int socket) : Socket(socket) {}

int RfcommClientSocket::read(std::vector<char> &buffer) {
  using ::read;

  unsigned char sz_buf[SZ_SIZE];
  ssize_t expt_sz; // expected size
  ssize_t recv_sz; // received size

  // read package size
  recv_sz = read(_socket, sz_buf, SZ_SIZE);
  if (recv_sz != SZ_SIZE)
    return -1;
  expt_sz = sz_buf[0] * 256 + sz_buf[1];

  // read data into buffer
  buffer.resize(expt_sz);
  recv_sz = read(_socket, &buffer[0], expt_sz);
  if (recv_sz != expt_sz)
    return -1;

  return expt_sz;
}

int RfcommClientSocket::write(const std::vector<char> &buffer) {
  using ::write;

  // write package size
  size_t size = buffer.size();
  unsigned char sz_buf[SZ_SIZE];
  sz_buf[0] = (size / 256) % 256;
  sz_buf[1] = size % 256;

  // write to socket
  write(this->_socket, sz_buf, 2);
  return write(this->_socket, &buffer[0], size);
}
}
