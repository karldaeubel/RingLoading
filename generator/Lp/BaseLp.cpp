#include "BaseLp.hpp"

namespace lp {

std::string BaseLp::getName() const { return "Lp"; }

void BaseLp::create() {
  unsigned n = _m;

  unsigned num_sol = (1 << n);
  int W1 = n;
  int W2 = n;

  std::vector<GRBVar> c(num_sol);
  std::vector<GRBVar> a(num_sol);
  std::vector<GRBVar> b(num_sol);
  std::vector<GRBVar> y(num_sol);
  std::vector<GRBVar> w(num_sol);
  std::vector<std::vector<GRBVar>> w_max(num_sol, std::vector<GRBVar>(n + 1));
  std::vector<std::vector<GRBVar>> w_min(num_sol, std::vector<GRBVar>(n + 1));

  GRBVar E = _model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS, "E");

  for (unsigned i = 0; i < num_sol; ++i) {
    c[i] = _model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS,
                         "c" + std::to_string(i));
    a[i] = _model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS,
                         "a" + std::to_string(i));
    b[i] = _model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS,
                         "b" + std::to_string(i));
    y[i] = _model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS,
                         "y" + std::to_string(i));
    w[i] = _model.addVar(0, 1, 0, GRB_BINARY, "w" + std::to_string(i));

    GRBLinExpr w_max_expr = 0;
    GRBLinExpr w_min_expr = 0;
    for (unsigned j = 0; j <= n; ++j) {
      w_max[i][j] =
          _model.addVar(0, 1, 0, GRB_BINARY,
                        "w_max_" + std::to_string(i) + "_" + std::to_string(j));
      w_min[i][j] =
          _model.addVar(0, 1, 0, GRB_BINARY,
                        "w_min_" + std::to_string(i) + "_" + std::to_string(j));

      w_max_expr += w_max[i][j];
      w_min_expr += w_min[i][j];
    }

    _model.addConstr(w_max_expr - 1 >= 0);
    _model.addConstr(w_min_expr - 1 >= 0);

    _model.addConstr(a[i] <= 0);
    _model.addConstr(b[i] >= 0);
    _model.addConstr(a[i] >= -W1 * (1 - w_min[i][0]));
    _model.addConstr(b[i] <= W1 * (1 - w_max[i][0]));

    GRBLinExpr expr = 0;
    for (unsigned j = 0; j < n; ++j) {
      if ((i & (1 << (n - 1 - j))) != 0) {
        expr += _v[j];
      } else {
        expr -= _u[j];
      }
      _model.addConstr(a[i] - expr <= 0);
      _model.addConstr(b[i] - expr >= 0);

      _model.addConstr(a[i] >= expr - W1 * (1 - w_min[i][j + 1]));
      _model.addConstr(b[i] <= expr + W1 * (1 - w_max[i][j + 1]));
    }

    _model.addConstr(y[i] - expr == 0);

    _model.addConstr(c[i] >= 2 * b[i] - y[i]);
    _model.addConstr(c[i] >= y[i] - 2 * a[i]);

    _model.addConstr(c[i] <= 2 * b[i] - y[i] + W2 * w[i]);
    _model.addConstr(c[i] <= y[i] - 2 * a[i] + W2 * (1 - w[i]));

    _model.addConstr(E <= c[i], "E" + std::to_string(i));
  }

  _model.setObjective(GRBLinExpr(E), GRB_MAXIMIZE);
}
} // namespace lp
