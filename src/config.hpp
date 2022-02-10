#pragma once

#include <boost/program_options.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace libwyag::config {
namespace fs = std::filesystem;
namespace po = boost::program_options;
using namespace std;

struct configuration {
  unsigned int version;
  bool filemode;
  bool bare;

  configuration() : version(0), filemode(false), bare(false) {}
  auto write(const fs::path& path) {
    ofstream file;
    file.open(path);
    file << "[core]" << endl;
    file << "version = " << version << endl;
    file << "filemode = " << filemode << endl;
    file << "bare = " << bare << endl;
    file.close();
  }

  friend ostream& operator<<(ostream& os, const configuration& cfg);
};

inline ostream& operator<<(ostream& os, const configuration& cfg) {
  os << "\tVersion: " << cfg.version << endl;
  os << "\tFilemode: " << cfg.version << endl;
  os << "\tBare: " << cfg.version << endl;
  return os;
}

class Config {
 public:
  Config(const fs::path& path) : path_(path), desc_("WYAGit config file") {
    // list of config values
    // clang-format off
    desc_.add_options()
        ("core.version", po::value(&cfg_.version)->required(), "Version of the repository")
        ("core.filemode", po::value(&cfg_.version)->required(), "Disable tracking of file mode changes in the worktree")
        ("core.bare", po::value(&cfg_.version)->required(), "Has a worktree.")
        ;
    // clang-format on
  }

  void read() {
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

  void print_info() { cout << cfg_; }

 private:
  fs::path path_;
  po::options_description desc_;
  po::variables_map vm_;
  configuration cfg_;

  void print_help() { cerr << desc_ << endl; }
};

}  // namespace libwyag::config
