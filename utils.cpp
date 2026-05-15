#include "utils.h"

#include <iostream>
#include <algorithm>


using namespace std;

bool validAge(int age) {

    return age > 0 && age < 100;
}

bool validContact(const string& contact) {

    return contact.length() == 10 && all_of(contact.begin(), contact.end(), ::isdigit);
}

void pauseScreen() {

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
}