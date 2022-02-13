#pragma once

#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>

#include <any>

#include "repo.hpp"

namespace libwyag::objects {
using namespace std;
using namespace repo;
using namespace CryptoPP;

/**
 * Generic wyagObject.
 */
class WyagObject {
 public:
  /**
   * Only constructor of an object.
   *
   * @param repo Owning repository.
   * @param data Data to load.
   */
  WyagObject(const GitRepository& repo, const any& data) : repo_(repo) {
    if (data.has_value()) {
      deserialize(data);
    }
  }

  /**
   * Deserialize data.
   *
   * @param data Data to deserialize.
   * @return Not sure yet!
   */
  any deserialize(const any& data);

  /**
   * Serialize data.
   *
   * @param data Data to serialize.
   * @return Not sure yet!
   */
  any serialize(const any& data);

 private:
  GitRepository repo_; /* Owning repository. */
};

/**
 * Read an object from a repository.
 *
 * @param repo Repository to read from.
 * @param sha SHA1 hash of object.
 * @return Not sure yet!
 */
any object_read(const GitRepository& repo, const SHA1& sha);

}  // namespace libwyag::objects
