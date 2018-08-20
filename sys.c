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
#include "sys.h"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Call an interrupt
// Return TRUE = carry set
bool intx(const int i, struct REGS *regs) {
  asm (
    "  push fs               ; \n"
    "  push ds               ; \n"  // save ds
    "  push es               ; \n"  // save es
    
    // the self-modifying code in huge model is quite elabrate
    "section .relot       \n"  // move to the relocation section
    "   dd  rel1          \n"  // insert a relocation item
    
    "section .text        \n"  // return to the code section
    "   db  0x66, 0xB8    \n"  // start of the mov eax,xxxxxxxx instruction
    "rel1:                \n"  // here is where the relocation happens
    "   dd  intx1         \n"  // assembler places SSSSOOOO in this location
    "  mov  bx,ax         \n"  // the assembler uses SSSSOOOO (where SmallerC uses xxxSSSSO, see below)
    "  shr  eax,16        \n"  // low 16-bits is offset
    "  mov  ds,ax         \n"  // high 16-bits is segment
    "  mov  eax,[bp+8]    \n"  // self modify code to interrupt number
                               // ds:bx -> intx1 below
    "  mov  [bx+1],al     \n"  // passed parameter is a 'constant', not a pointer, so is a literal value
    
    // serialize the instruction stream
    "  jmp .next_instruct    ; \n"  // self mod code needs serialization...
    ".next_instruct:         ; \n"
    
    "  mov  esi,[bp+12]      ; \n"  // esi contains both segment and offset values (xxxSSSSO)
    "  ror  esi,4            ; \n"  // OxxxSSSS
    "  mov  ds,si            ; \n"  // 
    "  mov  fs,si            ; \n"  //
    "  shr  esi,28           ; \n"  // esi = 4 bit offset
    "  push ebp              ; \n"  // save ebp
    "  push esi              ; \n"  // save the pointer to our regs data (offset only)
    "  mov  eax,[si+ 0]      ; \n"
    "  mov  ebx,[si+ 4]      ; \n"
    "  mov  ecx,[si+ 8]      ; \n"
    "  mov  edx,[si+12]      ; \n"
    "  mov  edi,[si+20]      ; \n"
    "  mov  ebp,[si+24]      ; \n"
    "  mov  es,[si+34]       ; \n"
    "  mov  ds,[si+32]       ; \n"
    "  mov  esi,[fs:si+16]   ; \n"
    "intx1:                  ; \n"
    "  int  0x00             ; zero will be replaced with value above \n"
    "  xchg esi,[esp]        ; swap the esi values (before and after values) \n"
    "  mov  [fs:si+ 0],eax   ; \n"
    "  mov  [fs:si+ 4],ebx   ; \n"
    "  mov  [fs:si+ 8],ecx   ; \n"
    "  mov  [fs:si+12],edx   ; \n"
    "  pop  eax              ; save the new esi value\n"
    "  mov  [fs:si+16],eax   ; \n"  // esi
    "  mov  [fs:si+20],edi   ; \n"
    "  mov  [fs:si+24],ebp   ; \n"
    "  mov  [fs:si+32],ds    ; \n"
    "  mov  [fs:si+34],es    ; \n"
    "  pushfd                ; retrieve eflags \n"
    "  pop  eax              ;  into eax \n"
    "  mov  [fs:si+28],eax   ; then into regs->eflags \n"
    "  pop  ebp               ; \n"
    "  pop  es                ; \n"
    "  pop  ds                ; \n"
    "  pop  fs                ; \n"
  );
  
  return (bool) (regs->eflags & 1);
}

// See if a key has been pressed.
// Only works on an Intel Legacy BIOS machine in real mode
int kbhit(void) {
  int ret = 0;
  
  asm (
    "  mov  ah,01 \n"
    "  int  16h   \n"
    "  pushfd     \n"
    "  pop  eax   \n"
    "  mov  [bp-4],eax \n"
  );
  
  // ZF clear if keystroke available
  return ((ret & 0x00000040) == 0);
}

// get the scancode of a key pressed
// (if no key has been pressed, it waits for one)
// Only works on an Intel Legacy BIOS machine in real mode
int getch(void) {
  int ret = 0;
  
  asm (
    "  mov  ah,00 \n"
    "  int  16h   \n"
    "  mov  [bp-4],eax \n"
  );
  
  return ret;
}
