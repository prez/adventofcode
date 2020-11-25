from sys import argv
from math import gcd
from itertools import combinations

def lcm(a, b, c):
	return a*b*c // gcd(a, b) // gcd(b, c)

def apply_steps(ms, n):	
	chk = lambda axis: [(m[0][axis], m[1][axis]) for m in ms]
	sign = lambda x: 1 if x > 0 else -1 if x < 0 else 0
	cycles, initial = [0, 0, 0], [chk(0), chk(1), chk(2)]
	step = 0
	while True:
		# velocity
		for m, mm in combinations(ms, 2):
			for axis in range(3):
				m[1][axis] += sign(mm[0][axis] - m[0][axis])
				mm[1][axis] -= sign(mm[0][axis] - m[0][axis])
		# position
		for m in ms:
			for axis in range(3):
				m[0][axis] += m[1][axis]
		# find cycles
		for axis in range(3):
			if not cycles[axis] and chk(axis) == initial[axis]:
				cycles[axis] = step + 1
		# energy for pt1
		if step == n-1:
			print("first:\t"+str(sum((sum(map(abs, m[0])) * sum(map(abs, m[1])) for m in ms))))
		# pt2
		if all(cycles):
			return cycles
		# next
		step += 1

if len(argv) != 2: print('invalid input file'), exit(1)
with open(argv[1]) as f:
	moons = [([int(x.split("=")[1]) for x in ln.strip(">").split(",")], [0, 0, 0]) for ln in f.read().strip().split('\n')]
# moons[0][1][2]: first moon / velocities / z coord
repeats = apply_steps(moons, 1000)
print("second:\t"+str(lcm(*repeats)))
