#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <string>

void appointmentMenu();

void bookAppointment();

void viewAppointments();

void searchAppointment();

void cancelAppointment();

bool doctorAvailable( int doctorID, const std::string& date,const std::string& time, const int duration);

#endif