#include "Interface.hpp"

namespace lp {

Interface::Interface(unsigned m)
    : _m(m), _env(GRBEnv()), _model(GRBModel(_env)), _u(std::vector<GRBVar>(m)),
      _v(std::vector<GRBVar>(m)) {
  for (unsigned i = 0; i < _m; ++i) {
    _u[i] = _model.addVar(0., 1., 0, GRB_CONTINUOUS, "u" + std::to_string(i));
    _v[i] = _model.addVar(0., 1., 0, GRB_CONTINUOUS, "v" + std::to_string(i));

    _model.addConstr(_u[i] + _v[i] <= 1);
  }
}

void Interface::solve() {
  std::cout << "Start solving..." << std::endl;
  _model.optimize();
  printSolution();
}

void Interface::writeInstance(std::string const &filename) {
  std::cout << "Write file..." << std::endl;
  _model.write(filename);
}

void Interface::printSolution() const {
  std::cout << "Objective: " << _model.get(GRB_DoubleAttr_ObjVal) << std::endl;
  for (unsigned i = 0; i < _m; ++i) {
    std::cout << _u[i].get(GRB_StringAttr_VarName) << " "
              << _u[i].get(GRB_DoubleAttr_X) << " "
              << _v[i].get(GRB_StringAttr_VarName) << " "
              << _v[i].get(GRB_DoubleAttr_X) << std::endl;
  }
}
} // namespace lp
