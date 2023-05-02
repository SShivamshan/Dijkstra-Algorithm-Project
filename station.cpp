#include <iostream>
#include <string>
#include <cctype>
#include <fstream> 
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include "station.hpp"

#define INF 10000000
using namespace std;
using namespace travel;

void Station_parser::read_stations(const string& _filename){
    // use the stations_hashmap attribute to build the stations hashmap
    ifstream fichier{_filename};
    string line;
    vector <string> *values = new vector<string>();
    
    if (fichier.is_open()){
        getline(fichier,line); // to remove the first line of csv(first line : names of the each columns) and to read from the 2nd line
        while(getline(fichier,line,'\n')){
            stringstream ss(line);
            while(getline(ss,line,',')){
                values->push_back(line);
            }
            // we put in the station_hashmap the retrieved values
            
            uint64_t id = static_cast<uint64_t>(stoul(values->at(1)));
            /// static_cast will convert the value of unsigned long sent by stoul to that of uint64_t
            Station s; // as the variables of struct Stations are public we access them and assign the values directly
            s.name = values->at(0);
            s.line_id = values->at(2);
            s.address = values->at(3);
            s.line_name = values->at(4);
            this->stations_hashmap[id] = s;
            // cout << s << endl;
            // cout << s.line_id  << " " << id << '\n';
            values->clear(); // We clear the vector values to get the new set of values 

        }
        
    }else{
        throw  runtime_error("Invalid file name for read_stations:" + _filename);  // runtime car nom donnees en argument est mauvaise 
    }
    delete values; // destroy the vector before exiting the function
}

void Station_parser::read_connections(const std::string& _filename){
    ifstream fichier{_filename};
    string line;
    
    vector <string> *values = new vector<string>();
    if (fichier.is_open()){
        getline(fichier,line); // to remove the first line of csv(first line : names of the each columns) and to read from the 2nd line
        while(getline(fichier,line,'\n')){
            stringstream ss(line);
            while(getline(ss,line,',')){
                values->push_back(line);
            }
            // We put in the connections hashmap the retrieved values 
            // we know that the connections hashmap is an unordered map with the key set to the starting station
            // the values is another unordered map with a key set to the the ending station and the value of this corresponds to 
            // the time between the start and end station
            
            uint64_t from_stop_id = static_cast<uint64_t>(stoul(values->at(0)));
            uint64_t to_stop_id = static_cast<uint64_t>(stoul(values->at(1)));
            uint64_t transfer_time = static_cast<uint64_t>(stoul(values->at(2)));
            
            //cout << from_stop_id << " " << to_stop_id << " " << transfer_time << "\n";
            this->connections_hashmap[from_stop_id][to_stop_id] = transfer_time;

            // cout << s.line_id  << " " << id << '\n';
            values->clear(); // We clear the vector values to get the new set of values 
        }
        
    }else{
        throw  runtime_error("Invalid file name for read_connections:" + _filename);  // runtime car nom donnees en argument est mauvaise 
    }
    delete values; //
}

/*Getters for the functions read_connections read_stations*/
void Station_parser::Useread_stations(const std::string& _filename){
    read_stations(_filename);
}
void Station_parser::Useread_connections(const std::string& _filename){
    read_connections(_filename);
}

/*
    Dijikstra algorithm:
        - Find out if the departure and arrival stations exist in the stations present in the hash map
        - We assume that all nodes are not yet visited and that they all have equal start time values INF except the start node which is 0.
        - As long as all nodes are not visited, we take the smallest node (at the beginning it will be the start node)
        - This node will then be removed from the unvisited nodes and will be put in the visited node. 
        - For the chosen parent node we take their child nodes:
            - We check that if this child node is not already visited 
            - If this child node has a time lower than the current distance 
            - Then the child node in question takes the new smaller distance
            - We also keep in memory the path(prev) the parent node we took to get to the child node (prev[child node] = parent node)
            - Then we add the accumulated time to go from this parent node to the child node in question 
        - Finally we use the prev(unordered map) to take the stations that allow us to leave from the starting station to the arrival station
*/

