#ifndef OPCODEGENERATOR_H
#define OPCODEGENERATOR_H
#include<bits/stdc++.h>


class OpcodeGenerator
{
    public:
        OpcodeGenerator();
        std::map <std::string, std::vector<std::string> > getOpcode() ;
    protected:
    private:
        std::map <std::string, std::vector<std::string> > optable ;
};

#endif // OPCODEGENERATOR_H
