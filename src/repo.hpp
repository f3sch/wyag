#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.hpp"

namespace libwyag::repo {
namespace fs = std::filesystem;

using namespace libwyag::config;
using namespace std;

class GitRepository {
 public:
  GitRepository(const fs::path path, const bool force = false)
      : worktree_(path),
        // .git is a subdirectory of the worktree
        gitdir_(worktree_ / fs::path(".wyagit")),
        conf_(gitdir_ / fs::path("config")),
        config_(conf_) {
    // check if this exists
    if (not force and not fs::exists(gitdir_)) {
      string msg = "Not a WYAGit repository: ";
      throw runtime_error(msg + worktree_.string());
    }

    // Read .git/config
    config_.read();
  }

  void print_info();

  // utility functions
  auto repo_path(const fs::path& path) noexcept { return gitdir_ / path; }
  auto repo_dir(const fs::path& path, bool mkdir = false) {
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
      fs::create_directory(rpath);
      return rpath;
    } else {
      return fs::path();
    }
  }
  auto repo_file(const fs::path& path, bool mkdir = false) {
    if (repo_dir(path.parent_path(), mkdir).empty()) {
      return repo_path(path);
    }
    return fs::path();
  }

  auto get_worktree() { return worktree_; }

 private:
  const fs::path worktree_;
  const fs::path gitdir_;
  const fs::path conf_;
  Config config_;
};

inline void GitRepository::print_info() {
  cout << "Worktree: " << worktree_ << endl;
  cout << "GitDir: " << gitdir_ << endl;
  cout << "Config: " << conf_ << endl;
  cout << "Repo Info:" << endl;
  config_.print_info();
}

inline auto repo_create(const fs::path& path) {
  auto repo = GitRepository(path, true);
  auto wktree = repo.get_worktree();

  // make sure path does not exists or is an empty directory
  if (fs::exists(wktree)) {
    if (not fs::is_directory(wktree)) {
      auto msg = " is not a directory!";
      throw std::runtime_error(wktree.string() + msg);
    }
    if (not fs::is_empty(wktree)) {
      auto msg = " is not an empty directory!";
      throw std::runtime_error(wktree.string() + msg);
    }
  } else {
    // create if it does not exist
    fs::create_directory(wktree);
  }

  // create subdirs
  repo.repo_dir("branches", true);
  repo.repo_dir("objects", true);
  repo.repo_dir("refs", true);
  repo.repo_dir("refs/tags", true);
  repo.repo_dir("refs/heads", true);

  ofstream file;
  // .git/description
  file.open(repo.repo_file("description"));
  file << "Unnamed repository; edit this file 'description' to name the "
          "repository.\n";
  file.close();

  // .git/HEAD
  file.open(repo.repo_file("HEAD"));
  file << "ref: refs/heads/master\n";
  file.close();

  // write config
  configuration cfg;
  cfg.write(repo.repo_file("config"));

  return repo;
}

}  // namespace libwyag::repo
