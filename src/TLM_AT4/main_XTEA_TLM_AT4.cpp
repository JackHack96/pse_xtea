#include "TLM_AT4/XTEA_TLM_AT4.h"
#include "TLM_AT4/XTEA_TLM_AT4_testbench.h"


class XTEA_TLM_AT4_TOP : public sc_module {

private:
  XTEA_TLM_AT4           m_target;
  XTEA_TLM_AT4_testbench m_initiator;

public:

  XTEA_TLM_AT4_TOP(sc_module_name name_)
      : sc_module(name_),
        m_target("target"),
        m_initiator("initiator") {
    m_initiator.initiator_socket(m_target.target_socket);
  }
};

int sc_main(int argc, char *argv[]) {
  XTEA_TLM_AT4_TOP top("top");

  sc_start();

  return 0;
}
