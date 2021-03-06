// libwyag
#include <cryptopp/cryptlib.h>
#include <zlib.h>

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <variant>

#include "cli.hpp"
#include "repo.hpp"
#include "util.hpp"

namespace libwyag {
namespace detail {
using namespace std;
using namespace cli;
using namespace repo;

void on_cmd(const Command& cmd, GitRepository& repo) {
  if (holds_alternative<monostate>(cmd)) {
  } else if (holds_alternative<Init>(cmd)) {
    // create a repo
    auto init = get<Init>(cmd);
    repo.repo_create(init.path_);
  } else if (holds_alternative<Cat_file>(cmd)) {
    repo.read_config();
    // cat-file
    auto cat = get<Cat_file>(cmd);

    repo.cat_file(cat.type_, cat.object_);
  } else if (holds_alternative<Hash_object>(cmd)) {
    repo.read_config();
    // hash-object
    auto hobj = get<Hash_object>(cmd);

    repo.hash_object(hobj.path_, hobj.type_, hobj.write_);
  } else {
    cerr << "Unhandled command! (Write better code)" << endl;
    throw runtime_error("Undhandled command");
  }
}
}  // namespace detail

int entry(const int argc, const char* argv[]) {
  using namespace cli;
  using namespace repo;
  // parse cmd line
  Cli cli{argc, argv};
  auto cmd = cli.parse();

  // get repo
  GitRepository repo;

  // react to command
  detail::on_cmd(cmd, repo);

  return SUCCESS;
};

}  // namespace libwyag
