#include "object.hpp"

#include <string>

namespace libwyag::object {
using namespace std;

WyagObject::WyagObject(const string& data) {}

auto WyagObject::get_fmt() -> string const { return fmt; }

// Blobs
WyagBlob::WyagBlob(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagBlob::serialize() -> string const { return blobdata; }

void WyagBlob::deserialize(const string& data) { blobdata = data; }

// Tree
WyagTree::WyagTree(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagTree::serialize() -> string const { return string(); }

void WyagTree::deserialize(const string& data) {}

// Tag
WyagTag::WyagTag(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagTag::serialize() -> string const { return string(); }

void WyagTag::deserialize(const string& data) {}

// Commit
WyagCommit::WyagCommit(const string& data) {
  if (!data.empty()) {
    deserialize(data);
  }
}

auto WyagCommit::serialize() -> string const { return string(); }

void WyagCommit::deserialize(const string& data) {}

}  // namespace libwyag::object
