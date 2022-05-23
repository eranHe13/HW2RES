//
// Created by ypinhas on 5/14/2022.
//

#include <fstream>
#include "Neverland.h"

const int MAX = 1000000;


Neverland:: Neverland(): outputfile_name("output.dat"){
    transport.insert({"bus", nullptr});
    transport.insert({"rail", nullptr});
    transport.insert({"tram", nullptr});
    transport.insert({"sprinter", nullptr});
}

void Neverland:: add_route(const string& from, const string& to, unsigned int time, const string& type){
    if(transport[type] == nullptr){
        transport[type] = std::make_shared<Graph<string, int>>();
    }
    transport[type]->add_node(from, to, time);
}

ostream& operator<<(ostream& os, const Neverland& N){
    for(const auto& m : N.transport){
        os << endl;
        cout << m.first << "\n" << *(m.second.get())<< "\n";
    }
    return os;
}

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
        cout << type << " " << config[type] << endl;
    }
    my_file.close();

}

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








void Neverland:: set_multigraph(const string& source ,const string& destination  ){
    /// TODO --- SET MAP : KEYS = STATIONS , VALUE = VECTOR[4](VEHICLES)
    /// MAP
    ///STATION -> (BUS  , { (STATION , DIST)  }   )
    ///        -> (RAIL  , { (STATION , DIST)  }   )
    ///        -> (SPRINTER  , { (STATION , DIST)  }   )
    ///        -> (TRAM  , { (STATION , DIST)  }   )
    map < string , Graph<string , int> > multigraph;
    for(const auto &vehicle : transport){
        for(const auto& graph: vehicle.second->graph){
            for(const auto& vec: graph.second){
                multigraph[graph.first].add_node(vehicle.first, vec.first, vec.second);
            }
        }
    }
    for(const auto& h: multigraph){

        cout<< h.first<< endl << h.second << endl;
    }




}

void Neverland:: set_station(const string& s1 , const string& s2 ){
    stations.insert(s1);
    stations.insert(s2);
}



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
    if(stations.find(from) == stations.end() || stations.find(to) == stations.end()){
        throw NodeNotExistException(from);
    }
    for(auto& v: transport){
        try {
            cout << v.first << " : ";
            map<string, pair<int, bool>> res = dijkstra(*transport[v.first].get(), from, config[v.first]);
            if (res.find(to) != res.end()) {
                cout << from << "--> " << to << " = " << res[to].first - config[v.first] << endl;
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
 * get the lowest element in the map
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
 * implementing the dijkstra algorithm to the graph
 * @param G
 * @param source
 */
map<string, pair<int, bool>> Neverland::dijkstra(Graph<string, int>& G, const string& source, const int& halt_time){
    map<string, pair<int, bool>> vertices = create_dijkstra_map(G);
    vertices[source].first = 0;
    for(const auto& v: vertices){
        string lowest = get_lowest(vertices);
        try{
//            vector<pair<string, int>>& lowest_node = G[lowest];
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
//            vertices[lowest].second = true;
//            continue;
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








