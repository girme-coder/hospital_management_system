#include <iostream>

#include "patient.h"
#include "doctor.h"
#include "appointment.h"

using namespace std;

int main () {

    int choice;

    do {

        cout << "\n===== HMS =====\n";

        cout << "1. Patient Management\n";
        cout << "2. Doctor Management\n";
        cout << "3. Appointment Management\n";
        cout << "0. Exit\n";

        cout << "Choice: ";
        cin >> choice;

        switch(choice) {

            case 1:
                patientMenu();
                break;

            case 2:
                doctorMenu();
                break;

            case 3:
                appointmentMenu();
                break;

            case 0:
                cout << "\nExiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while(choice != 0);
}