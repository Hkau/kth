# 63. Skriv en funktion krock(u,v) som kollar om något element
#     finns i båda listorna och i så fall returnerar True

def krock(u,v):
	for i in u:
		for j in v:
			if i == j:
				return True
	return False

print(krock([],[]))

print(krock([2],[2]))

print(krock([3,1,2],[2,1,2,2]))

