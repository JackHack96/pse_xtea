#include "XTEA_RTL.h"

int sc_main(int argc, char *argv[]) {
  // Signals
  sc_signal<bool> clk("clk");
  sc_signal<bool> load_key("load_key"), encrypt("encrypt"), decrypt("decrypt");

  sc_signal<sc_uint<64>> data_input;
  sc_signal<sc_uint<64>> data_output;

  // Model signal bindings
  XTEA_RTL xt("XTEA_RTL");
  xt.clk(clk);
  xt.load_key(load_key);
  xt.encrypt(encrypt);
  xt.decrypt(decrypt);

  xt.data_input(data_input);
  xt.data_output(data_output);

  // Setting up the signal tracing
  sc_trace_file *fp = sc_create_vcd_trace_file("wave");
  fp->set_time_unit(1, SC_NS);
  sc_trace(fp, clk, "clk");
  sc_trace(fp, load_key, "load_key");
  sc_trace(fp, encrypt, "encrypt");
  sc_trace(fp, decrypt, "decrypt");
  sc_trace(fp, data_input, "data_input");
  sc_trace(fp, data_output, "data_output");

  // Temporary variable
  sc_uint<64> text = 0;

  /// SIMULATION START HERE
#ifdef DEBUG
  cout << "#########################################################" << endl;
  cout << "@" << sc_time_stamp() << " Starting simulation\n" << endl;
#endif

  // Starting the circuit without signals
  load_key = false;
  encrypt  = false;
  decrypt  = false;
  for (int i = 0; i < 2; i++) {
    clk = false;
    sc_start(1, SC_NS);
    clk = true;
    sc_start(1, SC_NS);
  }

  // Load the key
#ifdef DEBUG
  cout << "--------------------------------------------------------" << endl;
  cout << "@" << sc_time_stamp() << " Starting key input\n" << endl;
#endif
  clk      = false;
  load_key = true;
  sc_start(1, SC_NS);

  clk = true;
  text.range(31, 0)  = 0x6a1d78c8;
  text.range(63, 31) = 0x8c86d67f;
  data_input.write(text);
  sc_start(1, SC_NS);

  clk = false;
  sc_start(1, SC_NS);

  clk = true;
  text.range(31, 0)  = 0x2a65bfbe;
  text.range(63, 31) = 0xb4bd6e46;
  data_input.write(text);
  sc_start(1, SC_NS);

#ifdef DEBUG
  cout << "--------------------------------------------------------" << endl;
  cout << "@" << sc_time_stamp() << " Stopping key input\n" << endl;
#endif

  clk      = false;
  load_key = false;
  sc_start(1, SC_NS);

  clk     = true;
  encrypt = true;
  sc_start(1, SC_NS);

  clk = false;
  sc_start(1, SC_NS);

  // Load data to be encrypted
#ifdef DEBUG
  cout << "--------------------------------------------------------" << endl;
  cout << "@" << sc_time_stamp() << " Starting encryption\n" << endl;
#endif
  clk = true;
  text.range(31, 0)  = 0x12345678;
  text.range(63, 31) = 0x9abcdeff;
  data_input.write(text);
  cout << "Original text: " << text << endl;
  sc_start(1, SC_NS);

#ifdef DEBUG
  cout << "--------------------------------------------------------" << endl;
  cout << "@" << sc_time_stamp() << " Stopping encryption\n" << endl;
#endif
  clk     = false;
  encrypt = false;
  text    = data_output.read();
  cout << "Encrypted text: " << text << endl;
  sc_start(1, SC_NS);

  clk     = true;
  decrypt = true;
  sc_start(1, SC_NS);

  clk = false;
  sc_start(1, SC_NS);

  // Load and decrypt data
#ifdef DEBUG
  cout << "--------------------------------------------------------" << endl;
  cout << "@" << sc_time_stamp() << " Starting decryption\n" << endl;
#endif
  clk = true;
  text.range(31, 0)  = text.range(31, 0);
  text.range(63, 31) = text.range(63, 31);
  data_input.write(text);
  sc_start(1, SC_NS);

#ifdef DEBUG
  cout << "--------------------------------------------------------" << endl;
  cout << "@" << sc_time_stamp() << " Stopping decryption\n" << endl;
#endif
  clk     = false;
  encrypt = false;
  text    = data_output.read();
  cout << "Decrypted text: " << text << endl;
  sc_start(1, SC_NS);

  clk     = true;
  decrypt = true;
  sc_start(1, SC_NS);

  clk = false;
  sc_start(1, SC_NS);

#ifdef DEBUG
  cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
  cout << "#########################################################" << endl;
#endif
  sc_close_vcd_trace_file(fp);

  return 0;
}