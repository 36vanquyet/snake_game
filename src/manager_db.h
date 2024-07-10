#ifndef MANAGER_DB_H
#define MANAGER_DB_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ManagerDBJson {
    public:
        ManagerDBJson(std::string file_path);
        ~ManagerDBJson();
        void UpdateJsonFile(const std::string &key, const json &value);
        json ReadJsonFile(const std::string &key);
        void SaveJsonFile(void);
        json ReadAllJsonFile(void);

    private:
        std::string file_path_;
        json json_data_;
        void LoadJsonFile(void);
};


#endif /* MANAGER_DB_H */