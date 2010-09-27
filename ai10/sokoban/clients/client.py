#!/usr/bin/python
import socket,sys

HOST = 'cvap103.nada.kth.se'
PORT = 5555
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
s.send(sys.argv[1])
data = s.recv(1024)
print data
s.send('U R R D U U L D L L U L L D R R R R L D D R U R U D L L U R');
#s.send('0 3 3 1 0 0 2 1 2 2 0 2 2 1 3 3 3 3 2 1 1 3 0 3 0 1 2 2 0 3')
data = s.recv(1024)
s.close()
print data
