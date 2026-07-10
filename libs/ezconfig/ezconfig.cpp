#include "ezconfig.h"

#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>

std::string EzConfig::timestamp() const
{
  auto now = std::chrono::system_clock::now();
  auto t = std::chrono::system_clock::to_time_t(now);
  auto tm = *std::localtime(&t);
  char buf[32];
  std::strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tm);
  return buf;
}

std::vector<fs::path> EzConfig::listFiles() const
{
  std::vector<fs::path> files;
  for (auto& entry : fs::directory_iterator(dir_)) {
    auto fname = entry.path().filename().string();
    if (fname.starts_with(prefix_))
      files.push_back(entry.path());
  }
  std::sort(files.begin(), files.end());
  return files;
}

EzConfig::EzConfig(const std::string& dir, const std::string& prefix, int max_files)
  : dir_(dir), prefix_(prefix),
    max_files_(std::clamp(max_files, 1, 10))
{
  fs::create_directories(dir_);
  loadLatest();
}

void EzConfig::loadLatest()
{
  auto files = listFiles();
  if (!files.empty())
    loadFrom(files.back());
}

void EzConfig::loadFrom(const fs::path& path)
{
  data_.clear();
  std::ifstream f(path);
  std::string line;
  while (std::getline(f, line)) {
    if (line.empty() || line.starts_with('#'))
      continue;
    if (auto pos = line.find('='); pos != std::string::npos)
      data_[line.substr(0, pos)] = line.substr(pos + 1);
  }
}

void EzConfig::save()
{
  auto files = listFiles();
  while (std::ssize(files) >= max_files_) {
    fs::remove(files.front());
    files.erase(files.begin());
  }
  auto fname = dir_ + "/" + prefix_ + "_" + timestamp() + ".ini";
  std::ofstream f(fname);
  for (auto& [key, val] : data_)
    f << key << '=' << val << '\n';
}

std::string EzConfig::get(const std::string& key, const std::string& def) const
{
  auto it = data_.find(key);
  return it != data_.end() ? it->second : def;
}

int EzConfig::getInt(const std::string& key, int def) const
{
  auto it = data_.find(key);
  if (it == data_.end())
    return def;
  try {
    return std::stoi(it->second);
  } catch (...) {
    return def;
  }
}

bool EzConfig::getBool(const std::string& key, bool def) const
{
  auto val = get(key);
  if (val == "true"  || val == "1")
    return true;
  if (val == "false" || val == "0")
    return false;
  return def;
}

void EzConfig::set(const std::string& key, const char* val) { data_[key] = std::string(val); }
void EzConfig::set(const std::string& key, const std::string& val) { data_[key] = val; }
void EzConfig::set(const std::string& key, int val)  { data_[key] = std::to_string(val); }
void EzConfig::set(const std::string& key, bool val) { data_[key] = val ? "true" : "false"; }

//std::vector<fs::path> EzConfig::history() const { return listFiles(); }
//int EzConfig::count() const { return std::ssize(listFiles()); }

// eof
