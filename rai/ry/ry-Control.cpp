/*  ------------------------------------------------------------------
    Copyright (c) 2011-2020 Marc Toussaint
    email: toussaint@tu-berlin.de

    This code is distributed under the MIT License.
    Please see <root-path>/LICENSE for details.
    --------------------------------------------------------------  */

#ifdef RAI_PYBIND

#include "types.h"
#include "ry-Config.h"
#include "../Control/control.h"

void init_CtrlSet(pybind11::module& m) {
  pybind11::class_<CtrlSet, std::shared_ptr<CtrlSet>>(m, "CtrlSet",
                                                      "A set of control objectives, which define a control mode and can be passed to a CtrlSolver")

          .def(pybind11::init<>())
          .def("addObjective", &CtrlSet::addObjective,
               pybind11::arg("feature"),
               pybind11::arg("type"),
               pybind11::arg("transientStep") = -1.
          )
          .def("add_qControlObjective", &CtrlSet::add_qControlObjective)

          .def("addSymbolicCommand", &CtrlSet::addSymbolicCommand)

          .def("canBeInitiated", [](std::shared_ptr<CtrlSet> &self, std::shared_ptr<CtrlSolver> &solver, double eqPrecision) {
              return self->canBeInitiated(solver->komo.pathConfig, eqPrecision);
          })

          .def("isConverged", [](std::shared_ptr<CtrlSet> &self, std::shared_ptr<CtrlSolver> &solver, double eqPrecision) {
              return self->isConverged(solver->komo.pathConfig, eqPrecision);
          })

          .def("getObjectives", [](std::shared_ptr<CtrlSet> &self) {
              pybind11::list list;
              for (const auto obj: self->getObjectives()) list.append(obj);
              //for(const auto obj: self->symbolicCommands) list.append(obj);
              return list;
          })

          .def("getSymbolicCommands", [](std::shared_ptr<CtrlSet> &self) {
              pybind11::list list;
              for (const auto obj: self->symbolicCommands) list.append(obj);
              return list;
          });

  pybind11::class_<CtrlObjective, shared_ptr<CtrlObjective>>(m, "CtrlObjective")
          .def(pybind11::init<>())
          .def("get_OT", [](std::shared_ptr<CtrlObjective> &self) {
              return self->type;
          })
          .def("feat", [](std::shared_ptr<CtrlObjective> &self) {
              return self->feat;
          })
          .def("getOriginalTarget", [](std::shared_ptr<CtrlObjective> &self) {
              return self->originalTarget;
          });

  pybind11::class_<CtrlSymCommand, shared_ptr<CtrlSymCommand>>(m, "SymbolicCommand")
          .def(pybind11::init<>())
          .def("getCommand", [](std::shared_ptr<CtrlSymCommand> &self) {
              return self->command;
          })
          .def("isCondition", [](std::shared_ptr<CtrlSymCommand> &self) {
              return self->isCondition;
          })
          .def("getFrameNames", [](std::shared_ptr<CtrlSymCommand> &self) {
              return I_conv(self->frames);
          });
  //  .def("feat", [](std::shared_ptr<CtrlSymCommand>& self){
  //   return self->feat;
  //});

#define ENUMVAL(pre, x) .value(#x, pre##_##x)
  //TODO
  pybind11::enum_<CtrlSymCommandType>(m, "SC")
          ENUMVAL(SC, OPEN_GRIPPER)
          ENUMVAL(SC, CLOSE_GRIPPER)
          .export_values();
}
void init_CtrlSolver(pybind11::module& m) {
  pybind11::class_<CtrlSolver, std::shared_ptr<CtrlSolver>>(m, "CtrlSolver", "A control solver")

      .def(pybind11::init<rai::Configuration&, double, uint>(),
           pybind11::arg("configuration"),
           pybind11::arg("tau"),
           pybind11::arg("order"))
      .def("set", &CtrlSolver::set)
      .def("update", &CtrlSolver::update)
      //      void report(ostream& os=std::cout);
      .def("solve", &CtrlSolver::solve)
      ;
}


#endif
