#ifndef DBCONNECTOR_HPP
#define DBCONNECTOR_HPP

#include <pqxx/pqxx>

class DBConnector {
    private:
        const std::string connection_str; 
        
        
    public:
        pqxx::connection conn;

        DBConnector(std::string connection_str);
        ~DBConnector() = default;

};




#endif