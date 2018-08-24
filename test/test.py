import sys
import time
import numpy as np
import networkx as nx
from spreading_CR import SpreadingProcess
import argparse

def main(arguments):
    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('-s', '--seed', help="Seed for the RNG", type=int,
        default=42)
    args = parser.parse_args(arguments)

    #create a random graph using networkx
    seed = 42
    N = 10000
    p = 10**(-3)
    G = nx.fast_gnp_random_graph(N,p, args.seed)

    #propagation parameters
    transmission_rate = 0.15
    recovery_rate = 1              #rate at which infected nodes become recovered
    waning_immunity_rate = 0  #rate at which recovered nodes become
    infected_fraction = 0.05       #Initial infected fraction - at random
    sample_size = 1000

    #initialize the propagation process
    sp = SpreadingProcess(list(G.edges()), transmission_rate, recovery_rate,
                      waning_immunity_rate)

    start = time.time()
    for i in range(sample_size):
        #simulate until an absorbing state is reached
        sp.initialize(infected_fraction, i)
        sp.evolve(np.inf)
        sp.reset()
    end = time.time()

    print((end-start)/sample_size)

if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))


