from sys import argv
from itertools import cycle, islice
from functools import reduce

def transform(ls, repeat):
	pattern = lambda n: reduce(lambda s1, s2: s1+s2, (n*[x] for x in repeat))
	zipl = lambda d: zip(ls, islice(cycle(pattern(d)), 1, None))
	tfdigit = lambda d: abs(sum(map(lambda x: x[0]*x[1], zipl(d)))) % 10
	return [tfdigit(i) for i in range(1, len(ls)+1)]

def repn(n, arr):
	for _ in range(n):
		arr = transform(arr, [0, 1, 0, -1])
		print("just 10 billion years to go")
	return arr

if len(argv) != 2: print('invalid input file'), exit(1)
with open(argv[1]) as f:
	xs = [int(x) for x in f.read().strip()]

tostr = lambda arr: ''.join(map(str, arr))
print("first:\t"+tostr(repn(100, xs))[:8])
print("second:\t"+tostr(repn(100, 10000*xs))[int(tostr(xs)[:7]):8])
