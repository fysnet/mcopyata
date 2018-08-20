
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

#include <stdio.h>

#include "ctype.h"
#include "mcopyata.h"

#include "disk.h"
#include "mserial.h"
#include "portio.h"
#include "sys.h"

bool sender = FALSE;    // TRUE if this is the sending side (we have both so we make sure only one is specified)
bool receiver = FALSE;  // TRUE if this is the receiver side (...)
bit32u start_lba = 0;   // starting lba of transfer
bit32u lba_count = 1;   // count of LBA's to transmit
bit32u port_num = -1;   // com 1, com 2, etc.
bit32u drv_num = -1;    // 0x00, 0x01, 0x80, or 0x81
bool maximum = FALSE;   // (if neither max or min specified, we do a track at a time)
bool minimum = FALSE;   // default

unsigned port_addr;       // port address

int main(int argc, char *argv[]) {
  struct S_HEADER header;
  struct S_RESPONSE response;
  unsigned sec_per_track, count_per_transfer;
  void *buffer = NULL;
  FILE *fp;
  int ret = 0;
  
  if (!parse_cmnd_line(argc, argv))
    return -1;
  
  // if the sender, get the disk parameters.
  // returning the sectors per track so that we can transfer
  //  this amount at one time.
  if (sender)
    if ((sec_per_track = get_drv_parms(drv_num)) == -1)
      return -3;
  
  // default to a full track of sectors
  count_per_transfer = sec_per_track;
  
  // if min specified, do a sector at a time
  if (minimum)
    count_per_transfer = 1;  // only do one sector at a time
  
  // if max specified, read as many tracks as we can per transfer
  // 0x7F sectors is the maximum older BIOSes will allow ??????
  if (maximum) {
    count_per_transfer = sec_per_track;
    while ((count_per_transfer + sec_per_track) <= 0x7F)
      count_per_transfer += sec_per_track;
  }
  
  // initialize the serial port
  // Note:
  //  If we transfer (512 + 32 + 32) = 576 bytes per sector transferred.
  //  At this rate, we should transfer 100 sectors per second, not counting disk read, etc.
  //  However, it is much slower due to everything that must happen between sector transfers (CRC checks, etc)
  if (!init_serial(port_addr, SER_BAUD_115200, SER_PARITY_NONE | SER_STOP_1 | SER_BITS_8)) {
    puts("Could not initialize serial port.");
    return -2;
  }
  
  // if the receiver, we need to create/open the disk image file
  if (receiver) {
    if ((fp = fopen("disk.img", "r+b")) == NULL) {    // try to open it
      if ((fp = fopen("disk.img", "w+b")) == NULL) {  // else, try to create it
        printf("Error opening/creating file....\n");
        return -2;
      }
    }
  } else {
    // if the sender, we know the size of the buffer we need
    // if the receiver, we don't know it yet...
    buffer = (void *) malloc(count_per_transfer * 512);
  }
  
  serial_clear_buffer(port_addr);
  bit32u lba = start_lba;
  if (sender) {
    while (lba < (start_lba + lba_count)) {
      // if the user hits the ESC key, exit (informing the receiver)
      if (kbhit() && (getch() == 0x011B)) { // send abort header
        header.code = HDR_CODE_ABORT;
        break;
      }
      // don't go past the count specified
      bit32u count = (((start_lba + lba_count) - lba) < count_per_transfer) ? ((start_lba + lba_count) - lba) : count_per_transfer;
      if (read_sector(buffer, lba, count, drv_num)) {
        header.magic = HDR_MAGIC;
        header.lba = lba;
        header.count = count;
        header.code = HDR_CODE_GOOD;
        header.crc = calc_crc(buffer, count * 512);
        printf("Sending LBA %li -> %li\n", lba, lba + count - 1);
        serial_send(port_addr, &header, sizeof(struct S_HEADER), STD_TIMEOUT);
        serial_send(port_addr, buffer, count * 512, STD_TIMEOUT);
      } else {
        printf("Error reading from lba %li\n", lba);
        header.code = HDR_CODE_ABORT;
        break;
      }
      serial_recv(port_addr, &response, sizeof(struct S_RESPONSE), STD_TIMEOUT);
      if (response.resp_magic == RESP_MAGIC) {
        switch (response.response) {
          case RESP_GOOD:
            break;
          case RESP_ABORT:
            puts("Received abort response from receiver.");
            ret = -4;
            goto end_transfer;
          default:
            printf("Received bad response: 0x%08lX\n", response.response);
            ret = -5;
            goto end_transfer;
        }
      } else {
        printf("Did not receive response... (0x%08lX 0x%08lX)\n", response.resp_magic, response.response);
        header.code = HDR_CODE_ABORT;
        break;
      }
      lba += count;
      header.code = HDR_CODE_DONE;  // in case we are done
    }
    // sending ending header
    header.magic = HDR_MAGIC;
    header.lba = 0;
    header.crc = 0; // only calculated for the disk buffer data
    serial_send(port_addr, &header, sizeof(struct S_HEADER), STD_TIMEOUT);
  } else {
    puts("Waiting for sender to connect.");
    // this waits indefinately for the sender...
    if (serial_recv(port_addr, &header, sizeof(struct S_HEADER), NO_TIMEOUT) == -1) {
      printf("Receiver timed out\n");
    } else {
      // we didn't know the size of the buffer until now, so allocate it now.
      buffer = (void *) malloc(header.count * 512);
      while (header.code == HDR_CODE_GOOD) {
        serial_recv(port_addr, buffer, header.count * 512, STD_TIMEOUT);
        
        //printf("\rReceived lba %li  ", header.lba);
        printf("Received buffer for lba %li -> %li\n", header.lba, header.lba + header.count - 1);
        
        // assume good response
        response.response = RESP_GOOD;
        if (header.crc != calc_crc(buffer, header.count * 512)) {
          puts("CRC does not compute...");
          response.response = RESP_ABORT;
        }
        // if the user hits the ESC key, exit (informing the sender)
        if (kbhit() && (getch() == 0x011B))  // send abort header
          response.response = RESP_ABORT;
        response.resp_magic = RESP_MAGIC;
        serial_send(port_addr, &response, sizeof(struct S_RESPONSE), STD_TIMEOUT);
        
        // if not a good response, break out and be done
        if (response.response != RESP_GOOD)
          break;
        
        fseek(fp, (long) (header.lba * 512), SEEK_SET);
        fwrite(buffer, header.count, 512, fp);
        
        serial_recv(port_addr, &header, sizeof(struct S_HEADER), STD_TIMEOUT);
      }
    }
  }
  
end_transfer:  
  close_serial(port_addr);
  
  if (receiver)
    fclose(fp);
  free(buffer);
  
  return ret;
}

