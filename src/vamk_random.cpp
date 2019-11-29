#include <fcntl.h>
#include <openssl/rand.h>
#include <unistd.h>

#include "vamk_random.h"

namespace vamk {
void seedRandom() {
  char buf[10];
  int fd = open("/dev/random", O_RDONLY);
  int n = read(fd, buf, sizeof buf);
  close(fd);
  RAND_add(buf, sizeof buf, n);
}

std::vector<char> getRandom(int size) {
  std::vector<char> result(size);
  RAND_bytes((unsigned char *)&result[0], size);
  return result;
}
}
