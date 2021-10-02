#include <array>
#include <vector>

#include <fstream>
#include <sstream>
#include <string>

#include <iomanip>
#include <iostream>

#include <boost/program_options.hpp>

#include "Lp/Forward.hpp"

int main(int argc, char *argv[]) {

  unsigned m;

  bool solve = false;
  bool writeFile = false;
  std::string fileName;
  lp::LpType lpType;

  try {
    boost::program_options::options_description desc("Allowed options");

    // clang-format off
    desc.add_options()
      ("help,h", "produce help message")
      ("demand,d", boost::program_options::value(&m)->required(),
        "the instance size (the number of demands)")
      ("solve,s", boost::program_options::bool_switch(&solve),
        "solve the model")
      ("write,w", boost::program_options::value(&fileName),
        "write the created instance to the given file")
      ("method,m",
        boost::program_options::value(&lpType)->default_value(
          lp::LpType::BASELP,
          lp::lpTypeKey[toUType(lp::LpType::BASELP)]),
        lp::create_option_description("the lp model to use", lp::lpTypeKey, lp::lpTypeDesc).c_str());
    // clang-format on

    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return 1;
    }

    boost::program_options::notify(vm);

    if (vm.count("write")) {
      writeFile = true;
    }
  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return -1;
  } catch (...) {
    std::cerr << "Unknown error!"
              << "\n";
    return -1;
  }

  try {
    std::unique_ptr<lp::Interface> instance;
    if (lpType == lp::LpType::BASELP) {
      instance = std::make_unique<lp::BaseLp>(m);
    } else if (lpType == lp::LpType::REDUCEDLP) {
      instance = std::make_unique<lp::ReducedLp>(m);
    } else {
      std::cout << "Invalid Lp Type. This should not happen." << std::endl;
      return -1;
    }

    instance->create();

    if (writeFile) {
      instance->writeInstance(fileName);
    }

    if (solve) {
      instance->solve();
    }

  } catch (GRBException e) {
    std::cout << "Error code = " << e.getErrorCode() << std::endl;
    std::cout << e.getMessage() << std::endl;
  } catch (...) {
    std::cout << "Exception during optimization" << std::endl;
  }

  return 0;
}
