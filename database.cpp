#include "database.h"

using namespace std;

unique_ptr<sql::Connection> DBConnection::con = nullptr;

sql::Connection* DBConnection::getConnection() {

    if (!con) {

        sql::mysql::MySQL_Driver* driver;

        driver = sql::mysql::get_mysql_driver_instance();

        con.reset(
            driver->connect(
                "tcp://127.0.0.1:3306",
                "name",
                "password"
            )
        );

        con->setSchema("hospital_management");
    }

    return con.get();
}