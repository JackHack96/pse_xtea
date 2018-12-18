#ifndef XTEA_XTEA_TLM_LT_H
#define XTEA_XTEA_TLM_LT_H

#include <systemc.h>
#include <tlm.h>

struct iostruct {
  bool mode;
  sc_uint<32> key[4];
  sc_uint<32> text[2];
  sc_uint<32> result[2];
};

class XTEA_TLM_LT : sc_module, tlm::tlm_fw_transport_if<> {
public:
  // TLM port - target interface
  tlm::tlm_target_socket<> target_socket;

  // function invoked by the initiator
  void b_transport(tlm::tlm_generic_payload &trans, sc_time &t) override;

  // functions that must be implemented to be compliant with the standard
  // not used here
  bool get_direct_mem_ptr(tlm::tlm_generic_payload &trans,
                          tlm::tlm_dmi &dmi_data) override;
  tlm::tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload &trans,
                                     tlm::tlm_phase &phase,
                                     sc_time &t) override;
  unsigned int transport_dbg(tlm::tlm_generic_payload &trans) override;

  // internal data
  iostruct ioDataStruct;
  tlm::tlm_generic_payload *pending_transaction;
  sc_uint<32> tmp_result[2];
  sc_time timing_annotation;

  // functionality
  void xtea_function();

  // constructors
  SC_HAS_PROCESS(XTEA_TLM_LT);
  explicit XTEA_TLM_LT(sc_module_name name_);
};

#endif
