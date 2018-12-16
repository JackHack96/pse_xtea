#ifndef XTEA_XTEA_RTL_TESTBENCH_H
#define XTEA_XTEA_RTL_TESTBENCH_H

#include <systemc.h>

SC_MODULE(XTEA_RTL_testbench) {
private:
  void run();

public:
  sc_in_clk clk;    // The clock signal
  sc_out<bool> rst; // The reset signal

  sc_out<sc_uint<32>> text_input[2]; // 32-bit data input ports
  sc_out<sc_uint<32>> key_input[4];  // 32-bit key input ports

  sc_in<sc_uint<32>> data_output[2]; // 32-bit output ports

  sc_out<bool> input_ready; // Flag for telling system is ready
  sc_out<bool> mode; // Flag for setting encryption/decryption mode (0=encrypt,
                     // 1=decrypt)

  SC_HAS_PROCESS(XTEA_RTL_testbench);

  XTEA_RTL_testbench(sc_module_name name);
};

#endif // XTEA_XTEA_RTL_TESTBENCH_H
