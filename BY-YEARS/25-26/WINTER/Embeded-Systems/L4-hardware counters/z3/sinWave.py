import math

vals = []
for i in range(256):
    kat = 2 * math.pi * i / 256.0
    val = int(round((1 - math.cos(kat)) * 127.5))  # 0..255
    vals.append(val)

for i in range(0, 256, 8):
    row = ", ".join(str(v) for v in vals[i:i+8])
    print(row + ",")
