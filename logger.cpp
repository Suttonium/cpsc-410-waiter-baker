/**
 * Authors: Chris Hurt & Austin Sutton
 * Participation: 50% & 50%
 */

#include "../includes/logger.h"
using namespace std;

Logger::Logger(std::string filename) {
	this->filename = filename;
}

Logger::~Logger() {
}

//open close and clear the log file
void Logger::clearlogfile() {
	// lock guard for async logging
	lock_guard<mutex> lk(m);
	myFile.open(filename, std::fstream::trunc);

	//close file
	if (myFile.is_open())
		myFile.close();
}

void Logger::log(std::string data) {
	// lock guard for async logging
	lock_guard<mutex> lk(m);
	myFile.open(filename, std::fstream::app);
	if (!myFile.is_open()) {
		return;
	}


	std::string myline;

	m.lock();
	myFile << data;
	m.unlock();

	//close file
	if (myFile.is_open())
		myFile.close();
}