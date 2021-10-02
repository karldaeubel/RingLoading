#pragma once

#include "Interface.hpp"

namespace lp {
class ReducedLp : public Interface {
public:
  ReducedLp(unsigned m) : Interface(m) {}

  virtual std::string getName() const override;
  virtual void create() override;

private:
};
} // namespace lp
