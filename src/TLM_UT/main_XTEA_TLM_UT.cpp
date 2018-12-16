#include "TLM_UT/XTEA_TLM_UT.h"
#include "TLM_UT/XTEA_TLM_UT_testbench.h"

class XTEA_TLM_UT_TOP : public sc_module {

private:
  XTEA_TLM_UT m_target;
  XTEA_TLM_UT_testbench m_initiator;

public:

  XTEA_TLM_UT_TOP(sc_module_name name) : sc_module(name), m_target("target"), m_initiator("initiator") {
    m_initiator.initiator_socket(m_target.target_socket);
  }

};

int sc_main(int argc, char **argv){
  XTEA_TLM_UT_TOP top("top");
  sc_start();
  return 0;
}