#include "repo.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.hpp"

namespace libwyag::repo {
namespace fs = std::filesystem;
using namespace config;
using namespace std;

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
      auto msg = " is not a directory!";
      throw runtime_error(wktree.string() + msg);
    }
    if (not fs::is_empty(wktree)) {
      auto msg = " is not an empty directory!";
      throw runtime_error(wktree.string() + msg);
    }
    // check if other paths contain .wyagit directory
    auto rpath = repo_find(wktree, false);
    if (not rpath.empty()) {
      auto msg = "Found a .wyagit repository in upwards recusrion in: ";
      throw runtime_error(msg + rpath.string());
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
      auto msg = "Not a directory: ";
      throw std::runtime_error(msg + rpath.string());
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
