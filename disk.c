
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

#include "ctype.h"

#include "disk.h"
#include "sys.h"

#define FP_SEG(x) (((bit32u) (x) >> 4) & 0xFFFF)
#define FP_OFF(x) ((bit32u) (x) >> 20 << 4) | ((bit32u) (x) & 0xF)

unsigned total_cyls;
unsigned sec_per_track;
unsigned heads;

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// This routine converts LBA to CHS
// Sector   = (LBA mod SPT) + 1
// Head     = (LBA  /  SPT) mod Heads
// Cylinder = (LBA  /  SPT)  /  Heads
//    (SPT = Sectors per Track)
void lba_to_chs(const bit32u lba, unsigned int *cyl, unsigned int *head, unsigned int *sect) {
  *sect = (lba % sec_per_track) + 1;
  *head = (lba / sec_per_track) % heads;
  *cyl  = (lba / sec_per_track) / heads;
}

// returns sectors per track
// or
// -1 if error
bit32u get_drv_parms(const bit32u drv_num) {
  struct REGS regs;
  
  regs.eax = 0x00000800;
  regs.edx = drv_num;
  regs.es = 0;
  regs.edi = 0;
  if (!intx(0x13, &regs)) {
    sec_per_track = regs.ecx & 0x3F;
    heads = ((regs.edx & 0xFF00) >> 8) + 1;
    printf("Found %li total cylinders\n", ((regs.ecx & 0xC0) << 2) | ((regs.ecx >> 8) & 0xFF));
    return sec_per_track;
  } else {
    printf("Error retrieving drive information...");
    return -1;
  }
}

bool read_sector(void *buffer, const bit32u lba, const int count, const bit32u drv_num) {
  struct REGS regs;
  unsigned cyl, head, sect;
  
  lba_to_chs(lba, &cyl, &head, &sect);
  
  regs.eax = 0x00000200 | (count & 0xFF);
  regs.ecx = ((cyl & 0xFF) << 8) | (((cyl >> 8) & 3) << 6) | (sect & 0x3F);  // CH = low 8 bits of cyl, CL = CCSSSSSS
  regs.edx = (head << 8) | drv_num;
  regs.ebx = FP_OFF(buffer);
  regs.es = FP_SEG(buffer);
  return ((intx(0x13, &regs) == FALSE) && ((regs.eax & 0x0000FF00) == 0));
}
