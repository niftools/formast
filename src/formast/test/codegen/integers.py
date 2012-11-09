# generated from integers.xml

import struct

def read_int(stream):
    return struct.unpack('<i', stream.read(4))[0]

class IntegerData:

    def __init__(self):
        self.version = 0
        self.has_integers = 0
        self.has_integers_2 = 0
        self.num_integers = 0
        self.integers = []
        self.num_integers_2 = 0
        self.integers_2 = []

    def read(self, stream):
        self.version = read_int(stream)
        self.has_integers = read_int(stream)
        if (self.version >= 2):
            self.has_integers_2 = read_int(stream)
        if self.has_integers:
            self.num_integers = read_int(stream)
        if self.has_integers:
            self.integers = [
                read_int(stream)
                for i in range(self.num_integers)]
        if (self.has_integers_2 and (self.version >= 2)):
            self.num_integers_2 = read_int(stream)
        if (self.has_integers_2 and (self.version >= 2)):
            self.integers_2 = [
                read_int(stream)
                for i in range(self.num_integers_2)]
