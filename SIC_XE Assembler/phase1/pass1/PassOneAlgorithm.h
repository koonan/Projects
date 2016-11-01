#ifndef PASSONEALGORITHM_H
#define PASSONEALGORITHM_H
#include <bits/stdc++.h>
#include <sstream>
using namespace std;


class PassOneAlgorithm
{
   public:
    // void setLength( double len );
    vector<string> take_source(vector<vector<string> > source_file);
    string form_line(vector<string> line);
    string convert_to_string(int number);
    bool is_number(string s);
    int is_hex(string s);
    bool is_valid_operand(string s);
    PassOneAlgorithm();  // This is the constructor
    string program_lenght;
    map<string,string> symbol_table;
   std::string errorStr = "NULL";


private:
    int location_counter  = 0;
    int start_address = 0;
    bool existError = false ;
    bool exist_end = false;
    vector<string> lisiting_file;
protected:
};



#endif // PASSONEALGORITHM_H
