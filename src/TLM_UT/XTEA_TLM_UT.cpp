#include "TLM_UT/XTEA_TLM_UT.h"

XTEA_TLM_UT::XTEA_TLM_UT(sc_module_name name_)
    : sc_module(name_), target_socket("target_socket"), pending_transaction(NULL) {
  target_socket(*this);
}

void XTEA_TLM_UT::b_transport(tlm::tlm_generic_payload &trans, sc_time &t) {
  ioDataStruct = *((iostruct *) trans.get_data_ptr());

  if (trans.is_write()) {
    xtea_function();
    trans.set_response_status(tlm::TLM_OK_RESPONSE);
    *((iostruct *) trans.get_data_ptr()) = ioDataStruct;
  } else if (trans.is_read()) {
    ioDataStruct.result[0] = tmp_result[0];
    ioDataStruct.result[1] = tmp_result[1];
    *((iostruct *) trans.get_data_ptr()) = ioDataStruct;
  }
}

bool XTEA_TLM_UT::get_direct_mem_ptr(tlm::tlm_generic_payload &trans, tlm::tlm_dmi &dmi_data) {
  return false;
}

tlm::tlm_sync_enum XTEA_TLM_UT::nb_transport_fw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t) {
  return tlm::TLM_COMPLETED;
}

unsigned int XTEA_TLM_UT::transport_dbg(tlm::tlm_generic_payload &trans) {
  return 0;
}

void XTEA_TLM_UT::xtea_function() {
  sc_uint<64> sum;
  sc_uint<32> i, delta, v0, v1, temp;
  v0 = ioDataStruct.text[0];
  v1 = ioDataStruct.text[1];
  sum = 0;
  tmp_result[0] = 0;
  tmp_result[1] = 0;

  if (!ioDataStruct.mode) {
    // encipher
    delta = 0x9e3779b9;
    for (i = 0; i < 32; i++) {

      if ((sum & 3) == 0)
        temp = ioDataStruct.key[0];
      else if ((sum & 3) == 1)
        temp = ioDataStruct.key[1];
      else if ((sum & 3) == 2)
        temp = ioDataStruct.key[2];
      else
        temp = ioDataStruct.key[3];

      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

      sum += delta;

      if (((sum >> 11) & 3) == 0)
        temp = ioDataStruct.key[0];
      else if (((sum >> 11) & 3) == 1)
        temp = ioDataStruct.key[1];
      else if (((sum >> 11) & 3) == 2)
        temp = ioDataStruct.key[2];
      else
        temp = ioDataStruct.key[3];

      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
    }
  } else if (ioDataStruct.mode) {
    // decipher
    delta = 0x9e3779b9;
    sum = delta * 32;
    for (i = 0; i < 32; i++) {

      if (((sum >> 11) & 3) == 0)
        temp = ioDataStruct.key[0];
      else if (((sum >> 11) & 3) == 1)
        temp = ioDataStruct.key[1];
      else if (((sum >> 11) & 3) == 2)
        temp = ioDataStruct.key[2];
      else
        temp = ioDataStruct.key[3];

      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);

      sum -= delta;

      if ((sum & 3) == 0)
        temp = ioDataStruct.key[0];
      else if ((sum & 3) == 1)
        temp = ioDataStruct.key[1];
      else if ((sum & 3) == 2)
        temp = ioDataStruct.key[2];
      else
        temp = ioDataStruct.key[3];

      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);
    }
  }
  tmp_result[0] = v0;
  tmp_result[1] = v1;
}