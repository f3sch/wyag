#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

#include "config.hpp"

namespace libwyag::repo {
namespace fs = std::filesystem;
using namespace config;

class GitRepository {
 public:
  GitRepository() = default;
  GitRepository(const fs::path path, const bool force = false);

  // utility functions
  auto repo_create(const fs::path& path) -> GitRepository;
  auto repo_path(const fs::path& path) noexcept -> fs::path;
  auto repo_dir(const fs::path& path, bool mkdir = false) -> fs::path;
  auto repo_file(const fs::path& path, bool mkdir = false) -> fs::path;
  auto get_worktree() -> fs::path;
  void set_worktree(const fs::path& path);
  void read_config();
  void make_config();
  void print_info();

 private:
  fs::path worktree_;
  fs::path gitdir_;
  fs::path conf_;
  Config config_;
};

auto repo_find(const fs::path& path, bool required = true) -> fs::path;
}  // namespace libwyag::repo
