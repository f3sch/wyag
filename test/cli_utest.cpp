#include <variant>

#include "cli.hpp"
#include "gtest/gtest.h"

namespace libwyag::tests {
namespace po = boost::program_options;
using namespace cli;
using namespace std;

TEST(cli, noCommand) {
  const int argc = 2;
  const char *argv[argc] = {"0", "nocommand"};
  Cli cli(argc, argv);

  EXPECT_THROW(cli.parse(), po::invalid_option_value);
}

TEST(cli, init) {
  const int argc = 4;
  const char *argv[argc] = {"0", "init", "--path", "here"};
  Cli cli(argc, argv);
  auto cmd = cli.parse();
  const Init &init = get<Init>(cmd);

  EXPECT_EQ(init.path_, "here");
}

}  // namespace libwyag::tests
