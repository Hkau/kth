def farenheit(c):
	return 32+1.8*c

def celsius(f):
	return (f-32)/1.8

f = farenheit(float(input('Skriv in grader i Celsius: ')))

print(f)

print(celsius(f))

