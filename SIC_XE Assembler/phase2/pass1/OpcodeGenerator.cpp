#include "ReadFile.h"
#include<bits/stdc++.h>
#include "OpcodeGenerator.h"

OpcodeGenerator::OpcodeGenerator()
{

    ReadFile read("optable.txt");
    std::vector<std::string> data = read.read();
    for(int i = 0 ; i <data.size() ; i++){
           std::string mnemonic , format , obcode;
           std::vector<std::string> optableData;
           std::istringstream line(data[i]);
           line>> mnemonic >> format >> obcode ;
           optableData.push_back(format);
           optableData.push_back(obcode);
           optable.insert(std::pair<std::string, std::vector<std::string> >(mnemonic , optableData));
    }
}
std::map <std::string, std::vector<std::string> >OpcodeGenerator:: getOpcode(){

  return optable ;
}
