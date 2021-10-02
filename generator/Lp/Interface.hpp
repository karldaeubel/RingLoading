#pragma once

#include <gurobi_c++.h>

#include <iostream>
#include <string>

namespace lp {

class Interface {
public:
  Interface(unsigned m);
  virtual ~Interface() = default;

  // Override these functions
  virtual std::string getName() const = 0;
  virtual void create() = 0;

  void solve();
  void printSolution() const;
  void writeInstance(std::string const &filename);

  unsigned getNumberOfDemands() const { return _m; }

protected:
  unsigned _m;

  GRBEnv _env;
  GRBModel _model;

  std::vector<GRBVar> _u;
  std::vector<GRBVar> _v;

private:
};
} // namespace lp
