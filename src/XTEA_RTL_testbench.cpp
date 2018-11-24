
#include <XTEA_RTL_testbench.h>

#include "XTEA_RTL_testbench.h"

XTEA_RTL_testbench::XTEA_RTL_testbench(sc_module_name name) : sc_module(name) {
  SC_THREAD(run);
  sensitive << clk.pos() << rst.pos();
}

void XTEA_RTL_testbench::run() {
  sc_uint<32> text[2] = {0x12345678, 0x9abcdeff};
  sc_uint<32> key[4]={0x6a1d78c8,0x8c86d67f,0x2a65bfbe,0xb4bd6e46};

  rst.write(false);
  input_ready.write(false);

  wait();

  /// Encrypt mode
  mode.write(false);

  cout << "Original text: " << hex << text[0] << "," << hex << text[1] << "\n";

  key_input[0].write(key[0]);
  key_input[1].write(key[1]);
  key_input[2].write(key[2]);
  key_input[3].write(key[3]);
  text_input[0].write(text[0]);
  text_input[1].write(text[1]);
  input_ready.write(true);
  wait(5);

  text[0] = data_output[0].read();
  text[1] = data_output[1].read();
  input_ready.write(false);
  wait();

  cout << "Encrypted text: " << hex << text[0] << "," << hex << text[1] << "\n";

  /// Decrypt mode
  mode.write(true);
  wait();

  key_input[0].write(key[0]);
  key_input[1].write(key[1]);
  key_input[2].write(key[2]);
  key_input[3].write(key[3]);
  text_input[0].write(text[0]);
  text_input[1].write(text[1]);
  input_ready.write(true);
  wait(5);

  text[0] = data_output[0].read();
  text[1] = data_output[1].read();
  input_ready.write(false);

  cout << "Decrypted text: " << hex << text[0] << "," << hex << text[1] << "\n";

  /// End simulation
  sc_stop();
}