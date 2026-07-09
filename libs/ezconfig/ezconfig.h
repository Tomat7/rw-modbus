#pragma once

#include <string>
#include <map>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class EzConfig
{
  std::string dir_;
  std::string prefix_;
  int max_files_;
  std::map<std::string, std::string> data_;

  std::string timestamp() const;
  std::vector<fs::path> listFiles() const;

public:
  EzConfig(const std::string& dir, const std::string& prefix, int max_files);

  void loadLatest();
  void loadFrom(const fs::path& path);
  void save();

  std::string get(const std::string& key, const std::string& def = "") const;
  int getInt(const std::string& key, int def = 0) const;
  bool getBool(const std::string& key, bool def = false) const;

  void set(const std::string& key, const char* val);
  void set(const std::string& key, const std::string& val);
  void set(const std::string& key, int val);
  void set(const std::string& key, bool val);

  std::vector<fs::path> history() const;
  int count() const;
};
