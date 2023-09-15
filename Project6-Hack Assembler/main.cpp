#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;
string convertToBinary(string str);

/*
12/09/2023
Description: an assembler for the hack assembly language written in C++ by Mohamed Elkaramany.
The program takes a file *.asm and converts it into a *.hack file(machine code) where each instruction is 16-bits.
Initially, the program initializes the symbol table and inserts the pre-defined symbols in it, then it performs a "first-pass" where all the
label symbols(xxx) are extracted and xxx now stands for address of next instruction in the program.
After that a "second-pass" is performed where both the variables symbols and the instructions themselves are translated into machine code, then
outputed into a *.txt file

*/

void init(map<string,string> &mp)
{

    mp["R0"] = convertToBinary("0");  mp["R1"] = convertToBinary("1"); mp["R2"] = convertToBinary("2"); mp["R3"] = convertToBinary("3");
    mp["R4"] = convertToBinary("4"); mp["R5"] = convertToBinary("5"); mp["R6"] = convertToBinary("6"); mp["R7"] = convertToBinary("7");
    mp["R8"] = convertToBinary("8"); mp["9"] = convertToBinary("9");   mp["10"] = convertToBinary("10");
    mp["R11"] = convertToBinary("11");    mp["R12"] = convertToBinary("12"); mp["R13"] = convertToBinary("13"); mp["R14"] = convertToBinary("14");
    mp["R15"] = convertToBinary("15"); mp["SCREEN"] = convertToBinary("16384"); mp["KBD"] = convertToBinary("24576");
    mp["SP"] = convertToBinary("0"); mp["LCL"] = convertToBinary("1"); mp["ARG"] = convertToBinary("2");
    mp["THIS"] = convertToBinary("3");   mp["THAT"] = convertToBinary("4");

}
void getSymbols(map<string,string>& mp)    //performs first and second pass to find symbols and fill the symbol table
{

    ifstream inputFile("Add.asm");  //file in assembly language
    string line;
    int cnt = 0;
    while ( getline (inputFile,line) ) //first pass
    {
       //skip comments
       if(line.size()==0)continue;
       if(line[0]=='/')continue;

       //remove whitespace from string using erase-remove idiom
       line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

       //some lines might have an instruction and then a comment, so we need to detect when comments start to isolate the instruction
       size_t commentBegin = line.find('/');
       if(commentBegin != string::npos)
       {
           line = line.substr(0 , commentBegin);
       }

       if(line[0] == '(')   //found symbol in the form (xxx)
       {
            //cout<<"found symbol "<<line<< endl;
            size_t endLine = line.find(')');
            string num = to_string(cnt);
            string binNum = convertToBinary(num);
            mp[line.substr(1,endLine-1)] = binNum;
            continue;
       }
       cnt++;
    }

    inputFile.close();
    inputFile.open("Add.asm");
    cnt = 16;
    while ( getline (inputFile,line) ) //second pass for variable symbols
    {
       //skip comments
       if(line.size()==0)continue;
       if(line[0]=='/')continue;    //skips comments

       //remove whitespace from string using erase-remove idiom
       line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

       //some lines might have an instruction and then a comment, so we need to detect when comments start to isolate the instruction
       size_t commentBegin = line.find('/');
       if(commentBegin != string::npos)
       {
           line = line.substr(0 , commentBegin);
       }

       if(line[0] == '@')   //found a instruction(might have variable symbol or just general a-instruction, so need some logic to determine).
       {
            string var = line.substr(1,line.size()-1);
            if(var[0]-'0' >= 0 && var[0]-'0'<=9 || mp.find(var)!=mp.end()) //a number because variables don't start with int
            {

                continue;
            }
            mp[var] = convertToBinary(to_string(cnt));
            cnt++;
       }
    }
}

//function that converts a number given as a string into a 15-bit binary number
string convertToBinary(string str)
{
    int num = stoi(str);
    string retVal = "";
    while(num)
    {
        int remainder = num%2;
        retVal = to_string(remainder) + retVal;
        num/=2;
    }
    while(retVal.size()<15)
    {
        retVal = '0' + retVal;
    }
    return retVal;
}

