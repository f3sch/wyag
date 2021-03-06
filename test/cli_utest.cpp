#include <cstddef>
#include <filesystem>

#include "cli.hpp"
#include "gtest/gtest.h"

namespace libwyag::tests {
using namespace cli;
using namespace std;
using namespace testing;

TEST(cli, exit_test) {
  const int argc = 1;
  const char *argv[argc] = {"0"};
  EXPECT_EXIT(Cli(argc, argv), ExitedWithCode(EXIT_FAILURE), ".*");
}

TEST(cli, no_command) {
  const int argc = 2;
  const char *argv[argc] = {"0", "nocommand"};
  Cli cli(argc, argv);

  EXPECT_ANY_THROW(cli.parse());
}

TEST(cli, init_set_path) {
  const int argc = 4;
  const char *argv[argc] = {"0", "init", "--path", "here"};
  Cli cli(argc, argv);
  auto cmd = cli.parse();
  const Init &init = get<Init>(cmd);

  EXPECT_EQ(init.path_, "here");
}

}  // namespace libwyag::tests
