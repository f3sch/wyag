#include "cli.hpp"

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>

namespace libwyag::cli {
namespace po = boost::program_options;
using namespace std;

Cli::Cli(const int argc, const char* argv[]) {
  // Add list of options
  // clang-format off
  global_.add_options()
      ("debug", "Turn on debug output")
      ("command", po::value<std::string>(), "Command to execute")
      ("subargs", po::value<std::vector<std::string> >(), "Arguments for command")
      ;
  // clang-format on

  // positional arguements
  // clang-format off
  pos_.add("command", 1).
      add("subargs", -1)
      ;
  // clang-format on
  // parsing
  parsed_ = po::command_line_parser(argc, argv)
                .options(global_)
                .positional(pos_)
                .allow_unregistered()
                .run();

  // Try parsing the command line options
  try {
    auto parsed = get<po::parsed_options>(parsed_);
    // storing
    po::store(parsed, vm_);

    // notify
    po::notify(vm_);

  } catch (po::error& e) {
    cerr << "ERROR: " << e.what() << endl << endl;
  }
}

Command Cli::parse() {
  string cmd = vm_["command"].as<string>();
  auto parsed = get<po::parsed_options>(parsed_);

  // Initialize a repository
  if (cmd == "init") {
    po::options_description init_desc("init options");
    // clang-format off
    init_desc.add_options()
        ("path", po::value<fs::path>()->default_value(fs::current_path()), "Path to init repository")
        ;
    // clang-format on

    try {
      vector<string> opts =
          po::collect_unrecognized(parsed.options, po::include_positional);
      opts.erase(opts.begin());

      // reparse
      po::store(po::command_line_parser(opts).options(init_desc).run(), vm_);
    } catch (po::error& e) {
      cerr << "Error: " << e.what() << endl;
      cerr << init_desc << endl;
    }

    Init init;
    init.path_ = vm_["path"].as<fs::path>();
    cout << init.path_ << endl;

    return init;
  } else if (cmd == "help") {
    print_help();
    return 0;
  } else if (cmd == "status") {
  } else if (cmd == "add") {
  } else if (cmd == "cat-file") {
  } else if (cmd == "checkout") {
  } else if (cmd == "commit") {
  } else if (cmd == "hash-object") {
  } else if (cmd == "log") {
  } else if (cmd == "ls-tree") {
  } else if (cmd == "merge") {
  } else if (cmd == "rebase") {
  } else if (cmd == "rev-parse") {
  } else if (cmd == "rm") {
  } else if (cmd == "show-ref") {
  } else if (cmd == "tag") {
  }

  print_help();
  throw po::invalid_option_value(cmd);
}

void Cli::print_help() {
  cout << "Available commands:" << endl;
  cout << "\t"
       << "add" << endl;
  cout << "\t"
       << "cat-file" << endl;
  cout << "\t"
       << "checkout" << endl;
  cout << "\t"
       << "commit" << endl;
  cout << "\t"
       << "hash-object" << endl;
  cout << "\t"
       << "init" << endl;
  cout << "\t"
       << "log" << endl;
  cout << "\t"
       << "ls-tree" << endl;
  cout << "\t"
       << "merge" << endl;
  cout << "\t"
       << "rebase" << endl;
  cout << "\t"
       << "rev-parse" << endl;
  cout << "\t"
       << "rm" << endl;
  cout << "\t"
       << "show-ref" << endl;
  cout << "\t"
       << "status" << endl;
  cout << "\t"
       << "tag" << endl;
}

}  // namespace libwyag::cli
