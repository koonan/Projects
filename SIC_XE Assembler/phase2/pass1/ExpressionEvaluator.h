#ifndef EXPRESSIONEVALUATOR_H
#define EXPRESSIONEVALUATOR_H
#include<bits/stdc++.h>

using namespace std;
class ExpressionEvaluator
{
    public:
        ExpressionEvaluator();
        virtual ~ExpressionEvaluator();
        int countReloc=0;
        string address = "";
        string evaluateExpression( vector<pair<string,string> > expression, string locctr , map <string,vector<string> > symTable);
        int getLabelValue(bool firstOperandSign , string first_term , string locctr , map <string,vector<string> > symTable);
        int fromHexToDecimal(string hex);
        string fromDecimalToHex(int num);
        bool negativeFlag = false;

    protected:

    private:

};

#endif // EXPRESSIONEVALUATOR_H
