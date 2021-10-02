#pragma once

#include <array>
#include <string>
#include <type_traits>
#include <vector>

#include "Interface.hpp"

#include "BaseLp.hpp"
#include "ReducedLp.hpp"

template <typename E> constexpr auto toUType(E e) noexcept {
  return static_cast<std::underlying_type_t<E>>(e);
}

namespace lp {

class Interface;
class BaseLp;
class ReducedLp;

enum class LpType { BASELP = 0, REDUCEDLP, NUM_LPTYPE };

const std::array<std::string, toUType(LpType::NUM_LPTYPE)> lpTypeKey = {
    "base", "reduced"};

const std::array<std::string, toUType(LpType::NUM_LPTYPE)> lpTypeDesc = {
    "the basic formulation",
    "the reduced formulation with symmetry breaking constraints"};

std::istream &operator>>(std::istream &in, LpType &lpType);
std::ostream &operator<<(std::ostream &out, LpType const &lpType);

std::istream &operator>>(std::istream &in, LpType &lpType) {
  std::string token;
  in >> token;
  if (token == lpTypeKey[toUType(LpType::BASELP)]) {
    lpType = LpType::BASELP;
  } else if (token == lpTypeKey[toUType(LpType::REDUCEDLP)]) {
    lpType = LpType::REDUCEDLP;
  } else {
    in.setstate(std::ios_base::failbit);
  }

  return in;
}

std::ostream &operator<<(std::ostream &out, const LpType &lpType) {
  out << lpTypeKey[toUType(lpType)];
  return out;
}

template <typename T, size_t N>
std::string
create_option_description(const std::string &option_desc,
                          const std::array<T, N> &key,
                          const std::array<std::string, N> &key_desc);

template <typename T, size_t N>
std::string
create_option_description(const std::string &option_desc,
                          const std::array<T, N> &key,
                          const std::array<std::string, N> &key_desc) {
  std::string result;
  result += option_desc;
  result += "\nValues:\n";
  bool first = true;
  for (unsigned i = 0; i < N; ++i) {
    if (first) {
      first = false;
    } else {
      result += "\n";
    }
    result += "  <";
    result += key[i];
    result += ">: \t";
    result += key_desc[i];
  }
  return result;
}
} // namespace lp
