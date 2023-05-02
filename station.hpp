#ifndef STATION_H
#define STATION_H
//#include "Generic_connection_parser.hpp"
#include "Generic_mapper.hpp"

// as the class Generic_connection_parser inherits from Generic_station_parser this can create ambiguity 
// from the stations_hashmaps variable and thus inherits only from Generic_connection_parser
// same way Station parser inherits directly from Generic mapper which inherits from Generic_connection_parser which inherits from Generic_station_parser
class Station_parser : public travel::Generic_mapper{
    protected:
        void read_stations(const std::string& _filename) override; // implementation of the pure virtual function of the class 
                                                                   // Generic_station_parser
        void read_connections(const std::string& _filename) override;// implementation of the pure virtual function of the class 
                                                                   // Generic_connection_parser
    public:
        // getters for read_connections, read_stations functions to be used outside the class
        void Useread_stations(const std::string& _filename); // to access the read_stations function which is protected
        void Useread_connections(const std::string& _filename);
    
        std::vector<std::pair<uint64_t,uint64_t> > compute_travel(uint64_t _start, uint64_t _end);
        std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(uint64_t _start, uint64_t _end);
        // Function over loading 
        std::vector<std::pair<uint64_t,uint64_t> > compute_travel(const std::string& _start, const std::string& _end);
        std::vector<std::pair<uint64_t,uint64_t> > compute_and_display_travel(const std::string& _start, const std::string& _end);
};

#endif