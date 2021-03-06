#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <SpreadingProcess.hpp>
#include <QSSpreadingProcess.hpp>

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
        .def(py::init<vector<pair<NodeLabel, NodeLabel> >&,
            double, double, double, double>(), R"pbdoc(
            This is the constructor of the class.

            Args:
               edge_list: Edge list of the network.
               transmission_rate: Rate of transmission per edge.
               recovery_rate: Rate of recovery of infected nodes.
               waning_immunity_rate: Rate for immunity loss. For the SIR model
                   the rate must be set to 0. For the SIS model, set it to
                   numpy.inf.
               base: Base (logarithm) for the event tree
            )pbdoc", py::arg("edge_list"), py::arg("transmission_rate"),
            py::arg("recovery_rate"), py::arg("waning_immunity_rate"),
            py::arg("base") = 2)

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

        .def("get_Inode_number",
            &SpreadingProcess::get_Inode_number, R"pbdoc(
            Returns the number of recovered nodes.
            )pbdoc")

        .def("get_Rnode_number",
            &SpreadingProcess::get_Rnode_number, R"pbdoc(
            Returns the number of recovered nodes.
            )pbdoc")



        .def("is_absorbed", &SpreadingProcess::is_absorbed, R"pbdoc(
            Returns true if the system has reached an absorbing state.
            )pbdoc")

        .def("get_Snode_set", &SpreadingProcess::get_Snode_set, R"pbdoc(
            Returns the current set of susceptible nodes.
            )pbdoc")

        .def("get_Inode_set", &SpreadingProcess::get_Inode_set, R"pbdoc(
            Returns the current set of infected nodes.
            )pbdoc")

        .def("get_Rnode_set", &SpreadingProcess::get_Rnode_set, R"pbdoc(
            Returns the current set of recovered nodes.
            )pbdoc")

        .def("initialize_random", &SpreadingProcess::initialize_random, R"pbdoc(
            Initialize the spreading process with randomly infected nodes.

            Args:
               fraction: Initial fraction of infected nodes.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("fraction"), py::arg("seed"))

        .def("initialize", (void (SpreadingProcess::*)(const vector<NodeLabel>&,
            unsigned int)) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("Inode_vector"), py::arg("seed"))

        .def("initialize", (void (SpreadingProcess::*)(const vector<NodeLabel>&,
            const vector<NodeLabel>&,unsigned int)) &SpreadingProcess::initialize,              R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
               Rnode_vector: List of nodes to be recovered initially.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("Inode_vector"), py::arg("Rnode_vector"),
                py::arg("seed"))

        .def("initialize", (void (SpreadingProcess::*)(const vector<NodeLabel>&)
                    ) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
            )pbdoc", py::arg("Inode_vector"))

        .def("initialize", (void (SpreadingProcess::*)(const vector<NodeLabel>&,
            const vector<NodeLabel>&)) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
               Rnode_vector: List of nodes to be recovered initially.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("Inode_vector"), py::arg("Rnode_vector"))

        .def("reset", &SpreadingProcess::reset, R"pbdoc(
            Reset the spreading process. It needs to be initialized again.
            )pbdoc")

        .def("next_state", &SpreadingProcess::next_state, R"pbdoc(
            Makes a Monte-Carlo step--the state of the system has changed due
            to a transmission, recovery or loss of immunity event.
            )pbdoc")

        .def("evolve", &SpreadingProcess::evolve, R"pbdoc(
            Let the system evolve for a time duration.

            Args:
               time_variation: Time duration for the simulation.
            )pbdoc", py::arg("time_variation"))

        .def("estimate_R0", &SpreadingProcess::estimate_R0, R"pbdoc(
            Estimate the basic reproduction number.

            Args:
               sample: Integer for the number of samples to draw.
               seed: Integer seed for the random number generator.
               Rnode_vector: List of nodes to be recovered initially.
            )pbdoc", py::arg("sample"), py::arg("seed"),
                py::arg("Rnode_vector") = vector<NodeLabel>())

        .def("final_size_sample", &SpreadingProcess::final_size_sample, R"pbdoc(
            Get a vector of final size sample for the SIR.

            Args:
               sample: Integer for the number of samples to draw.
               seed: Integer seed for the random number generator.
               threshold: Double for min final size to keep.
            )pbdoc", py::arg("sample"), py::arg("seed"), py::arg("threshold") = 1e-4);

    /* =====================================
     * Class deriving from SpreadingProcess
     * =====================================*/

    py::class_<QSSpreadingProcess, SpreadingProcess>(m, "QSSpreadingProcess")

        .def(py::init<vector<pair<NodeLabel, NodeLabel> >&, double, double,
                double,double,double,unsigned int>(), R"pbdoc(
            Args:
               edge_list: Edge list of the network.
               transmission_rate: Rate of transmission per edge.
               recovery_rate: Rate of recovery of infected nodes.
               waning_immunity_rate: Rate for immunity loss. For the SIR model
                   the rate must be set to 0. For the SIS model, set it to
                   numpy.inf.
               base: Base (logarithm) for the event tree.
               update_history_rate: Rate to store current state in history.
               history_vector_size: History vector size.
            )pbdoc", py::arg("edge_list"), py::arg("transmission_rate"),
            py::arg("recovery_rate"), py::arg("waning_immunity_rate"),
            py::arg("base") = 2, py::arg("update_history_rate") = 0.1,
            py::arg("history_vector_size") = 100)

        .def("initialize_random", &QSSpreadingProcess::initialize_random, R"pbdoc(
            Initialize the spreading process with randomly infected nodes.

            Args:
               fraction: Initial fraction of infected nodes.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("fraction"), py::arg("seed"))

        .def("initialize", (void (QSSpreadingProcess::*)(const vector<NodeLabel>&,
            unsigned int)) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("Inode_vector"), py::arg("seed"))

        .def("initialize", (void (QSSpreadingProcess::*)(const vector<NodeLabel>&,
            const vector<NodeLabel>&,unsigned int)) &SpreadingProcess::initialize,              R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
               Rnode_vector: List of nodes to be recovered initially.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("Inode_vector"), py::arg("Rnode_vector"),
                py::arg("seed"))

        .def("initialize", (void (QSSpreadingProcess::*)(const vector<NodeLabel>&)
                    ) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
            )pbdoc", py::arg("Inode_vector"))

        .def("initialize", (void (QSSpreadingProcess::*)(const vector<NodeLabel>&,
            const vector<NodeLabel>&)) &SpreadingProcess::initialize, R"pbdoc(
            Initialize the spreading process.

            Args:
               Inode_vector: List of nodes to be infected initially.
               Rnode_vector: List of nodes to be recovered initially.
               seed: Integer seed for the random number generator.
            )pbdoc", py::arg("Inode_vector"), py::arg("Rnode_vector"))

        .def("set_update_history_rate", &QSSpreadingProcess::set_update_history_rate,
                R"pbdoc(
            Set a new value for the update history rate.
            )pbdoc")

        .def("reset", &QSSpreadingProcess::reset, R"pbdoc(
            Reset the spreading process. It needs to be initialized again.
            )pbdoc")

        .def("evolve", &QSSpreadingProcess::evolve, R"pbdoc(
            Let the system evolve for a time duration.

            Args:
               time_variation: Time duration for the simulation.
            )pbdoc", py::arg("time_variation"));
}
