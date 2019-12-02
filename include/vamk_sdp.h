#ifndef VAMK_SDP
#define VAMK_SDP

#include <memory>

namespace vamk {

struct SdpServiceInfo {
  const char *name;
  const char *desc;
  const char *prov;
};

class Sdp {
public:
  Sdp();
  Sdp(const Sdp &);
  Sdp operator=(const Sdp &) = delete;
  ~Sdp();

  void startAdvertise(void *uuid, char channel, SdpServiceInfo *info);
  void endAdvertise();

private:
  // pImpl technique
  struct SdpImpl;
  std::unique_ptr<SdpImpl> _pimpl;
};
}

#endif