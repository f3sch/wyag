// libwyag

#include <cryptopp/cryptlib.h>
#include <zlib.h>

#include <boost/program_options.hpp>

namespace libwyag {
namespace po = boost::program_options;

class Cli {
 public:
  Cli() : desc("Allowd options.") {
    // Add list of options
    desc.add_options()("help", "produce a help message");
  }

 private:
  po::options_description desc;
};

void entry(int argc, char *argv[]) { Cli cli; };

}  // namespace libwyag
