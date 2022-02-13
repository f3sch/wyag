#pragma once

#include <boost/program_options.hpp>
#include <filesystem>
#include <variant>

namespace libwyag::cli {
namespace po = boost::program_options;
namespace fs = std::filesystem;
using namespace std;

/**
 * Represent the init command and its optionals.
 */
struct Init {
  fs::path path_; /* Where to initialize repository. */
};

/**
 * Wrapper for all positional arguements.
 * @note int is a placeholder, since it is easy to initialize.
 */
typedef variant<int, Init> Command;

/**
 * Represents the command line interface.
 * It provides a parse() function to try parsing.
 */
class Cli {
 public:
  /**
   * Only Cunstructor for this class.
   * @param argc Is the count of given command line arguements
   * @param argv C-sytle array of argument list
   */
  Cli(const int argc, const char* argv[]);

  /**
   * Parsing function.
   * @warning May throw!
   *
   * @return A std::variant of all options.
   */
  Command parse();

 private:
  po::options_description global_{
      "Allowed global options."}; /* Global argument description. */
  po::positional_options_description
      pos_; /* Position arguemnts given via cli. */
  variant<int, po::parsed_options> parsed_{0}; /* Parsed arguements. */
  po::variables_map vm_; /* Storage for arguements after parsing. */

  /**
   * In case of error, print help message
   */
  void print_help();
};

}  // namespace libwyag::cli
