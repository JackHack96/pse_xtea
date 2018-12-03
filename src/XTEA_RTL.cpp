#include "XTEA_RTL.h"
#include <XTEA_RTL.h>

void XTEA_RTL::fsm() {
  NEXT_STATUS = STATUS;
  switch (STATUS) {
  case IDLE:
    if (input_ready.read()) {
      NEXT_STATUS = BUSY;
    } else
      NEXT_STATUS = IDLE;
    break;
  case BUSY:
    if (input_ready.read())
      NEXT_STATUS = BUSY;
    else
      NEXT_STATUS = IDLE;
    break;
  }
}

void XTEA_RTL::datapath() {
  if (rst.read()) {
    STATUS = IDLE;
  } else {
    STATUS = NEXT_STATUS;
    sc_uint<32> tmp[2];

    switch (STATUS) {
    case IDLE:break;
    case BUSY:key[0] = key_input[0].read();
      key[1] = key_input[1].read();
      key[2] = key_input[2].read();
      key[3] = key_input[3].read();
      text[0] = text_input[0].read();
      text[1] = text_input[1].read();
      xtea(text[0], text[1], key[0], key[1], key[2], key[3], mode, &tmp[0],
           &tmp[1]);
      data_output[0].write(tmp[0]);
      data_output[1].write(tmp[1]);
      break;
    }
  }
}

void XTEA_RTL::xtea(sc_uint<32> word0, sc_uint<32> word1, sc_uint<32> key0,
                    sc_uint<32> key1, sc_uint<32> key2, sc_uint<32> key3,
                    bool mode, sc_uint<32> *result0, sc_uint<32> *result1) {
  sc_uint<64> sum;
  sc_uint<32> i, delta, v0, v1, temp;
  v0 = word0;
  v1 = word1;
  sum = 0;
  *result0 = 0;
  *result1 = 0;

  if (mode == 0) {
    // encipher
    delta = 0x9e3779b9;
    for (i = 0; i < 32; i++) {

      if ((sum & 3) == 0)
        temp = key0;
      else if ((sum & 3) == 1)
        temp = key1;
      else if ((sum & 3) == 2)
        temp = key2;
      else
        temp = key3;

      v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);

      sum += delta;

      if (((sum >> 11) & 3) == 0)
        temp = key0;
      else if (((sum >> 11) & 3) == 1)
        temp = key1;
      else if (((sum >> 11) & 3) == 2)
        temp = key2;
      else
        temp = key3;

      v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);
    }
  } else if (mode == 1) {
    // decipher
    delta = 0x9e3779b9;
    sum = delta * 32;
    for (i = 0; i < 32; i++) {

      if (((sum >> 11) & 3) == 0)
        temp = key0;
      else if (((sum >> 11) & 3) == 1)
        temp = key1;
      else if (((sum >> 11) & 3) == 2)
        temp = key2;
      else
        temp = key3;

      v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + temp);

      sum -= delta;

      if ((sum & 3) == 0)
        temp = key0;
      else if ((sum & 3) == 1)
        temp = key1;
      else if ((sum & 3) == 2)
        temp = key2;
      else
        temp = key3;

      v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + temp);
    }
  }
  *result0 = v0;
  *result1 = v1;
}
