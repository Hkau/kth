import struct
from sys import stdin

# right-rotate function, (32-bit)
def rotr(x, n):
	return ((x >> n) | (x << (32-n))) & 0xFFFFFFFF

def sha256(msg):
	# Pre-processing:
	# append the bit '1' to the message
	# append k bits '0', where k is the minimum number >= 0 such that the resulting message
	#     length (in bits) is modulo 512, minus 64 bits for the length.
	# append length of message (before pre-processing), in bits, as 64-bit big-endian integer

	#preprocessing message, appending a 1-bit, padding with zeroes to even block length
	length = len(msg)
	msg += "80".decode('hex_codec') # append 1-bit
	blocks = (length + 1 + 8 + (64-1)) // 64 # block padding and storing length
	real_length = blocks * 64
	pad = real_length - length - 1 - 8
	msg += pad * "00".decode('hex_codec')
	msg += ("%016X" % (length*8)).decode('hex_codec') # length IN BITS

	# initial hash values
	hval = [0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19]

	# round constants, k
	k = [0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	     0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	     0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	     0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	     0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	     0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	     0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2]
	
	# break padded message into chunks.
	chunks = []
	for i in range(0, len(msg), 64):
		chunks.append(msg[i:i+64])

	for chunk in chunks:
		# Prepare message schedule
		w = []
		# 15 first ones are simply the chunk's words themselves
		for i in range(0, len(chunk), 4):
			num = struct.unpack(">I", chunk[i:i+4])
			w.append(num[0])

		# extend the 16 words to 64 for the message schedule
		for i in range(16,64):
			tmp = w[i-2]
			s1 = rotr(tmp, 17) ^ rotr(tmp, 19) ^ (tmp >> 10)
			tmp = w[i-15]
			s0 = rotr(tmp, 7) ^ rotr(tmp, 18) ^ (tmp >> 3)
			w.append((s1 + w[i-7] + s0 + w[i-16]) & 0xFFFFFFFF)

		# init a-h for this chunk (last hash values)
		a = hval[0]
		b = hval[1]
		c = hval[2]
		d = hval[3]
		e = hval[4]
		f = hval[5]
		g = hval[6]
		h = hval[7]
		for i in range(64):
			# generate T1, T2
			s1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)
			ch = (e & f) ^ ((0xFFFFFFFF^e) & g)
			t1 = (h + s1 + ch + k[i] + w[i]) & 0xFFFFFFFF
			s0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)
			maj = (a & b) ^ (a & c) ^ (b & c)
			t2 = (s0 + maj) & 0xFFFFFFFF

			h = g
			g = f
			f = e
			e = (d + t1) & 0xFFFFFFFF
			d = c
			c = b
			b = a
			a = (t1 + t2) & 0xFFFFFFFF

			# print "%d: "% i, "a = %08x" % a, "b = %08x" % b, "c = %08x" %c, "d = %08x" %d, "e = %08x" % e, "f = %08x" % f, "g = %08x" % g, "h = %08x" % h

		# update hash value after this chunk's processing, mod 2^32
		hval[0] += a
		hval[1] += b
		hval[2] += c
		hval[3] += d
		hval[4] += e
		hval[5] += f
		hval[6] += g
		hval[7] += h
		for j in range(len(hval)):
			hval[j] = hval[j] & 0xFFFFFFFF

	# Concatenate hvals for final hash value
	return "".join([struct.pack(">I", h) for h in hval])

for line in stdin:
	print sha256(line.strip().decode('hex_codec')).encode('hex_codec')
