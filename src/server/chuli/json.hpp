#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <nlohmann/json.hpp>


// 将结构变成josn 字符串
std::string structureToJson(const std::map<std::string, std::vector<std::string>>& structure) {
    std::string json = "{";
    for (const auto& item : structure) {
        json += "\"" + item.first + "\":[";
        for (const auto& value : item.second) {
            json += "\"" + value + "\",";
        }
        json.pop_back(); // 删除最后一个逗号
        json += "],";
    }
    json.pop_back(); // 删除最后一个逗号
    json += "}";
    

    return json;

}

// //
// void to_json(json& j, const Person& p) {
// j = json{{"name", p.name}, 
// {"age", p.age}, 
// {"city", p.city}};

// }
