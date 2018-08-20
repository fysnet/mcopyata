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

#ifndef PORTIO_H
#define PORTIO_H

void outpb(const bit16u address, const bit8u val);
//void outpw(const bit16u address, const bit16u val);
//void outpd(const bit16u address, const bit32u val);
bit8u inpb(const bit16u address);
//bit16u inpw(const bit16u address);
//bit32u inpd(const bit16u address);

#endif  // PORTIO_H
