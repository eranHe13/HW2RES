

#ifndef HW2RES_CONFIGURATION_CPP
#define HW2RES_CONFIGURATION_CPP
#include <map>
#include <string>
#include <iostream>

using namespace std;
struct Configuration {
    map<string , int> c ={{"bus" , 1},{"tram", 2}, {"sprinter" ,  3},
                          { "rail", 5 }, { "intercity", 15 },
                          { "central", 10 },{ "stad", 5 }};

    const int& operator[] (const string& key) const {
        return c.at(key);
    }
    int& operator[] ( const string& key ){
        return c[key] ;
    }
};


#endif //HW2RES_CONFIGURATION_CPP
