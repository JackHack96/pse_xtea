#ifndef XTEA_XTEA_RTL_H
#define XTEA_XTEA_RTL_H

//#define DEBUG

#include <systemc.h>

SC_MODULE(XTEA_RTL) {
  sc_in_clk clk; // The clock signal

  sc_in<sc_uint<64>> data_input;   // 64-bit input port
  sc_out<sc_uint<64>> data_output; // 64-bit output port

  sc_in<bool> load_key; // Flag for key input
  sc_in<bool> encrypt;  // Flag for starting encryption
  sc_in<bool> decrypt;  // Flag for starting decryption

  typedef enum {
    IDLE,
    BUSY_KEY,
    BUSY_ENC,
    BUSY_DEC
  } STATES; // Possible states of the FSM

  STATES STATUS, NEXT_STATUS;
  sc_uint<32> key[4];
  sc_uint<32> text[2];

  int key_index = 0;

  void fsm();

  void datapath();

  void xtea(sc_uint<32> word0, sc_uint<32> word1, sc_uint<32> key0,
            sc_uint<32> key1, sc_uint<32> key2, sc_uint<32> key3, bool mode,
            sc_uint<32> *result0, sc_uint<32> *result1);

  SC_CTOR(XTEA_RTL) {
    SC_METHOD(datapath);
    sensitive << clk.pos();

    SC_METHOD(fsm);
    sensitive << load_key << encrypt << decrypt;
  }
};

#endif // XTEA_XTEA_RTL_H
