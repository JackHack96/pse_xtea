#include "XTEA_RTL.h"
#include "XTEA_RTL_testbench.h"

int sc_main(int argc, char **argv) {
  sc_clock clk;
  sc_signal<bool> rst, input_ready, mode;

  sc_signal<sc_uint<32>> text_input[2];
  sc_signal<sc_uint<32>> key_input[4];
  sc_signal<sc_uint<32>> data_output[2];

  XTEA_RTL xtea_rtl("xtea_rtl");
  XTEA_RTL_testbench xtea_rtl_testbench("xtea_rtl_testbench");

  // Setting up the signal tracing
  sc_trace_file *fp = sc_create_vcd_trace_file("wave");
  sc_trace(fp, clk, "clk");
  sc_trace(fp, rst, "rst");
  sc_trace(fp, input_ready, "input_ready");
  sc_trace(fp, mode, "mode");
  sc_trace(fp, text_input[0], "text_input(0)");
  sc_trace(fp, text_input[1], "text_input(1)");
  sc_trace(fp, key_input[0], "key_input(0)");
  sc_trace(fp, key_input[1], "key_input(1)");
  sc_trace(fp, key_input[2], "key_input(2)");
  sc_trace(fp, key_input[3], "key_input(3)");
  sc_trace(fp, data_output[0], "data_output(0)");
  sc_trace(fp, data_output[1], "data_output(1)");

  xtea_rtl.clk(clk);
  xtea_rtl.rst(rst);
  xtea_rtl.input_ready(input_ready);
  xtea_rtl.mode(mode);
  xtea_rtl.text_input[0](text_input[0]);
  xtea_rtl.text_input[1](text_input[1]);
  xtea_rtl.key_input[0](key_input[0]);
  xtea_rtl.key_input[1](key_input[1]);
  xtea_rtl.key_input[2](key_input[2]);
  xtea_rtl.key_input[3](key_input[3]);
  xtea_rtl.data_output[0](data_output[0]);
  xtea_rtl.data_output[1](data_output[1]);

  xtea_rtl_testbench.clk(clk);
  xtea_rtl_testbench.rst(rst);
  xtea_rtl_testbench.input_ready(input_ready);
  xtea_rtl_testbench.mode(mode);
  xtea_rtl_testbench.text_input[0](text_input[0]);
  xtea_rtl_testbench.text_input[1](text_input[1]);
  xtea_rtl_testbench.key_input[0](key_input[0]);
  xtea_rtl_testbench.key_input[1](key_input[1]);
  xtea_rtl_testbench.key_input[2](key_input[2]);
  xtea_rtl_testbench.key_input[3](key_input[3]);
  xtea_rtl_testbench.data_output[0](data_output[0]);
  xtea_rtl_testbench.data_output[1](data_output[1]);

  sc_start();

  sc_close_vcd_trace_file(fp);

  return 0;
}