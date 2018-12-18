#include "TLM_LT/XTEA_TLM_LT.h"
#include "TLM_LT/XTEA_TLM_LT_testbench.h"

class XTEA_TLM_LT_TOP : public sc_module {

private:
  XTEA_TLM_LT m_target;
  XTEA_TLM_LT_testbench m_initiator;

public:

  XTEA_TLM_LT_TOP(sc_module_name name) : sc_module(name), m_target("target"), m_initiator("initiator") {
    m_initiator.initiator_socket(m_target.target_socket);
  }

};

int sc_main(int argc, char **argv){
  XTEA_TLM_LT_TOP top("top");
  sc_start();
  return 0;
}