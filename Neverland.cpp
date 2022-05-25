//
// Created by ypinhas on 5/14/2022.
//

#include <fstream>
#include "Neverland.h"

const int MAX = 1000000;

/**
 * init the transport
 */
Neverland:: Neverland(): outputfile_name("output.dat"){
    transport.insert({"bus", nullptr});
    transport.insert({"rail", nullptr});
    transport.insert({"tram", nullptr});
    transport.insert({"sprinter", nullptr});
}


/**
 * add rout on the graph
 */
void Neverland:: add_route(const string& from, const string& to, unsigned int time, const string& type){
    if(transport[type] == nullptr){
        transport[type] = std::make_shared<Graph<string, int>>();
    }
    transport[type]->add_node(from, to, time);
}

ostream& operator<<(ostream& os, const Neverland& N){
    for(const auto& m : N.transport){
        if(m.second != nullptr) {
            os << m.first << "\n" << *(m.second.get()) ;
        }}
    return os;
}


/**
 * updating the config object
 * @param file_name
 */
void Neverland::update_config(string& file_name) {
    ifstream my_file(file_name);             /// OPEN FILE
    if(!my_file){
        cout << file_name;
        throw NeverlandException(" ERROR: file " + file_name +" cannot open\n");
    }

    string type;
    string time;
    while(my_file >> type){
        my_file >> time;
        config[type] = stoi(time);
    }
    my_file.close();

}


/**
 * print the configuration obgect
 */
void Neverland::print_configuration(){
    for(const auto& k : config.c){
        cout <<  k.first << " " << k.second << endl;
    }
}

void Neverland:: BFS(const string& type ,const string& station  , map< string,bool>& reachables ){
  for(const auto& s :o_transport.empty()? transport[type]->graph[station]:o_transport[type].graph[station]){
      if(!reachables[s.first]){
          reachables[s.first] = true;
          BFS(type , s.first , reachables);
      }
  }
}


/**
 * calculating the inbound_outbound stations of station
 * @param station
 * @param func
 */
void Neverland::inbound_outbound(const string& station ,const string& func){
    if(func == "inbound"){ // MAKING OPPOSITE MAP OF TRANSPORTS
        for(auto &v: transport){ // loop on vehicles
            for(auto &s : v.second->graph){ // loop on each station
                for(auto &s1 : s.second){ // loop on each station that connect to s
                    o_transport[v.first].add_node(s1.first , s.first , 0  );
                }
            }
        }
    }
    map< string,bool> is_visited1;
    for(const auto &t : transport){ // loop on vehicles
        BFS(t.first , station , is_visited1  );
        cout << t.first  << ": " ;
        if(!is_visited1.empty()){
            for(const auto& s : is_visited1){
                if(s.first != station){
                    cout  << s.first << "   " ;}
            }}
        else{
            cout << "no outbound travel";
        }
        cout << endl;
        is_visited1.clear();
    }
    if(func == "inbound"){
        o_transport.clear();}
}

void Neverland::set_outputfile(string filename) {
    this->outputfile_name = move(filename);
}

void Neverland::set_multigraph(map < string , Graph<string , int> >& multigraph) {
    for(const auto& s: stations){ //station
        multigraph[s] = {};
        for(const auto& v: transport) { // each vehicle
            if(v.second != nullptr){
                for(const auto& n_s : v.second->graph[s]){ // each station in the vehicle
                    multigraph[s].add_node(v.first, n_s.first, n_s.second);
                }
            }
        }
    }
}

/**
 * get the transit time of some station
 * @param from
 * @param transit_time
 */
void Neverland::get_transit_time(const string& from , int& transit_time) {
    if(from.substr(0 ,2) == "IC") {transit_time = config["intercity"];}
    else if(from.substr(0 ,2) == "CS") {transit_time = config["central"];}
    else {transit_time = config["stad"];}
}


/**
 * get the shortest path between source and destination
 * with any vehicle
 * @param source
 * @param destination
 */
