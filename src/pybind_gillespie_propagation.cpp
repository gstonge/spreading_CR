#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <PropagationProcess.hpp>

using namespace std;
using namespace net;

namespace py = pybind11;

PYBIND11_MODULE(gillespie_propagation, m)
{
    py::class_<PropagationProcess>(m, "PropagationProcess")
        .def(py::init<vector<std::pair<NodeLabel, NodeLabel> >&,
            double, double, double>())
        .def("get_time_vector", &PropagationProcess::get_time_vector)
        .def("get_Inode_number_vector", 
            &PropagationProcess::get_Inode_number_vector)
        .def("get_Rnode_number_vector", 
            &PropagationProcess::get_Rnode_number_vector)
        .def("initialize", &PropagationProcess::initialize)
        .def("next_state", &PropagationProcess::next_state)
        .def("evolve", &PropagationProcess::evolve);
}
