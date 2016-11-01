#include "PassOneAlgorithm.h"
#include <bits/stdc++.h>
#include <sstream>
#include "ExpressionEvaluator.h"
#include "OperandValidator.h"
using namespace std;

PassOneAlgorithm::PassOneAlgorithm()
{
    existError = false;
    generalError = false;
}
void PassOneAlgorithm::print_literals(void)
{
    for (map<string,vector <string> > ::iterator it =literal_table.begin(); it!=literal_table.end(); ++it)
    {

        string key = it->first;
        vector <string> val2 =  it->second ;
        if (val2[2] == "*")
        {
            val2[2]=convert_to_string(location_counter) ; // GEHAD
            it -> second = val2 ;
            vector <string> literal_line ;
            literal_line.push_back("*");
            literal_line.push_back("=" + key);
            literal_line.push_back(errorStr);
            //here we put to esraa literal to her vector
            vector<string> literal_pass2;
            literal_pass2.push_back(convert_to_string(location_counter));
            literal_pass2.push_back("*");
            literal_pass2.push_back(key);
            literal_pass2.push_back("");
            literal_pass2.push_back("");

            pass2.push_back(literal_pass2);
            //here we put to esraa literal to her vector
            string s  = form_line(literal_line);
            lisiting_file.push_back( s);
            location_counter += is_hex("0"+val2[1]);
        }
    }
}
string PassOneAlgorithm::get_value (string s)
{
    string answer ;
    for (int i = 0 ; i < s.length() ; i++)
    {
        int number = s.at(i);
        stringstream ss;
        ss << hex <<uppercase <<number;
        answer += ss.str();
    }
    return answer ;
}

