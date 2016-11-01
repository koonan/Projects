#include "PassOneAlgorithm.h"
#include <iostream>
#include "OpcodeGenerator.h"
#include "WriteFile.h"
#include "Parser.h"
#include<bits/stdc++.h>

using namespace std;
int main(int argc, char* const argv[])
{
    string path ;
    path = argv[1];
    Parser parser;
    parser.parse(path);
    vector <vector <string> > statements = parser.getStatements();
    //=========================================
    if (statements.size()!=0) {

        PassOneAlgorithm pass ;
        vector<string> list_file =  pass.take_source(statements);
        WriteFile write;
        write.write(list_file);
    }else cout << "file is empty" <<endl;
//=========================================================

    return 0;
}


