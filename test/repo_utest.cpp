#include <filesystem>

#include "cli.hpp"
#include "gtest/gtest.h"
#include "repo.hpp"

namespace libwyag::tests {
namespace fs = std::filesystem;
using namespace cli;
using namespace repo;
using namespace std;

TEST(repo, create) {
  auto tpath = fs::temp_directory_path() / "wyagit_test/repo.create";
  fs::create_directories(tpath);
  const int argc = 4;
  const char *argv[argc] = {"0", "init", "--path", tpath.c_str()};
  Cli cli(argc, argv);
  auto cmd = cli.parse();
  const Init &init = get<Init>(cmd);

  GitRepository repo;
  EXPECT_NO_THROW(repo.repo_create(init.path_));
  EXPECT_NO_THROW(fs::remove_all(init.path_ / ".wyagit"));
  fs::remove(tpath);
}

}  // namespace libwyag::tests