vector<string> PassOneAlgorithm::split(const string &s, const string ch )
{
    std::vector<std::string> elems;
    std::string rgx_str = ch;
    std::regex rgx (rgx_str);
    std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
    std::sregex_token_iterator end;
    while (iter != end)
    {
        if ((*iter).length()>0)
            elems.push_back(*iter);
        ++iter;
    }
    return elems;
}
string PassOneAlgorithm::isInSymbolTable (string s)
{
    map<string,vector<string> >::const_iterator itr = symbol_table.find(s);
    if(itr==symbol_table.end())
    {
        existError =true;
        string error = "        *** illegal operand***";
        lisiting_file.push_back(error);
        return "error";
    }
    else
    {
        vector<string> address =  itr -> second;
        type_label = address[1];
        return address[0] ;
    }

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


    unsigned int result ;
    std::stringstream ss;
    ss << std::hex << s;
    ss >> result;
    return static_cast<int>(result);
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
            if (line[0].compare(errorStr)!=0)
            {
                start_label=line[0];
            }
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
        if(line[5].compare(errorStr) == 0)
        {
            vector <string>inPass2 ;
            inPass2.push_back(convert_to_string(location_counter));
            if (line[3]=="4")
                inPass2.push_back(line[1].substr(1,line[1].length()-1));
            else
                inPass2.push_back(line[1]);
            if (line[2].compare(errorStr) ==0)
                inPass2.push_back("");
            else
                inPass2.push_back(line[2]);
            inPass2.push_back(line[3]);
            if (line[0].compare(errorStr) ==0)
                inPass2.push_back("");
            else
                inPass2.push_back(line[0]);
            pass2.push_back(inPass2);
        }

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
            map<string,vector<string> >::const_iterator itr = symbol_table.find(line[0]);
            if(itr!=symbol_table.end())
            {
                existError =true;
                string error = "        ***duplicate label***";
                lisiting_file.push_back(error);
                continue;
            }
            else if ( line[1].compare("EQU") !=0)
            {
                string loc = convert_to_string(location_counter);
                vector<string> symbol;
                symbol.push_back(loc);
                symbol.push_back("RELOCATABLE");
                symbol_table.insert(pair<string,vector<string> >(line[0],symbol));
            }
        }
        //==================================literal ========================================
        if (line[6]=="12")
        {
            line [2]= line[2].substr(1,line[2].length()-1);
            map<string,vector<string> >::const_iterator itr = literal_table.find(line[2]);
            if(itr==literal_table.end()) //not found
            {
                vector <string>  value ;
                if (line[2].at(0)=='X')
                {
                    value.push_back(line[2].substr(2,line[2].length()-3));
                    if((line[2].length()-3)%2 == 1 )
                    {
                        existError =true;
                        string error = "        ***odd length for hex string***";
                        lisiting_file.push_back(error);
                        continue;
                    }
                    else
                    {
                        value.push_back(convert_to_string((line[2].length()-3)/2));
                        value.push_back("*");
                    }
                }
                else if (line[2].at(0)=='C')
                {
                    value.push_back(get_value(line[2].substr(2,line[2].length()-3)));
                    value.push_back(convert_to_string(line[2].length()-3));
                    value.push_back("*");
                }
                else if(line[2].at(0)=='W')
                {
                    value.push_back(convert_to_string(atoi((line[2].substr(2,line[2].length()-3)).c_str())));
                    value.push_back("3");
                    value.push_back("*");
                }
                else
                {
                    value.push_back(convert_to_string(location_counter));
                    value.push_back("3");
                    value.push_back("*");
                }
                literal_table.insert(pair<string,vector <string> >(line[2],value));
            }
        }
        //==================================literal ========================================
        if (line[1].compare("LTORG")==0)
        {
            print_literals();
        }

        if(line[1].compare("END") == 0)
        {
            if(i != source_file.size()-1)
            {
                existError =true;
                string error = "        ***end statement can't be followed by line***";
                lisiting_file.push_back(error);
                print_literals();
                continue;
            }

            exist_end = true;
            if(line[2].compare(errorStr) != 0)
            {
                map<string,vector<string> >::const_iterator itr = symbol_table.find(line[2]);
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
            print_literals();
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
        else if (line[1].compare("ORG") == 0 )
        {
            if(line[6] == "1") //label
            {
                string address;
                if(line[2].at(0) == '@' || line[2].at(0) == '#')
                {
                    address = isInSymbolTable(line[2].substr(1,line[2].length()-1));
                }
                else
                {
                    address = isInSymbolTable(line[2]);
                }

                if (address  != "error" &&  type_label != "ABSOLUTE")
                    location_counter =  is_hex("0"+address);

                else if(type_label == "ABSOLUTE")
                {
                    existError =true;
                    string error = "        ***illegal operand***";
                    lisiting_file.push_back(error);
                }
            }

            else if(line[6] == "2")
            {
                validator.validate(line[1],line[2]);
                expression = validator.getExpression();
                string type = evaluator.evaluateExpression(expression,convert_to_string(location_counter), symbol_table);
                if(type == "ERROR" || type == "ABSOLUTE" )
                {
                    existError =true;
                    string error = "        ***illegal operand***";
                    lisiting_file.push_back(error);
                }
                else
                {
                    string address = evaluator.address;
                    if (evaluator.negativeFlag)
                    {
                        existError =true;
                        string error = "        ***location counter can't be negative***";
                        lisiting_file.push_back(error);
                    }
                    else
                        location_counter =  is_hex("0"+address);

                }
            }
        }
        else if (line[1].compare("EQU") == 0 )
        {
            if (line[0].compare(errorStr)==0)
            {
                existError =true;
                string error = "        ***EQU must have label***";
                lisiting_file.push_back(error);
            }

            if(line[6] == "0") //label
            {
                string address ;
                if(line[2].at(0) == '@' || line[2].at(0) == '#')
                {
                    line[2] = line[2].substr(1,line[2].length()-1);
                }
                if(line[2].at(0) == '-')
                {
                    address = isInSymbolTable(line[2].substr(1,line[2].length()-1));
                }
                else
                {
                    address = isInSymbolTable(line[2]);
                }

                if (address  != "error")
                {
                    vector<string>symbol;
                    int num_address = is_hex("0"+address);
                    if(line[2].at(0) == '-')
                    {
                        num_address  *= -1;
                    }
                    symbol.push_back(convert_to_string(num_address));
                    symbol.push_back(type_label);
                    symbol_table.insert(pair<string,vector<string> >(line[0],symbol));
                }
            }

            else if (line[6]=="1")     //number
            {
                vector<string>symbol;
                // symbol_table.insert(pair<string,string>(line[0],line[2]));
                if(line[2].at(0) == '@' || line[2].at(0) == '#')
                {
                    line[2] = line[2].substr(1,line[2].length()-1);
                }
                if(line[2].at(0) == '-')
                {
                    string temp = line[2].substr(1,line[2].length()-1);
                    int address = is_hex("0"+temp);
                    address*= -1;
                    symbol.push_back(convert_to_string(address));
                }
                else
                {
                    symbol.push_back(line[2]);
                }
                symbol.push_back("ABSOLUTE");
                symbol_table.insert(pair<string,vector<string> >(line[0],symbol));
            }

            else if(line[6]=="13") // 13 *
            {
                vector<string>symbol;
                symbol.push_back(convert_to_string(location_counter));
                symbol.push_back("RELOCATABLE");
                symbol_table.insert(pair<string,vector<string> >(line[0],symbol));
            }
            else if(line[6]=="2")
            {
                validator.validate(line[1],line[2]);
                expression = validator.getExpression();
                string type = evaluator.evaluateExpression(expression,convert_to_string(location_counter), symbol_table);
                if(type =="ERROR" )
                {
                    existError =true;
                    string error = "        ***illegal operand***";
                    lisiting_file.push_back(error);
                }
                else
                {
                    string address =evaluator.address;
                    vector<string>symbol;
                    symbol.push_back(address);
                    symbol.push_back(type);
                    symbol_table.insert(pair<string,vector<string> >(line[0],symbol));
                }
            }
        }

    }
    if(!exist_end &&!generalError)
    {
        existError =true;
        string error = "        ***nnnnnn missing end instruction***";
        lisiting_file.push_back(error);
    }
    if (!existError)
    {
        string symTab="\n\n>>    e n d    o f   p a s s   1 \n\n";
        symTab += ">>   *****************************************************\n\n";
        symTab +=">>         s y m b o l     t a b l e\n";
        symTab +="           name           value     flag\n "  ;
        symTab +="----------------------------------------------------";
        lisiting_file.push_back(symTab);
        std::map <std::string,std::vector<string> >::iterator it;
        for (it= symbol_table.begin() ; it!=symbol_table.end() ; it++)
        {
            symTab="           " +it->first;

            for (int i=symTab.size(); i<26 ; i++)
            {
                symTab += " ";
            }
            vector<string> val = it->second;
            symTab+= val[0];
            for (int i=symTab.size(); i<36 ; i++)
            {
                symTab += " ";
            }
            symTab+= val[1];
            lisiting_file.push_back(symTab);

        }
        symTab += "\n\n>>   *****************************************************\n\n";
         lisiting_file.push_back(symTab);

    }
    else
    {
        string symTab="           Incomplete assembly";
        lisiting_file.push_back(symTab);
    }
    return lisiting_file;
}