// parse the command line
// usage:
//   /s        = sender
//   /r        = receiver
//   /x [0x,0] = starting lba (if sender) value can be in hex or integer (required for sender, ignored if receiver)
//   /l [0x,0] = count of sectors (if sender) value can be in hex or integer (required for sender, ignored if receiver)
//   /c [0x,0] = com port number (required for both sender and receiver) (1, 2, 3, 4)
//   /d [0x,0] = drive number 0x80, 0x81, etc. (required for sender, ignored if receiver)
//   /M        = transfer maximum amount of data each transfer (optional for sender, ignored if receiver)
//   /m        = transfer only one sector at a time (optional for sender, ignored if receiver)
//
// example: (sender)
//    mcopyata /s /x 0 /l 1024 /c 2 /d 0x80 /M
// example: (receiver)
//    mcopyata /r /c 1
bool parse_cmnd_line(int argc, char *argv[]) {
  int i = 1;
  
  while (i<argc) {
    if (!strcmp(argv[i], "/s")) {
      // is the sender
      sender = TRUE;
    } else if (!strcmp(argv[i], "/r")) {
      // is the receiver
      receiver = TRUE;
    } else if (!strcmp(argv[i], "/M")) {
      maximum = TRUE;
      minimum = FALSE;
    } else if (!strcmp(argv[i], "/m")) {
      minimum = TRUE;
      maximum = FALSE;
    } else if (!strcmp(argv[i], "/x")) {
      i++;
      start_lba = strtol(argv[i], NULL, 0);
    } else if (!strcmp(argv[i], "/l")) {
      i++;
      lba_count = strtol(argv[i], NULL, 0);
    } else if (!strcmp(argv[i], "/c")) {
      i++;
      port_num = strtol(argv[i], NULL, 0);
    } else if (!strcmp(argv[i], "/d")) {
      i++;
      drv_num = strtol(argv[i], NULL, 0);
    } else {
      printf(" Unknown parameter: %s\n", argv[i]);
      break;
    }
    i++;
  }
  
  // check so parameters
  if ((sender && receiver) || (!sender && !receiver)) {
    puts("Must specify sender *or* receiver");
    return FALSE;
  }
  
  // check the COMx number
  switch (port_num) {
    case 1:
      port_addr = 0x3F8;
      break;
    case 2:
      port_addr = 0x2F8;
      break;
    case 3:
      port_addr = 0x3E8;
      break;
    case 4:
      port_addr = 0x2E8;
      break;
    default:
      puts("Must specify valid COMx number (1, 2, 3, 4)");
      return FALSE;
  }
  
  // if the drive number isn't 0, 1, or 0x80, 0x81, error
  if (sender)
    if ((drv_num != 0) && (drv_num != 1) && (drv_num != 0x80) && (drv_num != 0x81)) {
      puts("Must have valid drive number (0, 1, 0x80, 0x81)");
      return FALSE;
    }
  
  // we got all the info we needed
  return TRUE;
}
