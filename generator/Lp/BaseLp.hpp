#pragma once

#include "Interface.hpp"

namespace lp {
class BaseLp : public Interface {
public:
  BaseLp(unsigned m) : Interface(m) {}

  virtual std::string getName() const override;
  virtual void create() override;

private:
};
} // namespace lp
