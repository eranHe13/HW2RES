#include <iostream>
#include "User_Interaction.h"



int main(int argc , const char** argv) {
    try {
        if(argc<2){
            cerr << "ERROR: need one file to initializing";
            exit(-1);
        }
        initializing(argc , argv);
        get_input();
    }
    catch (NeverlandException& e){
        cerr << e.what();
    }

    return 0;
}

