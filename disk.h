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

#ifndef DISK_H
#define DISK_H

bit32u get_drv_parms(const bit32u drv_num);
bool read_sector(void *buffer, const bit32u lba, const int count, const bit32u drv_num);



#endif  // DISK_H
