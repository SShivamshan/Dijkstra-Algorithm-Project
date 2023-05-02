#  Advanced C++ Project: RATP Application 


**Project Description :**   
Implementation of the Dijkstra algorithm on two databases: a small database with 10 stations and their connections (data/stations.csv, data/connections.csv) and a large database with 760 stations and their connections (data/s.csv, data/c.csv).   
For this project, we received three files containing all the functions to be implemented in the child class of the algorithm. These files are Generic_connection_parser.hpp, Generic_station_parser.hpp and Generic_mapper.hpp. These functions define how we should retrieve and store data, as well as the return types for the functions as well as the variable types used. 
Another important file is Grade.hpp which can be used to test the function we implemented and which encountered some problems when testing the algorithm. The outputs of the tests performed are visible in the pdf file.   

**Language used :** C++11 

**Code editor used :** VS Code Editor 

**TYPE of OS :** Linux Ubuntu  

**Compiling the code :** 
For compiling the program the following flags were used : -Wall -Wextra -Werror -pedantic -pedantic-errors -O3  
Compiled : g++ -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -g main.cpp station.hpp station.cpp -o main.exe    

After compiling, there are two possibilities for running the program :   
* Run the program by giving some arguments through the command line :   
    * Linux : ./main.exe "stationfilename" "connectionfilename" "start station" "end station"  
    * Windows : main.exe "stationfilename" "connectionfilename" "start station" "end station"  
    * Unix : ./main.exe "stationfilename" "connectionfilename" "start station" "end station"  
        **Example :** 
        * ./main.exe "data/stations.csv" "data/connections.csv" "A" "C" or ./main.exe "data/stations.csv" "data/connections.csv" 3 10 for Linux  
        * main.exe "data\stations.csv" "data\connections.csv" "A" "C" for Windows   
            It's also possible to send the integers as arguments then the compute_travel function which takes an integer as argument is called.   

* Run the program without arguments, by default the filenames for station and connection are set to the small network(stations.csv and connection.csv) and the start,end stations to the 3(station C) and 10(stations J).   
    **Example :** ./main.exe   

**It is possible to simply run the program without arguments and change the default values directly in the no arguments section inside the program.**   
**There are three files called : station.cpp station.hpp main.cpp which contains the implementation of the Dijkstra algorithm**  

