#ifndef READFILE_H
#define READFILE_H
#include<bits/stdc++.h>


class ReadFile
{
    public:
        ReadFile(std::string p);
        std::vector<std::string> read();
    protected:
    private:
        std::string path ;
        bool checkSpace(std::string line);

};

#endif // READFILE_H
