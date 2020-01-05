#include <stdio.h>

#include "vamk_socket.h"

#include "vamk_aes.h"
#include "vamk_random.h"
#include "vamk_rfcomm.h"
#include "vamk_rsa.h"

#include "espp_bt_client.h"

using std::unique_ptr;
using std::vector;
using vamk::Aes;
using vamk::Rsa;
using vamk::RfcommClientSocket;

namespace espp {

BtClient::BtClient(std::shared_ptr<vamk::Rsa> rsa,
                   std::unique_ptr<vamk::RfcommClientSocket> &socket)
    : _rsa(rsa) {
  using std::swap;
  swap(_socket, socket);
}

BtClient::~BtClient() {
  if (_aes)
    _aes.reset();
}

bool BtClient::exchangeKey() {
  static const vector<char> init_message{'E', 'S', 'P', 'P'};
  static const vector<char> ack_message{'O', 'K'};
  static const vector<char> ready_message{'R', 'E', 'A', 'D', 'Y'};

  vector<char> buf;
  int size;

  // receive init message
  size = read(buf);
  if (size < 0)
    return false;
  if (buf != init_message)
    return false;

  // send public key
  auto rsa_key = _rsa->getPublicKey();
  size = write(rsa_key);
  if (size < 0)
    return false;

  // receive rsa key
  size = read(buf);
  if (size < 0)
    return false;

  // setup aes
  _aes = unique_ptr<Aes>(new Aes);
  auto aes_key = _rsa->decrypt(buf);
  _aes->setKey(aes_key);

  // verify aes key
  size = write(ack_message);
  if (size < 0)
    return false;
  size = read(buf);
  if (size < 0)
    return false;
  if (buf != ack_message)
    return false;

  // send ready message
  size = write(ready_message);
  if (size < 0)
    return false;

  return true;
}

int BtClient::read(std::vector<char> &data) {
  if (_aes) {
    // read from socket
    vector<char> buf;
    int size = _socket->read(buf);
    if (size < 16)
      return -1;

    // split iv and cipher text, then decrypt
    vector<char> iv(buf.begin(), buf.begin() + 16);
    vector<char> ctext(buf.begin() + 16, buf.end());
    vector<char> ptext = _aes->decrypt(ctext, iv);

    // return values
    data = ptext;
    return ptext.size();
  } else
    return _socket->read(data);
}

int BtClient::write(const std::vector<char> &data) {
  if (_aes) {
    vector<char> buf = vamk::getRandom(16); // iv
    vector<char> ctext = _aes->encrypt(data, buf); // use buf as iv

    // append cipher text to iv and send
    buf.insert(buf.end(), ctext.begin(), ctext.end());
    return _socket->write(buf);
  } else
    return _socket->write(data);
}
}
