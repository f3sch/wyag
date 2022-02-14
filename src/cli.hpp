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
  fs::path path_;  //!< Where to initialize repository.
};

/**
 * Represent the cat-file command and its optionals.
 */
struct Cat_file {
  string type_;    //!< Type to look at.
  string object_;  //!< Object to inspect.
};

/**
 * Represent the hash-object command and its optionals.
 */
struct Hash_object {
  bool write_;     //!< Actually write the object into the database
  string type_;    //!< Type to look at.
  fs::path path_;  //!< Read object from file
};

/**
 * Wrapper for all positional arguements.
 */
typedef variant<monostate, Init, Cat_file, Hash_object> Command;

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
  variant<int, po::parsed_options> parsed_; /* Parsed arguements. */
  po::variables_map vm_; /* Storage for arguements after parsing. */

  /**
   * In case of error, print help message
   */
  void print_help();
};

}  // namespace libwyag::cli
