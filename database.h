#ifndef DATABASE_H
#define DATABASE_H

#include <memory>

#include <mysql_driver.h>
#include <mysql_connection.h>

class DBConnection {

private:
    static std::unique_ptr<sql::Connection> con;

public:
    static sql::Connection* getConnection();
};

#endif