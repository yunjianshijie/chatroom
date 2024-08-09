#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>

std::string getPassword() {
    struct termios old_tio, new_tio;
    std::string password;
    int ch;

    // 保存当前终端设置
    tcgetattr(STDIN_FILENO, &old_tio);

    // 修改终端设置,禁用回显
    new_tio = old_tio;
    new_tio.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  std::cout << "Enter password: ";

    // 读取密码输入并实时显示 *
    while ((ch = getchar()) != '\n') {
        if (ch == '\b' || ch == 127) { // 删除键
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b"; // 删除最后一个 * 号
            }
        } else {
            password += static_cast<char>(ch);
            std::cout << "*"; // 实时显示 * 号
        }
    }
    std::cout << std::endl;
    // 恢复终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    return password;
}

int main() {
    std::string password = getPassword();
    std::cout << "Entered password: " << password << std::endl;
    return 0;
}