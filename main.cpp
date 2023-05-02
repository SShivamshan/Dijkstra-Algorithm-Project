#include <iostream>
#include <string>
#include <ctype.h>
#include <unordered_map>
#include <cassert>
#include "station.hpp"
//#include "Grade.hpp"

int main(int argc, char *argv[]){
    Station_parser s; 
    
    std::string station_filename,connection_filename;
    std::string start, end;
    
    /*
        If the arguments cannot be passed, please modify the section with no arguments directly in order to 
        make it work instead of passing these arguments through the command line
    */
    //  no arguments  //
    if (argc == 1 || argv[1] == NULL){  // default values 
        station_filename = "data/stations.csv";
        connection_filename = "data/connections.csv";
        start = "3";
        end = "10";
    }
    else{
        //we verify that the start and end station names are given after the first 2 arguments 
        assert(argv[3] != NULL && "The start station name wasn't defined "); 
        assert(argv[4] != NULL && "The end station name wasn't defined "); 
        
        //the first 2 arguments are the names of the files used
        station_filename = argv[1]; 
        connection_filename = argv[2];
        // start and end stations
        start = argv[3];
        end = argv[4];
    }
    
    
    /* We get the values in their respective hash maps */
    
    try{
        s.Useread_stations(station_filename);
        s.Useread_connections(connection_filename);
    }catch(std::runtime_error &e){
        std::cerr<< "Error: "<< e.what()<< std::endl;
        exit(1); // sortie du progrmme 
    }
    
    /* Test dijikstra */
    if (isdigit(start[0]) && isdigit(end[0])){
        try{
            s.compute_and_display_travel(atoi(start.c_str()), atoi(end.c_str()));
        }catch(std::invalid_argument &e){
            std::cerr<<"Error : " << e.what() << std::endl;
            exit(1);
        }
    }else{
        try{
            s.compute_and_display_travel(start,end);
        }catch(std::invalid_argument &e){
            std::cerr<<"Error : " << e.what() << std::endl;
            exit(1);
        }
    }

    ///////////////////////////////   Grade Test  /////////////////////////////////////////
    
    //travel::evaluate_small.connections(s); // we get an ok for stations and connections made with the small database
    //travel::evaluate.connections(s);  // methods evaluated on the biggest network
    
    // try{
        // travel::evaluate_small.dijkstra(s,false); 
    // }catch(const char* e ){
        // std::cout << e << std::endl; // threw an error stating that the starting id doesn't exist. 
    // }
    
    return 0;


}