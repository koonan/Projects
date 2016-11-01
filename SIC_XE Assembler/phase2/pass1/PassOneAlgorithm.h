
#ifndef PASSONEALGORITHM_H
#define PASSONEALGORITHM_H
#include <bits/stdc++.h>
#include "ExpressionEvaluator.h"
#include "OperandValidator.h"
#include <sstream>
using namespace std;


class PassOneAlgorithm
{
   public:
    // void setLength( double len );
    bool generalError = false;
    int counter_literal =1 ;
    vector<string> split(const string &s , const string ch );
    string isInSymbolTable (string s);
    vector<string> take_source(vector<vector<string> > source_file);
    string form_line(vector<string> line);
    void print_literals(void);
    string convert_to_string(int number);
    bool is_number(string s);
    int is_hex(string s);
    bool is_valid_operand(string s);
    PassOneAlgorithm();  // This is the constructor
    string start_label= "";
    string program_lenght;
    string get_value (string s);
    map<string,vector<string> > symbol_table;
    map<string,vector<string> > literal_table;
    vector <vector <string> > pass2 ;
    std::string errorStr = "NULL";
        bool existError = false ;
            ExpressionEvaluator evaluator;
     OperandValidator validator;

private:
    int location_counter  = 0;
    int start_address = 0;
     vector <pair<string,string>> expression;
      string type_label;
    bool exist_end = false;
    vector<string> lisiting_file;
protected:
};
#endif // PASSONEALGORITHM_H
