#ifndef XTEA_XTEA_TLM_LT_TESTBENCH_H
#define XTEA_XTEA_TLM_LT_TESTBENCH_H

#include <systemc.h>
#include <tlm.h>
#include <tlm_utils/tlm_quantumkeeper.h>

class XTEA_TLM_LT_testbench : public sc_module, public tlm::tlm_bw_transport_if<> {
private:
  SC_HAS_PROCESS(XTEA_TLM_LT_testbench);

  virtual void invalidate_direct_mem_ptr(uint64 start_range, uint64 end_range);

  virtual tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload &trans, tlm::tlm_phase &phase, sc_time &t);

  tlm_utils::tlm_quantumkeeper m_qk;

  void run();

public:

  tlm::tlm_initiator_socket<> initiator_socket;

  explicit XTEA_TLM_LT_testbench(sc_module_name name);
};

#endif //XTEA_XTEA_TLM_LT_TESTBENCH_H
