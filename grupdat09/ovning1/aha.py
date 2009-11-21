# coding=utf8

import time

birth = int(raw_input("Vilket år är du född? "))
year = time.localtime().tm_year

print "Aha,", year - birth, "år i år!"
print "Bara", 65 - (year - birth), "år till pensionen..."
# notera skillnad på year - birth och (year - birth) i fallet.

