#pragma once

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>

#include "repo.hpp"
#include "util.hpp"

namespace libwyag::cli {
namespace po = boost::program_options;
using namespace std;
namespace fs = std::filesystem;

class Cli {
 public:
  Cli(int argc, char* argv[]) : desc_("Allowd options.") {
    // Add list of options
    // clang-format off
    desc_.add_options()
        ("help,h", "produce a help message")
        ("add", "add files to staging area")
        ("cat-file,cat", "description")
        ("checkout,co", "checkout")
        ("commit", "commit staging area to ...")
        ("init", po::value<fs::path>()->default_value("."), "initialize a wyag repository")
        ;
    // clang-format on

    // Parse the command line options letting boost take the wheel
    try {
      po::store(po::parse_command_line(argc, argv, desc_), vm_);
      po::notify(vm_);
    } catch (po::error& e) {
      cerr << "ERROR: " << e.what() << endl << endl;
      print_help();
    }
  }

  void parse() noexcept {
    // Help message
    if (vm_.count("help")) {
      print_help();
    }

    // init
    if (vm_.count("init")) {
      repo::repo_create(vm_["init"].as<fs::path>());
    }
  }

 private:
  po::options_description desc_;
  po::variables_map vm_;

  void print_help() { cout << desc_ << endl; }
};

}  // namespace libwyag::cli
