#include "config.hpp"

#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.hpp"

namespace libwyag::config {
namespace fs = std::filesystem;
using namespace std;

namespace internal {
Configuration::Configuration() : version(0), filemode(false), bare(false) {}

void Configuration::write(const fs::path& path) {
  ofstream file;
  file.open(path);
  file << "[core]" << endl;
  file << "version = " << version << endl;
  file << "filemode = " << filemode << endl;
  file << "bare = " << bare << endl;
  file.close();
}

ostream& operator<<(ostream& os, const internal::Configuration& cfg) {
  os << "\tVersion: " << cfg.version << endl;
  os << "\tFilemode: " << cfg.version << endl;
  os << "\tBare: " << cfg.version << endl;
  return os;
}
}  // namespace internal

Config::Config(const fs::path& path) : path_(path) {
  // list of config values
  // clang-format off
    desc_.add_options()
        ("core.version", po::value(&cfg_.version)->required(), "Version of the repository")
        ("core.filemode", po::value(&cfg_.version)->required(), "Disable tracking of file mode changes in the worktree")
        ("core.bare", po::value(&cfg_.version)->required(), "Has a worktree.")
        ;
  // clang-format on
}

void Config::read() {
  // check existance
  if (!fs::exists(path_)) {
    std::string msg = "Did not find .wyagit/config";
    throw std::runtime_error(msg);
  }

  // try to parse
  try {
    ifstream ifs{path_.c_str()};
    if (ifs) {
      po::store(po::parse_config_file(ifs, desc_), vm_);
      po::notify(vm_);
    } else {
      throw std::runtime_error("Could not parse .wyagit/config!");
    }

  } catch (po::error& e) {
    cerr << "ERROR: " << e.what() << endl << endl;
    print_help();
  }
}

void Config::print_info() { cout << cfg_ << endl; }

void Config::make_config(const fs::path& path) { cfg_.write(path); }

}  // namespace libwyag::config
