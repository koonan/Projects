#include <bits/stdc++.h>
#include "PassTwoAlgorithm.h"
#include "OperandValidator.h"
#include "WriteFile.h"
#include "ExpressionEvaluator.h"

using namespace std;
PassTwoAlgorithm::PassTwoAlgorithm(vector <vector<string> > statements,map <string,vector<string> > optable,map <string,vector<string> > symTable,map <string,vector<string> > literalTable,string progName,string progLen)
{
    this-> progName = progName;
    this->progLen = progLen;
    this->statements = statements;
    this->optable = optable;
    this->symTable = symTable;
    this->literalTable = literalTable;
    getRegistersTable();
    getHexTable();
    listing.push_back("\n\n>>   **************************PASS II***************************\n\n");

}


string PassTwoAlgorithm::charToHex(string operand)
{
    string hexStr = "" ;
    stringstream ss;

    for(int i = 0 ; i < operand.length() ; i++)
    {
        ss << hex << (int)operand[i];
    }
    hexStr = ss.str();
    transform(hexStr.begin(),hexStr.end(),hexStr.begin(), ::toupper);
    return hexStr ;

}
string PassTwoAlgorithm::completeHexAddress(string hexadd,int len)
{
    // check =
    // 3 5
    if (hexadd.length()>len)
    {
        return hexadd.substr(hexadd.length()-len, len) ;
    }
    for (int i=hexadd.length(); i<len; i++)
    {
        hexadd='0'+hexadd;
    }

    return hexadd;
}
void PassTwoAlgorithm::initNewRecord(string startingAdd)
{

    string len = completeHexAddress(validator.fromDecimalToHex(recordLenCnt),2);

    record[9]=len[0];
    record[10]=len[1];

    objectcode.push_back(record);
    ////////////////////////////////////////////
    record = "T^"+completeHexAddress(startingAdd,6)+"^00";
    recordLenCnt=0;
}
void PassTwoAlgorithm::updateRecord (string hex,int format,string startingAdd)
{
    if (recordLenCnt+format > maxRecordLen )
    {
        initNewRecord(startingAdd);
    }
    record += "^"+hex;
    recordLenCnt+=format;
}
void PassTwoAlgorithm::getRegistersTable ()
{
    registers['A']='0';
    registers['X']='1';
    registers['L']='2';
    registers['B']='3';
    registers['S']='4';
    registers['T']='5';
    registers['F']='6';
}


