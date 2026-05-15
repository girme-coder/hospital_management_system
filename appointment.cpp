#include "appointment.h"

#include <iostream>
#include <memory>
#include<cstring>

#include "database.h"
#include "models.h"
#include"utils.h"

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;

bool patientExists(int patientId){
	try {

		sql::Connection* con =
			DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT * FROM Patients "
				"WHERE PatientID=? "
			)
		);

		pstmt->setInt(1, patientId);
		unique_ptr<sql::ResultSet> res(
			pstmt->executeQuery()
		);

		res->next();
		if(res->getInt("PatientID") == patientId){
			return true;
		}else{
			return false;
		}

	} catch(sql::SQLException &e) {

		cout << e.what() << endl;
		return false;
	}
}

bool doctorExists(int doctorId){
	try {

		sql::Connection* con =
			DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT * FROM Doctors "
				"WHERE doctorID=? "
			)
		);

		pstmt->setInt(1, doctorId);
		unique_ptr<sql::ResultSet> res(
			pstmt->executeQuery()
		);

		return res->next();

	} catch(sql::SQLException &e) {

		cout << "Database error: " << e.what() << endl;
		return false;
	}
}

bool doctorAvailable(int doctorID, const string& date, const string& time, int duration) {
	try {

		sql::Connection* con = DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT AppointmentTime, Duration "
				"FROM Appointments "
				"WHERE DoctorID=? "
				"AND AppointmentDate=? "
				"AND Status='Scheduled'"
			)
		);

		pstmt->setInt(1, doctorID);
		pstmt->setString(2, date);

		unique_ptr<sql::ResultSet> res(pstmt->executeQuery());


		int newStartSec =
			stoi(time.substr(0,2)) * 3600 +
			stoi(time.substr(3,2)) * 60 +
			stoi(time.substr(6,2));

		int newEndSec = newStartSec + duration * 60;

		while (res->next()) {

			string existingTime = res->getString("AppointmentTime");
			int existingDuration = res->getInt("Duration");

			int existStartSec =
				stoi(existingTime.substr(0,2)) * 3600 +
				stoi(existingTime.substr(3,2)) * 60 +
				stoi(existingTime.substr(6,2));

			int existEndSec = existStartSec + existingDuration * 60;

			if (newStartSec < existEndSec && existStartSec < newEndSec) {
				return false;
			}
		}

		return true; 

	} catch (sql::SQLException &e) {

		cout << "Database error:" << e.what() << endl;
		return false;
	}
}

void bookAppointment() {

	Appointment a;

	cout << "\nPatient ID: ";
	cin >> a.patientID;

	if(!patientExists(a.patientID)){
		cout<<"\nPatient not found.\n";
		return;
	}

	cout << "Doctor ID: ";
	cin >> a.doctorID;

	if(!doctorExists(a.doctorID)){
		cout<<"\nDoctor not found.\n";
		return;
	}

	cout << "Date (YYYY-MM-DD): ";
	cin >> a.appointmentDate;
	
	if (!isValidDate(a.appointmentDate)) {
		cout << "\nInvalid date format\n";
		return;
	}

	cout << "Time (HH:MM:SS): ";
	cin >> a.appointmentTime;
	
	if(!isValidTime(a.appointmentTime))
	{
		cout << "\nInvalid Time format\n";
		return;
	}


	cout<<"Duration for appointment in minutes: ";
	cin>> a.duration;

	cin.ignore();

	cout << "Notes: ";
	getline(cin, a.notes);



	if (!doctorAvailable(a.doctorID, a.appointmentDate,a.appointmentTime, a.duration) ){

		cout << "\nDoctor unavailable.\n";
		return;
	}

	try {

		sql::Connection* con =
			DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"INSERT INTO Appointments "
				"(PatientID, DoctorID, AppointmentDate, AppointmentTime, Duration, Notes) "
				"VALUES (?, ?, ?, ?, ?, ?)"
			)
		);

		pstmt->setInt(1, a.patientID);
		pstmt->setInt(2, a.doctorID);
		pstmt->setString(3, a.appointmentDate);
		pstmt->setString(4, a.appointmentTime);
		pstmt->setInt(5, a.duration);  
		pstmt->setString(6, a.notes);   

		pstmt->execute();

		cout << "\nAppointment booked.\n";

	} catch(sql::SQLException &e) {

		cout << "Database error: " << e.what() << endl;
	}
}

