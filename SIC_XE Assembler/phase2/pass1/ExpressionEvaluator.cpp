#include "ExpressionEvaluator.h"

ExpressionEvaluator::ExpressionEvaluator()
{

}

ExpressionEvaluator::~ExpressionEvaluator()
{

}
int ExpressionEvaluator::fromHexToDecimal(string hex)
{
    unsigned int result ;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> result;
    return static_cast<int>(result);
}
int ExpressionEvaluator :: getLabelValue(bool firstOperandSign, string first_term, string locctr,map <string,vector<string> > symTable )
{

    if(first_term=="*")
    {
        first_term = locctr ;
        if(firstOperandSign)
        {
            countReloc -- ;
        }
        else
        {
            countReloc ++ ;
        }

    }

    else if(symTable.find(first_term)!=symTable.end())
    {
        if(symTable[first_term][1] == "RELOCATABLE")
        {
            if(firstOperandSign)
            {
                countReloc -- ;
            }
            else
            {
                countReloc ++ ;
            }

        }
        first_term =symTable[first_term][0] ;
    }
    else
    {
        return -1;
    }
    if(firstOperandSign)
    {
        first_term = "-" + first_term ;
    }
    return fromHexToDecimal(first_term) ;

}
string ExpressionEvaluator::fromDecimalToHex(int num)
{
    stringstream stream;
    stream << std::hex << num;
    string result( stream.str());
    transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}
string ExpressionEvaluator ::evaluateExpression( vector<pair<string,string> > expression, string locctr, map <string,vector<string> > symTable)
{
    countReloc = 0 ;
    negativeFlag = false ;
    string sign = expression[2].first ;
    string first_term = expression[1].first;
    string second_term = expression[1].second;
    bool firstOperandSign = false ;
    bool secondOperandSign = false ;

    int first_num ;
    int second_num ;


    if(sign=="-"){
        secondOperandSign = true ;
    }
    if(expression[0].first == "HEX")
    {
        first_num = fromHexToDecimal(first_term) ;
    }
    else if(expression[0].first == "LABEL")
    {
        if(first_term.at(0)=='-')
        {
            first_term = first_term.substr(1,first_term.length());
            firstOperandSign = true ;
        }

        first_num = getLabelValue(firstOperandSign, first_term, locctr, symTable) ;
        if(first_num == -1)
        {

            return "ERROR";
        }

    }


    if(expression[0].second == "HEX")
    {
        if(secondOperandSign){
            second_term = "-" + second_term ;
        }
        second_num = fromHexToDecimal(second_term) ;

    }
    else if(expression[0].second == "LABEL")
    {
        second_num = getLabelValue(secondOperandSign, second_term, locctr, symTable);
        if(second_num == -1)
        {
            return "ERROR";
        }
    }

    if((countReloc == 2 || countReloc == -1 || countReloc == -2 ) || (countReloc == 1 && sign == "*") || (countReloc == 1 && sign == "/"))
    {
        return "ERROR" ;
    }
    if(sign == "+" || sign == "-")
    {
        int result = first_num + second_num ;
        if(result < 0){
            negativeFlag = true ;
        }
        address = fromDecimalToHex(result);
        if(countReloc == 0)
        {
            return "ABSOLUTE" ;
        }
        else
        {
            return "RELOCATABLE";
        }
    }
    else if(sign == "*")
    {
        int result = first_num * second_num ;
        if(result < 0){
            negativeFlag = true ;
        }
        address = fromDecimalToHex(result);
        return "ABSOLUTE" ;
    }
    else if(sign == "/")
    {
        int result = first_num / second_num ;
        if(result < 0){
            negativeFlag = true ;
        }
        address = fromDecimalToHex(result);
        return "ABSOLUTE" ;
    }
}
