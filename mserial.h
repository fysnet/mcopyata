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
 *    http://www.fysnet.net/blog/2018/08/index.html#20aug2018
 *  Contact:
 *    fys [at] fysnet [dot] net
 *
 * Last update:  20 Aug 2018
 *
 * compile using SmallerC  (https://github.com/alexfru/SmallerC/)
 *  smlrcc @make.txt
 *
 */

#ifndef MSERIAL_H
#define MSERIAL_H

#define  SER_TIMEOUT  (20 * 18)  // 20 seconds

// serial stuff
#define  SER_DIVISOR_LSB  0x000
#define  SER_DIVISOR_MSB  0x001
#define  SER_RECEIVE      0x000
#define  SER_TRANSMIT     0x000
#define  SER_INT_ENABLE   0x001
#define  SER_INT_ID       0x002
#define  SER_LINE_CNTRL   0x003
#define  SER_FIFO         0x003
#define  SER_MODEM_CNTRL  0x004
#define  SER_LINE_STATUS  0x005
#define  SER_MODEM_STATUS 0x006
#define  SER_SCRATCH      0x007
#define  SER_SPECIAL      0x007

#define SER_DIV_LATCH_ON  0x80  // used to turn reg 0,1 into divisor latch

#define SER_BAUD_1200   96   // baud rate divisors
#define SER_BAUD_2400   48
#define SER_BAUD_4800   24
#define SER_BAUD_9600   12
#define SER_BAUD_14400   8
#define SER_BAUD_19200   6
#define SER_BAUD_28800   4
#define SER_BAUD_38400   3
#define SER_BAUD_57600   2
#define SER_BAUD_115200  1

#define SER_STOP_1      0     // 1 stop bit per character
#define SER_STOP_2      4     // 2 stop bits per character

#define SER_BITS_5      0     // send 5 bit characters
#define SER_BITS_6      1     // send 6 bit characters
#define SER_BITS_7      2     // send 7 bit characters
#define SER_BITS_8      3     // send 8 bit characters

#define SER_PARITY_NONE    0     // no parity
#define SER_PARITY_ODD     8     // odd parity
#define SER_PARITY_EVEN   24     // even parity

#define SER_ID_FIFO_TRIG_1   (0<<6)
#define SER_ID_FIFO_TRIG_4   (1<<6)
#define SER_ID_FIFO_TRIG_8   (2<<6)
#define SER_ID_FIFO_TRIG_14  (3<<6)
#define SER_ID_TX_CLR        (1<<2)
#define SER_ID_RX_CLR        (1<<1)
#define SER_ID_FIFO_ENB      (1<<0)

#define SER_MODEM_CTLR_RTS   2
#define SER_MODEM_CTLR_DTR   1

// time-out counts
#define NO_TIMEOUT   0
#define STD_TIMEOUT  256           // depending on the speed of the ISA bus, this can be around (256 * 200ns) = 51uS between each byte received/sent
#define MAX_TIMEOUT  5000000       // depending on the speed of the ISA bus, this can be around (5,000,000 * 200ns) = 1,000,000uS = 1 Sec between each byte received/sent

void serial_clear_buffer(const bit32u address);
bool init_serial(const bit32u address, const int baud, const int config);
void close_serial(const bit32u address);
int serial_send(const bit32u address, void *buffer, const int count, const int timeout);
int serial_recv(const bit32u address, void *buffer, const int count, const int timeout);

#endif  // MSERIAL_H
