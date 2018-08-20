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

#ifndef CTYPE_H
#define CTYPE_H

// standard true and false
#define TRUE   1
#define FALSE  0
#define NULL   0

typedef unsigned  char      bool;

// size of memory operands
typedef   signed  char      bit8s;
typedef unsigned  char      bit8u;
typedef   signed short      bit16s;
typedef unsigned short      bit16u;
typedef   signed  long      bit32s;
typedef unsigned  long      bit32u;

#endif // CTYPE_H
