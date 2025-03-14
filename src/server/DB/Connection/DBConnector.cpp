#include "DBConnector.hpp"

#include <pqxx/pqxx>

DBConnector::DBConnector(const std::string& connection_str) : m_connection(connection_str) {}

pqxx::connection& DBConnector::GetConnection()
{
	return m_connection;
}