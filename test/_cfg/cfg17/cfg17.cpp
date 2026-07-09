#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;

class Config {
    std::string dir_;       // директория хранения
    std::string prefix_;    // префикс имени файла
    int max_files_;         // максимум файлов (1-10)
    std::map<std::string, std::string> data_;

    // Получить timestamp для имени файла
    std::string timestamp() const {
        auto now = std::time(nullptr);
        auto tm = *std::localtime(&now);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        return oss.str();
    }

    // Найти все наши файлы в директории, отсортированные по имени (= по времени)
    std::vector<fs::path> listFiles() const {
        std::vector<fs::path> files;
        for (auto& entry : fs::directory_iterator(dir_)) {
            auto fname = entry.path().filename().string();
            if (fname.rfind(prefix_, 0) == 0)  // начинается с prefix_
                files.push_back(entry.path());
        }
        std::sort(files.begin(), files.end());
        return files;
    }

public:
    Config(const std::string& dir, const std::string& prefix, int max_files)
        : dir_(dir), prefix_(prefix), max_files_(std::clamp(max_files, 1, 10))
    {
        fs::create_directories(dir_);
        loadLatest();
    }

    // Загрузить из самого свежего файла
    void loadLatest() {
        auto files = listFiles();
        if (files.empty()) return;
        loadFrom(files.back());  // последний = самый новый
    }

    void loadFrom(const fs::path& path) {
        data_.clear();
        std::ifstream f(path);
        std::string line;
        while (std::getline(f, line)) {
            if (line.empty() || line[0] == '#') continue;
            auto pos = line.find('=');
            if (pos != std::string::npos)
                data_[line.substr(0, pos)] = line.substr(pos + 1);
        }
    }

    void save() {
        auto files = listFiles();

        // Удаляем самый старый если достигли лимита
        while ((int)files.size() >= max_files_) {
            fs::remove(files.front());
            files.erase(files.begin());
        }

        // Новое имя файла с timestamp
        std::string fname = prefix_ + "_" + timestamp() + ".ini";
        std::ofstream f(dir_ + "/" + fname);
        for (auto& [key, val] : data_)
            f << key << "=" << val << "\n";
    }

    // Геттеры
    std::string get(const std::string& key, const std::string& def = "") const {
        auto it = data_.find(key);
        return it != data_.end() ? it->second : def;
    }

    int getInt(const std::string& key, int def = 0) const {
        auto it = data_.find(key);
        if (it == data_.end()) return def;
        try { return std::stoi(it->second); }
        catch (...) { return def; }
    }

    bool getBool(const std::string& key, bool def = false) const {
        auto val = get(key);
        if (val == "true" || val == "1") return true;
        if (val == "false" || val == "0") return false;
        return def;
    }

    // Сеттеры
    void set(const std::string& key, const std::string& val) { data_[key] = val; }
    void set(const std::string& key, int val) { data_[key] = std::to_string(val); }
    void set(const std::string& key, bool val) { data_[key] = val ? "true" : "false"; }

    // Список всех сохранённых файлов
    std::vector<fs::path> history() const { return listFiles(); }
};
