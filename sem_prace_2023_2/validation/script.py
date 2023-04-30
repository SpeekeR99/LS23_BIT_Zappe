import sys

filename = sys.argv[1]

with open(filename, mode="rb") as frb:
    data = frb.read()
print(''.join(format(byte, '02x') for byte in data))