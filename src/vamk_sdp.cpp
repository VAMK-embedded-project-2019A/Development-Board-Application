#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#include "vamk_sdp.h"

namespace vamk {
struct Sdp::SdpImpl {
  sdp_session_t *session = nullptr;
};

Sdp::Sdp() : _pimpl(new SdpImpl) {}
Sdp::Sdp(const Sdp &) {}
Sdp::~Sdp() {}

void Sdp::startAdvertise(void *uuid, char channel_number,
                         SdpServiceInfo *info) {

  uint8_t rfcomm_channel = channel_number;

  uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid;
  sdp_list_t *l2cap_list = 0, *rfcomm_list = 0, *root_list = 0, *proto_list = 0,
             *access_proto_list = 0;
  sdp_data_t *channel = 0, *psm = 0;

  sdp_record_t *record = sdp_record_alloc();

  // set the general service ID
  sdp_uuid128_create(&svc_uuid, uuid);
  sdp_set_service_id(record, svc_uuid);

  // make the service record publicly browsable
  sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
  root_list = sdp_list_append(0, &root_uuid);
  sdp_set_browse_groups(record, root_list);

  // set l2cap information
  sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
  l2cap_list = sdp_list_append(0, &l2cap_uuid);
  proto_list = sdp_list_append(0, l2cap_list);

  // set rfcomm information
  sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
  channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
  rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
  sdp_list_append(rfcomm_list, channel);
  sdp_list_append(proto_list, rfcomm_list);

  // attach protocol information to service record
  access_proto_list = sdp_list_append(0, proto_list);
  sdp_set_access_protos(record, access_proto_list);

  // set the name, provider, and description
  sdp_set_info_attr(record, info->name, info->prov, info->desc);
  int err = 0;

  // connect to the local Sdp server, register the service record, and
  // disconnect
  bdaddr_t any = {0, 0, 0, 0, 0, 0};
  bdaddr_t local = {0, 0, 0, 0xff, 0xff, 0xff};
  _pimpl->session = sdp_connect(&any, &local, SDP_RETRY_IF_BUSY);
  err = sdp_record_register(_pimpl->session, record, 0);

  // cleanup
  sdp_data_free(channel);
  sdp_list_free(l2cap_list, 0);
  sdp_list_free(rfcomm_list, 0);
  sdp_list_free(root_list, 0);
  sdp_list_free(access_proto_list, 0);
}

void Sdp::endAdvertise() {
  if (_pimpl->session)
    sdp_close(_pimpl->session);
  _pimpl->session = nullptr;
}
}
