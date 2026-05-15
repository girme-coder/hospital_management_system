#include "report.h"

#include <iostream>
#include <memory>

#include "database.h"

#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

using namespace std;



void systemSummaryReport() {

		try {

				sql::Connection* con = DBConnection::getConnection();

				
				unique_ptr<sql::PreparedStatement> ps1(
						con->prepareStatement(
								"SELECT COUNT(*) FROM Patients"
						)
				);

				unique_ptr<sql::ResultSet> rs1(ps1->executeQuery());
				int patients = (rs1->next()) ? rs1->getInt(1) : 0;

				
				unique_ptr<sql::PreparedStatement> ps2(
						con->prepareStatement(
								"SELECT COUNT(*) FROM Doctors"
						)
				);

				unique_ptr<sql::ResultSet> rs2(ps2->executeQuery());
				int doctors = (rs2->next()) ? rs2->getInt(1) : 0;

				
				unique_ptr<sql::PreparedStatement> ps3(
						con->prepareStatement(
								"SELECT COUNT(*) FROM Appointments"
						)
				);

				unique_ptr<sql::ResultSet> rs3(ps3->executeQuery());
				int appointments = (rs3->next()) ? rs3->getInt(1) : 0;

				cout << "\n========== SYSTEM SUMMARY REPORT ==========\n";
				cout << "Total Patients     : " << patients << "\n";
				cout << "Total Doctors      : " << doctors << "\n";
				cout << "Total Appointments : " << appointments << "\n";
				cout << "===========================================\n";

		} catch (sql::SQLException &e) {
				cout << "Database error: " << e.what() << endl;
		}
}

void doctorActivityReport() {

		try {

				sql::Connection* con =
						DBConnection::getConnection();

				unique_ptr<sql::PreparedStatement> pstmt(
						con->prepareStatement(
								"SELECT d.Name, COUNT(a.AppointmentID) "
								"FROM Doctors d "
								"LEFT JOIN Appointments a ON d.DoctorID = a.DoctorID "
								"GROUP BY d.DoctorID, d.Name "
								"ORDER BY COUNT(a.AppointmentID) DESC"
						)
				);

				unique_ptr<sql::ResultSet> rs(pstmt->executeQuery());

				cout << "\n========== DOCTOR ACTIVITY REPORT ==========\n";
				cout << "Doctor Name\t\tAppointments\n";
				cout << "--------------------------------------------\n";

				while (rs->next()) {

						string name = rs->getString(1);
						int count = rs->getInt(2);

						if (name.length() < 8)
								cout << name << "\t\t\t" << count << "\n";
						else if (name.length() < 16)
								cout << name << "\t\t" << count << "\n";
						else
								cout << name << "\t" << count << "\n";
				}

				cout << "============================================\n";

		} catch (sql::SQLException &e) {
				cout << "Database error: " << e.what() << endl;
		}
}

void reportMenu() {

    int choice;

    do {

        cout << "\n=========== REPORT MENU ===========\n";
        cout << "1. System Summary Report\n";
        cout << "2. Doctor Activity Report\n";
        cout << "0. Exit\n";
        cout << "===================================\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

            case 1:
                systemSummaryReport();
                break;

            case 2:
                doctorActivityReport();
                break;

            case 3:
                cout << "Exiting report system...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);
}