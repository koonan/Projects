#include "WriteFile.h"
#include<bits/stdc++.h>
#include <fstream>
using namespace std;

void WriteFile::write(vector<string> listing_file,string path, bool append)
{
    ofstream myfile;
    if(!append){
        myfile.open(path);
    }else{
        myfile.open(path, std::ios_base::app);
    }
    int size_file = listing_file.size() ;
    for(int i = 0 ; i < size_file ; i++)
    {
        myfile << listing_file.at(i);
        myfile << "\n";
    }
    myfile.close();
    return ;
}
