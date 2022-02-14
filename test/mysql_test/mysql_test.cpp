#include <mysql/mysql.h>
#include <iostream>
int main(int argc, char **argv) {
    MYSQL mysql;
    const char host[] = "localhost";
    const char user[] = "root";
    const char passwd[] = "root";
    const char db[] = "yourdb";
    unsigned int port = 5555;
    mysql_init(&mysql);
    if (mysql_real_connect(&mysql, host, user, passwd, db, port, nullptr, 0)) {
        std::cout << "connect with mysql success!" << std::endl;
        int res = mysql_query(&mysql, "select * from user");
        if (!res) {
            MYSQL_RES *result = mysql_store_result(&mysql);
            std::cout << result->fields->table << std::endl;
        }
    } else {
        std::cout << "connect with mysql failed!" << std::endl;
    }
    return 0;
}
