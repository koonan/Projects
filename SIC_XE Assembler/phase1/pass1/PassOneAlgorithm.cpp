#include "PassOneAlgorithm.h"
#include <bits/stdc++.h>
#include <sstream>
using namespace std;

PassOneAlgorithm::PassOneAlgorithm()
{
    existError = false;
}
// Member functions definitions including constructor
string PassOneAlgorithm::convert_to_string(int number)
{
    stringstream ss;
    ss << hex <<uppercase <<number;
    string location_c = ss.str();
    return location_c ;
}
int PassOneAlgorithm::is_hex(string s)
{

    if(s.at(0) != 48)
    {
        return -1;
    }
    for(int i = 1 ; i < s.size() ; i++)
    {
        if((!(s.at(i) >= 48 && s.at(i) <= 57)) &&  (!(s.at(i) >= 65 && s.at(i) <= 70)) && (!(s.at(i) >= 97 && s.at(i) <= 102)))
        {
            return -1;
        }
    }

    stringstream str;
    str << s;
    int value;
    str >> std::hex >> value;
    return value;
}

bool PassOneAlgorithm::is_number(string s)
{
    for(int i = 0 ; i < s.size(); i++)
    {
        if(!isdigit(s.at(i)))
        {
            return false;
        }
    }
    return true;
}

string PassOneAlgorithm::form_line( vector<string>  line )
{
    string new_line = "";
    string location_c = convert_to_string(location_counter);
    for(int i = 0 ; i < 6-location_c.size() ; i++)
    {
        new_line +="0";
    }
    new_line += location_c +" ";
    if(line[0].compare(errorStr) == 0)
    {
        new_line += "          ";
    }
    else
    {
        new_line += line[0];
        for(int i = line[0].size() ; i < 10 ; i++)
        {
            new_line +=" ";
        }
    }
    if(line[1].compare(errorStr) == 0)
    {
        new_line += "        ";
    }
    else
    {
        new_line += line[1];
        for(int i = line[1].size() ; i < 9 ; i++)
        {
            new_line +=" ";
        }
    }

    if(line[2].compare(errorStr) == 0)
    {
        new_line += "        ";
    }
    else
    {
        new_line += line[2];
        for(int i = line[2].size() ; i < 9 ; i++)
        {
            new_line +=" ";
        }
    }
    return new_line;
}

