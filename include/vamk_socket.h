#ifndef VAMK_SOCKET
#define VAMK_SOCKET

namespace vamk {

class Socket {
public:
    void close();
protected:
    Socket(int socket);
    int _socket;
};
}

#endif