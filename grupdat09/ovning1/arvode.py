# coding=utf8
print 'Programmet förväntar sig tid i HH:MM-format, t.ex. 04:02' 
starttime = raw_input("Började jobba: ")

startminutes = int(starttime[0:2])*60 + int(starttime[3:5])

endtime = raw_input("Slutade jobba: ")
endminutes = int(endtime[0:2])*60 + int(endtime[3:5])

print "Arvodet blir", int(round((endminutes-startminutes) * 97.0 / 60)), "kr."