vector<string> PassOneAlgorithm::take_source( vector<vector<string> > source_file )
{
    int i = 0;
    vector<string> line = source_file[i];
    while(line[5].compare(errorStr) != 0)
    {
        lisiting_file.push_back(line[5]);
        i++;
        line = source_file[i];
    }

    if(line[4].compare(errorStr)  == 0 )// no error
    {
        if (line[1].compare("START") == 0)
        {
            i++;

            if(is_number(line[2]))
            {
                location_counter = is_hex("0"+line[2]);
                start_address = location_counter;
                string new_line =  form_line(line);
                lisiting_file.push_back(new_line);
            }
            else if(is_hex(line[2]) != -1)
            {
                location_counter = is_hex(line[2]);
                start_address = location_counter;
                string new_line =  form_line(line);
                lisiting_file.push_back(new_line);
            }

            else
            {
                string new_line =  form_line(line);
                lisiting_file.push_back(new_line);
                existError =true;
                string error = "        ***illegal operand field*** ";
                lisiting_file.push_back(error);
            }
        }
    }
    for( ; i < source_file.size() ; i++)
    {
        line = source_file[i];
        if(line[5].compare(errorStr) != 0)
        {
            lisiting_file.push_back(line[5]);
            continue;
        }
        string new_line = form_line(line);
        lisiting_file.push_back(new_line);
        if(line[4].compare(errorStr) != 0) //invalid instruction
        {
            if (line[4].find("Warning") == std::string::npos)
            {
                // no warning
                existError= true ;
            }
            lisiting_file.push_back(line[4]);
            continue;
        }
        if(line[0].compare(errorStr) != 0) //FOUND LABEL
        {

            map<string,string>::const_iterator itr = symbol_table.find(line[0]);

            if(itr!=symbol_table.end())
            {
                existError =true;
                string error = "        ***duplicate label***";
                lisiting_file.push_back(error);
                continue;
            }
            else
            {
                string loc = convert_to_string(location_counter);
                symbol_table.insert(pair<string,string>(line[0],loc));
            }
        }
        if(line[1].compare("END") == 0)
        {
            if(i != source_file.size()-1)
            {
                existError =true;
                string error = "        ***end statement can't be followed by line***";
                lisiting_file.push_back(error);
                continue;
            }
            exist_end = true;
            if(line[2].compare(errorStr) != 0)
            {
                map<string,string>::const_iterator itr = symbol_table.find(line[2]);
                if(itr==symbol_table.end())
                {
                    existError =true;
                    string error = "        ***illegal operand***";
                    lisiting_file.push_back(error);
                }
            }
            if(line[0].compare(errorStr) != 0)
            {
                existError =true;
                string error = "        ***illegal label***";
                lisiting_file.push_back(error);
            }
            program_lenght = convert_to_string(location_counter-start_address);
        }
        if((line[1].compare("START") == 0) && (i != 0))
        {
            existError =true;
            string error = "        ***START statement can't be preceded with instructions ***";
            lisiting_file.push_back(error);
            continue;
        }
        if(line[3].compare("3")==0 || line[3].compare("4")==0 || line[3].compare("2")==0)
        {
            location_counter += atoi( line[3].c_str() );

        }
        else if(line[1].compare("RESW")==0 || line[1].compare("RESB")==0 || line[1].compare("WORD")==0)
        {
            if(!is_number(line[2]) && line[1].compare("WORD") !=0)
            {
                existError =true;
                string error = "        ***illegal operand***";
                lisiting_file.push_back(error);
            }
            else
            {
                if(line[1].compare("RESW") == 0)
                {
                    location_counter += atoi( line[2].c_str() )*3;
                }
                else if(line[1].compare("RESB") == 0)
                {
                    location_counter += atoi( line[2].c_str() );
                }
                else
                {
                    // word
                    size_t t = line[2].find(',');
                    char * pch;
                    char str1[6];
                    strcpy(str1, line[2].c_str());
                    int group_size = 0;
                    if (t != std::string::npos)
                    {
                        pch = strtok (str1,",");
                        while (pch != NULL)
                        {
                            group_size++;
                            pch = strtok (NULL, ",");
                        }

                        location_counter += group_size*3;

                    }
                    else
                    {

                        location_counter += 3;

                    }
                }
            }
        }
        else if(line[1].compare("BYTE") == 0)
        {
            char first_char = line[2].at(0);
            int lenght = line[2].size()-3;
            if(first_char == 'X')
            {
                if(lenght %2 != 0)
                {
                    existError =true;
                    string error = "        ***odd length for hex string***";
                    lisiting_file.push_back(error);
                }
                else
                {
                    location_counter += lenght/2;
                }
            }
            else
            {
                location_counter += lenght;
            }

        }

    }
    if(!exist_end)
    {
        existError =true;
        string error = "        ***missing end instruction***";
        lisiting_file.push_back(error);
    }
    if (!existError)
    {
        string symTab="\n\n>>    e n d    o f   p a s s   1 \n\n";
        symTab += ">>   *****************************************************\n\n";
        symTab +=">>         s y m b o l     t a b l e\n";
        symTab +="           name         value\n "  ;
        symTab +="----------------------------------";
        lisiting_file.push_back(symTab);
        std::map <std::string,std::string >::iterator it;
        for (it= symbol_table.begin() ; it!=symbol_table.end() ; it++)
        {
            symTab="           " +it->first;

            for (int i=symTab.size(); i<26 ; i++)
            {
                symTab += " ";
            }
            symTab+=it->second;
            lisiting_file.push_back(symTab);

        }
    }
    else
    {
        string symTab="           Incomplete assembly";
        lisiting_file.push_back(symTab);
    }
    return lisiting_file;
}
