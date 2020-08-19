import random

json = '{"RTYPE" : "0", "n" : '
n = random.randint(1, 10)
json += ('"' + str(n) + '"' + " , ")
m = random.randint(1, 5 * n)
work = [[] for i in range(n)]
order = [[] for i in range(m)]
print(n)
json += '"workers" : ['
for i in range(n):
    x = random.uniform(52.231015, 52.381738)
    y = random.uniform(104.145614, 104.397613)
    a = random.randint(0, 10)
    b = random.randint(a + 1, 24)
    S = random.randint(5, 20) * 100
    S1 = random.randint(1, 5) * 100
    S2 = random.randint(0, 2) * 100
    work[i] = (x, y, a, b, S, S1, S2)
    json += "{"
    json += '"id" : ' + '"' + str(i + 1) + '"' + ", "
    json += '"x" : ' + '"' + str(x)[:8] + '"' + ", "
    json += '"y" : ' + '"' + str(y)[:8] + '"' + ", "
    json += '"start" : '+ '"' + str(a) + '"' + ", "
    json += '"end" : ' + '"' + str(b) + '"' + ", "
    json += '"price" : ' + '"' + str(S) + '"'
    json += "}"
    if (i != n - 1):
        json += ", "
    print(x, y)
    print(a, b)
    print(S, S1, S2)
json += '], '
json += '"m" : '
json += ('"' + str(m) + '"' + " , ")
json += '"orders" : ['
print(m)
for i in range(m):
    x = random.uniform(52.231015, 52.381738)
    y = random.uniform(104.145614, 104.397613)
    a = random.randint(0, 10)
    b = random.randint(a + 3, 24)
    z = random.randint(1, 2)
    json += "{"
    json += '"id" : ' + '"' + str(i + 1) + '"' + ", "
    json += '"x" : ' + '"' + str(x)[:8] + '"' + ", "
    json += '"y" : ' + '"' + str(y)[:8] + '"' + ", "
    json += '"start" : '+ '"' + str(a) + '"' + ", "
    json += '"end" : ' + '"' + str(b) + '"' + ", "
    json += '"duration" : ' + '"' + str(z) + '"'
    json += "}"
    if (i != m - 1):
        json += ", "    
    print(x, y)
    print(a, b)
    print(z)
    order[i] = (x, y, a, b, z)
json += "]"
json += "}"

print(json)
print()
print()
print()
print()

'''
maps = [['.'] * 50 for i in range(50)]
for i in range(n):
    maps[work[i][0]][work[i][1]] = chr(65 + i)
for i in range(m):
    maps[order[i][0]][order[i][1]] = str(i + 1)
for i in range(50):
    for elem in maps[i]:
        print(elem, end='')
    print()
'''
print()
print()
print(json)