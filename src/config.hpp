#pragma once

#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace libwyag::config {
namespace fs = std::filesystem;
namespace po = boost::program_options;
using namespace std;

namespace internal {

/**
 * Container for available config options.
 */
struct Configuration {
  unsigned int version;  //!< Version number.
  bool filemode;         //!< filemode.
  bool bare;             //!< bare init.

 public:
  Configuration();
  /**
   *  Write container to file.
   * @param path Location of write.
   */
  void write(const fs::path& path);

  /**
   * Custom stream output.
   * @param os Out stream.
   * @param cfg Configuration. */
  friend ostream& operator<<(ostream& os, const Configuration& cfg);
};
}  // namespace internal

/**
 * Represents the config file (.wyagit/config).
 */
class Config {
 public:
  /**
   * Default constructor.
   */
  Config() = default;

  /**
   * Constructor for specific path.
   * @param path Where to construct config file
   */
  Config(const fs::path& path);

  /**
   * Read/parse the config file.
   */
  void read();

  /**
   * Print out parsed information.
   * @note Used in debug.
   */
  void print_info();

  /**
   * Write config to file.
   * @param path Where to write.
   */
  void make_config(const fs::path& path);

 private:
  fs::path path_; /* Location of config file. */
  po::options_description desc_{
      "WYAGit config options."}; /* Description of options. */
  po::variables_map vm_;         /* Storage of options. */
  internal::Configuration cfg_;  /* Container for options. */

  /**
   * Print help information for available options.
   */
  void print_help() { cerr << desc_ << endl; }
};

}  // namespace libwyag::config