void viewAppointments() {

	try {

		sql::Connection* con =
			DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT a.AppointmentID, "
				"p.Name AS PatientName, "
				"d.Name AS DoctorName, "
				"a.AppointmentDate, "
				"a.AppointmentTime "
				"FROM Appointments a "
				"JOIN Patients p "
				"ON a.PatientID = p.PatientID "
				"JOIN Doctors d "
				"ON a.DoctorID = d.DoctorID"
			)
		);

		unique_ptr<sql::ResultSet> res(
			pstmt->executeQuery()
		);

		if(!res->next()) {
	cout << "No Appointment found.\n\n";
	return;
}

		do {

			cout << "\nAppointment ID: "
				 << res->getInt("AppointmentID");

			cout << "\nPatient: "
				 << res->getString("PatientName");

			cout << "\nDoctor: "
				 << res->getString("DoctorName");

			cout << "\nDate: "
				 << res->getString("AppointmentDate");

			cout << "\nTime: "
				 << res->getString("AppointmentTime");

			cout << "\n-----------------\n";
		} while(res->next());

	} catch(sql::SQLException &e) {

		cout << "Database error: " << e.what() << endl;
	}
}

void searchAppointment() {

	int appointmentID;

	cout << "\nEnter Appointment ID: ";
	cin >> appointmentID;

	try {

		sql::Connection* con =
			DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"SELECT a.AppointmentID, "
				"p.Name AS PatientName, "
				"d.Name AS DoctorName, "
				"a.AppointmentDate, "
				"a.AppointmentTime, "
				"a.Status, "
				"a.Notes "
				"FROM Appointments a "
				"JOIN Patients p "
				"ON a.PatientID = p.PatientID "
				"JOIN Doctors d "
				"ON a.DoctorID = d.DoctorID "
				"WHERE a.AppointmentID=?"
			)
		);

		pstmt->setInt(1, appointmentID);

		unique_ptr<sql::ResultSet> res(
			pstmt->executeQuery()
		);

		if(res->next()) {

			cout << "\nAppointment ID: "
				 << res->getInt("AppointmentID");

			cout << "\nPatient: "
				 << res->getString("PatientName");

			cout << "\nDoctor: "
				 << res->getString("DoctorName");

			cout << "\nDate: "
				 << res->getString("AppointmentDate");

			cout << "\nTime: "
				 << res->getString("AppointmentTime");

			cout << "\nStatus: "
				 << res->getString("Status");

			cout << "\nNotes: "
				 << res->getString("Notes");

			cout << "\n-----------------\n";

		} else {

			cout << "\nAppointment not found.\n";
		}

	} catch(sql::SQLException &e) {

		cout << "Database error: " << e.what() << endl;
	}
}

void cancelAppointment() {

	int appointmentID;

	cout << "\nEnter Appointment ID to cancel: ";
	cin >> appointmentID;

	try {

		sql::Connection* con =
			DBConnection::getConnection();

		unique_ptr<sql::PreparedStatement> checkStmt(
			con->prepareStatement(
				"SELECT * FROM Appointments "
				"WHERE AppointmentID=?"
			)
		);

		checkStmt->setInt(1, appointmentID);

		unique_ptr<sql::ResultSet> res(
			checkStmt->executeQuery()
		);

		if(!res->next()) {

			cout << "\nAppointment not found.\n";
			return;
		}

		unique_ptr<sql::PreparedStatement> pstmt(
			con->prepareStatement(
				"UPDATE Appointments "
				"SET Status='Cancelled' "
				"WHERE AppointmentID=?"
			)
		);

		pstmt->setInt(1, appointmentID);

		pstmt->executeUpdate();

		cout << "\nAppointment cancelled successfully.\n";

	} catch(sql::SQLException &e) {

		cout << "Database error: " << e.what() << endl;
	}
}


void appointmentMenu() {

	int choice;

	do {

		cout << "\n===== APPOINTMENT MENU =====\n";

		cout << "1. Book Appointment\n";
		cout << "2. View Appointments\n";
		cout << "3. Search Appointments\n";
		cout << "4. Cancel Appointment\n";
		cout << "0. Back\n";

		cout << "Choice: ";
		cin >> choice;

		switch(choice) {

			case 1:
				bookAppointment();
				break;

			case 2:
				viewAppointments();
				break;

			case 3: 
				searchAppointment(); 
				break;

			case 4: 
				cancelAppointment();
				break;

		}

	} while(choice != 0);
}


