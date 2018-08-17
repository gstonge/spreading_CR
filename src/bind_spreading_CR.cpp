#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <SpreadingProcess.hpp>

using namespace std;
using namespace net;

namespace py = pybind11;

PYBIND11_MODULE(spreading_CR, m)
{
    m.doc() = R"pbdoc(
        SpreadingProcess
        -----------------
        
        Class for the efficient simulation of spreading processes on networks.

        .. currentmodule:: spreading_CR

        .. autosummary::
           :toctree: _generate
            
           SpreadingProcess.__init__
           SpreadingProcess.get_time_vector
           SpreadingProcess.get_Inode_number_vector
           SpreadingProcess.get_Rnode_number_vector
           SpreadingProcess.is_absorbed
           SpreadingProcess.initialize 
           SpreadingProcess.reset
           SpreadingProcess.next_state
           SpreadingProcess.evolve
    )pbdoc";

    py::class_<SpreadingProcess>(m, "SpreadingProcess")
        .def(py::init<vector<std::pair<NodeLabel, NodeLabel> >&,
            double, double, double>(), R"pbdoc(
            This is the constructor of the class.

            Args:
               edge_list: Edge list of the network.
               transmission_rate: Rate of transmission per edge.
               recovery_rate: Rate of recovery of infected nodes.
               waning_immunity_rate: Rate for immunity loss. For the SIR model
                   the rate must be set to 0. For the SIS model, set it to 
                   numpy.inf.
            )pbdoc", py::arg("edge_list"), py::arg("transmission_rate"), 
            py::arg("recovery_rate"), py::arg("waning_immunity_rate"))
        
        .def("get_time_vector", &SpreadingProcess::get_time_vector, R"pbdoc(
            Returns the vector of time at which events took place.
            )pbdoc")
        
        .def("get_Inode_number_vector", 
            &SpreadingProcess::get_Inode_number_vector, R"pbdoc(
            Returns the vector of number of infected nodes for each event.
            )pbdoc")
        
        .def("get_Rnode_number_vector", 
            &SpreadingProcess::get_Rnode_number_vector, R"pbdoc(
            Returns the vector of number of recovered nodes for each event.
            )pbdoc")
        
        .def("is_absorbed", &SpreadingProcess::is_absorbed, R"pbdoc(
            Returns true if the system has reached an absorbing state.
            )pbdoc")
        
        .def("initialize", (void (SpreadingProcess::*)(double, unsigned int))
            &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               fraction: Initial fraction of infected nodes.

               seed: Integer seed for the random number generator. 
            )pbdoc", py::arg("fraction"), py::arg("seed"))
        
        .def("initialize", (void (SpreadingProcess::*)(vector<NodeLabel>&,
            unsigned int)) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.

               seed: Integer seed for the random number generator. 
            )pbdoc", py::arg("Inode_vector"), py::arg("seed"))

        .def("reset", &SpreadingProcess::reset, R"pbdoc(
            Reset the spreading process. It needs to be initialized again.
            )pbdoc")
        
        .def("next_state", &SpreadingProcess::next_state, R"pbdoc(
            Makes a Monte-Carlo step--the state of the system has changed due             to a transmission, recovery or loss of immunity event.
            )pbdoc")

        .def("evolve", &SpreadingProcess::evolve, R"pbdoc(
            Let the system evolve for a time duration.

            Args:
               time_variation: Time duration for the simulation.
            )pbdoc", py::arg("time_variation"));
}
