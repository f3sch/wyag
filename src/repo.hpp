#pragma once

#include <cryptopp/sha.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.hpp"
#include "object.hpp"

namespace libwyag::repo {
namespace fs = std::filesystem;
using namespace config;
using namespace object;
using namespace CryptoPP;

/**
 * Represents a wyagit repository.
 */
class GitRepository {
 public:
  /**
   * Default constructor.
   */
  GitRepository() = default;

  /**
   * Constructor for specific path with force option.
   *
   * @param path Path to repository.
   * @param force Force construction at path.
   */
  GitRepository(const fs::path path, const bool force = false);

  // utility functions
  /**
   * Create a repository at a path.
   *
   * @param path Location to create .wyagit directory.
   * @return Constructed Repo.
   */
  auto repo_create(const fs::path& path) -> GitRepository;

  /**
   * Join a path with repository path.
   *
   * @param path Path to append.
   * @return joined path.
   */
  auto repo_path(const fs::path& path) noexcept -> fs::path;

  /**
   * Get/create a directory under the repository.
   *
   * @param path Name of directory.
   * @param mkdir Create if not existant.
   * @return Path to (new) directory.
   */
  auto repo_dir(const fs::path& path, bool mkdir = false) -> fs::path;

  /**
   * Get/create a file under the repository.
   *
   * @param path Name of file.
   * @param mkdir Create if not existant.
   * @return Path to (new) file.
   */
  auto repo_file(const fs::path& path, bool mkdir = false) -> fs::path;

  /**
   * Get path to worktree.
   *
   * @return Worktree.
   */
  auto get_worktree() -> fs::path;

  /**
   * Set path to worktree.
   */
  void set_worktree(const fs::path& path);

  /**
   * Read config file of repository (.wyagit/config).
   */
  void read_config();

  /**
   * Create config file of repository (.wyagit/config).
   */
  void make_config();

  /**
   * Print information of repository.
   */
  void print_info();

  // object interaction
  /**
   * Read an object from a repository.
   *
   * @param hash SHA1 hash of object.
   * @return wyag object.
   */
  auto object_read(const string& hash) -> WyagObj;

  /**
   * Write an object to a repository.
   *
   * @param obj wyag object to write.
   * @param actually_write Perfrom actual write.
   * @return hash digest of object.
   */
  auto object_write(WyagObject& obj, const bool actually_write = true)
      -> string;

  /**
   * Find an object in a repository.
   *
   * @param name Name of the object.
   * @param fmt Object type
   * @param follow UNIMPLEMENTED!
   * @return Name of wyag object.
   */
  auto object_find(const string& name, const string& fmt,
                   const bool follow = true) -> string;

  /**
   * Get the hash ID of a file.
   *
   * @param file File to get the hash from.
   * @param fmt Format.
   * @param write Perform write.
   */
  auto object_hash(const fs::path& file, const string& fmt,
                   const bool write = false) -> string;

  /**
   * Cat an object in the repo.
   *
   * @param obj Hash of object.
   * @param fmt Format of object.
   */
  void cat_file(const string& obj, const string& fmt = "");

  /**
   * Compute object ID and optionally creates a blob from a file.
   * @param file File to get ID from.
   * @param fmt Format of object.
   * @param write Actually write the blob
   */
  void hash_object(const string& file, const string& fmt,
                   const bool write = false);

 private:
  fs::path worktree_; /* Path to worktree. */
  fs::path gitdir_;   /* Path to gitdir. */
  fs::path conf_;     /* Path to config file. */
  Config config_;     /* Configuration of repository. */
};

/**
 * Find a repository.
 * Recursively traverse upwards until .wyagit directory is found or root.
 *
 * @param path Start from here
 * @param required Throw exception if not found.
 * @return Path to directory.
 */
auto repo_find(const fs::path& path, bool required = true) -> fs::path;

}  // namespace libwyag::repo
