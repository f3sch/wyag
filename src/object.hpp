#pragma once

#include <string>
#include <variant>

namespace libwyag::object {
using namespace std;

/**
 * Generic wyagObject.
 */
class WyagObject {
 public:
  /**
   * Deserialize data.
   *
   * @param data Data to deserialize.
   * @return Deserialized data.
   */
  virtual void deserialize(const string& data) = 0;

  /**
   * Serialize data.
   *
   * @return String of serialized data!
   */
  virtual auto serialize() -> string const = 0;

  /**
   * Get format of object.
   * @return Format
   */
  virtual auto get_fmt() -> string const = 0;

 private:
  string fmt_; /* format */
};

/**
 * Blob object
 */
class WyagBlob : public WyagObject {
 public:
  /**
   * Only constructor.
   *
   * @param data Data to store as blobdata.
   */
  WyagBlob(const string& data);

  /**
   * Deserialize data.
   *
   * @param data Data to deserialize.
   */
  void deserialize(const string& data) override;

  /**
   * Serialize data stored in blobdata.
   */
  auto serialize() -> string const override;

  /**
   * @return "blob"
   */
  virtual auto get_fmt() -> string const override;

 private:
  string blobdata;     /* blobdata */
  string fmt = "blob"; /* format */
};

/**
 * Tree object.
 */
class WyagTree : public WyagObject {
 public:
  /**
   * Only constructor.
   *
   * @param data
   */
  WyagTree(const string& data);

  /**
   * Deserialize data.
   *
   * @param data Data to deserialize.
   */
  void deserialize(const string& data) override;

  /**
   * Serialize data stored in blobdata.
   */
  auto serialize() -> string const override;

  /**
   * @return "tree"
   */
  virtual auto get_fmt() -> string const override;

 private:
  string fmt = "tree"; /* format */
};

/**
 * Tag object.
 */
class WyagTag : public WyagObject {
 public:
  /**
   * Only constructor.
   *
   * @param data
   */
  WyagTag(const string& data);

  /**
   * Deserialize data.
   *
   * @param data Data to deserialize.
   */
  void deserialize(const string& data) override;

  /**
   * Serialize data stored in blobdata.
   */
  auto serialize() -> string const override;

  /**
   * @return "tag"
   */
  virtual auto get_fmt() -> string const override;

 private:
  string fmt = "tag"; /* format */
};

/**
 * Commit object.
 */
class WyagCommit : public WyagObject {
 public:
  /**
   * Only constructor.
   *
   * @param data
   */
  WyagCommit(const string& data);

  /**
   * Deserialize data.
   *
   * @param data Data to deserialize.
   */
  void deserialize(const string& data) override;

  /**
   * Serialize data stored in blobdata.
   */
  auto serialize() -> string const override;

  /**
   * @return "commit"
   */
  virtual auto get_fmt() -> string const override;

 private:
  string fmt = "commit"; /* format */
};

/**
 * Generic return type
 */
typedef variant<monostate, WyagBlob, WyagCommit, WyagTag, WyagTree> WyagObj;

}  // namespace libwyag::object
