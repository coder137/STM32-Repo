#!/usr/bin/python
import sys
import string

class data_rec:
    def __init__(self, addr, data):
        if type(data) != type(b""):
            raise Exception("data is not a binary string")
        self.addr = addr
        self.data = data

    def __repr__(self):
        return "%08x: %x" % (self.addr, len(self.data))

class ihex:
    TYPE_DATA = 0
    TYPE_EOF = 1
    TYPE_EXTENDED_SEGMENT_ADDR = 2
    TYPE_START_SEGMENT_ADDR = 3
    TYPE_EXTENDED_LINEAR_ADDR = 4
    TYPE_START_LINEAR_ADDR = 5

    def __init__(self, filename):
        fd = open(filename, "r")

        self.data = []

        self.start_addr = None

        base_addr = 0

        line_no = 0
        for line in fd:
            l = line.strip()
            line_no += 1
            if l[0] != ":":
                raise Exception("invalid initial character '%s' line %d",
                        (l[0], line_no))
            if len(l[1:]) % 2 != 0:
                raise Exception("line length not a multiple of two: line %d" %
                            line_no)

            (l, addr, type, data) = self.line_parse(l[1:], line_no)

            if l != len(data):
                print("data length does not match length field %d" % line_no)

            if type == self.TYPE_EOF:
                return

            if type == self.TYPE_EXTENDED_SEGMENT_ADDR:
                base_addr = 0x10 * self.multi_val(data)

            if type == self.TYPE_EXTENDED_LINEAR_ADDR:
                base_addr = 0x10000 * self.multi_val(data)

            if type == self.TYPE_START_SEGMENT_ADDR or \
                type == self.TYPE_START_LINEAR_ADDR:
                if self.start_addr:
                    print("start address set twice: line %d" % line_no)
                self.start_addr = self.multi_val(data)

            if type == self.TYPE_DATA:
                self.data.append(data_rec(addr + base_addr,
                        self.intlist_tostr(data)))

    def line_parse(self, hexstring, line_no):
        list = []
        cksum = 0
        for i in range(0, len(hexstring), 2):
            val = int(hexstring[i:i+2], 16)
            list.append(val)
            cksum += val

        cksum %= 0x100
        if cksum != 0:
            raise Exception("invalid checksum: line %d" % line_no)

        return (list[0], self.multi_val(list[1:3]), list[3], list[4:-1])

    def intlist_tostr(self, list):
        data = bytes(list)
        if isinstance(data, str):
            data = b''.join([chr(x) for x in list])
        return data

    def multi_val(self, data):
        sum = 0
        for val in data:
            sum = sum * 0x100 + val
        return sum

    def dump(self):
        for d in self.data:
            print(d)

    def padding(self, fill, len):
        intlist = [fill for _ in range(len)]
        return self.intlist_tostr(intlist)

    def flatten(self, fill = 0xff):
        sort_list = []
        for d in self.data:
            sort_list.append((d.addr, d.data))
        sort_list.sort(key=lambda x: x[0])
        data = b''
        last_addr = sort_list[0][0]
        start_addr = last_addr
        for e in sort_list:
            addr = e[0]
            l = len(e[1])
            pad = addr - last_addr
            if pad < 0:
                raise Exception("overlapping sections in file")
            if pad > 0:
                data += self.padding(fill, pad)
                l += pad
            data += e[1]
            last_addr += l
        return (start_addr, data)
