from sys import argv
from math import ceil

dc = {k.strip().split(' ')[1]: (int(k.strip().split(' ')[0]), {x.strip(' ').split(' ')[1]: int(x.strip(' ').split(' ')[0]) for x in v.split(',')}) for v, k in (line.strip().split("=>") for line in open(argv[1]))}
ds = {k: 0 for k in dc.keys()}

def walk(crr, amount):
	if crr == 'ORE':
		yield amount
	else:
		crrmult = dc[crr][0]
		repeat_times = ceil((amount - ds[crr]) / crrmult)
		ds[crr] += repeat_times * crrmult - amount
		for req, reqmult in dc[crr][1].items():
			yield from walk(req, reqmult * repeat_times) 

def binsearch(n, start, end):
	m = (start + end) // 2
	if ore_eval(m + 1) > n >= ore_eval(m):
		return m
	elif ore_eval(m) < n:
		return binsearch(n, m + 1, end)
	else:
		return binsearch(n, start, m - 1)

ore_eval = lambda f: sum(walk('FUEL', f))
print("first:\t"+str(ore_eval(1)))
print("second:\t"+str(binsearch(1000000000000, 0, 1000000000000)))
