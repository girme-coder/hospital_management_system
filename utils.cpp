#include "utils.h"

#include <iostream>
#include <algorithm>
#include <ctime>


using namespace std;

bool validAge(int age) {

	return age > 0 && age < 100;
}

bool validContact(const string& contact) {

	return contact.length() == 10 && all_of(contact.begin(), contact.end(), ::isdigit);
}

bool isValidDate(const string& date){
	
	if (date.length() != 10) return false;

	if (date[4] != '-' || date[7] != '-') return false;

	string yearStr  = date.substr(0, 4);
	string monthStr = date.substr(5, 2);
	string dayStr   = date.substr(8, 2);


	int year, month, day;

	try {
		year  = stoi(yearStr);
		month = stoi(monthStr);
		day   = stoi(dayStr);
	} catch (...) {
		return false;
	}


	if (month < 1 || month > 12) return false;
	if (day < 1 || day > 31) return false;

	
	int eachMonthLength[] = {31,28,31,30,31,30,31,31,30,31,30,31};

	
	bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (isLeap) eachMonthLength[1] = 29;

	if (day > eachMonthLength[month - 1]) return false;

	return true;
}

bool isValidTime(const string& time) {
	int hour,minute,second;
	
	if (time.length() != 8 || time[2] != ':' || time[5] != ':')
		return false;

	try {
		hour   = stoi(time.substr(0, 2));
		minute = stoi(time.substr(3, 2));
		second = stoi(time.substr(6, 2));
	} catch (...) {
		return false;
	}

	if (hour < 0 || hour > 23) return false;
	if (minute < 0 || minute > 59) return false;
	if (second < 0 || second > 59) return false;

	return true;
}

bool isFutureDateTime(const string& date, const string& inputTime) {
    int year  = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day   = stoi(date.substr(8, 2));

    int hour   = stoi(inputTime.substr(0, 2));
    int minute = stoi(inputTime.substr(3, 2));
    int second = stoi(inputTime.substr(6, 2));

    tm input = {};
    input.tm_year = year - 1900;
    input.tm_mon  = month - 1;
    input.tm_mday = day;
    input.tm_hour = hour;
    input.tm_min  = minute;
    input.tm_sec  = second;
    input.tm_isdst = -1;

    time_t Time = mktime(&input);
    time_t now = time(0);

    return Time >= now;
}