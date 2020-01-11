from sys import argv
from math import atan2
from itertools import product

def walk_n(ordr, n):
	i, num = 0, 0
	while True:	
		if i and ordr[i-1][2] != ordr[i][2]:
			num += 1
		if num == n - 1:
			return ordr[i][0]*100 + ordr[i][1]
		i = (i + 1) % len(ordr)

def gen_order(as_table, station_pos):
	angl = lambda h, v: atan2(h-station_pos[0], v-station_pos[1])
	ndist = lambda h, v: -(abs(h-station_pos[0])+abs(v-station_pos[1]))
	station_table = ((x, y, angl(x, y)) for x, y in as_table.keys() if (x, y) != station_pos)
	return list(reversed(sorted(station_table, key=lambda x: (x[2], ndist(x[0], x[1])))))

def find_best(as_table):			
	max_counts, max_counts_key = 0, 0
	for x, y in asteroids.keys():
		crr_counts = 0
		for xi, yi in asteroids.keys():
			if (a := atan2(x-xi, y-yi)) not in asteroids[(x, y)]:
				asteroids[(x, y)] |= {a}
				crr_counts += 1
		if crr_counts >= max_counts:
			max_counts, max_counts_key = crr_counts, (x, y)
	return (max_counts_key, max_counts)

if len(argv) != 2: print('invalid input file'), exit(1)
with open(argv[1]) as f:
	lines = [[1 if x == '#' else 0 for x in ln] for ln in f.read().split()]
width, height = len(lines[0]), len(lines)
asteroids = {(x, y): set() for x, y in product(range(width), range(height)) if lines[y][x]}
best_k, best_c = find_best(asteroids)
print("first:", best_c)
print("second:", walk_n(gen_order(asteroids, best_k), 200))
