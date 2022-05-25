
#ifndef HW2RES_NEVERLAND_H
#define HW2RES_NEVERLAND_H
#include "Graph.h"
#include <iostream>
#include <memory>
#include "set"
#include "Configuration.cpp"
#include "exceptions.cpp"

/**
 * ENUM to represent the station type
 */
enum StationType{
    INTERCITY,
    CENTRAL,
    STAD
};

/**
 * TODO - intercity = "IC"
 * TODO - centeral = "CS"
 */


class Neverland {

private:
    map<string, shared_ptr<Graph<string , int>>> transport;
    map<string, Graph<string , int>> o_transport;
    Configuration config;
    set<string> stations;
public:
    string outputfile_name;
    Neverland();
    void add_route(const string& from, const string& to, unsigned int time, const string& type);
    void update_config(string& file_name);
    friend ostream& operator<<(ostream& os, const Neverland& N);
    void print_configuration();
    void BFS(const string& type ,const string& station  , map< string,bool>& reachables );
    void inbound_outbound(const string& station ,const string& func);
    void set_outputfile(string filename);
    void multiExpress(const string& source ,const string& destination  );
    void set_station(const string& s1 , const string& s2  );
    void print_stations();
    void uniExpress(const string& from, const string& to);
    map<string, pair<int, bool>> dijkstra(Graph<string, int>& G, const string& source, const int& halt_time);
    map<string, pair<int, bool>> create_dijkstra_map(const Graph<string, int>& G);
    void get_transit_time(const string& from , int& transit_time) ;
    void set_multigraph(map < string , Graph<string , int> >& multigraph) ;


    void print_to_file();
};

#endif
