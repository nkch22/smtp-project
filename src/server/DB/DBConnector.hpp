#ifndef DBCONNECTOR_HPP
#define DBCONNECTOR_HPP

#include <pqxx/pqxx>

class DBConnector
{
private:
	pqxx::connection m_connection;

public:
	DBConnector(const std::string& connection_str);
	~DBConnector() = default;

	pqxx::connection& GetConnection() const;
};

#endif
