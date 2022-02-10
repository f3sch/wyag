#pragma once

#include <boost/program_options.hpp>
#include <exception>
#include <filesystem>

#include "config.hpp"

namespace libwyag::repo {
namespace fs = std::filesystem;
namespace po = boost::program_options;

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

 private:
  fs::path worktree_;
  fs::path gitdir_;
  fs::path conf_;
  Config config_;
};

inline void GitRepository::print_info() {
  cout << "Worktree: " << worktree_ << endl;
  cout << "GitDir: " << gitdir_ << endl;
  cout << "Config: " << conf_ << endl;
  cout << "Repo Info:" << endl;
  config_.print_info();
}

}  // namespace libwyag::repo
