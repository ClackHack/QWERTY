#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

void Error(string message);

int main(int argc, char *argv[]){
    if (argc != 2){
        Error("Expected file path argument");
    }
    fstream File;
    File.open(argv[1]);


    File.close();
    return 0;
}

void Error(string msg){
    cout << "Error :> " << msg << endl;
}