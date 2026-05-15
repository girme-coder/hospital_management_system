#include "patient.h"

#include <iostream>
#include <memory>

#include "database.h"
#include "models.h"
#include "utils.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;

void addPatient() {

    Patient p;

    cin.ignore();

    cout << "\n===== ADD PATIENT =====\n";

    cout << "Name: ";
    getline(cin, p.name);

    cout << "Age: ";
    cin >> p.age;

    if (!validAge(p.age)) {

        cout << "Invalid age.\n";
        return;
    }

    cin.ignore();

    cout << "Gender: ";
    getline(cin, p.gender);

    cout << "Contact: ";
    getline(cin, p.contact);

    if (!validContact(p.contact)) {

        cout << "Invalid contact.\n";
        return;
    }

    cout << "Medical History: ";
    getline(cin, p.medicalHistory);

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "INSERT INTO Patients "
                "(Name, Age, Gender, Contact, MedicalHistory) "
                "VALUES (?, ?, ?, ?, ?)"
            )
        );

        pstmt->setString(1, p.name);
        pstmt->setInt(2, p.age);
        pstmt->setString(3, p.gender);
        pstmt->setString(4, p.contact);
        pstmt->setString(5, p.medicalHistory);

        pstmt->execute();

        cout << "\nPatient added successfully.\n";

    } catch(sql::SQLException &e) {

        cout <<  "Database error: " << e.what() << endl;
    }
}

void viewPatients() {

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "SELECT * FROM Patients"
            )
        );

        unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        cout << "\n===== PATIENTS =====\n";

if(!res->next()) {
    cout << "No patient found.\n\n";
    return;
}

do {

    cout << "\nID: "
         << res->getInt("PatientID");

    cout << "\nName: "
         << res->getString("Name");

    cout << "\nAge: "
         << res->getInt("Age");

    cout << "\nGender: "
         << res->getString("Gender");

    cout << "\nContact: "
         << res->getString("Contact");

    cout << "\nHistory: "
         << res->getString("MedicalHistory");

    cout << "\n----------------------\n";

} while(res->next());        

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}

void searchPatient() {

    int id;

    cout << "\nEnter Patient ID: ";
    cin >> id;

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "SELECT * FROM Patients "
                "WHERE PatientID=?"
            )
        );

        pstmt->setInt(1, id);

        unique_ptr<sql::ResultSet> res(
            pstmt->executeQuery()
        );

        if (res->next()) {

            cout << "\nName: "
                 << res->getString("Name");

            cout << "\nAge: "
                 << res->getInt("Age");

            cout << "\nGender: "
                    << res->getString("Gender") <<"\n";

            cout << "Contact: "<<res->getString("Contact") <<endl;

            cout<<"History: "<<res->getString("MedicalHistory");

            cout << endl;

        } else {

            cout << "Patient not found.\n";
        }

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}

void updatePatient() {

    int id;

    cout << "\nEnter Patient ID: ";
    cin >> id;

    Patient p;

    cin.ignore();

    cout << "New Name: ";
    getline(cin, p.name);

    cout << "New Age: ";
    cin >> p.age;

    cin.ignore();

    cout << "New Gender: ";
    getline(cin, p.gender);

    cout << "New Contact: ";
    getline(cin, p.contact);

    cout << "New History: ";
    getline(cin, p.medicalHistory);

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "UPDATE Patients "
                "SET Name=?, Age=?, Gender=?, "
                "Contact=?, MedicalHistory=? "
                "WHERE PatientID=?"
            )
        );

        pstmt->setString(1, p.name);
        pstmt->setInt(2, p.age);
        pstmt->setString(3, p.gender);
        pstmt->setString(4, p.contact);
        pstmt->setString(5, p.medicalHistory);
        pstmt->setInt(6, id);

        pstmt->executeUpdate();

        cout << "\nPatient updated.\n";

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}

void deletePatient() {

    int id;

    cout << "\nEnter Patient ID: ";
    cin >> id;

    try {

        sql::Connection* con =
            DBConnection::getConnection();

        unique_ptr<sql::PreparedStatement> pstmt(
            con->prepareStatement(
                "DELETE FROM Patients "
                "WHERE PatientID=?"
            )
        );

        pstmt->setInt(1, id);

        pstmt->executeUpdate();

        cout << "\nPatient deleted.\n";

    } catch(sql::SQLException &e) {

        cout << "Database error: " << e.what() << endl;
    }
}

void patientMenu() {

    int choice;

    do {

        cout << "\n===== PATIENT MENU =====\n";

        cout << "1. Add Patient\n";
        cout << "2. View Patients\n";
        cout << "3. Search Patient\n";
        cout << "4. Update Patient\n";
        cout << "5. Delete Patient\n";
        cout << "0. Back\n";

        cout << "Choice: ";
        cin >> choice;

        switch(choice) {

            case 1:    addPatient(); break;
            case 2:    viewPatients(); break;
            case 3:    searchPatient();    break;
            case 4:    updatePatient();    break;
            case 5:    deletePatient();    break;
        }

    } while(choice != 0);
}