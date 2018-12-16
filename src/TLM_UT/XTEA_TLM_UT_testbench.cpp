#include <TLM_UT/XTEA_TLM_UT.h>
#include "TLM_UT/XTEA_TLM_UT_testbench.h"

void XTEA_TLM_UT_testbench::invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range) {

}

tlm::tlm_sync_enum XTEA_TLM_UT_testbench::nb_transport_bw(tlm::tlm_generic_payload &trans,
                                                          tlm::tlm_phase &phase,
                                                          sc_time &t) {
  return tlm::TLM_COMPLETED;
}

void XTEA_TLM_UT_testbench::run() {
  sc_time local_time;
  // First transaction (initialization)
  iostruct xtea_packet;
  tlm::tlm_generic_payload payload;
  sc_uint<32> text[2] = {0x12345678, 0x9abcdeff};
  sc_uint<32> key[4] = {0x6a1d78c8, 0x8c86d67f, 0x2a65bfbe, 0xb4bd6e46};

  cout << "Encrypting text..." << endl;

  xtea_packet.mode = false; // Set to encrypt mode
  xtea_packet.key[0] = key[0];
  xtea_packet.key[1] = key[1];
  xtea_packet.key[2] = key[2];
  xtea_packet.key[3] = key[3];
  xtea_packet.text[0] = text[0];
  xtea_packet.text[1] = text[1];

  payload.set_data_ptr((unsigned char *) &xtea_packet);
  payload.set_address(0);
  payload.set_write();

  // start write transaction
  initiator_socket->b_transport(payload, local_time);

  // start read transaction
  payload.set_read();

  initiator_socket->b_transport(payload, local_time);
  if (payload.get_response_status() == tlm::TLM_OK_RESPONSE) {
    cout << "\tresult:\t" << hex <<  xtea_packet.result[0] << "," << hex <<  xtea_packet.result[1] << endl;
    cout << endl;
  }

  sc_stop();
}

XTEA_TLM_UT_testbench::XTEA_TLM_UT_testbench(sc_module_name name) : sc_module(name) {
  initiator_socket(*this);

  SC_THREAD(run);
}
