#ifndef OPERANDVALIDATOR_H
#define OPERANDVALIDATOR_H
#include<iostream>
#include<vector>
#include<regex>
#include<algorithm>
using namespace std;

class OperandValidator
{
public:
    OperandValidator();
    void AddPatterns();
    int validate(std::string operation, std::string operand );
    vector <pair<string,string>> getExpression();
protected:

private:
    void buildExpression(const string source,int size,int cases);
    vector <regex> patterns;
    vector <pair<string,string>> expression;
};
#endif // OPERANDVALIDATOR_H
