#include "doctor.h"

#include <iostream>
#include <memory>

#include "database.h"
#include "models.h"
#include "utils.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

void addDoctor() {

    Doctor d;

    cin.ignore();

    cout << "\n===== ADD DOCTOR =====\n";

    cout << "Name: ";
    getline(cin, d.name);

    cout << "Speciality: ";
    getline(cin, d.speciality);

    cout << "Contact: ";
    getline(cin, d.contact);

    if (!validContact(d.contact)) {

        cout << "Invalid contact.\n";
        return;
    }

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "INSERT INTO Doctors "
                "(Name, Speciality, Contact) "
                "VALUES (?, ?, ?)"
            )
        );

        pstmt->setString(1, d.name);
        pstmt->setString(2, d.speciality);
        pstmt->setString(3, d.contact);

        pstmt->execute();

        cout << "\nDoctor added.\n";

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}

void viewDoctors() {

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "SELECT * FROM Doctors"
            )
        );

        unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        cout << "\n===== DOCTORS =====\n";

        if(!res->next()) {

            cout << "No doctors found.\n";
            return;
        }

        do {

            cout << "\nID: "
                 << res->getInt("DoctorID");

            cout << "\nName: "
                 << res->getString("Name");

            cout << "\nSpecialty: "
                 << res->getString("Speciality");

            cout << "\n-----------------\n";

        } while(res->next());

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}

void searchDoctor() {

    int id;

    cout << "\nEnter Doctor ID: ";
    cin >> id;

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "SELECT * FROM Doctors "
                "WHERE DoctorID=?"
            )
        );

        pstmt->setInt(1, id);

        unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        if (res->next()) {

            cout << "\nName: "
                 << res->getString("Name");

            cout << "\nSpeciality: "
                 << res->getString("Speciality");

            cout << "Contact: "<<res->getString("Contact") <<endl;
            

        } else {

            cout << "Doctor not found.\n";
        }

    } catch(sql::SQLException &e) {

        cout <<  "Database error: " << e.what() << endl;
    }
}

void updateDoctor() {

    int id;

    cout << "\nEnter Doctor ID: ";
    cin >> id;

    Doctor d;

    cin.ignore();

    cout << "New Name: ";
    getline(cin, d.name);

    cin.ignore();
    cout << "New Speciality: ";
    getline(cin, d.speciality);

    cin.ignore();

    cout << "New Contact: ";
    getline(cin, d.contact);

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "UPDATE Doctors "
                "SET Name=?, Speciality=?, Contact=? "
                "WHERE DoctorID=?"
            )
        );

        pstmt->setString(1, d.name);
        pstmt->setString(2, d.speciality);
        pstmt->setString(3, d.contact);
        pstmt->setInt(4, id);

        pstmt->executeUpdate();

        cout << "\nDoctor updated.\n";

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}


void deleteDoctor() {

    int id;

    cout << "\nEnter Doctor ID: ";
    cin >> id;

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "DELETE FROM Doctors "
                "WHERE DoctorID=?"
            )
        );

        pstmt->setInt(1, id);

        pstmt->executeUpdate();

        cout << "\nDoctor deleted.\n";

    } catch(sql::SQLException &e) {

        cout << "Database error: "<< e.what() << endl;
    }
}

void doctorMenu() {

    int choice;

    do {

        cout << "\n===== DOCTOR MENU =====\n";

        cout << "1. Add Doctor\n";
        cout << "2. View Doctors\n";
        cout << "3. Search Doctor\n";
        cout << "4. Update Doctor\n";
        cout << "5. Delete Doctor\n";
        cout << "0. Back\n";

        cout << "Choice: ";
        cin >> choice;

        switch(choice) {

            case 1:
                addDoctor();
                break;

            case 2:
                viewDoctors();
                break;

            case 3:
                searchDoctor();
                break;

            case 4:
                updateDoctor();
                break;

            case 5:
                deleteDoctor();
                break;
        }

    } while(choice != 0);
}