# spreading_CR

Gillespie algorithm for the simulation of Markovian spreading processes on static complex networks using a composition and rejection scheme. The core is implemented in C++ for efficiency. A user-friendly python interface is provided, thanks to pybind11 (https://github.com/pybind/pybind11).

## Requirements

**On Unix (Linux, OS X)**

* A compiler with C++11 support
* CMake >= 3.1

**On Windows**

* Visual Studio 2015 (required for all Python versions)
* CMake >= 3.1

## Installation

First, clone this repository and the submodule (pybind11). The --recursive flag is needed to import correctly the pybind11 subdirectory.
```bash
git clone --recursive https://github.com/gstonge/spreading_CR.git
```
Second, use pip to install the module.
```bash
pip install ./spreading_CR
```
or
```bash
pip install -e ./spreading_CR
```
for developer mode.

## Important note

We use the convention that nodes are labeled from 0 to N-1, where N is the
number of nodes. Also, make sure node N-1 is of degree > 0, otherwise the code
won't be able to infer correctly the network size when the edge list is passed.


## Examples

The following examples make use of the [networkx module](https://github.com/networkx) as well as standard python scientific modules.

### Time evolution : SIS model

In this example, we consider the SIS model on an Erdős–Rényi random graph.
A certain fraction of the nodes are infected initially at random.
We draw the evolution of the prevalence as a function of time.

```python
from spreading_CR import SpreadingProcess
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
sp = SpreadingProcess(list(G.edges()), transmission_rate, recovery_rate,
    waning_immunity_rate)
sp.initialize_random(infected_fraction, seed)

#simulate the process
t = 20
sp.evolve(t)

#get data vectors
Inode_number_vector = sp.get_Inode_number_vector()
time_vector = sp.get_time_vector()

#format
prevalence_vector = [i/N for i in Inode_number_vector]

#draw the temporal sequence
plt.plot(time_vector, prevalence_vector)
plt.xlabel(r'Time')
plt.ylabel(r'Prevalence')
plt.show()
```

### Phase transition : SIR model

In this example, we consider the SIR model on the Watts-Strogatz random graph.
A certain specified list of nodes are initially infected. We sample the final size--the fraction of recovered nodes at the end--for different transmission rates.
We draw the final size as a function of the transmission rate to discern the phase transition.

```python
from spreading_CR import SpreadingProcess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

#create a random graph using networkx
seed = 42
N = 1000
k = 10
p = 0.05
G = nx.watts_strogatz_graph(N, k, p, seed=seed)

#propagation parameters
transmission_rate_list = np.linspace(0.0,0.5,10)
recovery_rate = 1           #rate at which infected nodes become recovered
waning_immunity_rate = 0    #rate at which recovered nodes become susceptible
Inode_list = np.arange(0,5) #Initially infected nodes

#prepare the sampling of the final size
sample_size = 1000
mean_final_size_list = []
std_final_size_list = []

for transmission_rate in transmission_rate_list:
    #initialize the propagation process
    sp = SpreadingProcess(list(G.edges()), transmission_rate,
                            recovery_rate, waning_immunity_rate)

    #get a sample of final size
    final_size_list = []
    for i in range(sample_size):
        #simulate until an absorbing state is reached
        sp.initialize(Inode_list, i)
        sp.evolve(np.inf)

        #get data and reset
        final_size_list.append(sp.get_Rnode_number_vector()[-1]/N)
        sp.reset()

    #get the statistics
    mean_final_size_list.append(np.mean(final_size_list))
    std_final_size_list.append(np.std(final_size_list))

#get the approximate std on the mean
std_mean_final_size_list = [std/np.sqrt(sample_size)
                            for std in std_final_size_list]

#draw phase transition
plt.errorbar(transmission_rate_list, mean_final_size_list,
             yerr = std_mean_final_size_list, fmt = 'o-')
plt.xlabel(r'Transmission rate')
plt.ylabel(r'Mean final size')
plt.show()
```

### Get recovered nodes set : SIR model

In this example, we consider the SIR model on the Watts-Strogatz random graph.
A certain specified list of nodes are initially infected.
We illustrate the set of recovered nodes at the end of the process.
```python
from spreading_CR import SpreadingProcess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

#create a random graph using networkx
seed = 42
N = 50
k = 10
p = 0.05
G = nx.watts_strogatz_graph(N, k, p, seed=seed)

#propagation parameters
transmission_rate = 0.2
recovery_rate = 1
waning_immunity_rate = 0
Inode_list = np.arange(0,2) #Initially infected nodes

#initialize spreading process and evolve until an frozen state is reached
sp = SpreadingProcess(list(G.edges()),transmission_rate,
                            recovery_rate,waning_immunity_rate)
sp.initialize(Inode_list, seed)
sp.evolve(np.inf)

#get the recovered nodes
recovered_node_set = sp.get_Rnode_set()

#show the recovered nodes in orange
node_color = ["#EDEDED" if n not in recovered_node_set else "#f19143" for n in G]
pos = nx.spectral_layout(G)
pos = nx.spring_layout(G,pos=pos)
nx.draw_networkx_nodes(G,pos=pos,node_color=node_color,edgecolors='k')
nx.draw_networkx_edges(G,pos=pos)
```

### Initialize the network with recovered (immunized) nodes

In this example, we consider the SIR model on a chain. Node number 5 is immunized
and node 0 is initially infected. We illustrate the set of recovered nodes at the end of the process.

***Note :*** this immunization method is more efficient for sampling than directly changing
the structure (removing edges connected to an immunized node). Indeed, one can
keep the same object `SpreadingProcess` all along.
```python
from spreading_CR import SpreadingProcess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

#create a chain graph using networkx
seed = 42
G = nx.Graph()
G.add_edges_from([(i,i+1) for i in range(10)])

#propagation parameters
transmission_rate = 1000.
recovery_rate = 1
waning_immunity_rate = 0
Inode_list = np.arange(0,1) #Initially infected nodes
Rnode_list = [5]

#initialize spreading process and evolve until an frozen state is reached
sp = SpreadingProcess(list(G.edges()),transmission_rate,
                            recovery_rate,waning_immunity_rate)
sp.initialize(Inode_list, Rnode_list, seed)
sp.evolve(np.inf)

#get the recovered nodes
recovered_node_set = sp.get_Rnode_set()

#show the recovered nodes in orange
node_color = ["#EDEDED" if n not in recovered_node_set else "#f19143" for n in G]
pos = nx.spectral_layout(G)
pos = nx.spring_layout(G,pos=pos)
nx.draw_networkx_nodes(G,pos=pos,node_color=node_color,edgecolors='k')
nx.draw_networkx_edges(G,pos=pos)
```

### Estimation of the basic reproduction number R0 or an effective one Reff

The basic reproduction number gives the average number of secondary cases in a network with a fully susceptible population.
To estimate it, we infect a randomly chosen single node.
If it successfully infects a second node, we track the number of new infection cases this second node causes.
We repeat this process multiple times to estimate the average and standard deviation associated with the basic reproduction number.

We can also specify a list of recovered nodes to obtain an effective
reproduction number.

***Note :*** although evident, one must use a transmission rate greater than 0.
```python
from spreading_CR import SpreadingProcess
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt

#create a random graph using networkx
seed = 42
N = 1000
k = 10
p = 0.05
G = nx.watts_strogatz_graph(N, k, p, seed=seed)

#propagation parameters
transmission_rate_list = np.linspace(0.1,0.4,20) #important, transmission rate > 0
recovery_rate = 1           #rate at which infected nodes become recovered
waning_immunity_rate = 0    #rate at which recovered nodes become susceptible

#prepare the list for the R0 estimates
sample = 10000 #number of sample for R0
R0_mean_list = []
R0_std_list = []

#do the same for Reff where some nodes are recovered at the beginning
Reff_mean_list = []
Reff_std_list = []
nb_recovered = 200
Rnode_list = np.random.randint(0,N,nb_recovered)

i = 0
for transmission_rate in transmission_rate_list:
    #initialize the propagation process
    sp = SpreadingProcess(list(G.edges()), transmission_rate,
                            recovery_rate, waning_immunity_rate)
    #R0 estimate
    R0_mean,R0_std = sp.estimate_R0(sample,seed+i)
    R0_mean_list.append(R0_mean)
    R0_std_list.append(R0_std/np.sqrt(sample)) #standard error
    #Reff estimate
    Reff_mean,Reff_std = sp.estimate_R0(sample,seed+i,Rnode_list)
    Reff_mean_list.append(Reff_mean)
    Reff_std_list.append(Reff_std/np.sqrt(sample)) #standard error
    i += 1

#draw R0 and Reff for different parameters
plt.errorbar(transmission_rate_list, R0_mean_list,
             yerr = R0_std_list, fmt = 'o-', label='$R_0$')
plt.errorbar(transmission_rate_list, Reff_mean_list,
             yerr = Reff_std_list, fmt = 'o-',  label='$R_\mathrm{eff}$')
plt.plot([min(transmission_rate_list),max(transmission_rate_list)],
        [1,1], '--', color='grey')
plt.xlabel(r'Transmission rate')
plt.ylabel(r'$R_0$ estimate')
plt.legend()
plt.show()
```
