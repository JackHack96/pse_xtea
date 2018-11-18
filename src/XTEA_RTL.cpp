#include "XTEA_RTL.h"
#include <XTEA_RTL.h>
#include <stdint.h>

void XTEA_RTL::fsm() {
  NEXT_STATUS = STATUS;
  switch (STATUS) {
    case IDLE:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Current status: IDLE" << endl;
#endif
      if (load_key.read() && !encrypt.read() && !decrypt.read()) {
        NEXT_STATUS = BUSY_KEY;
#ifdef DEBUG
        cout << "@" << sc_time_stamp() << " Next status: BUSY_KEY" << endl;
#endif
      } else if (encrypt.read() && !load_key.read() && !decrypt.read()) {
        NEXT_STATUS = BUSY_ENC;
#ifdef DEBUG
        cout << "@" << sc_time_stamp() << " Next status: BUSY_ENC" << endl;
#endif
      } else if (decrypt.read() && !load_key.read() && !encrypt.read()) {
        NEXT_STATUS = BUSY_DEC;
#ifdef DEBUG
        cout << "@" << sc_time_stamp() << " Next status: BUSY_DEC" << endl;
#endif
      } else {
        NEXT_STATUS = IDLE;
#ifdef DEBUG
        cout << "@" << sc_time_stamp() << " Next status: IDLE" << endl;
#endif
      }
      break;
    case BUSY_KEY:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Current status: BUSY_KEY" << endl;
#endif
      if (load_key.read())
        NEXT_STATUS = BUSY_KEY;
      else
        NEXT_STATUS = IDLE;
      break;
    case BUSY_ENC:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Current status: BUSY_ENC" << endl;
#endif
      if (encrypt.read())
        NEXT_STATUS = BUSY_ENC;
      else
        NEXT_STATUS = IDLE;
      break;
    case BUSY_DEC:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Current status: BUSY_DEC" << endl;
#endif
      if (decrypt.read())
        NEXT_STATUS = BUSY_DEC;
      else
        NEXT_STATUS = IDLE;
      break;
  }
}

void XTEA_RTL::datapath() {
  STATUS = NEXT_STATUS;

  switch (STATUS) {
    case IDLE:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Datapath status: IDLE" << endl;
#endif
      key_index = 0;
      data_output.write(0);
      break;
    case BUSY_KEY:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Datapath status: BUSY_KEY" << endl;
#endif
      key[key_index++] = data_input.read().range(31, 0);
      key[key_index++] = data_input.read().range(63, 32);
      break;
    case BUSY_ENC:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Datapath status: BUSY_ENC" << endl;
#endif
      xtea(data_input.read().range(63, 32), data_input.read().range(31, 0),
           key[0], key[1], key[2], key[3], false, &text[0], &text[1]);
      data_output.write((text[0], text[1]));
      break;
    case BUSY_DEC:
#ifdef DEBUG
      cout << "@" << sc_time_stamp() << " Datapath status: BUSY_DEC" << endl;
#endif
      xtea(data_input.read().range(63, 32), data_input.read().range(31, 0),
           key[0], key[1], key[2], key[3], true, &text[0], &text[1]);
      data_output.write((text[0], text[1]));
      break;
  }
}

void XTEA_RTL::xtea(sc_uint<32> word0, sc_uint<32> word1, sc_uint<32> key0,
                    sc_uint<32> key1, sc_uint<32> key2, sc_uint<32> key3,
                    bool mode, sc_uint<32> *result0, sc_uint<32> *result1) {
  sc_uint<64> sum;
  sc_uint<32> i, delta, v0, v1, temp;
  v0  = word0;
  v1  = word1;
  sum = 0;
  *result0 = 0;
  *result1 = 0;

  if (mode == 0) {
    // encipher
    delta  = 0x9e3779b9;
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
    delta  = 0x9e3779b9;
    sum    = delta * 32;
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
