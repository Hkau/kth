import hashlib
from sys import stdin

def sha256(msg):
	return hashlib.sha256(msg).digest()

for line in stdin.readlines():
	if line == '':
		break
	print sha256(line.strip().decode('hex_codec')).encode('hex_codec')
