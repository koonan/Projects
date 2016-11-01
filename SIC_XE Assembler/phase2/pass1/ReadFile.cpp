#include "ReadFile.h"
#include<fstream>
#include<bits/stdc++.h>


using namespace std;

ReadFile::ReadFile(std::string p)
{
      path = p ;
}

vector<string> ReadFile::read()
{

    string line;
    ifstream infile;
    // remember that
    infile.open(path.c_str(),ios::in);
    vector<string> data ;
    while (getline(infile, line))
    {
        if(line.length()!=0 && !checkSpace(line)){
            transform(line.begin(), line.end(), line.begin(), ::toupper);
            data.push_back(line);
        }
    }
    return data ;
}
bool ReadFile::checkSpace(string line){
    regex pattern("(\\s*)");
    bool match = regex_match(line,pattern);
    return match;
}


