#include "XTEA_RTL.h"
#include <stdint.h>

void XTEA_RTL::fsm() {
  NEXT_STATUS = STATUS;

  switch (STATUS) {
  case IDLE:
    if (load_key.read())
      NEXT_STATUS = BUSY_KEY;
    else if (encrypt.read())
      NEXT_STATUS = BUSY_ENC;
    else if (decrypt.read())
      NEXT_STATUS = BUSY_DEC;
    else
      NEXT_STATUS = IDLE;
    break;
  case BUSY_KEY:
    NEXT_STATUS = IDLE;
    break;
  case BUSY_ENC:
    NEXT_STATUS = IDLE;
    break;
  case BUSY_DEC:
    NEXT_STATUS = IDLE;
    break;
  }
}

void XTEA_RTL::datapath() {
  if (rst.read()) {
    STATUS = IDLE;
  } else {
    STATUS = NEXT_STATUS;

    switch (STATUS) {
    case IDLE:
      data_output.write(0);
      break;
    case BUSY_KEY:
      break;
    case BUSY_ENC: {
      unsigned int i;
      sc_uint<32> v0 = text[0], v1 = text[1];
      sc_uint<32> sum = 0, delta = 0x9E3779B9;
      for (i = 0; i < 32; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
      }
      text[0] = v0;
      text[1] = v1;
    } break;
    case BUSY_DEC: {
      unsigned int i;
      sc_uint<32> v0 = text[0], v1 = text[1];
      sc_uint<32> delta = 0x9E3779B9, sum = delta * 32;
      for (i = 0; i < 32; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
      }
      text[0] = v0;
      text[1] = v1;
    } break;
    }
  }
}
