# mcopyata
#### Copy a floppy/hard drive from one computer to another via a serial null modem connection

One day, a few years ago, I wanted to preserve the contents of the hard drive of an old, somewhat obscure machine. If it was a modern machine, I could have simply written a simple app (not counting the many I could have downloaded) to read each sector of the disk and save it to a USB thumb drive. Easy, right?

Well, it wasn't a modern machine and no USB. With this in mind, if I read each sector and try to save it to the disk, the disk itself will fill up fast, let alone write to sectors I have already read. It doesn't have another Hard Drive Controller slot either, so no Slave drive...Where do I send the sector data? Also, it was an old enough machine and the drive didn't look like a standard IDE cabled drive, so I couldn't even pull it out and put it in the modern machine's empty HDC slot.

I got out my Serial Port notes and wrote a small/quick app to send these sectors across a (null modem) serial line to another computer with a hard drive large enough to hold the disk image, which was easy to find since it was at most 20meg, if memory serves.

Therefore, the code on this page was created.

As long as you have a null modem serial cable (you can find one online for practically nothing) and each computer has a standard serial port, you can read sectors from one computer's drive and store them as an image file on another computer.

For the receiver, I used a standard x86 desktop machine running Windows XP (with a DOS session), with the following command line:

`mcopyata /r /c 1`

For the sender, I used the following command line:

`mcopyata /s /x 0 /l 205360 /d 0x80 /c 2 /M`

In mcopyata.c, at the `parse command line` function, you will see the parameter meanings:

```
parse the command line
 usage:
   /s        = sender
   /r        = receiver
   /x [0x,0] = starting lba (if sender) value can be in hex or integer (required for sender, ignored if receiver)
   /l [0x,0] = count of sectors (if sender) value can be in hex or integer (required for sender, ignored if receiver)
   /c [0x,0] = com port number (required for both sender and receiver) (1, 2, 3, 4)
   /d [0x,0] = BIOS drive number 0x80, 0x81, etc. (required for sender, ignored if receiver)
   /M        = transfer maximum amount of data each transfer (optional for sender, ignored if receiver)
   /m        = transfer only one sector at a time (optional for sender, ignored if receiver)
```

This code is designed to read up to 127 sectors at a time sending the data across the line. It sends and receives 32-byte packets for services such as: `Okay, I'm ready`, `Okay, I will send a buffer of 'count' sectors`, `Okay, I received it, send the next`, etc.

It needs work. For example, I don't check for timeouts (the machine will wait for a character indefinitely), as well as other errors. I only needed it to copy the whole drive. Once I did that, the task was over. No need to make it any more robust.

If you use this code, you might need to slow down the baud-rate, transfer a sector at a time (the /m command parameter), and/or modify the code a little to get it to work for your particular situation. 

Of course since it uses the BIOS disk read services, you know that it is x86 real-mode only. More to the point, the sender must be 16-bit real-mode to be able to use the BIOS services. The reciever can in fact be 32-bit, or even 64-bit. However, here, the same code is compiled whether it is the sender or the receiver. If you modify the code a bit, you might be able to compile it, as the receiver, as a 64-bit console app on a different platform since it only needs to access the serial hardware and write to a disk file.

For more information on why I did this, see the [Blog Post](https://www.fysnet.net/blog/2018/08/index.html#20aug2018) where I showcase this obscure computer.

```
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
```
