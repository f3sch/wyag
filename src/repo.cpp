#include "repo.hpp"

#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/zlib.h>
#include <fmt/format.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "config.hpp"
#include "object.hpp"
#include "util.hpp"

namespace libwyag::repo {
namespace fs = std::filesystem;
using namespace config;
using namespace std;
using namespace object;
using namespace CryptoPP;
using namespace fmt;

GitRepository::GitRepository(const fs::path path, const bool force)
    : worktree_(path),
      // .git is a subdirectory of the worktree
      gitdir_(worktree_ / fs::path(".wyagit")),
      conf_(gitdir_ / fs::path("config")),
      config_(conf_) {}

// utility functions
auto GitRepository::repo_create(const fs::path& path) -> GitRepository {
  this->set_worktree(path);
  auto wktree = this->get_worktree();

  // make sure path does not exists or is an empty directory
  if (fs::exists(wktree)) {
    if (not fs::is_directory(wktree)) {
      throw runtime_error(format("{} is not a directory!", wktree.string()));
    }
    if (not fs::is_empty(wktree)) {
      throw runtime_error(
          format("{} is not an empty directory!", wktree.string()));
    }
    // check if other paths contain .wyagit directory
    auto rpath = repo_find(wktree, false);
    if (not rpath.empty()) {
      throw runtime_error(
          format("Found a .wyagit repository in upwards recusrion in: ",
                 rpath.string()));
    }
  } else {
    // create if it does not exist
    fs::create_directory(wktree);
  }

  // create subdirs
  this->repo_dir("branches", true);
  this->repo_dir("objects", true);
  this->repo_dir("refs/tags", true);
  this->repo_dir("refs/heads", true);

  ofstream file;
  // .git/description
  file.open(this->repo_file("description"));
  file << "Unnamed repository; edit this file 'description' to name the "
          "repository.\n";
  file.close();

  // .git/HEAD
  file.open(this->repo_file("HEAD"));
  file << "ref: refs/heads/master\n";
  file.close();

  // write config
  this->make_config();

  return *this;
}

auto GitRepository::repo_path(const fs::path& path) noexcept -> fs::path {
  return gitdir_ / path;
}

auto GitRepository::repo_dir(const fs::path& path, bool mkdir) -> fs::path {
  auto rpath = repo_path(path);

  if (fs::exists(rpath)) {
    if (fs::is_directory(rpath)) {
      return rpath;
    } else {
      throw runtime_error(format("Not a directory: ", rpath.string()));
    }
  }

  if (mkdir) {
    fs::create_directories(rpath);
    return rpath;
  } else {
    return fs::path();
  }
}

auto GitRepository::repo_file(const fs::path& path, bool mkdir) -> fs::path {
  if (repo_dir(path.parent_path(), mkdir).empty()) {
    return repo_path(path);
  }
  return fs::path();
}

auto GitRepository::get_worktree() -> fs::path { return worktree_; }

void GitRepository::set_worktree(const fs::path& path) {
  worktree_ = path;
  gitdir_ = worktree_ / fs::path(".wyagit");
  conf_ = gitdir_ / fs::path("config");
}

void GitRepository::read_config() {}

void GitRepository::make_config() { config_.make_config(conf_); }

void GitRepository::print_info() {
  cout << "Worktree: " << worktree_ << endl;
  cout << "GitDir: " << gitdir_ << endl;
  cout << "Config: " << conf_ << endl;
  cout << "Repo Info:" << endl;
  config_.print_info();
}

auto GitRepository::object_read(const string& hash) -> WyagObj {
  auto path =
      this->repo_file(fs::path("objects") / fs::path(hash.substr(0, 2)) /
                      fs::path(hash.substr(2)));

  // inflate data from file
  string raw;
  FileSource fs(path.c_str(), true, new Inflator(new StringSink(raw)));

  // read object type
  auto x = raw.find(' ');
  auto fmt = raw.substr(0, x);

  // read and validate object size
  auto y = raw.find('\x00', x);
  auto size = static_cast<size_t>(stoi(raw.substr(x, y)));
  if (size != raw.length() - y - 1) {
    throw runtime_error(format("Malformed object: bad length -> ", hash));
  }

  // Pick Constructor
  if (fmt == "commit") {
    return WyagCommit(raw.substr(y + 1));
  } else if (fmt == "tree") {
    return WyagTree(raw.substr(y + 1));
  } else if (fmt == "tag") {
    return WyagTag(raw.substr(y + 1));
  } else if (fmt == "blob") {
    return WyagBlob(raw.substr(y + 1));
  }

  throw runtime_error("Unkown type " + fmt + " for object " + hash);
}

auto GitRepository::object_write(WyagObject& obj, const bool actually_write)
    -> string {
  // serialize data
  string data = obj.serialize();

  // add header
  string result = format("{} {} {} ", obj.get_fmt(), data.length(), "\x00");
  result += data;

  // compute hash
  SHA1 hash;
  string digest;
  StringSource(result, true, new HashFilter(hash, new StringSink(digest)));

  if (actually_write) {
    // compute path
    auto path =
        this->repo_file(fs::path("objects") / fs::path(digest.substr(0, 2)) /
                        fs::path(digest.substr(2)));

    // deflate
    StringSource ss(result, true,
                    new Deflator(new FileSink(path.c_str(), true)));
  }

  return digest;
}

auto GitRepository::object_find(const string& name, const string& fmt,
                                const bool follow) -> string {
  return name;
}

auto GitRepository::object_hash(const fs::path& file, const string& fmt,
                                const bool write) -> string {
  string data;
  try {
    ifstream ifs(file, ios::in | ios::binary);
    const size_t size = fs::file_size(file);
    data.resize(size);
    ifs.read(data.data(), size);
    ifs.close();
  } catch (...) {
    cerr << format("Could not read this file: {}", file.string());
  }

  WyagObj obj;
  if (fmt == "commit") {
    obj = WyagCommit(data);
    return object_write(get<WyagCommit>(obj), write);
  } else if (fmt == "tree") {
    obj = WyagTree(data);
    return object_write(get<WyagTree>(obj), write);
  } else if (fmt == "tag") {
    obj = WyagTag(data);
    return object_write(get<WyagTag>(obj), write);
  } else if (fmt == "blob") {
    obj = WyagBlob(data);
    return object_write(get<WyagBlob>(obj), write);
  }

  throw runtime_error(format("Unknown type {}!", fmt));
}

void GitRepository::cat_file(const string& obj, const string& fmt) {
  auto obj_ = object_read(object_find(obj, fmt));

  if (holds_alternative<WyagBlob>(obj_)) {
    cout << get<WyagBlob>(obj_).serialize() << endl;
  } else if (holds_alternative<WyagCommit>(obj_)) {
    cout << get<WyagCommit>(obj_).serialize() << endl;
  } else if (holds_alternative<WyagTag>(obj_)) {
    cout << get<WyagTag>(obj_).serialize() << endl;
  } else if (holds_alternative<WyagTree>(obj_)) {
    cout << get<WyagTree>(obj_).serialize() << endl;
  }

  throw runtime_error(
      format("Unhandled variant ({}) in cat-file command!", fmt));
}

void GitRepository::hash_object(const string& file, const string& fmt,
                                const bool write) {
  if (!fs::exists(file)) {
    throw runtime_error(format("File does not exist: {}!", file));
  }

  auto sha = object_hash(file, fmt, write);
  cout << sha << endl;
}

auto repo_find(const fs::path& path, bool required) -> fs::path {
  auto apath = path.parent_path();
  if (fs::exists(apath / ".wyagit")) {
    return apath / ".wyagit";
  }

  auto parent = apath.parent_path();

  if (parent == apath) {
    if (required) {
      throw runtime_error("No .wyagit directory find in recursion.");
    }
    return fs::path();
  }

  return repo_find(parent, required);
}

}  // namespace libwyag::repo
