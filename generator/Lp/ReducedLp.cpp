#include "ReducedLp.hpp"

namespace lp {

std::string ReducedLp::getName() const { return "Reduced Lp"; }

void ReducedLp::create() {
  unsigned n = _m;

  unsigned num_sol = (1 << n);
  int W1 = n;
  int W2 = n;

  std::vector<GRBVar> c(num_sol);
  std::vector<GRBVar> a(num_sol);
  std::vector<GRBVar> b(num_sol);
  std::vector<GRBVar> y(num_sol);
  std::vector<GRBVar> w(num_sol);
  std::vector<std::vector<std::pair<unsigned, GRBVar>>> w_max(num_sol);
  std::vector<std::vector<std::pair<unsigned, GRBVar>>> w_min(num_sol);

  GRBVar E = _model.addVar(-GRB_INFINITY, GRB_INFINITY, 0, GRB_CONTINUOUS, "E");

  for (unsigned i = 0; i < n; ++i) {
    _model.addConstr(_u[0] <= _v[i]);

    if (i != 0) {
      _model.addConstr(_u[0] <= _u[i]);
    }
  }

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
    if ((i & (1 << (n - 1))) == 0) {
      w_max[i].push_back(
          std::make_pair(0, _model.addVar(0, 1, 0, GRB_BINARY,
                                          "w_max" + std::to_string(i) + "_0")));
      w_max_expr += w_max[i].back().second;
    } else {
      w_min[i].push_back(
          std::make_pair(0, _model.addVar(0, 1, 0, GRB_BINARY,
                                          "w_min" + std::to_string(i) + "_0")));
      w_min_expr += w_min[i].back().second;
    }

    for (unsigned j = 1; j < n; ++j) {
      if (((i & (1 << (n - 1 - j))) != 0) && ((i & (1 << (n - j))) == 0)) {
        w_min[i].push_back(
            std::make_pair(j, _model.addVar(0, 1, 0, GRB_BINARY,
                                            "w_min" + std::to_string(i) + "_" +
                                                std::to_string(j))));
        w_min_expr += w_min[i].back().second;
      }
      if (((i & (1 << (n - 1 - j))) == 0) && ((i & (1 << (n - j))) != 0)) {
        w_max[i].push_back(
            std::make_pair(j, _model.addVar(0, 1, 0, GRB_BINARY,
                                            "w_max" + std::to_string(i) + "_" +
                                                std::to_string(j))));
        w_max_expr += w_max[i].back().second;
      }
    }

    if ((i & 1) == 0) {
      w_min[i].push_back(
          std::make_pair(n, _model.addVar(0, 1, 0, GRB_BINARY,
                                          "w_min" + std::to_string(i) + "_" +
                                              std::to_string(n))));
      w_min_expr += w_min[i].back().second;
    } else {
      w_max[i].push_back(
          std::make_pair(n, _model.addVar(0, 1, 0, GRB_BINARY,
                                          "w_max" + std::to_string(i) + "_" +
                                              std::to_string(n))));
      w_max_expr += w_max[i].back().second;
    }

    // don't know why, but these constraints solve the _model better
    // for (auto& t1 : w_min[i]) {
    //   for (auto& t2 : w_min[i]) {
    //     if (t1.first != t2.first) {
    //       _model.addConstr(t1.second + t2.second <= 1);
    //     }
    //   }
    // }
    //
    // for (auto& t1 : w_max[i]) {
    //   for (auto& t2 : w_max[i]) {
    //     if (t1.first != t2.first) {
    //       _model.addConstr(t1.second + t2.second <= 1);
    //     }
    //   }
    // }

    GRBLinExpr test = 1;
    _model.addConstr(w_max_expr - 1 >= 0);
    _model.addConstr(w_min_expr - 1 >= 0);

    auto min_it = w_min[i].begin();
    auto max_it = w_max[i].begin();
    if (min_it != w_min[i].end() && min_it->first == 0) {
      _model.addConstr(a[i] <= 0);
      _model.addConstr(a[i] >= -W1 * (1 - min_it->second));
      ++min_it;
    }
    if (max_it != w_max[i].end() && max_it->first == 0) {
      _model.addConstr(b[i] >= 0);
      _model.addConstr(b[i] <= W1 * (1 - max_it->second));
      ++max_it;
    }

    GRBLinExpr expr = 0;
    for (unsigned j = 0; j < n; ++j) {
      if ((i & (1 << (n - 1 - j))) != 0) {
        expr += _v[j];
      } else {
        expr -= _u[j];
      }

      if (min_it != w_min[i].end() && min_it->first == j + 1) {
        _model.addConstr(a[i] - expr <= 0);
        _model.addConstr(a[i] >= expr - W1 * (1 - min_it->second));
        ++min_it;
      }

      if (max_it != w_max[i].end() && max_it->first == j + 1) {
        _model.addConstr(b[i] - expr >= 0);
        _model.addConstr(b[i] <= expr + W1 * (1 - max_it->second));
        ++max_it;
      }
    }

    _model.addConstr(y[i] - expr == 0);

    _model.addConstr(c[i] >= 2 * b[i] - y[i]);
    _model.addConstr(c[i] >= y[i] - 2 * a[i]);

    _model.addConstr(c[i] <= 2 * b[i] - y[i] + W2 * w[i]);
    _model.addConstr(c[i] <= y[i] - 2 * a[i] + W2 * (1 - w[i]));

    _model.addConstr(E <= c[i]);

    _model.setObjective(GRBLinExpr(E), GRB_MAXIMIZE);
  }
}
} // namespace lp
