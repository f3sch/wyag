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
struct Configuration {
  unsigned int version;
  bool filemode;
  bool bare;

 public:
  Configuration();
  void write(const fs::path& path);

  friend ostream& operator<<(ostream& os, const Configuration& cfg);
};
}  // namespace internal

class Config {
 public:
  Config() = default;
  Config(const fs::path& path);

  void read();
  void print_info();
  void make_config(const fs::path& path);

 private:
  fs::path path_;
  po::options_description desc_{"WYAGit config options."};
  po::variables_map vm_;
  internal::Configuration cfg_;

  void print_help() { cerr << desc_ << endl; }
};

}  // namespace libwyag::config
