#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
std::string get_josn(std::string new_string, int num);
// 将结构变成josn 字符串
std::string structureToJson(const std::map<std::string, std::vector<std::string>> &structure);
