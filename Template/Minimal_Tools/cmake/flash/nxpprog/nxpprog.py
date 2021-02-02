# -*- coding: utf-8 -*-

#!/usr/bin/python3
#
# Copyright (c) 2009 Brian Murphy <brian@murphy.dk>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# A simple programmer which works with the ISP protocol on NXP LPC arm
# processors.

import argparse
import binascii
import os
import sys
import struct
import getopt
import logging
import signal
import socket
import time

import ihex

import serial
import serial.serialutil
import serial.tools.list_ports as port_list

CMD_SUCCESS = 0
INVALID_COMMAND = 1
SRC_ADDR_ERROR = 2
DST_ADDR_ERROR = 3
SRC_ADDR_NOT_MAPPED = 4
DST_ADDR_NOT_MAPPED = 5
COUNT_ERROR = 6
INVALID_SECTOR = 7
SECTOR_NOT_BLANK = 8
SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION = 9
COMPARE_ERROR = 10
BUSY = 11
PARAM_ERROR = 12
ADDR_ERROR = 13
ADDR_NOT_MAPPED = 14
CMD_LOCKED = 15
INVALID_CODE = 16
INVALID_BAUD_RATE = 17
INVALID_STOP_BIT = 18
CODE_READ_PROTECTION_ENABLED = 19

COM_PORT_DELAY = 0.1/2
COM_PORT_TIMEOUT = 0.250

parser = argparse.ArgumentParser()


parser.add_argument(
    '--binary',
    required=True,
    help='path to the firmware.bin file you want to program the board with.')

parser.add_argument(
    '--device',
    required=True,
    type=str,
    default="",
    help='Path to serial device file. In linux the name should be '
    'something similar to "/dev/ttyUSB0", WSL "/dev/ttyS0", and '
    'Max OSX "/dev/tty-usbserial-AJ20A5".')

parser.add_argument(
    '--udp',
    help='program processor using Ethernet.',
    action='store_true')

parser.add_argument(
    '--cpu',
    type=str,
    default="",
    help='set the cpu type.')

parser.add_argument(
    '--osfreq',
    type=int,
    default=0,
    help='set the oscillator frequency.')

parser.add_argument(
    '--baud',
    type=int,
    nargs='?',
    default=0,
    help='set the baud rate.')

parser.add_argument(
    '--xonxoff',
    help='enable xonxoff flow control.',
    action='store_true')

parser.add_argument(
    '--control',
    help='use RTS and DTR to control reset and int0.',
    action='store_true')

parser.add_argument(
    '--start',
    help='start the device at a set address.',
    action='store_true')

parser.add_argument(
    '-v',
    '--verbose',
    help='Enable version debug message output.',
    action='store_true')

parser.add_argument(
    '--read',
    type=str,
    default="",
    help='read from a file.')

parser.add_argument(
    '--len',
    type=int,
    default=0,
    help='number of bytes to be read.')

parser.add_argument(
    '--serialnumber',
    help='get the device serial number.',
    action='store_true')

parser.add_argument(
    '--list',
    help='list supported processors.',
    action='store_true')

parser.add_argument(
    '--addr',
    type=str,
    default='',
    help='set the base address for the image.')

parser.add_argument(
    '--verify',
    help='read the device after programming.',
    action='store_true')

parser.add_argument(
    '--verifyonly',
    help='don\'t program, just verify.',
    action='store_true')

parser.add_argument(
    '--eraseonly',
    help='don\'t program, just erase. Implies --eraseall.',
    action='store_true')

parser.add_argument(
    '--eraseall',
    help='erase all flash not just the area written to.',
    action='store_true')

parser.add_argument(
    '--blankcheck',
    help='don\'t program, just check that the flash is blank.',
    action='store_true')

parser.add_argument(
    '--filetype',
    type=str,
    default='bin',
    help='set filetype to intel hex format or raw binary.')

parser.add_argument(
    '--bank',
    type=int,
    default=0,
    help='set filetype to intel hex format or raw binary.')

parser.add_argument(
    '--port',
    type=int,
    default=41825,
    help='UDP port number to use (default 41825).')

parser.add_argument(
    '--mac',
    type=str,
    default='',
    help='MAC address to associate IP address with.')

# flash sector sizes for lpc23xx/lpc24xx/lpc214x processors
flash_sector_lpc23xx = (
                        4, 4, 4, 4, 4, 4, 4, 4,
                        32, 32, 32, 32, 32, 32, 32,
                        32, 32, 32, 32, 32, 32, 32,
                        4, 4, 4, 4, 4, 4
                       )

# flash sector sizes for 64k lpc21xx processors (without bootsector)
flash_sector_lpc21xx_64 = (
                            8, 8, 8, 8, 8, 8, 8, 8
                           )

# flash sector sizes for 128k lpc21xx processors (without bootsector)
flash_sector_lpc21xx_128 = (
                            8, 8, 8, 8, 8, 8, 8, 8,
                            8, 8, 8, 8, 8, 8, 8
                           )

# flash sector sizes for 256k lpc21xx processors (without bootsector)
flash_sector_lpc21xx_256 = (
                            8, 8, 8, 8, 8, 8, 8, 8,
                            64, 64,
                            8, 8, 8, 8, 8, 8, 8,
                           )

# flash sector sizes for lpc17xx processors
flash_sector_lpc17xx = (
                        4, 4, 4, 4, 4, 4, 4, 4,
                        4, 4, 4, 4, 4, 4, 4, 4,
                        32, 32, 32, 32, 32, 32, 32,
                        32, 32, 32, 32, 32, 32, 32,
                       )

# flash sector sizes for lpc40xx processors
flash_sector_lpc40xx = (
                        4, 4, 4, 4, 4, 4, 4, 4,
                        4, 4, 4, 4, 4, 4, 4, 4,
                        32, 32, 32, 32, 32, 32, 32,
                        32, 32, 32, 32, 32, 32, 32,
                       )

# flash sector sizes for lpc11xx processors
flash_sector_lpc11xx = (
        4, 4, 4, 4, 4, 4, 4, 4,
        )

