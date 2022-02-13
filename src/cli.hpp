#pragma once

#include <boost/program_options.hpp>
#include <filesystem>
#include <variant>

namespace libwyag::cli {
namespace po = boost::program_options;
namespace fs = std::filesystem;
using namespace std;

struct Init {
  fs::path path_;
};

typedef variant<int, Init> Command;

class Cli {
 public:
  Cli(const int argc, const char* argv[]);

  Command parse();

 private:
  po::options_description global_{"Allowed global options."};
  po::positional_options_description pos_;
  variant<int, po::parsed_options> parsed_{0};
  po::variables_map vm_;

  void print_help();
};

}  // namespace libwyag::cli
