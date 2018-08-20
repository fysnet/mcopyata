
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

#ifndef SYS_H
#define SYS_H

struct REGS {
  bit32u eax;    //  0
  bit32u ebx;    //  4
  bit32u ecx;    //  8
  bit32u edx;    // 12
  bit32u esi;    // 16
  bit32u edi;    // 20
  bit32u ebp;    // 24
  bit32u eflags; // 28
  bit16u ds;     // 32
  bit16u es;     // 34
};

bool intx(const int i, struct REGS *regs);

int kbhit(void);
int getch(void);


#endif  // SYS_H