vector<std::pair<uint64_t,uint64_t>> Station_parser::compute_travel(uint64_t _start, uint64_t _end){
    vector<std::pair<uint64_t,uint64_t>> nodes; // all paths from the start node to their neighboring node and their cumulative time
    vector<uint64_t> visited; // keep in memory the stations already visited
    int distances; 
    int temps = 0; // Used to calculate the cumulative time to get from one station to another
    vector<uint64_t> unvisited; // set of unvisited nodes
    std::unordered_map<uint64_t,uint64_t> prev; // Keeps in memory the parent node taken to get to the child node 
    vector<std::pair<uint64_t,uint64_t>> paths; 
    std::pair<uint64_t,uint64_t> pos(_start,0);
    vector<uint64_t> pnodes; // the nodes of the shortest path
    // insert the first start peer and its time of 0 in nodes
    nodes.emplace_back(pos); 

    // We verify that the start and end ids exist within the database
    auto start_station = this->stations_hashmap.find(_start);
    if (start_station == this->stations_hashmap.end()){
        throw std::invalid_argument("This starting station id doesn't exist in the database ");
    }

    auto end_station = this->stations_hashmap.find(_end);
    if (end_station == this->stations_hashmap.end()){
        throw std::invalid_argument("This ending station id doesn't exist in the database");
    }
    for (std::unordered_map<uint64_t,travel::Station>::iterator itr = this->stations_hashmap.begin(); itr != this->stations_hashmap.end();itr++){
        unvisited.push_back(itr->first); // get all the stations nodes 
        prev[itr->first] = 0; // For each node we set the parent nodes to zero at first 
    }

    // set all node values to INF except for the start node
    std::unordered_map<uint64_t,int> dist;
    for (std::unordered_map<uint64_t,travel::Station>::iterator itr = this->stations_hashmap.begin(); itr != this->stations_hashmap.end();itr++){
        if (itr->first == _start){
            dist[itr->first] = 0;
        }else{
            dist[itr->first] = INF;
        }
    }
    //cout << prev.size() << endl;
    while(!unvisited.empty()){
        uint64_t current = *unvisited.begin(); // we take the first node present in the unvisited nodes 
        uint64_t actual_dist = dist[current]; // we take the current distance for this node

        for (unsigned int v=0;v!=unvisited.size();v++){
            if ((uint64_t)dist[unvisited[v]] < actual_dist ){
                current = unvisited[v];
                actual_dist = dist[unvisited[v]];
            }
        }
        
        //cout << "Current : " << current << endl;
        // remove the visited node in the unvisited vector
        for(std::vector<uint64_t>::iterator it = unvisited.begin(); it != unvisited.end();){
            if (*it == current){
                //cout << *it << endl;
                visited.emplace_back(*it); // we push in the visited vector the node that is visited
                unvisited.erase(it); // remove the visited node from the unvisited vector 
            }else{
                ++it;   
            }
        }
        auto station = this->connections_hashmap.find(current);
        for (unordered_map<uint64_t,uint64_t>::iterator itr = station->second.begin(); itr != station->second.end();itr++){
            distances = this->connections_hashmap[station->first][itr->first] + dist[current]; // current distance from the node
            // cout << itr->first << endl;
            if (std::find(visited.begin(),visited.end(),itr->first) == visited.end()){ // we check that the node is not already visited
                if (distances < dist[itr->first] && dist[current] != INF ){
                    dist[itr->first] = distances; // we get the new smallest distance
                    prev[itr->first] = current; // the child node get the parent node used to get to it

                    // we create the peer node and its cumulative time
                    pos.first = itr->first;
                    pos.second = distances;
                    nodes.emplace_back(pos);
                    //cout << itr->first << " " << distance << endl;
                } 
            }
        }
        
        // cout << "Unvisited nodes size :" << unvisited.size() << endl;
        // cout << "visited nodes size :" << visited.size() << endl;
        // cout << "-------------------------------------"<< endl;
    }

    // To get the nodes to go from start station to end station 
    // We use the unordered map prev which contains for each child node, the parent node that we took to get there. 
    // We need to do the following : 
    uint64_t end = _end;
    while (end != _start){ // 
        pnodes.emplace_back(end);
        end = prev[end]; // the variable end will take the parent value of the current end value 
    }
    pnodes.emplace_back(_start); // We finally put the start station at the end 

    // we reverse the vector containing the nodes of the path
    std::reverse(pnodes.begin(),pnodes.end());
    //// We sum up the time to go from one station to another
    for(unsigned int i=0;i!= pnodes.size(); i++){
      pos.first = pnodes[i];
      pos.second = temps;
      paths.emplace_back(pos);
      auto p = this->connections_hashmap[pnodes[i]];
      temps += p[pnodes[i+1]];
    }

    // for(auto &v: paths){
        // cout << "Station : "<< v.first << " " << "Accumlated time : " <<   v.second << "s" << endl;
    // }
    
    return paths;
}
 
