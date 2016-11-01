#ifndef PASSTWOALGORITHM_H
#define PASSTWOALGORITHM_H
#include<bits/stdc++.h>
#include "OperandValidator.h"
#include "ExpressionEvaluator.h"

using namespace std;
class PassTwoAlgorithm
{
    public:
        PassTwoAlgorithm(vector <vector<string> > statements,map <string,vector<string> > optable,map <string,vector<string> > symTable,map <string,vector<string> > literalTable,string progName,string progLen);
        void generateObjCode();
    protected:
    private:
        ExpressionEvaluator validator ;
        OperandValidator evaluator;
        vector<string> listing ;
        string progName , progLen;
        int maxRecordLen = 30;
        string record = "";
        int recordLenCnt=0;
        string address;
        map<string ,string> binaryToHex;
        vector <vector<string> > statements;
        map <string,vector<string> > optable ;
        map <string,vector<string> > symTable;
        map <string,vector<string> > literalTable;
        vector<string> objectcode ;
        map<char,char> registers ;
        string errorLine = "";
        string form_line_pass2(vector<string> line , string opcode) ;
        std::vector<string>directives = {"BASE","CSECT","END","EQU","EXTDEF","EXTREF","LTORG","ORG","START","USE"};
        vector <string> MRecords;
        /////////////////////////////////////////////////
        void initNewRecord(string startingAdd);
        void getHexTable();
        string completeHexAddress(string hexadd,int len);
        void updateRecord(string hex,int format,string startingAdd);
        void getRegistersTable();
        string charToHex(string operand);
};
#endif // PASSTWOALGORITHM_H
