#include <string>
#include <vector>
#include <tuple>
#include <iostream>
void init(const std::string &m_pattern) {
    size_t n = m_pattern.size();
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;
    for (auto i = 0; i < n; ++i) {
        if (m_pattern[i] != '%') {
            nstr += m_pattern[i];
            continue;
        }           
        // 普通字符串或空格 
        if (!nstr.empty()) {
            vec.push_back(std::make_tuple(nstr, "", 0));
            nstr.clear();
        }
        // %%
        if (i+1 < n && m_pattern[i+1] == '%') {
            vec.push_back(std::make_tuple("%", "", 0));
            continue;
        }

        auto j = i + 1;
        std::string str;
        std::string fmt;
        int fmt_begin;
        int fmt_status = 0;
        while (j < n) {
            if (fmt_status == 0) {
                if (m_pattern[j] == '{') {
                    str = m_pattern.substr(i+1, j-i-1);
                    fmt_begin = j;
                    fmt_status = 1;
                } else if (!isalpha(m_pattern[j])) {
                    str = m_pattern.substr(i+1, j-i-1);
                    i = j - 1;
                    break;
                }
                ++j;
            }
            if (fmt_status == 1) {
                if (m_pattern[j] == '}') {
                    fmt = m_pattern.substr(fmt_begin+1, j-fmt_begin-1);
                    i = j; 
                    break;
                }
                ++j;
            }
        } 
        if (fmt_status == 0) {
            if (str.empty() && j == n) {
                str = m_pattern.substr(i+1, n-i-1);
            }
            vec.push_back(std::make_tuple(str, "", 0));
        } else if (fmt_status == 1) {
            vec.push_back(std::make_tuple(str, fmt, 0));
        }
    }

    for (auto t : vec) {
        std::cout << std::get<0>(t) << " " << std::get<1>(t) << " " << std::get<2>(t) << std::endl;
    }
}
int main(int argc, char *argv[])
{
    init("hello log: %d{yyy-MM-dd HH:mm:ss} %p %filename %l %t %fiberid %r %m end log!%n");

    return 0;
}
