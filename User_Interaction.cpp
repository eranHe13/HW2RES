

#include "User_Interaction.h"

const string INPUT_SIGN = "-i";
const string OUTPUT_SIGN = "-o";
const string CONFIG_SIGN = "-c";
const string BUS ="bus";
const string TRAM = "tram";
const string SPIRNTER = "sprinter";
const string RAIL = "rail";

Neverland N;
vector<vector<string>> data_container;

void set_pNeverland(string file_name){
    set<string> stations;
    /// GET VEHICLE TYPE
    int pos;
    for(int i = 0 ; i < file_name.size() ; i++)
    {
        if( !isalpha(file_name[i])){
            pos = i;
            break;
        }
    }
    string vehicle = file_name.substr(0 , pos);
    for(auto d : data_container){
        N.set_station(d[0] , d[1]);
        N.add_route(d[0] , d[1], stoi(d[2]) , vehicle);
        ///  CRATE OBJECT BY DATA AND TYPE
    }
    data_container.clear();
}

void init_input_file(const string& file){
    check_input_file(file);
    set_pNeverland(file);
}

void initializing(int size, const char** files){
    ///  initializing first files
    vector<string> input;
    for(int i =1 ; i < size; i++){
        input.emplace_back(files[i]);
    }
    try {
        for (int i = 0; i < input.size(); i++) {
            /// INPUT FILES
            if (input[i] == INPUT_SIGN) {
                i++;
                for (int j = i; j < input.size(); j++) {
                    if (input[j] == CONFIG_SIGN || input[j] == OUTPUT_SIGN) break;
                    init_input_file(input[j]);
                    i = j;
                }
            }
                /// CONFIG FILES
            else if (input[i] == CONFIG_SIGN) {
                N.update_config(input[++i]);
            }

                /// OUTPUT FILES
            else if (input[i] == OUTPUT_SIGN) {
                N.set_outputfile(input[++i]);
            }
        }
    }
    catch(NeverlandException& e) { ///////ERROR ON INIT THE PROGRAM , EXIT
        cout << e.what();
        exit(0);
    }
}

void get_input(){
    /// Interaction with user -- simulation control
    string input;
    cout << "LEST BEGIN:" << endl;
    while(input != "exit" ||input != "EXIT"){
        vector<string> details;
        input= "";
        getline(cin , input);
        if(input.empty()) {
            cout << "command not found" << endl;
            continue;
        }
        stringstream s(input);
        istream_iterator<std::string> begin(s);
        istream_iterator<std::string> end;
        vector<std::string> data(begin, end);

        try {
            if (data[0] == "load") {
                /// load details file from user
                init_input_file(data[1]);

            } else if (data[0] == "outbound") {
                ///get all stations which can be reached in a few steps from the requested  start station
                N.inbound_outbound(data[1] , data[0]);

            } else if (data[0] == "inbound") {
                ///get all stations which can be reached in a few steps from the requested  destination station
                N.inbound_outbound(data[1] , data[0]);

            } else if (data[0] == "uniExpress") {
                /// get the shortest route between two stations -- without vehicle replacement
                    if(data.size() != 3){
                        throw NeverlandException("ERROR : need to stations \n");
                    }
                    N.uniExpress( data[1] , data[2]);

            } else if (data[0] == "multiExpress") {
                /// get the shortest route between two stations -- with vehicle replacement
                if(data.size() != 3){
                    throw NeverlandException("ERROR : need to stations \n");
                }
                N.multiExpress(data[1] , data[2]);
                ///TODO - multiExpress
            }
            else if (data[0] == "print") {
                print_to_file(N.outputfile_name);
            }
            else if(data[0] == "EXIT" || data[0] == "exit"){
                break;
            }
            else{
                cout << "commend not found\n";
            }
        }
        catch(NeverlandException& e){
            cerr << e.what() << endl;
        }

    }
}

void print_to_file(const string& out_file){
    ofstream myfile(out_file);
    if (myfile.is_open())
    {
        myfile << N;
    }
    else {
        cerr << "NO" << endl;
    }
}
