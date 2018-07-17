# gillespie_propagation

Gillespie algorithm for the simulation of Markovian propagation processes on static complex networks using a composition and rejection scheme.


## Requirements

**On Unix (Linux, OS X)**

* A compiler with C++11 support
* CMake >= 3.1

**On Windows**

* Visual Studio 2015 (required for all Python versions, see notes below)
* CMake >= 3.1


## Installation

First, clone this repository and the submodule (pybind11). 
```bash
git clone --recursive https://github.com/gstonge/gillespie_propagation.git
```
Second, use pip to install the package.
```bash
pip install ./gillespie_propagation
```
To install it in developer mode
```bash
pip install -e ./gillespie_propagation
```

## Examples


