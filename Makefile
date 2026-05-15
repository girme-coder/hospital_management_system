CXX = g++

CXXFLAGS = -std=c++17

LIBS = -lmysqlcppconn

SRC = main.cpp \
database.cpp \
patient.cpp \
doctor.cpp \
appointment.cpp \
utils.cpp

hms:
	$(CXX) $(SRC) -o hms $(CXXFLAGS) $(LIBS)
clean:
	rm -f hms
# build using make
# run using ./hms