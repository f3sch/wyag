#include "object.hpp"

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>

#include <any>

#include "repo.hpp"

namespace libwyag::objects {
using namespace std;
using namespace repo;
using namespace CryptoPP;

any object_read(const GitRepository& repo, const SHA1& sha) { return any(); }

}  // namespace libwyag::objects
