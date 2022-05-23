#ifndef HW2RES_GRAPH_H
#define HW2RES_GRAPH_H
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>
#include "exceptions.cpp"

using namespace std;

template <typename V, typename E>
class Graph {
public:
    std::map<V, vector<pair<V, E>>> graph; // from: to, time
    Graph(): graph(){};
    ~Graph()= default;

    void add_node(V from, V to, E time);
    void update_node(V from, pair<V, E> p);
    bool is_from_exist(V node);
    bool is_to_exist(V node);

    vector<pair<V, E>>& get_node(V station_name);
    vector<pair<V, E>>& operator[](V station_name);
    friend ostream& operator<<(ostream& os, const Graph<V, E>& graph)
    {
        for(auto const g: graph.graph){
            for(auto v: g.second){
                os << g.first << " --> " << v.first <<" " << v.second<< endl;
            }
            os << "\n";
        }
        return os;
    }

};

template <typename V, typename E>
void Graph<V,E>::add_node(V from, V to, E time){
    pair<V, E> p = make_pair(to, time);
    auto a = graph.find(from);
    if(a == graph.end()){
        graph.insert({from, vector<pair<V, E>>{p}});
    }
    else{
        update_node(from, p);
    }
}

template <typename V, typename E>
void Graph<V,E>::update_node(V from, pair<V, E> p){
    for(auto& e: graph[from]){
        if(e.first == p.first){
            e.second = e.second < p.second ? e.second: p.second; // the lower
            return;
        }
    }
    graph[from].emplace_back(p); // if dest not found
}

/**
     * checking if the node is exist in the graph
     * @param node
     * @return bool
     */
template <typename V, typename E>
bool Graph<V, E>::is_from_exist(V node){
    return graph.find(node) != graph.end();
}

/**
 * get the row of the wanted node
 * @tparam V
 * @tparam E
 * @param station_name
 * @return
 */
template <typename V, typename E>
vector<pair<V, E>>&  Graph<V,E>::get_node(V station_name){
    if(graph.find(station_name) == graph.end()){
        if(!is_to_exist(station_name)) {
            throw NodeNotExistException(station_name);
        }
        throw DeadEndNodeException();
    }
    return graph[station_name];
}

/**
 * for each row in the graph search for destination node
 * @tparam V
 * @tparam E
 * @param to
 * @return
 */
template<typename V, typename E>
bool Graph<V, E>::is_to_exist(V to) {
    for(pair<V, vector<pair<V, E>>> n: graph){
        auto& iter = n.second;
        auto node = find_if(iter.begin(), iter.end(), [to](pair<V, E> p){return p.first == to;});
        if(node != iter.end()) return true;
    }
    return false;
}

template<typename V, typename E>
vector<pair<V, E>> &Graph<V, E>::operator[](V station_name){
    return get_node(station_name);
}

#endif