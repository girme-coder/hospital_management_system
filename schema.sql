CREATE DATABASE hospital_management;

USE hospital_management;

CREATE TABLE Patients (
    PatientID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    Age INT NOT NULL,
    Gender VARCHAR(20),
    Contact VARCHAR(20) UNIQUE,
    MedicalHistory TEXT
);

CREATE TABLE Doctors (
    DoctorID INT AUTO_INCREMENT PRIMARY KEY,
    Name VARCHAR(100) NOT NULL,
    Speciality VARCHAR(100),
    Contact VARCHAR(20) UNIQUE
);

CREATE TABLE Appointments (
    AppointmentID INT AUTO_INCREMENT PRIMARY KEY,

    PatientID INT,
    DoctorID INT,
    Duration INT,
    AppointmentDate DATE,
    AppointmentTime TIME,

    Status VARCHAR(20) DEFAULT 'Scheduled',

    Notes TEXT,

    FOREIGN KEY (PatientID)
    REFERENCES Patients(PatientID),

    FOREIGN KEY (DoctorID)
    REFERENCES Doctors(DoctorID)
);