int main(int argc , char* argv[])
{
    //initially the maps(destToBinary, jmpToBinary and compToBinary) are initialized
    //as they would be used in translating C-instructions later on
    map<string , string> destToBinary;

    destToBinary[""]= {"000"};    destToBinary["M"] = {"001"};  destToBinary["D"] =  {"010"};
    destToBinary["MD"] = {"011"}; destToBinary["A"] = {"100"};  destToBinary["AM"] = {"101"};
    destToBinary["AD"] = {"110"}; destToBinary["AMD"] = {"111"};

    map<string , string> jmpToBinary;

    jmpToBinary[""]={"000"}; jmpToBinary["JGT"] = {"001"}; jmpToBinary["JEQ"] = {"010"};
    jmpToBinary["JGE"] = {"011"}; jmpToBinary["JLT"] = {"100"}; jmpToBinary["JNE"] = {"101"};
    jmpToBinary["JLE"] = {"110"}; jmpToBinary["JMP"] = {"111"};

    map<string , string> compToBinary;

    compToBinary["0"] = {"0101010"}; compToBinary["1"] = {"0111111"}; compToBinary["-1"] = {"0111010"};
    compToBinary["D"] = {"0001100"};
    compToBinary["A"] = {"0110000"}; compToBinary["M"] = {"1110000"}; compToBinary["!D"] = {"0001101"};
    compToBinary["!A"] = {"0110001"}; compToBinary["!M"] = {"1110001"}; compToBinary["-D"] = {"0001111"};
    compToBinary["-A"] = {"0110011"}; compToBinary["-M"] = {"1110011"}; compToBinary["D+1"] = {"0011111"};
    compToBinary["A+1"] = {"0110111"}; compToBinary["M+1"] = {"1110111"}; compToBinary["D-1"] = {"0001110"};
    compToBinary["A-1"] = {"0110010"}; compToBinary["M-1"] = {"1110010"}; compToBinary["D+A"] = {"0000010"};
    compToBinary["D+M"] = {"1000010"};
    compToBinary["D-A"] = {"0010011"}; compToBinary["D-M"] = {"1010011"};
    compToBinary["A-D"] = {"0000111"}; compToBinary["M-D"] = {"1000111"};
    compToBinary["D&A"] = {"0000000"};  compToBinary["D&M"] = {"1000000"};
    compToBinary["D|A"] = {"0010101"}; compToBinary["D|M"] = {"1010101"};


    map<string,string> symbolTable; // the symbol table is a map that has key:value(int) pairs


    init(symbolTable); //initalize symbolTable with predefined symbols

    //this method goes twices over the file.asm file and initially puts all labels in symbolTable, then second pass it inserts
    //the variable symbols(from n=16 onwards)
    getSymbols(symbolTable);

    cout<<"Symbols are "<<endl;
    for(auto x : symbolTable)
    {

        cout<<x.first<<" "<<x.second<<endl;
    }


    //from here on, the program gets each instruction(A or C) and just translates it to machine code and outputs it in the output file(file.hack)
  string line;
  ifstream inputFile("Add.asm");  //file in assembly language
  ofstream outputFile ("Add.hack"); //file in machine code


  while ( getline (inputFile,line) ) //open a file called
  {
       //skip comments
       if(line.size()==0)continue;
       if(line[0]=='/' || line[0]=='(')continue;    //ignore comments and symbols

       //remove whitespace from string using erase-remove idiom
       line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());


       //some lines might have an instruction and then a comment, so we need to detect when comments start to isolate the instruction
       size_t commentBegin = line.find('/');
       if(commentBegin != string::npos)
       {
           line = line.substr(0 , commentBegin);
       }

      if(line[0] == '@')    //A-instruction
      {
        //get number/symbol after @
        string currInstruction = "0";
        string rest = line.substr(1 , line.size()-1);
        if(symbolTable.find(rest) != symbolTable.end())  //we found a symbol
        {
            currInstruction += symbolTable[rest];
        }
        else//we just have an integer, so need to convert to 15-bit binary and append to current instruction
        {
            currInstruction += convertToBinary(rest);
        }

        outputFile<<currInstruction<<endl;
      }
      else      //C-instruction
      {

            string currInstruction = "111";
            string comp = "";
            string dest = "";
            string jmp = "";

            size_t pos1 = line.find('=');
            size_t pos2 = line.find(';');
            string currDest = "";
            string currComp = "";
            string currJmp = "";

            //break the assembly instruction into different parts and convert each part using the maps
            if(pos1!=string::npos)  //found =
            {
                currDest = line.substr(0 , pos1);
                currComp = line.substr(pos1+1 , pos2-pos1+1);
            }
            if(pos2!= string::npos)
            {
                currJmp = line.substr(pos2+1 , line.size()-pos2+1);
            }
            if(pos1 == string::npos && pos2!= string::npos) //no destination
            {
                currComp = line.substr(0 , pos2);
            }
            //add C-instruction to file
            currInstruction = currInstruction + compToBinary[currComp] + destToBinary[currDest] + jmpToBinary[currJmp];
            outputFile<<currInstruction<<endl;
      }

  }

    inputFile.close();
    outputFile.close();
    return 0;
}
