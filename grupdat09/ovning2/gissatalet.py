# Gissa talet av Henrik Eriksson 1984-01-19

min=1
max=99
print "Jag tänker på ett tal 1...99,",
while max>min:
    if max<min+3: print "sista gissningen,",
    print "gissa talet och vinn! "
    gissning=input()
    if gissning>(max+min)/2:
        print "För stor gissning,",
        max=gissning-1
    else:                           
	 print "För liten gissning,",
         min=gissning+1
print "det var tyvärr sista gissningen."                 ;
print "Om du gissat rätt hade du vunnit en rubinlaser!"
