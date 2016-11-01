#include "OperandValidator.h"
#include<regex>
#include<algorithm>
#include <bits/stdc++.h>

using namespace std;


OperandValidator::OperandValidator()
{
    AddPatterns();
}

vector <pair<string,string>> OperandValidator::getExpression()
{
    return expression;

}
void OperandValidator::buildExpression(const string source,int size,int cases)
{

    pair<string,string> temp;
    expression.clear();
    if(size==4)
    {
        std::regex  pattern1("(.{1}\\w+)(.{1})(\\w+)");
        if(cases==0)
        {
            temp.first="HEX";
            temp.second="HEX";
        }
        else if(cases==1)
        {
            temp.first="HEX";
            temp.second="LABEL";

        }

        else if(cases==2)
        {
            temp.first="LABEL";
            temp.second="LABEL";

        }
        else if(cases==3)
        {
            temp.first="LABEL";
            temp.second="HEX";

        }

        expression.push_back(temp);
        std::smatch match;
        if (std::regex_search(source.begin(),source.end(), match, pattern1));
        temp.first=match[1];
        temp.second=match[3];
        expression.push_back(temp);
        temp.first=match[2];
        temp.second="";
        expression.push_back(temp);


    }
    else if(size==3)
    {
        std::regex  pattern2("(\\w+)(.{1})(\\w+)");
        if(cases==0)
        {
            temp.first="LABEL";
            temp.second="HEX";
        }
        else if(cases==1)
        {
            temp.first="HEX";
            temp.second="LABEL";

        }

        else if(cases==2)
        {
            temp.first="HEX";
            temp.second="HEX";

        }
        else if(cases==3)
        {
            temp.first="LABEL";
            temp.second="LABEL";

        }
        expression.push_back(temp);
        std::smatch match;
        if (std::regex_search(source.begin(),source.end(), match, pattern2));
        temp.first=match[1];
        temp.second=match[3];

        expression.push_back(temp);
        temp.first=match[2];
        temp.second="";
        expression.push_back(temp);
    }
    else
    {
        if(cases==0)
        {
            temp.first="LABEL";
            temp.second="HEX";
        }
        else if(cases==1)
        {
            temp.first="LABEL";
            temp.second="LABEL";
        }
        std::regex  pattern2("(\\*)(.{1})(\\w+)");
        expression.push_back(temp);
        std::smatch match;
        if (std::regex_search(source.begin(),source.end(), match, pattern2));
        temp.first=match[1];
        temp.second=match[3];

        expression.push_back(temp);
        temp.first=match[2];
        temp.second="";
        expression.push_back(temp);

    }

}

void OperandValidator::AddPatterns()
{
    //0 Register,Register
    patterns.push_back(regex("((A|X|L|B|S|T|F),(A|X|L|B|S|T|F))"));
    //1 Register,num
    patterns.push_back(regex("(A|X|L|B|S|T|F),[0-6]{1}"));
    //2 egister
    patterns.push_back(regex("(A|X|L|B|S|T|F)"));
    //3 -?Label,X
    patterns.push_back(regex("[-]?(([A-Z](\\w|\\d)*),X)"));
    //4
    patterns.push_back(regex("(@|#)?([-]?([A-Z](\\w|\\d)*))"));
    //5
    patterns.push_back(regex("([-]?([0][0-9A-F]+|\\d+),X)"));
    //6
    patterns.push_back(regex("(@|#)?([-]?([0][0-9A-F]+|\\d+))"));


    //Simple Expression
    // 7 Label op hex
    patterns.push_back(regex("(@|#)?(([A-Z](\\w|\\d)*)[-|+|*|(\\/)](([0][0-9A-F]+)|\\d+))"));

    // 8 Hex op Label
    patterns.push_back(regex("(@|#)?(([0][0-9A-F]+|\\d+)[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));
    // 9 Hex op Hex
    patterns.push_back(regex("(@|#)?(([0][0-9A-F]+|\\d+)[-|+|*|(\\/)]([0][0-9A-F]+|\\d+))"));
    //10 LabeL op Label
    patterns.push_back(regex("(@|#)?(([A-Z](\\w|\\d)*)[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));

    //11 -Hex op Hex
    patterns.push_back(regex("(@|#)?(([-]([0][0-9A-F]+|\\d+))[-|+|*|(\\/)]([0][0-9A-F]+|\\d+))"));
    // 12 -HEX  OP LABEL
    patterns.push_back(regex("(@|#)?(([-]([0][0-9A-F]+|\\d+))[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));
    //13 -LABEL  OP  LABEL
    patterns.push_back(regex("(@|#)?(([-]([A-Z](\\w|\\d)*))[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));
    //14 -LABEL OP   HEX
    patterns.push_back(regex("(@|#)?(([-]([A-Z](\\w|\\d)*))[-|+|*|(\\/)]([0][0-9A-F]+|\\d+))"));

    // 15*
    patterns.push_back(regex("(@|#)?[*]"));
    //16
    patterns.push_back(regex("(@|#)?[*]([+|-]([0][0-9A-F]+|\\d+))"));
    //17
    patterns.push_back(regex("(@|#)?[*]([+|-]([A-Z](\\w|\\d)*))"));

    //18
    patterns.push_back(regex("=C'.+'"));
    //19
    patterns.push_back(regex("=X'[0-9A-F]+'"));
    //20
    patterns.push_back(regex("=W'\\d+'"));
    //21
    patterns.push_back(regex("=\\*"));


}

int OperandValidator::validate(string operation,string operand )
{
    bool match;
    match=0;
    if(operation=="RMO"|| operation=="ADDR" || operation=="MULR" || operation== "DIVR" || operation=="SUBR"||operation=="COMPR" )
    {
        match=0;
        match=regex_match(operand,patterns[0]);
        if(match)
            return 6;
    }
    else if(operation=="SHIFTL" || operation=="SHIFTR")
    {
        match=0;
        match=regex_match(operand,patterns[1]);
        if(match)
            return 7;
    }
    else if(operation=="TIXR" || operation=="CLEAR")
    {
        match=0;
        match=regex_match(operand,patterns[2]);
        if(match)
            return 5;
    }


    else
    {

        if(operand.at(0)== '@' ||operand.at(0)== '#' )
        {
            operand.erase (operand.begin());
        }
        for(int i =3 ; i<15; i++)
        {

            match=0;
            match=regex_match(operand,patterns[i]);
            if(match && ( i==4 || i==3))
            {
                return 1;
            }
            else if(match && (i==5 || i==6))
            {
                return 2;
            }
            else if(match)
            {

                if(i==11 || i==12 || i==13 || i==14)
                {
                    buildExpression(operand,4,i%11);
                }
                else
                {
                    buildExpression(operand,3,i%7);

                }
                return 3;
            }
        }
        match=0;
        match=regex_match(operand,patterns[15]);
        if(match)
        {
            return 1;
        }
        match=0;
        match=regex_match(operand,patterns[16]);
        if(match)
        {

            buildExpression(operand,2,0);
            return 3;

        }
        match=regex_match(operand,patterns[17]);
        if(match)
        {

            buildExpression(operand,2,1);
            return 3;

        }
        for(int i =18  ; i <patterns.size(); i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match)
            {
                return 0;
            }
        }


    }
    return -1;
}
