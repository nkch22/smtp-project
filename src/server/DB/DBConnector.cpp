#ifndef DBCONNECTOR_CPP 
#define DBCONNECTOR_CPP

#include <pqxx/pqxx>
#include "DBConnector.hpp"

DBConnector::DBConnector(std::string connection_str): connection_str(connection_str), conn(connection_str) {}

// DBConnector::~DBConnector() {}


#endif