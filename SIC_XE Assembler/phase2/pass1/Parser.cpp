#include "Parser.h"
#include "OpcodeGenerator.h"
#include "WriteFile.h"
#include "ReadFile.h"
#include "PassOneAlgorithm.h"
#include <bits/stdc++.h>
#include<regex>
using namespace std;

Parser::Parser()
{
}

vector <vector <string> > Parser::getStatements()
{
    return statements;
}

std::vector<std::string> Parser::split(const std::string &s)
{
    std::istringstream iss(s);
    std::vector<std::string> result
    {
        std::istream_iterator<std::string>(iss), {}
    };
    return result;
}

// Get all the whole operand if separated by white spaces
std::string Parser::getOperand(const std::string source, std::regex rgx,int index)
{
    std::smatch match;
    if (std::regex_search(source.begin(),source.end(), match, rgx));
    string temp = match[index];
    struct RemoveDelimiter
    {
        bool operator()(char c)
        {
            return (c =='\r' || c =='\t' || c == ' ' || c == '\n');
        }
    };

    temp.erase( std::remove_if( temp.begin(), temp.end(), RemoveDelimiter()), temp.end());
    return temp;
}
bool Parser::isDirective (string str)
{
    return binary_search(directives.begin(),directives.end(),str);
}
bool Parser::isUncheckdDir (string str)
{
    return binary_search(unCheckedDirs.begin(),unCheckedDirs.end(),str);
}
bool Parser::isMnemonic  (string str)
{
    if (str[0]=='+')
    {
        str = str.substr(1,str.length()-1);
    }
    return (optable.find(str)!=optable.end());
}