void PassTwoAlgorithm::getHexTable()
{
    binaryToHex["0000"]="0";
    binaryToHex["0001"]="1";
    binaryToHex["0010"]="2";
    binaryToHex["0011"]="3";
    binaryToHex["0100"]="4";
    binaryToHex["0101"]="5";
    binaryToHex["0110"]="6";
    binaryToHex["0111"]="7";
    binaryToHex["1000"]="8";
    binaryToHex["1001"]="9";
    binaryToHex["1010"]="A";
    binaryToHex["1011"]="B";
    binaryToHex["1100"]="C";
    binaryToHex["1101"]="D";
    binaryToHex["1110"]="E";
    binaryToHex["1111"]="F";
}
void PassTwoAlgorithm::generateObjCode()
{
    // handle RSUB
    // handle the Modification records & N records
    bool error = false;
    bool gap = false;
    string hex;
    int type;
    string endRecord = "";
    record = "H^"+progName;
    // check 6
    // program name,length
    for(int len = progName.length() ; len < 6 ; len++)
    {
        record +=" ";
    }
    record+="^"+completeHexAddress(statements[0][0],6)+"^"+completeHexAddress(progLen,6);
    objectcode.push_back(record);
    record = "T^"+completeHexAddress(statements[0][0],6)+"^00";
    recordLenCnt=0;
//    /** check -1 **/
    for (int i=0; i<statements.size(); i++)
    {
        errorLine = "" ;
        if ((statements[i][1]=="RESW"||statements[i][1]=="RESB"))
        {
            listing.push_back(form_line_pass2(statements[i], "")) ;
            gap = true;
            continue;
        }
        if (gap)
        {
            if (recordLenCnt!=0)
                initNewRecord(statements[i][0]);
            gap = false;
        }
        hex="";
        address = "";
        //literal statement
        if (statements[i][1]=="*")
        {
            hex = literalTable[statements[i][2]].at(0);
            if (statements[i][2].at(0)!='X'&&statements[i][2].at(0)!='C')
            {
                hex = completeHexAddress(hex,6);
            }
            updateRecord(hex,atoi(literalTable[statements[i][2]].at(1).c_str()),statements[i][0]);
            listing.push_back(form_line_pass2(statements[i], hex));

        }
        else if (statements[i][1]=="END")
        {
            listing.push_back(form_line_pass2(statements[i], "")) ;
            if (statements[i][2].length()==0)
            {
                endRecord="E^"+ completeHexAddress(statements[0][0],6);
            }
            else if(symTable.find(statements[i][2])!=symTable.end())
            {
                endRecord="E^"+ completeHexAddress(symTable[statements[i][2]].at(0),6);
            }
            else
            {

                error = true;
                errorLine += "       ***operand Label Doesn't exist in SymbTable!!***\n";
            }
        }
        else if(binary_search(directives.begin(),directives.end(),statements[i][1]))
        {
            listing.push_back(form_line_pass2(statements[i], "")) ;
            continue;
        }
        else if(statements[i][1]=="RSUB")
        {
            if (statements[i][3]=="3")
            {
                hex = "4F0000";
                updateRecord(hex, 3,statements[i][0] );
                //do sth ;
            }
            else if (statements[i][3]=="4")
            {
                hex = "4F000000";
                updateRecord(hex, 4,statements[i][0] );
                //do sth ;
            }
            if(statements[i][3]=="4")
            {
                statements[i][1] = "+" + statements[i][1];
            }
            listing.push_back(form_line_pass2(statements[i], hex)) ;

        }

        else if (statements[i][1]=="BYTE")
        {
            int format;
            if (statements[i][2].at(0)=='X')
            {
                // test
                hex = statements[i][2].substr(2,statements[i][2].length()-3);
                format = hex.length()/2;
            }
            else
            {
                hex = charToHex(statements[i][2].substr(2,statements[i][2].length()-3));
                format = hex.length();
            }
            updateRecord(hex,format,statements[i][0]);
            listing.push_back(form_line_pass2(statements[i], hex)) ;
        }
        else if (statements[i][1]=="WORD")
        {
            hex=completeHexAddress(validator.fromDecimalToHex(atoi(statements[i][2].c_str())),6);
            updateRecord(hex,3,statements[i][0]);
            listing.push_back(form_line_pass2(statements[i], hex)) ;
        }
        else
        {
            string opcode = optable[statements[i][1]].at(1);

            type = evaluator.validate(statements[i][1],statements[i][2]);
            if (statements[i][3]=="3"||statements[i][3]=="4")
            {
                //error
                hex = "";


                hex += opcode[0];

                // n i x b p e
                string binary = "";
                switch (opcode[1])
                {
                case '0' :
                    binary = "00";
                    break ;
                case '4' :
                    binary = "01";
                    break ;
                case '8' :
                    binary = "10";
                    break ;

                case 'C' :
                    binary = "11";
                    break ;
                }

                string operand = statements[i][2];
                if(statements[i][2].at(0)=='@')
                {
                    binary+="10";
                    operand = statements[i][2].substr(1,statements[i][2].length()-1);
                }
                else if(statements[i][2].at(0) == '#')
                {
                    operand = statements[i][2].substr(1,statements[i][2].length()-1);
                    binary+="01";
                }
                else
                {
                    binary+="11";
                }

                hex += binaryToHex[binary];

                string xbpe = "";
                if (statements[i][2].length() >=3 && statements[i][2].at(statements[i][2].size()-2)==','&&statements[i][2].at(statements[i][2].size()-1)=='X')
                {

                    xbpe += '1';
                    operand = statements[i][2].substr(0,statements[i][2].length()-2);

                }
                else xbpe += '0';
                // b
                xbpe += '0';
//                // n i x b p e
                // set address or displacement

                // evaluate the address


                switch (type)
                {
                case 0:
                {
                    // literals!!!!!!!!!!!!!!!!
                    string literal = statements[i][2].substr(1,statements[i][2].length()-1);
                    if(literalTable.find(literal)!=literalTable.end())
                    {
                        address = literalTable[literal].at(2);

                        if (literal=="*")
                        {
                            if (statements[i][3]=="4")
                            {
                                MRecords.push_back("L^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^05^+"+progName);
                            }
                            else
                            {
                                MRecords.push_back("L^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^03^+"+progName);
                            }
                        }
                    }
                    else
                    {
                        errorLine += "      ***Invalid Literal*** " ;
                    }
                    break;
                }
                case 1:



                {


                    bool negFlag = false ;
                    if(operand[0]=='-')
                    {
                        negFlag=true;
                        operand = operand.substr(1,operand.length()-1);
                    }
                    if(operand == "*")
                    {
                        address =statements[i][0];
                    }
                    else if(symTable.find(operand)!=symTable.end())
                    {

                        address = symTable[operand].at(0);

                        if (symTable[operand].at(1)=="ABSOLUTE")
                        {
                            if (statements[i][3]=="3")
                            {
                                MRecords.push_back("M^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^03^-"+progName);
                            }
                        }
                        else if (statements[i][3]=="4")
                        {
                            MRecords.push_back("M^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^05^+"+progName);
                        }

                    }
                    else
                    {
                        error = true;
                        errorLine += "       ***operand Label Doesn't exist in SymbTable!!***\n";

                    }
                    if(negFlag)
                    {
                        // -1000
                        address="-"+address;
                    }

                }
                break;
                case 2:
                    // hex value
                    // we should add N record for this address
                    // what about negative value
                    address = operand;

                    if (statements[i][2].at(0)!='#')
                    {
                        if(statements[i][3]=="3")
                        {
                            // M^000007^
                            MRecords.push_back("M^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^03^"+"-"+progName);
                        }

                    }
                    break;

                case 3:
                {
                    string addType = validator.evaluateExpression(evaluator.getExpression(),statements[i][0], symTable);
                    address = validator.address ;
                    if (addType == "ERROR" || validator.fromHexToDecimal(address) < 0)
                    {
                        error = true;
                        errorLine += "       ***Invalid Address Value From Expression***\n";
                        //continue;
                    }

                    else if (addType == "ABSOLUTE")
                    {
                        // add N Record
                        if (statements[i][3]=="3")
                        {
                            // M^000007^
                            MRecords.push_back("M^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^03^"+"-"+progName);
                        }

                    }else if (statements[i][3]=="4")
                    {
                            MRecords.push_back("M^"+completeHexAddress(validator.fromDecimalToHex(validator.fromHexToDecimal(statements[i][0])+1),6)+"^05^+"+progName);
                    }
                }
                break;

                case -1 :
                    error = true;
                    errorLine += "       ***Invalid operand!!***\n";

                    listing.push_back(form_line_pass2(statements[i], hex)) ;
                    break;


                }
                if (validator.fromHexToDecimal(address) < 0 && !(statements[i][2].at(0)=='#'&& type==2))
                {
                    error=true;
                    errorLine += "       ***Invalid Negative Address***\n";
                }
                if (statements[i][3]=="3")
                {
                    int disp;
                    if (statements[i][2].at(0)=='#'&&type==2)
                    {
                        //-1000
                        disp = validator.fromHexToDecimal(statements[i][2].substr(1,statements[i][2].length()-1));
                        xbpe+="00";
                    }
                    else
                    {

                        int ta = validator.fromHexToDecimal(address);
                        int lc = validator.fromHexToDecimal (statements[i+1][0]);
                        disp = ta - lc;

                        if (disp >= 2047 || disp < -2048)
                        {
                            error=true;
                            errorLine += "       ***Displacement value is out of Range!!***\n";
                        }
                        else
                        {

                            xbpe+="10";
                        }
                    }
                    hex += binaryToHex[xbpe]+completeHexAddress(validator.fromDecimalToHex(disp),3);

                    updateRecord(hex,3,statements[i][0]);
                    if(statements[i][3]=="4")
                    {
                        statements[i][1] = "+" + statements[i][1];
                    }
                    listing.push_back(form_line_pass2(statements[i], hex)) ;

                }
                else
                {
                    xbpe+="01";
                    hex += binaryToHex[xbpe]+completeHexAddress(address,5);
                    //
                    updateRecord(hex,4,statements[i][0]);
                    if(statements[i][3]=="4")
                    {
                        statements[i][1] = "+" + statements[i][1];
                    }
                    listing.push_back(form_line_pass2(statements[i], hex)) ;

                }
                // set address or displacement
            }
            /////////////////////////////////////////////////////////////////////////////////
            else if (statements[i][3]=="2")
            {


                hex = "";
                hex+= opcode;
                if (type == -1)
                {
                    error = true;
                    errorLine += "       ***Invalid Operand!!***\n";
                    listing.push_back(form_line_pass2(statements[i], hex)) ;


                }
                else
                {
                    if (type==5)
                    {
                        hex += registers[statements[i][2].at(0)];
                        hex +="0";
                    }
                    else
                    {
                        char r1 = statements[i][2].at(0);
                        char r2 = statements[i][2].at(2);
                        hex += registers[r1];
                        if (type==7)
                        {
                            hex += r2;
                        }
                        else if (type==6)
                            hex += registers[r2];
                    }
                    // add starting address
                    updateRecord(hex,2,statements[i][0]);
                    if(statements[i][3]=="4")
                    {
                        statements[i][1] = "+" + statements[i][1];
                    }
                    listing.push_back(form_line_pass2(statements[i], hex)) ;

                }
            }
            // BYTE || WORD are checked in pass1
        }
        // loop for the literal table and write it to object code
        if(errorLine.size()!=0)
        {
            listing.push_back(errorLine);
            errorLine = "" ;
        }
    }// end for loop of source code
    // we have to write object file

    //if (record.length()!=0)
    WriteFile write ;
    if (!error)
    {
        if(recordLenCnt != 0)
        {
            initNewRecord(statements[0][0]);
        }
        for(int itr = 0 ; itr < MRecords.size() ; itr++ )
        {
            objectcode.push_back(MRecords.at(itr));
        }
        objectcode.push_back(endRecord);
        WriteFile write ;
        write.write(objectcode,"obcode.txt",false);

    }
    write.write(listing,"ListingFile.txt",true);

}
string PassTwoAlgorithm::form_line_pass2(  vector<string>  line, string opcode )
{
    string new_line = "";


    for(int i = 0 ; i < 6-line[0].size() ; i++)
    {
        new_line +="0";
    }
    new_line += line[0] +" ";
    if (opcode.compare("") == 0 )
        new_line += "        ";
    else
    {
        for (int i = 0 ; i < 8-opcode.size() ; i++)
        {
            new_line += " ";
        }
    }
    new_line += opcode+" " ;
    if(line[4].compare("") == 0) //label
    {
        new_line += "          ";
    }
    else
    {
        new_line += line[4];
        for(int i = line[4].size() ; i < 10 ; i++)
        {
            new_line +=" ";
        }
    }
    if(line[1].compare("") == 0)
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

    if(line[2].compare("") == 0)
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

