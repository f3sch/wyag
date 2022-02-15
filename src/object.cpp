#include "object.hpp"

#include <string>

namespace libwyag::object {
using namespace std;

// Blobs
WyagBlob::WyagBlob(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagBlob::serialize() -> string const { return blobdata; }

void WyagBlob::deserialize(const string& data) { blobdata = data; }

auto WyagBlob::get_fmt() -> string const { return fmt; }

// Tree
WyagTree::WyagTree(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagTree::serialize() -> string const { return string(); }

void WyagTree::deserialize(const string& data) {}

auto WyagTree::get_fmt() -> string const { return fmt; }

// Tag
WyagTag::WyagTag(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagTag::serialize() -> string const { return string(); }

void WyagTag::deserialize(const string& data) {}

auto WyagTag::get_fmt() -> string const { return fmt; }

// Commit
WyagCommit::WyagCommit(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagCommit::serialize() -> string const { return string(); }

void WyagCommit::deserialize(const string& data) {}

auto WyagCommit::get_fmt() -> string const { return fmt; }

}  // namespace libwyag::object
