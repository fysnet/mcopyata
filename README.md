# mcopyata
Copy a floppy/hard drive from one computer to another via a serial null modem connection

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
     *  Notes:
     *    - This hard codes to the maximum Serial BAUD rate.  You may need to adjust
     *      to a slower rate and recompile.
     *
     *  Things TODO:
     *    - actually check the return value of the serial_send() and serial_recv()
     *      functions to see if we timed-out.
     *    - along with this time-out thing, the check for time-out withint the two
     *      functions will dramatically slow down the transfer rate.  For now, we have
     *      it commented out.
     *
     */
