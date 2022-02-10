// libwyag
#include <cryptopp/cryptlib.h>
#include <zlib.h>

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>

#include "cli.hpp"
#include "repo.hpp"
#include "util.hpp"

namespace libwyag {

struct Interface {
  cli::Cli cli;
  repo::GitRepository repo;

  Interface(int argc, char* argv[])
      : cli(argc, argv), repo(std::filesystem::current_path()) {
    // get the command line options
    cli.parse();

    // print information
    repo.print_info();
  }
};

int entry(int argc, char* argv[]) {
  // get repo info
  Interface iface(argc, argv);

  return SUCCESS;
};

}  // namespace libwyag
