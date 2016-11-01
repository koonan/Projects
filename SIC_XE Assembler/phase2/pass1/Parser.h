#include "ReadFile.h"
#include "OpcodeGenerator.h"
#include "WriteFile.h"
#include "PassOneAlgorithm.h"


#ifndef PARSER_H
#define PARSER_H

using namespace std;

class Parser
{
public:
    Parser();
    void parse(string path,std::map <std::string,std::vector<std::string> > optable);
    vector <vector <string> > getStatements();
protected:
private:
    PassOneAlgorithm passOne ;
    string temp;
    bool isDirective (string str);
    bool isUncheckdDir (string str);
    bool isMnemonic  (string str);
    vector<regex>patterns;
    void getPatterns();
    bool validateOperand(std::string operand,std::string menomenic);
    int validateOperand2(std::string operand,std::string menomenic);
    std::map <std::string,std::vector<std::string> > optable;
    bool validateLabel(std::string label);


    vector <string> sourceCode;
    vector <vector <string> > statements;
    std::vector<std::string> split (const std::string &s);
    std::string getOperand(const std::string source,std::regex rgx,int index);
    std::vector<string>directives = {"BYTE","END","EQU","LTORG","ORG","RESB","RESW","START","WORD"};
    std::vector<string>unCheckedDirs = {"BASE","CSECT","EXTDEF","EXTREF","USE"};
    std::vector<std::string>registers = {};
};

#endif // PARSER_H
