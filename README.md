# gillespie_propagation

Gillespie algorithm for the simulation of Markovian propagation processes on static complex networks using a composition and rejection scheme.


## Requirements

**On Unix (Linux, OS X)**

* A compiler with C++11 support
* CMake >= 3.1

**On Windows**

* Visual Studio 2015 (required for all Python versions)
* CMake >= 3.1


## Installation

First, clone this repository and the submodule (pybind11). 
```bash
git clone --recursive https://github.com/gstonge/gillespie_propagation.git
```
Second, use pip to install the module.
```bash
pip install ./gillespie_propagation
```
To install it in developer mode
```bash
pip install -e ./gillespie_propagation
```

## Examples

The following examples make use of the networkx module (https://github.com/networkx) as well as standard python scientific modules.

### Time evolution : SIS model

In this example, we consider the SIS model on an Erdős–Rényi random graph. A certain fraction of the nodes are infected initially at random. We draw the evolution of the prevalence as a function of time. 

```python
from gillespie_propagation import PropagationProcess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

#create a random graph using networkx
seed = 42
N = 10000
p = 10**(-3)
G = nx.fast_gnp_random_graph(N,p, seed = seed)

#propagation parameters
transmission_rate = 0.15         
recovery_rate = 1              #rate at which infected nodes become recovered
waning_immunity_rate = np.inf  #rate at which recovered nodes become susceptible
infected_fraction = 0.05       #Initial infected fraction - at random

#initialize the propagation process
pp = PropagationProcess(list(G.edges()), transmission_rate, recovery_rate, 
    waning_immunity_rate)
pp.initialize(infected_fraction, seed)

#simulate the process
t = 20
pp.evolve(t)

#get data vectors
Inode_number_vector = pp.get_Inode_number_vector()
time_vector = pp.get_time_vector()

#format 
prevalence_vector = [i/N for i in Inode_number_vector]

#draw the temporal sequence
plt.plot(time_vector, prevalence_vector)
plt.xlabel(r'Time')
plt.ylabel(r'Prevalence')
plt.show()
```


