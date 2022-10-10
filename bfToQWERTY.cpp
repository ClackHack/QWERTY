#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    if (argc != 3){
        cout << "Expected file path argument\n";
        exit(1);
    }
    string content;
    char tmp;
    ifstream inFile;
    inFile.open(argv[1]);
    if (inFile.fail()){
        cout << "Cannot Locate File Specified...\n";
        inFile.close();
        exit(1);
    }
    
    while (!inFile.eof()){
        inFile.get(tmp);
        content+=tmp;
    }
    inFile.close();
    string output="";
    cout << "Analyzing...\n";
    for (int i=0;i< content.length();i++){
        //cout << output
        switch (content.at(i)){
            case '.':
                output+='O';
                break;
            case ',':
                output+='I';
                break;
            case '+':
                output+='U';
                break;
            case '-':
                output+='V';
                break;
            case '>':
                output+='D';
                break;
            case '<':
                output+='A';
                break;
            case '[':
                output+="EQ";
                break;
            case ']':
                output+="L";
                break;
        }
    }
    ofstream outFile;
    outFile.open(argv[2]);
    if (outFile.fail()){
        cout << "Cannot Locate File Specified...\n";
        outFile.close();
        exit(1);
    }
    outFile << output;
    outFile.close();


}