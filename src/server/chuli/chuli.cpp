#include "chuli.hpp"
#include <vector>
#include <string>
#include "../../include/back.hpp"

std::string numToStr(int num) {
    return std::to_string(num);
}
char *fanhui(char *buffer) {
    // 这里是处理主函数
    std::cout << "发出信息" << std::endl;
    std::string buf = buffer;
    std::vector<std::string> strs(10);
    int qian = -1;
    int hou = -1;
    int count2 = 0;
    for (int i = 0; i < buf.size() - 1; i++) {
        if (buf[i] == '<' && buf[i + 1] == '|') {
            qian = i + 1;
        } else if (buf[i] == '|' && buf[i + 1] == '>') {
            hou = i;
        }
        if (qian > 0 && hou > 0 && qian < hou) {
            strs[count2++] = buf.substr(qian + 1, hou - qian - 1);
            qian = -1;
            hou = -1;
            std::cout << strs[count2 - 1] << std::endl;
        }
    }
    std::cout << "" << std::endl;
    char *ret = (char *)malloc(sizeof(char) * 1024);
    printf("fsd");
    if (count2 == 1 && strs[0] == "1") {
        std::string str_num = numToStr(SUCCESS);
        strcpy(ret, str_num.c_str());
        return ret;
    }
    std::cout << "发出信息" << std::endl;
    // 失败
    std::string str_num = numToStr(FAILURE);
    strcpy(ret, str_num.c_str());
    return ret;
}
