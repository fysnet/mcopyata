/*             Author: Benjamin David Lunt
 *                     Forever Young Software
 *                     Copyright (c) 1984-2018
 *  
 *  This code is donated to the Freeware communitee.  You have the
 *   right to use it for learning purposes only.  You may not modify it
 *   for redistribution for any other purpose unless you have written
 *   permission from the author.
 *
 *  You may modify and use it in your own projects as long as they are
 *   for non-profit only and not distributed.  Any project for profit that 
 *   uses this code must have written permission from the author.
 *
 *  For more information:
 *    http://www.fysnet.net/blog/2018/08/index.htm#20aug2018
 *  Contact:
 *    fys [at] fysnet [dot] net
 *
 * Last update:  20 Aug 2018
 *
 * compile using SmallerC  (https://github.com/alexfru/SmallerC/)
 *  smlrcc @make.txt
 *
 */

#include "ctype.h"
#include "mserial.h"

// com1 is at 0x3F8, irq 4
// com2 is at 0x2F8, irq 3
// com3 is at 0x3E8, irq 4
// com4 is at 0x2E8, irq 3
bool init_serial(const bit32u address, const int baud, const int config) {
  
  outpb(address + SER_INT_ENABLE, 0x00); // disable all interrupts
	outpb(address + SER_LINE_CNTRL, SER_DIV_LATCH_ON);
	outpb(address + SER_DIVISOR_LSB, (bit8u) (baud & 0xFF));
	outpb(address + SER_DIVISOR_MSB, (bit8u) ((baud >> 8) & 0xFF));
	outpb(address + SER_LINE_CNTRL, (bit8u) config); // set the configuration for the port
  outpb(address + SER_INT_ID, SER_ID_FIFO_TRIG_14 | SER_ID_TX_CLR | SER_ID_RX_CLR | SER_ID_FIFO_ENB);
  outpb(address + SER_MODEM_CNTRL, SER_MODEM_CTLR_RTS | SER_MODEM_CTLR_DTR);
  
  return TRUE;
}

void close_serial(const bit32u address) {
  outpb(address + SER_MODEM_CNTRL, 0x00); // lower the RTS and DTR
}

// if timeout != 0, this will check for a byte 'timeout' times before it times-out
int serial_send(const bit32u address, void *buffer, const int count, const int timeout) {
  int cnt = 0, time;
  bit8u byte;
  bit8u *p = (bit8u *) buffer;
  
  outpb(address + SER_MODEM_CNTRL, SER_MODEM_CTLR_RTS | SER_MODEM_CTLR_DTR);
  while (cnt < count) {
//    time = timeout;
    do {
      byte = inpb(address + SER_MODEM_STATUS);
      if ((byte & 0x80) == 0) {
        printf("Error on send: 0x%02X\n", byte);
        return -1;
      }
      // if we have a timeout count and we have expired, return
//      if (timeout && ((time -= 2) < 1))  // -2 because there are 2 inpb's in this loop
//        return -1;
      // wait for transmit buffer to be empty
      byte = inpb(address + SER_LINE_STATUS);
    } while ((byte & 0x20) == 0);
		outpb(address + SER_TRANSMIT, p[cnt]);
    cnt++;
  }
  
  return cnt;
}

void serial_clear_buffer(const bit32u address) {
  while (inpb(address + SER_LINE_STATUS) & 0x01)
    inpb(address + SER_RECEIVE);
}

// if timeout != 0, this will check for a byte 'timeout' times before it times-out
int serial_recv(const bit32u address, void *buffer, const int count, const int timeout) {
  int cnt = 0, time;
  bit8u byte, *p = (bit8u *) buffer;
  
  memset(buffer, 0xFF, 4);  // 'clear' out the first dword so we don't get a false positive on the magic member
  
  outpb(address + SER_MODEM_CNTRL, SER_MODEM_CTLR_RTS | SER_MODEM_CTLR_DTR);
  while (cnt < count) {
//    time = timeout;
    do {
      byte = inpb(address + SER_LINE_STATUS);
      if (byte & 0x8E) {
        printf("Error on receive: 0x%02X\n", byte);
        return -1;
      }
      // if we have a timeout count and we have expired, return
//      if (timeout && (--time == 0))
//        return -1;
    } while ((byte & 0x01) == 0);
    p[cnt] = inpb(address + SER_RECEIVE);
    cnt++;
  }
  
  return cnt;
}