# flash sector sizes for lpc18xx processors
flash_sector_lpc18xx = (
                        8, 8, 8, 8, 8, 8, 8, 8,
                        64, 64, 64, 64, 64, 64, 64,
                       )


flash_prog_buffer_base_default = 0x40001000
flash_prog_buffer_size_default = 4096

# cpu parameter table
cpu_parms = {
        # 128k flash
        "lpc2364" : {
            "flash_sector" : flash_sector_lpc23xx,
            "flash_sector_count": 11,
            "devid": 369162498
        },
        # 256k flash
        "lpc2365" : {
            "flash_sector" : flash_sector_lpc23xx,
            "flash_sector_count": 15,
            "devid": 369158179
        },
        "lpc2366" : {
            "flash_sector" : flash_sector_lpc23xx,
            "flash_sector_count": 15,
            "devid": 369162531
        },
        # 512k flash
        "lpc2367" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 369158181
        },
        "lpc2368" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 369162533
        },
        "lpc2377" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 385935397
        },
        "lpc2378" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 385940773
        },
        "lpc2387" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 402716981

        },
        "lpc2388" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 402718517
        },
        # lpc21xx
        # some incomplete info here need at least sector count
        "lpc2141": {
            "devid": 196353,
            "flash_sector": flash_sector_lpc23xx,
            "flash_sector_count": 8,
        },
        "lpc2142": {
            "flash_sector": flash_sector_lpc23xx,
            "flash_sector_count": 9,
            "devid": 196369,
        },
        "lpc2144": {
            "flash_sector": flash_sector_lpc23xx,
            "flash_sector_count": 11,
            "devid": 196370,
        },
        "lpc2146": {
            "flash_sector": flash_sector_lpc23xx,
            "flash_sector_count": 15,
            "devid": 196387,
        },
        "lpc2148": {
            "flash_sector": flash_sector_lpc23xx,
            "flash_sector_count": 27,
            "devid": 196389,
        },
        "lpc2109" : {
            "flash_sector": flash_sector_lpc21xx_64,
            "devid": 33685249
        },
        "lpc2119" : {
            "flash_sector": flash_sector_lpc21xx_128,
            "devid": 33685266
        },
        "lpc2129" : {
            "flash_sector": flash_sector_lpc21xx_256,
            "devid": 33685267
        },
        "lpc2114" : {
            "flash_sector" : flash_sector_lpc21xx_128,
            "devid": 16908050
        },
        "lpc2124" : {
            "flash_sector" : flash_sector_lpc21xx_256,
            "devid": 16908051
        },
        "lpc2194" : {
            "flash_sector" : flash_sector_lpc21xx_256,
            "devid": 50462483
        },
        "lpc2292" : {
            "flash_sector" : flash_sector_lpc21xx_256,
            "devid": 67239699
        },
        "lpc2294" : {
            "flash_sector" : flash_sector_lpc21xx_256,
            "devid": 84016915
        },
        # lpc22xx
        "lpc2212" : {
            "flash_sector" : flash_sector_lpc21xx_128
        },
        "lpc2214" : {
            "flash_sector" : flash_sector_lpc21xx_256
        },
        # lpc24xx
        "lpc2458" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 352386869,
        },
        "lpc2468" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 369164085,
        },
        "lpc2478" : {
            "flash_sector" : flash_sector_lpc23xx,
            "devid": 386006837,
        },
        # lpc17xx
        "lpc1769" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10000200,
            "csum_vec": 7,
            "devid": 0x26113f37,
            "cpu_type": "thumb",
        },
        "lpc1768" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x26013f37,
            "cpu_type": "thumb",
        },
        "lpc1767" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x26012837,
            "cpu_type": "thumb",
        },
        "lpc1766" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x26013f33,
            "cpu_type": "thumb",
        },
        "lpc1765" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x26013733,
            "cpu_type": "thumb",
        },
        "lpc1764" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x26011922,
            "cpu_type": "thumb",
        },
        "lpc1763" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x26012033,
            "cpu_type": "thumb",
        },
        "lpc1759" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x25113737,
            "cpu_type": "thumb",
        },
        "lpc1758" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x25013f37,
            "cpu_type": "thumb",
        },
        "lpc1756" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x25011723,
            "cpu_type": "thumb",
        },
        "lpc1754" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x25011722,
            "cpu_type": "thumb",
        },
        "lpc1752" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x25001121,
            "cpu_type": "thumb",
        },
        "lpc1751" : {
            "flash_sector" : flash_sector_lpc17xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x25001110,
            "cpu_type": "thumb",
        },
        "lpc4078" : {
            "flash_sector" : flash_sector_lpc40xx,
            "flash_prog_buffer_base" : 0x10001000,
            "csum_vec": 7,
            "devid": 0x47193F47,
            "cpu_type": "thumb",
        },
        "lpc1114" : {
            "flash_sector" : flash_sector_lpc11xx,
            "flash_prog_buffer_base" : 0x10000400,
            "devid": 0x0444102B,
            "flash_prog_buffer_size" : 1024
        },
        # lpc18xx
        "lpc1817" : {
            "flash_sector" : flash_sector_lpc18xx,
            "flash_bank_addr": (0x1a000000, 0x1b000000),
            "flash_prog_buffer_base" : 0x10081000,
            "devid": (0xF001DB3F, 0),
            "devid_word1_mask": 0xff,
            "csum_vec": 7,
            "cpu_type": "thumb",
        },
        "lpc1832" : {
            "flash_sector" : flash_sector_lpc18xx,
            "flash_bank_addr": (0x1a000000),
            "flash_prog_buffer_base" : 0x10081000,
            "csum_vec": 7,
            "cpu_type": "thumb",
        },
        "lpc1833" : {
            "flash_sector" : flash_sector_lpc18xx,
            "flash_sector_count": 11,
            "flash_bank_addr": (0x1a000000, 0x1b000000),
            "flash_prog_buffer_base" : 0x10081000,
            "devid": (0xf001da30, 0x44),
            "csum_vec": 7,
            "cpu_type": "thumb",
        },
        "lpc1837" : {
            "flash_sector" : flash_sector_lpc18xx,
            "flash_bank_addr": (0x1a000000, 0x1b000000),
            "flash_prog_buffer_base" : 0x10081000,
            "devid": (0xf001da30, 0),
            "csum_vec": 7,
            "cpu_type": "thumb",
        },
        "lpc1853" : {
            "flash_sector" : flash_sector_lpc18xx,
            "flash_sector_count": 11,
            "flash_bank_addr": (0x1a000000, 0x1b000000),
            "flash_prog_buffer_base" : 0x10081000,
            "devid": (0xf001d830, 0),
            "csum_vec": 7,
            "cpu_type": "thumb",
        },
        "lpc1857" : {
            "flash_sector" : flash_sector_lpc18xx,
            "flash_bank_addr": (0x1a000000, 0x1b000000),
            "flash_prog_buffer_base" : 0x10081000,
            "devid": (0xf001d830, 0x44),
            "csum_vec": 7,
            "cpu_type": "thumb",
        },
}

