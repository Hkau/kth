foo = float(raw_input("Ett decimaltal: "))

if (foo-int(foo)) >= 0.5:
	print int(foo) + 1
else:
	print int(foo)