vector<std::pair<uint64_t,uint64_t>> Station_parser::compute_and_display_travel(uint64_t _start, uint64_t _end){
    vector<std::pair<uint64_t,uint64_t>> paths;
    paths = compute_travel(_start, _end);
    vector<std::pair<uint64_t,uint64_t>>::iterator itr;
    cout << "Best way from " << this->stations_hashmap[_start].name << " ( line " << this->stations_hashmap[_start].line_id << ") to " << this->stations_hashmap[_end].name << " ( line " << this->stations_hashmap[_end].line_id << ") is: " << "\n"; 
    for(itr = paths.begin(); itr != paths.end();itr++){
        if (itr->first != _end){
            cout << "Walk to " << this->stations_hashmap[itr->first].name <<", " << "line " << this->stations_hashmap[(itr)->first].line_id <<"\n";
            cout << "Take line " << this->stations_hashmap[(itr +1) ->first].line_id << ", " << this->stations_hashmap[(itr+1)->first].line_name << "\n"; 
            cout << "From " << this->stations_hashmap[(itr)->first].name << " to " << this->stations_hashmap[(itr + 1)->first].name <<  " (" << this->connections_hashmap[itr->first][(itr+1)->first] << " secs)"   << "\n";
        }
    }
    //the iterators are pointers that we can make point to a desired element by doing as for a normal pointer *(ptr+1) will return the value at ptr+1
    itr = paths.begin();
    cout << "After " << (itr + paths.size() - 1 )->second << " secs, you have reached your destination!" << "\n";   
    return paths;
}

vector<std::pair<uint64_t,uint64_t> >Station_parser::compute_travel(const std::string& _start, const std::string& _end){
    vector<std::pair<uint64_t,uint64_t>> paths;
    uint64_t start_temp = 0; uint64_t end_temp = 0; // We use these variables to retrieve the ids corresponding to the 
                                                    // of the stations given as arguments
    
    string start = _start;
    string end = _end;
    // transform the strings that we get to lower case 
    transform(start.begin(), start.end(),start.begin(),::tolower); 
    transform(end.begin(), end.end(),end.begin(),::tolower);
    // We verify that the start station and end station given exist within the hash map
    for(unordered_map<uint64_t,Station>::iterator itr =this->stations_hashmap.begin();itr != this->stations_hashmap.end() ;itr++){
        
        string station = itr->second.name;
        transform(station.begin(), station.end(),station.begin(),::tolower);
        if(start.compare(station) == 0){
            start_temp = itr->first; // on va utiliser les ids pour récuperer le chemin le plus court
        }
        else if(end.compare(station) == 0){
            end_temp = itr->first;
        }
        //cout << station << endl;
        
    }
    // check if start_temp and end_time are not equal to zero 
    // in this case the stations in question do not exist in the stations_hashmap
    if (start_temp == 0){
        throw std::invalid_argument("The following station doesn't exist : " + _start);
    }else if(end_temp == 0){
        throw std::invalid_argument("The following station doesn't exist : " + _end);
    }else{
        paths = compute_travel(start_temp, end_temp); // we use directly the compute travel function with the ids to
                                                      // to retrieve the shortest path 
    }

    // for(auto &v: paths){
        // cout << v.first << " " << v.second << endl;
    // }
    
    return paths;
}
std::vector<std::pair<uint64_t,uint64_t> >Station_parser::compute_and_display_travel(const std::string& _start, const std::string& _end){
    vector<std::pair<uint64_t,uint64_t>> paths;
    uint64_t start_temp = 0; uint64_t end_temp = 0; // We use these variables to retrieve the ids corresponding to the 
                                                    // of the stations given as arguments
     
    string start = _start;
    string end = _end;
    transform(start.begin(), start.end(),start.begin(),::tolower);
    transform(end.begin(), end.end(),end.begin(),::tolower);
    for(unordered_map<uint64_t,Station>::iterator itr =this->stations_hashmap.begin();itr != this->stations_hashmap.end() ;itr++){
        
        string station = itr->second.name;
        transform(station.begin(), station.end(),station.begin(),::tolower);
        if(start.compare(station) == 0){
            start_temp = itr->first; 
        }
        else if(end.compare(station) == 0){
            end_temp = itr->first;
        }
        //cout << station << endl;
        
    }
    // check if start_temp and end_time are not equal to zero 
    // in this case the stations in question do not exist in the stations_hashmap
    if (start_temp == 0){
        throw std::invalid_argument("The following station doesn't exist : " + _start);
    }else if(end_temp == 0){
        throw std::invalid_argument("The following station doesn't exist : " + _end);
    }else{
        paths = compute_and_display_travel(start_temp, end_temp); // we use directly the compute and display travel function with the ids to
                                                                  // to retrieve the shortest path 
    }
    return paths;
}