prog = None


LPC_CHAR = {'Question': b'?',
            'Synchronized': b'Synchronized\r\n',
            'SynchronizedLeadingZeros': b'\x00\x00Synchronized\r\n',
            'OK': b'OK\r\n'}

## Animation stuff
ANIMATIONS = {
    "circles": [0x25D0, 0x25D3, 0x25D1, 0x25D2],
    "quadrants": [0x259F, 0x2599, 0x259B, 0x259C],
    "trigrams": [0x2630, 0x2631, 0x2632, 0x2634],
    "squarefills": [0x25E7, 0x25E9, 0x25E8, 0x25EA],
    "spaces": [0x2008, 0x2008, 0x2008, 0x2008],
    "braille":
    [0x2840, 0x2844, 0x2846, 0x2847, 0x28c7, 0x28e7, 0x28f7, 0x28fF],
    "dots12": [
        "⢀⠀","⡀⠀","⠄⠀","⢂⠀","⡂⠀","⠅⠀","⢃⠀","⡃⠀","⠍⠀","⢋⠀","⡋⠀","⠍⠁","⢋⠁",
        "⡋⠁","⠍⠉","⠋⠉","⠋⠉","⠉⠙","⠉⠙","⠉⠩","⠈⢙","⠈⡙","⢈⠩","⡀⢙","⠄⡙","⢂⠩",
        "⡂⢘","⠅⡘","⢃⠨","⡃⢐","⠍⡐","⢋⠠","⡋⢀","⠍⡁","⢋⠁","⡋⠁","⠍⠉","⠋⠉","⠋⠉",
        "⠉⠙","⠉⠙","⠉⠩","⠈⢙","⠈⡙","⠈⠩","⠀⢙","⠀⡙","⠀⠩","⠀⢘","⠀⡘","⠀⠨","⠀⢐",
        "⠀⡐","⠀⠠","⠀⢀","⠀⡀"
    ]
}
selected_animation = ANIMATIONS["dots12"]


def unichar(i):
    try:
        return chr(i)
    except ValueError:
        return struct.pack('i', i).decode('utf-32')


def progress_bar(bar_length, current_block, total_blocks):
    bar_len = bar_length
    filled_len = int(round(bar_len * (current_block + 1) / float(total_blocks)))

    percent = round(100.0 * (current_block + 1) / float(total_blocks), 1)

    bar = "=" * (filled_len - 1)
    bar = bar + "-" * (bar_len - filled_len)

    suffix = "Block # 0x{:X} of 0x{:X}".format(current_block + 1, int(total_blocks))

    sys.stdout.write("{} {}% {}\r".format(bar, percent, suffix))
    sys.stdout.flush()


class AutoLPCPortFinder:
    def __init__(self, device=None):
        self.device = device

    def port_write_and_verify(self, port, payload, error_message="", debug_message=""):
        bytes_sent = port.write(bytearray(payload))
        if bytes_sent != len(payload):
            log(error_message)
        elif debug_message:
            log(debug_message)

    def port_read(self, port, number_of_bytes):
        return bytearray(port.read(number_of_bytes))

    def find_lpc_port(self):
        log('Scanning ports for NXP LPC devices...')
        if self.device:
            serial_device_list = [
                {
                    "device": self.device
                }
            ]
        else:
            serial_device_list = list(port_list.comports())

        for port_info in serial_device_list:
            try:
                # On Macs, we have extra ports that are likely not real serial ports
                if "Bluetooth" in port_info.device or "AirPods" in port_info.device:
                    logging.info('Skipping port %s because it is probably not a serial port', port_info.device)
                    continue
                else:
                    logging.info("Trying port '%s'", port_info.device)

                port = serial.Serial(
                    port=port_info.device,
                    baudrate=115200,
                    parity=serial.PARITY_NONE,
                    stopbits=serial.STOPBITS_ONE,
                    bytesize=serial.EIGHTBITS,
                    timeout=1)

                sync_successful = self.attempt_sync(port)
                if sync_successful:
                    logging.info("LPC device found on %s", port_info.device)
                    port.close()
                    return port_info.device
            except (OSError, serial.SerialException) as e:
                logging.debug('excpetion on port %s', e)
                continue

        return None

    def attempt_sync(self, serial):
        log('-'*80)
        logging.info('Attempting to sync with device at %s', serial.port)
        enter_isp(serial)
        self.port_write_and_verify(serial, LPC_CHAR['Question'])

        # We should not need the sleep while reading
        # time.sleep(COM_PORT_DELAY)
        response = self.port_read(serial, 14)
        if len(response) > 0:
            logging.info('Response: %s', response)
        else:
            logging.warning('No response from %s', port.port)

        if (response == LPC_CHAR['Synchronized'] or
            response == LPC_CHAR['SynchronizedLeadingZeros']):
            #time.sleep(COM_PORT_DELAY)
            self.port_write_and_verify(serial, LPC_CHAR['Synchronized'])

            #time.sleep(COM_PORT_DELAY)
            response = self.port_read(serial, 17)
            return len(response) >= 4 and response[-4:] == LPC_CHAR['OK']
        return False


