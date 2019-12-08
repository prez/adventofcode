with open("input3") as f:
	g = {c: p for p, c in (ln.split(")") for ln in f.read().split())}

def walkback(x):
    while x in g:
        x = g[x]
        yield x

print(f"first:\t{sum(len(list(walkback(x))) for x in g)}")
print(f"second:\t{len({*walkback('SAN')} ^ {*walkback('YOU')})}")
