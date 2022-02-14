#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>

#include "gtest/gtest.h"
#include "object.hpp"

namespace libwyag::tests {
using namespace std;
using namespace object;
using namespace CryptoPP;

TEST(object, hash_to_string) {
  string msg = "Yoda said, Do or do not. There is no try.";
  SHA1 hash;
  HexEncoder encoder;
  string output;
  CryptoPP::byte digest[SHA1::DIGESTSIZE];
  hash.CalculateDigest(digest, (const CryptoPP::byte*)msg.c_str(),
                       msg.length());

  SHA1 hash_copy = hash;

  // Drop internal hex encoder and use this, returns uppercase by default
  encoder.Attach(new CryptoPP::StringSink(output));
  encoder.Put(digest, sizeof(digest));
  encoder.MessageEnd();
  transform(output.begin(), output.end(), output.begin(), ::tolower);

  // EXPECT_EQ(output, test);
}

}  // namespace libwyag::tests