def log(str):
    sys.stderr.write("%s\n" % str)
    sys.stderr.flush()


def dump(name, str):
    logging.info("%s:\n" % name)
    ct = 0
    for i in str:
        logging.info("%x, " % ord(i))
        ct += 1
        if ct == 4:
            ct = 0
            logging.info("\n")
    logging.info("\n")


def panic(str):
    logging.error(str)
    if prog:
        prog.device.close()
    sys.exit(1)


def syntax():
    panic(
"""\
{0} <serial device> <image_file> : program image file to processor.
{0} --udp <ip address> <image_file> : program processor using Ethernet.
{0} --start=<addr> <serial device> : start the device at <addr>.
{0} --read=<file> --addr=<address> --len=<length> <serial device>:
            read length bytes from address and dump them to a file.
{0} --serialnumber <serial device> : get the device serial number
{0} --list : list supported processors.
options:
    --cpu=<cpu> : set the cpu type.
    --oscfreq=<freq> : set the oscillator frequency.
    --baud=<baud> : set the baud rate.
    --xonxoff : enable xonxoff flow control.
    --control : use RTS and DTR to control reset and int0.
    --addr=<image start address> : set the base address for the image.
    --verbose : enable debug message output.
    --verify : read the device after programming.
    --verifyonly : don't program, just verify.
    --eraseonly : don't program, just erase. Implies --eraseall.
    --eraseall : erase all flash not just the area written to.
    --blankcheck : don't program, just check that the flash is blank.
    --filetype=[ihex|bin] : set filetype to intel hex format or raw binary.
    --bank=[0|1] : select bank for devices with flash banks.
    --port=<udp port> : UDP port number to use (default 41825).
    --mac=<mac address> : MAC address to associate IP address with.\
""".format(os.path.basename(sys.argv[0])))


def enter_isp(serial):
    serial.dtr = 1
    serial.rts = 1
    time.sleep(COM_PORT_DELAY)
    serial.reset_input_buffer()
    serial.reset_output_buffer()
    serial.dtr = 0
    time.sleep(COM_PORT_DELAY)
    serial.rts = 0

class SerialDevice(object):
    def __init__(self, device, baud, xonxoff=False, control=False):
        # Create the Serial object without port to avoid automatic opening
        self._serial = serial.Serial(port=None, baudrate=baud)

        # Disable RTS and DRT to avoid automatic reset to ISP mode (use --control for explicit reset)
        self._serial.rts = 0
        self._serial.dtr = 0

        # Select and open the port after RTS and DTR are set to zero
        self._serial.port = device

        try:
            self._serial.open()
        except serial.serialutil.SerialException as err:
            logging.error(err)
            sys.exit(-1)  # Exit early

        # set a two second timeout just in case there is nothing connected
        # or the device is in the wrong mode.
        # This timeout is too short for slow baud rates but who wants to
        # use them?
        self._serial.timeout = COM_PORT_TIMEOUT
        # device wants Xon Xoff flow control
        if xonxoff:
            self._serial.xonxoff = 1

        # reset pin is controlled by DTR implying int0 is controlled by RTS
        self.reset_pin = "dtr"

        if control:
            self.isp_mode()

        self._serial.flushInput()

    # put the chip in isp mode by resetting it using RTS and DTR signals
    # this is of course only possible if the signals are connected in
    # this way
    def isp_mode(self):
        enter_isp(self._serial)

    def reset(self, level):
        if self.reset_pin == "rts":
            self._serial.rts = level
        else:
            self._serial.dtr = level

    def int0(self, level):
        # if reset pin is rts int0 pin is dtr
        if self.reset_pin == "rts":
            self._serial.dtr = level
        else:
            self._serial.rts = level

    def write(self, data):
        self._serial.write(data)

    def readline(self, timeout=None):
        if timeout:
            ot = self._serial.timeout
            self._serial.timeout = timeout

        line = b''
        while True:
            c = self._serial.read(1)
            if not c:
                break
            if c == b'\r':
                if not line:
                    continue
                else:
                    break
            if c == b'\n':
                if not line:
                    continue
                else:
                    break
            line += c

        if timeout:
            self._serial.timeout = ot

        return line.decode("UTF-8", "ignore")

    def close(self):
        if self._serial.is_open:
          self._serial.flush()
          self._serial.reset_input_buffer()
          self._serial.reset_output_buffer()
          # This is a workaround for a problem with serial terminals after the
          # usage of pySerial where the vmin (or min) serial flag is set to 0.
          # This causes chrome.serial and other serial port readers to throw an
          # exception and lose control over the serial device.
          self._serial.inter_byte_timeout = 1
          self._serial._reconfigure_port()
          self._serial.close()


class UdpDevice(object):
    def __init__(self, address):
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._sock.timeout = 5
        self._inet_addr = address[0]
        self._udp_port = address[1]
        self._eth_addr = address[2]

        if self._eth_addr:
            import subprocess
            # Try add host to ARP table
            obj = subprocess.Popen(['arp', '-s', self._inet_addr, self._eth_addr],
                                   stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                                   shell=True)
            res = obj.communicate()
            stdout_text = res[0].decode('ascii', 'ignore') if res[0] else ""
            stderr_text = res[1].decode('ascii', 'ignore') if res[1] else ""
            if obj.returncode or stderr_text:
                panic("Failed to register IP address " +
                      "(Administrative privileges may be required)\r\n" +
                      stderr_text.replace('\r', '').replace('\n', ''))

        self._sock.bind(('', self._udp_port))

    def write(self, data):
        self._sock.sendto(data, (self._inet_addr, self._udp_port))

    def readline(self, timeout=None):
        if timeout:
            ot = self._sock.timeout
            self._sock.timeout = timeout

        try:
            line, addr = self._sock.recvfrom(1024)
        except Exception as e:
            line = b""

        if timeout:
            self._sock.timeout = ot

        return line.decode("UTF-8", "ignore").replace('\r','').replace('\n','')


