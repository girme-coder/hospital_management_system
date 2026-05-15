#ifndef UTILS_H
#define UTILS_H

#include <string>

bool validAge(int age);

bool validContact(const std::string& contact);

bool isValidDate(const std::string& date);

bool isValidTime(const std::string& time);

bool isFutureDateTime(const std::string& date ,const std::string& time);


#endif