#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <SpreadingProcess.hpp>

using namespace std;
using namespace net;

namespace py = pybind11;

PYBIND11_MODULE(spreading_CR, m)
{
    py::class_<SpreadingProcess>(m, "SpreadingProcess")
        .def(py::init<vector<std::pair<NodeLabel, NodeLabel> >&,
            double, double, double>())
        .def("get_time_vector", &SpreadingProcess::get_time_vector)
        .def("get_Inode_number_vector", 
            &SpreadingProcess::get_Inode_number_vector)
        .def("get_Rnode_number_vector", 
            &SpreadingProcess::get_Rnode_number_vector)
        .def("is_absorbed", &SpreadingProcess::is_absorbed)
        .def("initialize", (void (SpreadingProcess::*)(double, unsigned int))
            &SpreadingProcess::initialize)
        .def("initialize", (void (SpreadingProcess::*)(vector<NodeLabel>&,
            unsigned int)) &SpreadingProcess::initialize)
        .def("reset", &SpreadingProcess::reset)
        .def("next_state", &SpreadingProcess::next_state)
        .def("evolve", &SpreadingProcess::evolve);
}