class NxpProg:
    def __init__(self, cpu, device, baud, osc_freq, xonxoff=False, control=False, address=None, verify=False):
        self.echo_on = True
        self.verify = verify
        self.OK = 'OK'
        self.RESEND = 'RESEND'
        self.sync_str = 'Synchronized'

        # for calculations in 32 bit modulo arithmetic
        self.U32_MOD = (2 ** 32)

        # uuencoded line length
        self.uu_line_size = 45
        # uuencoded block length
        self.uu_block_size = self.uu_line_size * 20

        if address:
            self.device = UdpDevice(address)
        else:
            self.device = SerialDevice(device, baud, xonxoff, control)
            self.device.isp_mode()

        self.cpu = cpu

        self.connection_init(osc_freq)

        self.banks = self.get_cpu_parm("flash_bank_addr", 0)

        if self.banks == 0:
            self.sector_commands_need_bank = False
        else:
            self.sector_commands_need_bank = True

    def connection_init(self, osc_freq):
        self.sync(osc_freq)

        if self.cpu == "autodetect":
            devid = self.get_devid()
            for dcpu in cpu_parms.keys():
                cpu_devid = cpu_parms[dcpu].get("devid")
                if not cpu_devid:
                    continue

                # mask devid word1
                devid_word1_mask = cpu_parms[dcpu].get("devid_word1_mask")
                if devid_word1_mask and isinstance(devid, tuple) and devid[0] == cpu_devid[0] and (devid[1] & devid_word1_mask) == (cpu_devid[1] & devid_word1_mask):
                    logging.debug("Detected %s" % dcpu)
                    self.cpu = dcpu
                    break

                if devid == cpu_devid:
                    logging.debug("Detected %s" % dcpu)
                    self.cpu = dcpu
                    break
            if self.cpu == "autodetect":
                panic("Cannot autodetect from device id %d(0x%x), set cpu name manually" %
                        (devid, devid))

        # unlock write commands
        self.isp_command("U 23130")

    def dev_write(self, data):
        self.device.write(data)

    def dev_writeln(self, data):
        data = data.encode('UTF-8') + b'\r\n'
        # print('> ' + data)
        self.device.write(data)

    def dev_readline(self, timeout=None):
        data = self.device.readline(timeout)
        # print('< ' + data)
        return data

    def errexit(self, str, status):
        if not status:
            panic("%s: timeout" % str)
        err = int(status)
        if err != 0:
            error_desc = [
                'CMD_SUCCESS: Command is executed successfully',
                'INVALID_COMMAND: Invalid command',
                'SRC_ADDR_ERROR: Source address is not on word boundary',
                'DST_ADDR_ERROR: Destination address is not on word or 256 byte boundary',
                'SRC_ADDR_NOT_MAPPED:  Source address is not mapped in the memory map',
                'DST_ADDR_NOT_MAPPED: Destination address is not mapped in the memory map',
                'COUNT_ERROR: Byte count is not multiple of 4 or is not a permitted value',
                'INVALID_SECTOR: Sector number is invalid or end sector number is greater than start sector number',
                'SECTOR_NOT_BLANK: Sector is not blank',
                'SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION: Command to prepare sector for write operation was not executed',
                'COMPARE_ERROR: Source and destination data not equal',
                'BUSY: Flash programming hardware interface is busy',
                'PARAM_ERROR: Insufficient number of parameters or invalid parameter',
                'ADDR_ERROR: Address not on word boundary',
                'ADDR_NOT_MAPPED: Address is not mapped in the memory map',
                'CMD_LOCKED: Command is locked',
                'INVALID_CODE: Unlock code is invalid',
                'INVALID_BAUD_RATE: Invalid baud rate setting',
                'INVALID_STOP_BIT: Invalid stop bit setting',
                'CODE_READ_PROTECTION_ENABLED: Code read protection enabled'
                ]
            str = str.replace('\r','').replace('\n','')
            errstr = error_desc[err] if err < len(error_desc) else ""
            panic("%s: %d - %s" % (str, err, errstr))

    def isp_command(self, cmd):
        retry = 3
        while retry > 0:
            retry -= 1
            self.dev_writeln(cmd)

            # throw away echo data
            if self.echo_on:
                echo = self.dev_readline()
                if self.verify and echo != cmd:
                    logging.debug('Invalid echo')

            status = self.dev_readline()
            if status:
                break
        self.errexit("'%s' error" % cmd, status)

        return status

    def sync(self, osc):
        self.dev_write(b'?')
        s = self.dev_readline()
        if not s:
            panic("Sync timeout")
        logging.debug("initial sync = %s" % s)
        if s != self.sync_str:
            panic("No sync string")

        self.dev_writeln(self.sync_str)
        s = self.dev_readline()

        # detect echo state
        if s == self.sync_str:
            self.echo_on = True
            s = self.dev_readline()
        elif s == self.OK:
            self.echo_on = False
        else:
            logging.debug("echo sync = %s" % s)
            panic("No sync string")

        if s != self.OK:
            panic("Not ok")

        # set the oscillator frequency
        self.dev_writeln('%d' % osc)
        if self.echo_on:
            s = self.dev_readline()
            if s != ('%d' % osc):
                panic('Invalid echo')

        s = self.dev_readline()
        if s != self.OK:
            if s == str(INVALID_COMMAND):
                pass
            else:
                self.errexit("'%d' osc not ok" % osc, s)
                panic("Osc not ok")

        # disable echo
        self.dev_writeln('A 0')
        if self.echo_on:
            s = self.dev_readline()
            if s != 'A 0':
                panic('Invalid echo')

        s = self.dev_readline()
        if s == str(CMD_SUCCESS):
            self.echo_on = False
        elif s == str(INVALID_COMMAND):
            pass
        else:
            self.errexit("'A 0' echo disable failed", s)
            panic("Echo disable failed")

    def sum(self, data):
        s = 0
        if isinstance(data, str):
            for ch in data:
                s += ord(ch)
        else:
            for i in data:
                s += i
        return s

    def write_ram_block(self, addr, data):
        data_len = len(data)

        for i in range(0, data_len, self.uu_line_size):
            c_line_size = data_len - i
            if c_line_size > self.uu_line_size:
                c_line_size = self.uu_line_size
            block = data[i:i+c_line_size]
            bstr = binascii.b2a_uu(block)
            self.dev_write(bstr)

        retry = 3
        while retry > 0:
            retry -= 1
            self.dev_writeln('%s' % self.sum(data))
            status = self.dev_readline()
            if status:
                break
        if not status:
            return "timeout"
        if status == self.RESEND:
            return "resend"
        if status == self.OK:
            return ""

        # unknown status result
        panic(status)

    def uudecode(self, line):
        # uu encoded data has an encoded length first
        linelen = ord(line[0]) - 32

        uu_linelen = (linelen + 3 - 1) // 3 * 4

        if uu_linelen + 1 != len(line):
            panic("Error in line length")

        # pure python implementation - if this was C we would
        # use bitshift operations here
        decoded = ""
        for i in range(1, len(line), 4):
            c = 0
            for j in line[i: i + 4]:
                ch = ord(j) - 32
                ch %= 64
                c = c * 64 + ch
            s = []
            for j in range(0, 3):
                s.append(c % 256)
                c = c // 256
            for j in reversed(s):
                decoded = decoded + chr(j)

        # only return real data
        return decoded[0:linelen]

    def read_block(self, addr, data_len, fd=None):
        self.isp_command("R %d %d" % ( addr, data_len ))

        expected_lines = (data_len + self.uu_line_size - 1) // self.uu_line_size

        data = ""
        for i in range(0, expected_lines, 20):
            lines = expected_lines - i
            if lines > 20:
                lines = 20
            cdata = ""
            for i in range(0, lines):
                line = self.dev_readline()

                decoded = self.uudecode(line)

                cdata += decoded

            s = self.dev_readline()

            if int(s) != self.sum(cdata):
                panic("Checksum mismatch on read got 0x%x expected 0x%x" % (int(s), self.sum(data)))
            else:
                self.dev_writeln(self.OK)

            if fd:
                fd.write(cdata)
            else:
                data += cdata

        if fd:
            return None
        else:
            return data

    def write_ram_data(self, addr, data):
        image_len = len(data)
        for i in range(0, image_len, self.uu_block_size):

            a_block_size = image_len - i
            if a_block_size > self.uu_block_size:
                a_block_size = self.uu_block_size

            self.isp_command("W %d %d" % ( addr, a_block_size ))

            retry = 3
            while retry > 0:
                retry -= 1
                err = self.write_ram_block(addr, data[i : i + a_block_size])
                if not err:
                    break
                elif err != "resend":
                    panic("Write error: %s" % err)
                else:
                    logging.debug("Resending")

            addr += a_block_size

    def find_flash_sector(self, addr):
        table = self.get_cpu_parm("flash_sector")
        flash_base_addr = self.get_cpu_parm("flash_bank_addr", 0)
        if flash_base_addr == 0:
            faddr = 0
        else:
            faddr = flash_base_addr[0] # fix to have a current flash bank
        for i in range(0, len(table)):
            n_faddr = faddr + table[i] * 1024
            if addr >= faddr and addr < n_faddr:
                return i
            faddr = n_faddr
        return -1

    def bytestr(self, ch, count):
        data = b''
        for i in range(0, count):
            data += bytes([ch])
        return data

    def insert_csum(self, orig_image):
        # make this a valid image by inserting a checksum in the correct place
        intvecs = struct.unpack("<8I", orig_image[0:32])

        # default vector is 5: 0x14, new cortex cpus use 7: 0x1c
        valid_image_csum_vec = self.get_cpu_parm("csum_vec", 5)
        # calculate the checksum over the interrupt vectors
        csum = 0
        intvecs_list = []
        for vec in range(0, len(intvecs)):
            intvecs_list.append(intvecs[vec])
            if valid_image_csum_vec == 5 or vec <= valid_image_csum_vec:
                csum = csum + intvecs[vec]
        # remove the value at the checksum location
        csum -= intvecs[valid_image_csum_vec]

        csum %= self.U32_MOD
        csum = self.U32_MOD - csum

        logging.debug("Inserting intvec checksum 0x%08x in image at offset %d" %
                (csum, valid_image_csum_vec))

        intvecs_list[valid_image_csum_vec] = csum

        image = b''
        for vecval in intvecs_list:
            image += struct.pack("<I", vecval)

        image += orig_image[32:]

        return image

    def prepare_flash_sectors(self, start_sector, end_sector):
        if self.sector_commands_need_bank:
            self.isp_command("P %d %d 0" % (start_sector, end_sector))
        else:
            self.isp_command("P %d %d" % (start_sector, end_sector))

    def erase_sectors(self, start_sector, end_sector, verify=False):
        self.prepare_flash_sectors(start_sector, end_sector)

        logging.info("Erasing flash sectors %d-%d" %
            (start_sector, end_sector))

        if self.sector_commands_need_bank:
            self.isp_command("E %d %d 0" % (start_sector, end_sector))
        else:
            self.isp_command("E %d %d" % (start_sector, end_sector))

        if verify:
            logging.info("Blank checking sectors %d-%d" %
                (start_sector, end_sector))
            self.blank_check_sectors(start_sector, end_sector)

    def blank_check_sectors(self, start_sector, end_sector):
        global panic
        old_panic = panic
        panic = log
        for i in range(start_sector, end_sector+1):
            if self.sector_commands_need_bank:
                cmd = ("I %d %d 0" % (i, i))
            else:
                cmd = ("I %d %d" % (i, i))
            result = self.isp_command(cmd)
            if result == str(CMD_SUCCESS):
                pass
            elif result == str(SECTOR_NOT_BLANK):
                self.dev_readline() # offset
                self.dev_readline() # content
            else:
                self.errexit("'%s' error" % cmd, status)
        panic = old_panic

    def erase_flash_range(self, start_addr, end_addr, verify=False):
        start_sector = self.find_flash_sector(start_addr)
        end_sector = self.find_flash_sector(end_addr)

        self.erase_sectors(start_sector, end_sector, verify)

    def get_cpu_parm(self, key, default=None):
        ccpu_parms = cpu_parms.get(self.cpu)
        if not ccpu_parms:
            panic("No parameters defined for cpu %s" % self.cpu)
        parm = ccpu_parms.get(key)
        if parm:
            return parm
        if default is not None:
            return default
        else:
            panic("No value for required cpu parameter %s" % key)

    def erase_all(self, verify=False):
        end_sector = self.get_cpu_parm("flash_sector_count",
            len(self.get_cpu_parm("flash_sector"))) - 1

        self.erase_sectors(0, end_sector, verify)

    def blank_check_all(self):
        end_sector = self.get_cpu_parm("flash_sector_count",
            len(self.get_cpu_parm("flash_sector"))) - 1

        self.blank_check_sectors(0, end_sector)

    def prog_image(self, image, flash_addr_base=0, erase_all=False, verify=False):
        global panic
        success = True

        # the base address of the ram block to be written to flash
        ram_addr = self.get_cpu_parm("flash_prog_buffer_base", flash_prog_buffer_base_default)
        # the size of the ram block to be written to flash
        # 256 | 512 | 1024 | 4096
        ram_block = self.get_cpu_parm("flash_prog_buffer_size", flash_prog_buffer_size_default)

        # if the image starts at the start of a flash bank then make it bootable
        # by inserting a checksum at the right place in the vector table
        if self.banks == 0:
            if flash_addr_base == 0:
                image = self.insert_csum(image)
        elif flash_addr_base in self.banks:
            image = self.insert_csum(image)

        image_len = len(image)
        # pad to a multiple of ram_block size with 0xff
        pad_count_rem = image_len % ram_block
        pad_count = 0
        if pad_count_rem != 0:
            pad_count = ram_block - pad_count_rem
            image += self.bytestr(0xff, pad_count)
            image_len += pad_count

        logging.debug("Padding with %d bytes" % pad_count)

        if erase_all:
            self.erase_all(verify)
        else:
            self.erase_flash_range(flash_addr_base, flash_addr_base + image_len - 1, verify)

        sys.stdout.write('\r\n')
        sys.stdout.flush()

        for image_index in range(0, image_len, ram_block):
            a_ram_block = image_len - image_index
            if a_ram_block > ram_block:
                a_ram_block = ram_block

            flash_addr_start = image_index + flash_addr_base
            flash_addr_end = flash_addr_start + a_ram_block - 1

            logging.debug("Writing %d bytes to 0x%x" %
                (a_ram_block, flash_addr_start))

            progress_bar(30, flash_addr_start, image_len)

            self.write_ram_data(ram_addr,
                    image[image_index: image_index + a_ram_block])

            s_flash_sector = self.find_flash_sector(flash_addr_start)

            e_flash_sector = self.find_flash_sector(flash_addr_end)

            self.prepare_flash_sectors(s_flash_sector, e_flash_sector)

            # copy ram to flash
            self.isp_command("C %d %d %d" %
                    (flash_addr_start, ram_addr, a_ram_block))

            # optionally compare ram and flash
            if verify:
                old_panic = panic
                panic = log
                result = self.isp_command("M %d %d %d" %
                                          (flash_addr_start, ram_addr, a_ram_block))
                panic = old_panic
                if result == str(CMD_SUCCESS):
                    pass
                elif result == str(COMPARE_ERROR):
                    self.dev_readline() # offset
                    success = False
                else:
                    self.errexit("'%s' '%s' error" % (cmd, status))

        progress_bar(30, image_len - 1, image_len)

        sys.stdout.write('\r\n\r\n')
        sys.stdout.flush()
        return success

    def verify_image(self, flash_addr_base, image):
        success = True

        image_length = len(image)
        start_addr = flash_addr_base
        end_addr = flash_addr_base + image_length

        start_sector = self.find_flash_sector(start_addr)
        end_sector = self.find_flash_sector(end_addr)

        table = self.get_cpu_parm("flash_sector")
        flash_base_addr = self.get_cpu_parm("flash_bank_addr", 0)
        if flash_base_addr == 0:
            faddr = 0
        else:
            faddr = flash_base_addr[0] # fix to have a current flash bank

        index = 0
        sector = start_sector
        while sector <= end_sector:
            start_of_sector = faddr + 1024 * sum(table[:sector])
            end_of_sector = faddr + 1024 * sum(table[:sector+1])

            start = start_addr if start_of_sector < start_addr else start_of_sector
            end = end_addr if end_of_sector > end_addr else end_of_sector
            length = 4 * ((end - start) // 4)

            logging.info("Verify sector %i: Reading %d bytes from 0x%x" %
                (sector, length, start))
            data = self.read_block(start, length)
            if isinstance(image[0], int):
                data = [ord(x) for x in data]

            if len(data) != length:
                panic("Verify failed! lengths differ")

            for (i, (x, y)) in enumerate(zip(data, image[index:index+(end-start)])):
                if x != y:
                    logging.error("Verify failed! content differ at location \
                        0x%x" % (faddr + i))
                    success = False
                    break

            index = index + length
            sector = sector + 1

        return success

    def start(self, addr=0):
        mode = self.get_cpu_parm("cpu_type", "arm")
        # start image at address 0
        if mode == "arm":
            m = "A"
        elif mode == "thumb":
            m = "T"
        else:
            panic("Invalid mode to start")

        self.isp_command("G %d %s" % (addr, m))

    def select_bank(self, bank):
        status = self.isp_command("S %d" % bank)

        if status == self.OK:
            return 1

        return 0

    def get_devid(self):
        self.isp_command("J")
        id1 = self.dev_readline()

        # FIXME find a way of doing this without a timeout
        id2 = self.dev_readline(.2)
        if id2:
            ret = (int(id1), int(id2))
        else:
            ret = int(id1)
        return ret

    def get_serial_number(self):
        self.isp_command("N")
        id1 = self.dev_readline()
        id2 = self.dev_readline(.2)
        id3 = self.dev_readline(.2)
        id4 = self.dev_readline(.2)
        return ' '.join([id1, id2, id3, id4])


def main(argv=None):
    global prog

    args = parser.parse_args()

    # defaults
    osc_freq = 16000 # kHz
    baud = 115200
    device = None
    cpu = "autodetect"
    filename = ""
    flash_addr_base = 0
    erase_all = False
    erase_only = False
    verbose = False
    verify = False
    verify_only = False
    blank_check = False
    xonxoff = False
    start = False
    control = False
    filetype = "autodetect"
    select_bank = False
    read = False
    readlen = 0
    get_serial_number = False
    udp = False
    port = -1
    mac = "" # "0C-1D-12-E0-1F-10"

    if args.list:
        logging.info("Supported cpus:")
        for val in sorted(cpu_parms.keys()):
            logging.info(" %s" % val)
        sys.exit(0)
    if args.verbose:
        logging.basicConfig(stream=sys.stdout, level=logging.DEBUG)
    else:
        logging.basicConfig(stream=sys.stdout, level=logging.INFO)
    if args.binary:
        filename = args.binary
    if args.device:
        device = args.device
    if args.cpu:
        cpu = args.cpu
    if args.xonxoff:
        xonxoff = True
    if args.osfreq:
        os_freq = args.osfreq
    if args.addr:
        addr = int(args.addr, 0)
    if args.baud:
        baud = args.baud
    if args.eraseall:
        erase_all = args.eraseall
    if args.eraseonly:
        erase_only = args.eraseonly
    if args.verify:
        verify = True
    if args.verifyonly:
        verify = True
        verify_only = True
    if args.blankcheck:
        verify = True
        blank_check = True
    if args.control:
        control = True
    if args.filetype:
        filetype = args.filetype
    if not (filetype == "bin" or filetype == "ihex" ):
        panic("Invalid filetype: %s" % filetype)
    if args.start:
        start = True
        if args.addr:
            startaddr = int(args.addr, 0)
        else:
            startaddr = 0
    if args.bank:
        select_bank = True
        bank = args.bank
    if args.read:
        read = True
        readfile = a
    if args.serialnumber:
        get_serial_number = True
    if args.len:
        readlen = args.len
    if args.udp:
        udp = True
    if args.port:
        port = int(args.port)
    if args.mac:
        mac = args.mac

    if cpu != "autodetect" and not cpu in cpu_parms:
        panic("Unsupported cpu %s" % cpu)

    port_finder = AutoLPCPortFinder()
    if not device:
        device = port_finder.find_lpc_port()
        if device == None:
            panic("Scanned serial ports, but could not find an LPC device. Are you sure it is connected to your computer?\n\n")

    if udp:
        if '.' in device:
            if ':' in device:
                device, port = tuple(device.split(':'))
                port = int(port)
                if port<0 or port>65535:
                    panic("Bad port number: %d" % port)
            parts = [int(x) for x in device.split('.')]
            if len(parts)!=4 or min(parts)<0 or max(parts)>255:
                panic("Bad IPv4-address: %s" % device)
            device = '.'.join([str(x) for x in parts])
        elif ':' in device:
            # panic("Bad IPv6-address: %s" % device)
            pass
        else:
            panic("Bad IP-address: %s" % device)
        if port < 0:
            port = 41825
        if mac:
            parts = [int(x, 16) for x in mac.split('-')]
            if len(parts)!=6 or min(parts)<0 or max(parts)>255:
                panic("Bad MAC-address: %s" % mac)
            mac = '-'.join(['%02x'%x for x in parts])
            logging.info("cpu=%s ip=%s:%d mac=%s" % (cpu, device, port, mac))
        else:
            logging.info("cpu=%s ip=%s:%d" % (cpu, device, port))
    else:
        logging.info("cpu=%s oscfreq=%d device=%s baud=%d" % (cpu, osc_freq, device, baud))

    prog = NxpProg(cpu, device, baud, osc_freq, xonxoff, control, (device, port, mac) if udp else None, verify)

    if erase_only:
        prog.erase_all(verify)
    elif blank_check:
        prog.blank_check_all()
    elif start:
        prog.start(startaddr)
    elif select_bank:
        prog.select_bank(bank)
    elif get_serial_number:
        sn = prog.get_serial_number()
        sys.stdout.write(sn)
    elif read:
        if not readlen:
            panic("Read length is 0")
        fd = open(readfile, "w")
        prog.read_block(flash_addr_base, readlen, fd)
        fd.close()
    else:
        if filetype == "autodetect":
            filetype = "ihex" if filename.endswith('hex') else "bin"

        if filetype == "ihex":
            ih = ihex.ihex(filename)
            (flash_addr_base, image) = ih.flatten()
        else:
            image = open(filename, "rb").read()

        if not verify_only:
            start = time.time()
            success = prog.prog_image(image, flash_addr_base, erase_all, verify)
            stop = time.time()
            elapsed = stop - start
            logging.info("Programmed %s in %.1f seconds" %
                ("successfully" if success else "with errors", elapsed))

        if verify:
            start = time.time()
            success = prog.verify_image(flash_addr_base, image)
            stop = time.time()
            elapsed = stop - start
            logging.info("Verified %s in %.1f seconds" %
                ("successfully" if success else "with errors", elapsed))

        if not verify_only:
            prog.start(flash_addr_base)

    prog.device.close()


def signal_handler(sig, frame):
    if prog:
        prog.device.close()
    logging.error('Ctrl+C Pressed bailing out!')
    sys.exit(0)


if __name__ == '__main__':
    signal.signal(signal.SIGINT, signal_handler)
    sys.exit(main())
