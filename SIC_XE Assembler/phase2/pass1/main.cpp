#include "PassOneAlgorithm.h"
#include <iostream>
#include "OpcodeGenerator.h"
#include "WriteFile.h"
#include "Parser.h"
#include "PassTwoAlgorithm.h"
#include<bits/stdc++.h>
using namespace std;

int main(int argc, char* const argv[])
{
    string path ;
   // cin>>path;
    path = argv[1] ;
    std::map <std::string,std::vector<std::string> > optable;
    OpcodeGenerator generator ;
    optable = generator.getOpcode();
    Parser parser;
    parser.parse(path,optable);
    vector <vector <string> > statements = parser.getStatements();

    //=========================================
    if (statements.size()!=0) {

        PassOneAlgorithm pass ;
        vector<string> list_file =  pass.take_source(statements);
        WriteFile write;
        write.write(list_file,"ListingFile.txt",false);
        if (!pass.existError){
            PassTwoAlgorithm passTwo (pass.pass2,optable,pass.symbol_table,pass.literal_table,pass.start_label,pass.program_lenght);
            passTwo.generateObjCode();
    }
//=========================================================
}
else cout << "file is empty" <<endl;
return 0;
}

