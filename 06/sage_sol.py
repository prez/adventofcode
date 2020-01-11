def parseInput (f):
    orbits = dict()
    for line in f:
        orbiting, orbiter = line.strip().split(')')
        if orbiting not in orbits:
            orbits[orbiting] = set()
        orbits[orbiting].add(orbiter)
    return orbits

def assignDepths (orbits):
    orbiterdepths = dict()
    depths = dict()
    depth = 0

    masses = ['COM']
    while len(masses) > 0:
        newMasses = []
        depths[depth] = set()
        for mass in masses:
            orbiterdepths[mass] = depth
            depths[depth].add(mass)
            if mass in orbits:
                for orbiter in orbits[mass]:
                    newMasses.append(orbiter)
        depth += 1
        masses = newMasses
    return depths, orbiterdepths

def countSteps (depths):
    steps = 0
    for depth in depths:
        steps += depth * len(depths[depth])
    return steps

def findPathLength (orbiter1, orbiter2, orbits, depths, orbiterdepths):
    def findOrbiting (orbiter):
        orbiterdepth = orbiterdepths[orbiter]
        for orbiting in depths[orbiterdepth - 1]:
            if orbiting in orbits:
                for orbiter2 in orbits[orbiting]:
                    if orbiter2 == orbiter:
                        return orbiting
    #Move to parents
    orbiter1 = findOrbiting(orbiter1)
    orbiter2 = findOrbiting(orbiter2)
    
    transfers = 0
    while orbiter1 != orbiter2:
        depth1 = orbiterdepths[orbiter1]
        depth2 = orbiterdepths[orbiter2]
        if depth1 > depth2:
            transfers += 1
            orbiter1 = findOrbiting(orbiter1)
        elif depth1 < depth2:
            transfers += 1
            orbiter2 = findOrbiting(orbiter2)
        elif depth1 == depth2:
            transfers += 2
            orbiter1 = findOrbiting(orbiter1)
            orbiter2 = findOrbiting(orbiter2)
    return transfers

def solution ():
    with open("bigboy") as f:
        orbits = parseInput(f)
    depths, orbiterdepths = assignDepths(orbits)
    print("Part 1:", countSteps(depths))
    print("Part 2:", findPathLength("YOU", "SAN", orbits, depths, orbiterdepths))

solution()
