#ifndef MODELS_H
#define MODELS_H

#include <string>


struct Patient {

    int patientID;
    std::string name;
    int age;
    std::string gender;
    std::string contact;
    std::string medicalHistory;
};

struct Doctor {

    int doctorID;
    std::string name;
    std::string speciality;
    std::string contact;
};

struct Appointment {

    int appointmentID;

    int patientID;
    int doctorID;

    std::string appointmentDate;
    std::string appointmentTime;
    int duration;

    std::string status;
    std::string notes;
};

#endif