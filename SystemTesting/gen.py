s = ""
for i in range(1,100):
	s += "s" + str(i) + ","

print(s)

s = ""
for i in range(1,100):
	s +=  "Next*(s" + str(i) + ",s" + str(i+1) + ") and "

print(s)
