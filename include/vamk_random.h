#ifndef VAMK_RANDOM
#define VAMK_RANDOM

#include <vector>

namespace vamk {
void seedRandom();
std::vector<char> getRandom(int);
}

#endif