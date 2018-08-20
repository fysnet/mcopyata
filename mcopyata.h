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

#define HDR_MAGIC  0x0BADBEEF

#define HDR_CODE_GOOD   0
#define HDR_CODE_DONE   1
#define HDR_CODE_ABORT  2

struct S_HEADER {
  bit32u magic;
  bit32u lba;
  bit32u count;
  bit32u code;
  bit32u crc;
  bit8u  resvd[12];
};

#define RESP_MAGIC  0xA5A5A5A5

#define RESP_GOOD   0
#define RESP_ABORT  1

struct S_RESPONSE {
  bit32u resp_magic;
  bit32u response;
  bit32u pad0;
  bit32u crc;
  bit8u  resvd[16];
};

bool parse_cmnd_line(int argc, char *argv[]);