void Neverland:: multiExpress(const string& source ,const string& destination  ){
    map < string , Graph<string , int> > multigraph;
    set_multigraph(multigraph);
    map < string , pair< int , string> >  route;  /// map  < station , {route time from source ,   vehicle  }>

    for(const auto& s : multigraph){
        route[s.first].first = MAX; /// set high distance on all stations
        route[s.first].second = "non"; /// set null from
    }
    route[source].first =  0;
    route[source].second = "source";

    for(const auto& A: multigraph){
    for(const auto& from: multigraph){ /// foreach stations
        for(const auto& v:from.second.graph){ /// foreach vehicle
            for(const auto& to:v.second){/// foreach neighborhood of station from in each vehicle
                if(to.first == source) continue;
                if(route[to.first].second == "non"){ /// if not visited yet
                    route[to.first].second = v.first; /// put curr vehicle
                }
                if(route[from.first].first != MAX){ /// if there is a route from --> to
                    if(route[to.first].second != route[from.first].second && from.first != source) {//// if there switch on the vehicles need to add transit time
                        int transit_time = 0;
                        get_transit_time(from.first , transit_time);
                        if(transit_time + route[from.first].first + to.second  < route[to.first].first ){
                            route[to.first].first = transit_time + route[from.first].first + to.second;
                            route[to.first].second = v.first;
                        }
                    }
                    else{
                        route[to.first].first = min(route[to.first].first , route[from.first].first + to.second +config[route[from.first].second]);
                    }
                }
        }}}}

    if(route[destination].first == MAX) { // not found route
        cout << "route unavailable" << endl;
        return;
    }
    cout << source  <<" --> "<< destination << " = " << route[destination].first   <<endl;
}


/**
 * set station for source and destination
 * @param s1
 * @param s2
 */
void Neverland:: set_station(const string& s1 , const string& s2 ){
    stations.insert(s1);
    stations.insert(s2);
}

/**
 * print the stations set
 */
void  Neverland::print_stations(){
    cout << "STATIONS : \n";
    for(const auto& s:stations){
        cout << s << endl;
    }
}

/**
 * calculate for each vehicle the shortest path
 */
void Neverland::uniExpress(const string& from, const string& to){
    if(stations.find(from) == stations.end()){
        throw NodeNotExistException(from);
    }
    if(stations.find(to) == stations.end()){
        throw NodeNotExistException(to);
    }
    for(auto& v: transport){
        try {
            cout << v.first << " : ";
            map<string, pair<int, bool>> res = dijkstra(*transport[v.first].get(), from, config[v.first]);
            if (res.find(to) != res.end()) {
                cout << from << " --> " << to << " = " << res[to].first - config[v.first] << endl;
            } else {
                cout << "route unavailable" << endl;
            }
        }
        catch(NodeNotExistException &e){
            cout << "route unavailable" << endl;
        }
    }

}


/**
 * get the lowest vertices in the graph
 * @param vertices
 * @return
 */
string get_lowest(map<string, pair<int, bool>> &vertices){
    string s = vertices.begin()->first;
    int n = -1;
    for(const auto& v: vertices){
        if(v.second.first < 0 || v.second.second == true) continue;
        if(v.second.first <= n || n < 0){
            s = v.first;
            n = v.second.first;
        }
    }
    return s;
}


/**
 * implementing the dijkstra algorithm for calculating the shortest path from source
 * including the halt time for every vehicle
 * @param G
 * @param source
 * @param halt_time
 * @return
 */
map<string, pair<int, bool>> Neverland::dijkstra(Graph<string, int>& G, const string& source, const int& halt_time){
    map<string, pair<int, bool>> vertices = create_dijkstra_map(G);
    vertices[source].first = 0;
    for(const auto& v: vertices){
        string lowest = get_lowest(vertices);
        try{
            for(const auto& u: G[lowest]){
                if(vertices[u.first].second == true) continue;
                else if(vertices[u.first].first == -1){
                    vertices[u.first].first = vertices[lowest].first + u.second + halt_time;
                }
                else{
                    int distance = vertices[lowest].first + u.second + halt_time;
                    if(distance < vertices[u.first].first){
                        vertices[u.first].first = distance;
                    }
                }
            }
        }
        catch(DeadEndNodeException& e){
        }
        vertices[lowest].second = true;
    }
    return vertices;
}

/**
 * create a map for dijkstra algorithm <name, total_time, is_checked>
 * @param G
 * @return
 */
map<string, pair<int, bool>> Neverland::create_dijkstra_map(const Graph<string, int>& G){
    map<string, pair<int, bool>> short_graph;
    for(const auto& dest: G.graph){
        short_graph[dest.first] = make_pair(-1, false);
        for(const auto& v: dest.second){
            short_graph[v.first] = make_pair(-1, false);
        }
    }
    return short_graph;
}



