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

void outpb(const bit16u address, const bit8u val) {
  asm (
    "  mov  edx,[ebp+8] \n"
    "  mov  eax,[ebp+12] \n"
    "  out  dx,al       \n"
  );
}

/*
void outpw(const bit16u address, const bit16u val) {
  asm (
    "  mov  edx,[ebp+8] \n"
    "  mov  eax,[ebp+12] \n"
    "  out  dx,ax       \n"
  );
}

void outpd(const bit16u address, const bit32u val) {
  asm (
    "  mov  edx,[ebp+8] \n"
    "  mov  eax,[ebp+12] \n"
    "  out  dx,eax      \n"
  );
}
*/

bit8u inpb(const bit16u address) {
  asm (
    "  mov  edx,[ebp+8] \n"
    "  xor  eax,eax     \n"
    "  in   al,dx       \n"
  );
}

/*
bit16u inpw(const bit16u address) {
  asm (
    "  mov  edx,[ebp+8] \n"
    "  xor  eax,eax     \n"
    "  in   ax,dx       \n"
  );
}

bit32u inpd(const bit16u address) {
  asm (
    "  mov  edx,[ebp+8] \n"
    "  in   eax,dx      \n"
  );
}
*/
