#include <fstream>
#include <iostream>
#include "manager_db.h"

ManagerDBJson::ManagerDBJson(std::string file_path):file_path_(file_path)
{
    LoadJsonFile();
}

ManagerDBJson::~ManagerDBJson()
{

}

/**
 * @brief Updates the JSON data with the given key and value.
 *
 * This function updates the JSON data with the provided key and value.
 * If the key already exists, its value will be replaced. If the key does not exist,
 * it will be added to the JSON data.
 *
 * @param key The key to update or add in the JSON data.
 * @param value The value to associate with the given key in the JSON data.
 *
 * @return void
 */
void ManagerDBJson::UpdateJsonFile(const std::string &key, const json &value)
{
    json_data_[key] = value;
}

/**
 * @brief Reads a specific key from the JSON data.
 *
 * This function reads a specific key from the JSON data. If the key exists in the JSON data,
 * its value will be returned. If the key does not exist, an empty JSON object will be returned.
 *
 * @param key The key to read from the JSON data.
 *
 * @return A JSON object containing the value associated with the given key, or an empty JSON object if the key does not exist.
 */
json ManagerDBJson::ReadJsonFile(const std::string &key)
{
    if (json_data_.contains(key)) {
        return json_data_[key];
    } else {
        return json({});
    }
}

/**
 * @brief Loads the JSON data from the specified file.
 *
 * This function reads the JSON data from the specified file and stores it in the internal data structure.
 * If the file cannot be opened, a std::runtime_error exception will be thrown.
 *
 * @param None
 *
 * @return void
 */
void ManagerDBJson::LoadJsonFile(void)
{
    std::ifstream file(file_path_);
    if (file.is_open()) {
        file >> json_data_;
        file.close();
    } else {
        throw std::runtime_error("Unable to open file");
    }
}

/**
 * @brief Saves the JSON data to the specified file.
 *
 * This function saves the JSON data stored in the internal data structure to the specified file.
 * If the file cannot be opened for writing, a std::runtime_error exception will be thrown.
 *
 * @param None
 *
 * @return void
 */
void ManagerDBJson::SaveJsonFile(void)
{
    std::ofstream file(file_path_);
    if (file.is_open()) {
        file << json_data_.dump(4);
        file.close();
    } else {
        throw std::runtime_error("Unable to open file for writing");
    }
}

/**
 * @brief Reads all the key-value pairs from the JSON data.
 *
 * This function reads all the key-value pairs from the JSON data stored in the internal data structure.
 * It returns a JSON object containing all the key-value pairs.
 *
 * @return A JSON object containing all the key-value pairs from the JSON data.
 */
json ManagerDBJson::ReadAllJsonFile(void)
{
    return json_data_;
}