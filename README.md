
#  Advanced C++ Project: RATP Application 
**The three files are called : station.cpp station.hpp main.cpp**
Code editor used : VS Code Editor
TYPE of OS : Linux Ubuntu
For compiling the program the following flags were used : -Wall -Wextra -Werror -pedantic -pedantic-errors -O3
Compiled : g++ -Wall -Wextra -Werror -pedantic -pedantic-errors -O3 -g main.cpp station.hpp station.cpp -o main.exe
After compiling, there are two possibilities for running the program : 
    - Run the program by giving some arguments through the command line : 
        - Linux : ./main.exe "stationfilename" "connectionfilename" "start station" "end station"
        - Windows : main.exe "stationfilename" "connectionfilename" "start station" "end station"
        - Unix : ./main.exe "stationfilename" "connectionfilename" "start station" "end station"
        Example : ./main.exe "data/stations.csv" "data/connections.csv" "A" "C" ou ./main.exe "data/stations.csv" "data/connections.csv" 3 10 for Linux
                  main.exe "data\stations.csv" "data\connections.csv" "A" "C" for Windows 

        It's also possible to send the integers as arguments then the compute_travel function which takes an integer as argument is called. 

    - Run the program without arguments, by default the filenames for station and connection are set to the small network(stations.csv and connection.csv) and the start,end stations to the 3(station C) and 10(stations J). 
    Example : ./main.exe 

**It is possible to simply run the program without arguments and change the default values directly in the no arguments section inside the program.** 