void Parser::parse(string path,std::map <std::string,std::vector<std::string> > optable)
{

    this-> optable = optable;
    ReadFile reader(path);
    sourceCode = reader.read();
    getPatterns();
    bool extraOperand;
    for (int i=0; i<sourceCode.size(); i++)
    {
        temp="";
        std::istringstream line(sourceCode[i]);
        string start;
        line>>start;
        vector<std::string> statement(7);
        std::fill(statement.begin(),statement.end(),"NULL");
        if (start[0]=='.')
        {
            if (sourceCode[i].length()>1)
            {
                string word;
                while(line>>word)
                {
                    start+=" " +word;
                }
                // all of the line is a comment
            }
            statement[5]=start;
        }
        else
        {
            vector<std::string> elements = split(sourceCode[i]);
            int sz = elements.size();
            bool isInstruction,isDir,isUncheckDir;
            isInstruction=isDir=isUncheckDir=false;
            int pos = -1;
            if (sz>=1&&isMnemonic(elements[0]))
            {
                isInstruction=true;
                if (sz>=2&&isMnemonic(elements[1]))pos=1;
                else pos=0;

            }
            else if (sz>1&&isMnemonic(elements[1]))
            {
                isInstruction=true;
                pos=1;
            }
            else if (sz>=1&&isDirective(elements[0]))
            {
                isDir=true;
                pos=0;
            }
            else if(sz>1&&isDirective(elements[1]))
            {
                isDir=true;
                pos=1;
            }
            else if (sz>=1&&isUncheckdDir(elements[0]))
            {
                isUncheckDir=true;
                pos=0;
            }
            else if(sz>1&&isUncheckdDir(elements[1]))
            {
                isUncheckDir=true;
                pos=1;
            }

            if (pos==-1)
            {
                std::istringstream line(sourceCode[i]);
                string comment="",str;
                int i=0;
                while(line>>str)
                {
                    if (i<3)
                        statement[i++] = str;
                    else
                        statement[2] = " "+str;
                }
                statement[4]="      **Invalid Statement";
            }
            else
            {
                // label
                if (pos==1)
                {
                    if(!validateLabel(elements[0]))
                    {
                        statement[4]="      **Illegal label name";
                    }

                    sz--;
                    statement[0]= elements[0];
                    elements.erase (elements.begin());
                }
                // format

                if (isInstruction)
                {
                    if (elements[0][0]=='+')
                    {
                        string str =  elements[0].substr(1,elements[0].length()-1);
                        if ((optable[str].at(0))=="2")
                        {

                            statement[3]="2";
                            statement[4]="      **Can not be format 4 instruction";
                        }
                        else
                            statement[3]="4";
                    }
                    else
                        statement[3]=optable[elements[0]].at(0);
                }
                else if (isDir||isUncheckDir)
                    statement[3]="directive";
                // format
                // validation safaa
                if(pos==1 && sz>=2)
                {
                    std::regex  pattern1("\\s*(\\S+)\\s+(\\S+)\\s+(.+)");
                    if( regex_match(sourceCode[i],pattern1))
                    {
                        temp=getOperand(sourceCode[i],pattern1,3);
                        statement[2]=temp;
                    }
                }
                else if(pos==0 && sz>=1)
                {
                    std::regex  pattern2("\\s*(\\S+)\\s+(.+)");
                    if(regex_match(sourceCode[i],pattern2))
                    {

                        temp=getOperand(sourceCode[i],pattern2,2);
                        statement[2]=temp;
                    }
                }
                // validation safaa
                //// missing field
                extraOperand = false;
                if ((isInstruction&&(elements[0]!="RSUB"&&elements[0]!="+RSUB")&&sz==1)||(isDir&&sz==1&&elements[0]!="END"&&elements[0]!="LTORG")||
                        (isUncheckDir&&elements[0]!="USE"&&elements[0]!="CSECT"&&sz==1))
                {
                    statement[4]="      **missing operand field";
                }
                else if (elements[0]=="CSECT"&&pos==0)
                    statement[4]="      **missing control section field";
                // extra characters at end of statement
                else if(((elements[0]=="RSUB"||elements[0]=="+RSUB"||elements[0]=="CSECT"||elements[0]=="LTORG")&&sz>1))
                {
                    //statement[4]="      **Warning::extra characters at end of statement";
                    extraOperand = true;
                }
                // safaa valid operand
                if (sz>=2&&!extraOperand)
                {
                    // here
                    statement[2]=temp;

                    if(isDir && (elements[0]=="BYTE" || elements[0]=="WORD") )
                    {

                        if(!validateOperand(temp,elements[0]))
                        {
                            statement[4]="      **unrecognized operand ";
                        }
                    }
                    else if(isDir && (elements[0]=="ORG" || elements[0]=="EQU"))
                    {
                        if(validateOperand2(temp,elements[0])!=-1)
                        {
                            stringstream ss;
                            ss <<validateOperand2(temp,elements[0]) ;
                            string str =ss.str();
                            statement[6]= str;
                        }
                        else
                        {
                            statement[4]="      **unrecognized operand ";

                        }

                    }
                    else
                    {
                        if(validateOperand2(temp,elements[0])!=-1)
                        {
                            statement[6]="12";
                        }

                    }


                }
                // safaa
                statement[1]=elements[0];
                if (isUncheckDir)
                {
                    statement[4]="**    Warning:Unsupported Directive  **";
                }
            }
        }
        if (extraOperand){
            vector<std::string> warning(7);
            warning[5]="      ** Warning::extra characters at end of statement '"+statement[1]+"'doesn't have operand field";;
            statements.push_back (warning);
        }
        statements.push_back (statement);

    }
}
bool Parser::validateOperand(string operand,string menomenic )
{
    bool match;
    match=0;
    if(menomenic=="BYTE")
    {
        for(int i =0 ; i <2 ; i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match)
                break;
        }
    }
    else
    {
        for(int i =2 ; i <5; i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match)
                break;
        }
    }
    return match && !(isMnemonic(operand)||isDirective(operand)||isUncheckdDir(operand));
}
int Parser::validateOperand2(string operand,string menomenic )
{
    bool match;
    match=0;
    if(menomenic=="ORG")
    {
        for(int i =4 ; i <9 ; i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match && i==4)
                return 1;
            else if (match)
            {
                return 2;
            }
        }
        for(int i=11 ; i<19 ;i++){
            match=0;
            match=regex_match(operand,patterns[i]);
             if (match)
            {
                return 2;
            }
        }
        match=regex_match(operand,patterns[23]);
        if(match)
            return 13;
        match=regex_match(operand,patterns[24]);
        if(match)
            return 2;
    }
    else if(menomenic=="EQU")
    {

        for(int i =9; i <11 ; i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match)
                return i-9;
        }
        match=regex_match(operand,patterns[23]);
        if(match)
            return 13;
        match=regex_match(operand,patterns[24]);
        if(match)
        {
            return 2;
        }
        for(int i =11; i <19 ; i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match)
                return 2;
        }
    }
    else
    {
        for(int i =19 ; i <patterns.size()-2 ; i++)
        {
            match=0;
            match=regex_match(operand,patterns[i]);
            if(match)
                return 1;
        }
    }
    return -1;
}
void Parser::getPatterns()
{
    //0
    patterns.push_back(regex("X'[0-9A-F]+'"));
    //1
    patterns.push_back(regex("C'.+'"));
    //2
    patterns.push_back(regex("((\\d+)(,(\\d+))*)"));
    //3
    patterns.push_back(regex("([-](\\d+)|(\\d+))"));
    //org
    //4
    patterns.push_back(regex("(@|#)?([A-Z](\\w|\\d)*)"));
    //5
    patterns.push_back(regex("([A-Z](\\w|\\d)*)[-]([0][0-9A-F]+|\\d+)"));
    //6
    patterns.push_back(regex("([A-Z](\\w|\\d)*)[+]([0][0-9A-F]+|\\d+)"));
    //7
    patterns.push_back(regex("([0][0-9A-F]+|\\d+)[+][A-Z](\\w|\\d)*"));
    //8
    patterns.push_back(regex("([-]([0][0-9A-F]+|\\d+))[+]([A-Z](\\w|\\d)*)"));

    //9
    patterns.push_back(regex("(@|#)?([-]?([A-Z](\\w|\\d)*))"));
    //10
    patterns.push_back(regex("(@|#)?([-]?([0][0-9A-F]+|\\d+))"));

    patterns.push_back(regex("(@|#)?(([A-Z](\\w|\\d)*)[-|+|*|(\\/)](([0][0-9A-F]+)|\\d+))"));

    // 12 Hex op Label
    patterns.push_back(regex("(@|#)?(([0][0-9A-F]+|\\d+)[-|+|*|(\\/)][A-Z](\\w|\\d)*)"));
    // 13 Hex op Hex
    patterns.push_back(regex("(@|#)?(([0][0-9A-F]+|\\d+)[-|+|*|(\\/)]([0][0-9A-F]+|\\d+))"));
    //14 LabeL op Label
    patterns.push_back(regex("(@|#)?(([A-Z](\\w|\\d)*)[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));

    //15 -Hex op Hex
    patterns.push_back(regex("(@|#)?(([-]([0][0-9A-F]+|\\d+))[-|+|*|(\\/)]([0][0-9A-F]+|\\d+))"));
    // 16 -HEX  OP LABEL
    patterns.push_back(regex("(@|#)?(([-]([0][0-9A-F]+|\\d+))[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));
    //17 -LABEL  OP  LABEL
    patterns.push_back(regex("(@|#)?(([-]([A-Z](\\w|\\d)*))[-|+|*|(\\/)]([A-Z](\\w|\\d)*))"));
    //18 -LABEL OP   HEX
    patterns.push_back(regex("(@|#)?(([-]([A-Z](\\w|\\d)*))[-|+|*|(\\/)]([0][0-9A-F]+|\\d+))"));


    // literal
    //19
    patterns.push_back(regex("=C'.+'"));
    //20
    patterns.push_back(regex("=X'[0-9A-F]+'"));
    //21
    patterns.push_back(regex("=W'\\d+'"));
    //22
    patterns.push_back(regex("=\\*"));
    //23
    patterns.push_back(regex("(@|#)?[*]"));
    //24
    patterns.push_back(regex("(@|#)?([*](([-|+][0-9A-F]+)|([-|+]([A-Z](\\w|\\d)*))))"));
}
bool Parser::validateLabel(string label)
{
    //should start with any char except digits
    regex pattern("([A-Z](\\w*|\\d*))");
    regex pattern2("(A|X|L|B|S|T|F)");
    return regex_match(label,pattern)&&!regex_match(label,pattern2)&&!(isMnemonic(label)||isDirective(label)||isUncheckdDir(label));
}
