#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

using namespace std;

fstream File;

const char TT_OUTPUT     ='O';
const char TT_INPUT      ='I';

const char TT_CHAR       ='C';
const char TT_BIN        ='B';
const char TT_INT        ='N';

const char TT_START_LOOP  ='E';
const char TT_RUN_TO_0  = 'Q';
const char TT_RUN_NUM   = 'T';
const char TT_CLOSE_LOOP ='L';

const char TT_MLEFT     = 'A';
const char TT_MRIGHT     ='D';
const char TT_MUP        ='W';
const char TT_MDOWN      ='S';

const char TT_INCREMENT  ='U';
const char TT_DECREMENT  ='V';

const char TT_AND        ='F';
const char TT_OR         ='R';
const char TT_INVERT     ='J';

const char TT_COLUMN     ='X';
const char TT_ROW=        'Y';
const char TT_GOTO_COL   ='G';
const char TT_GOTO_ROW   ='H';

int Row=0;
int Col=0;


const int MEMORY_SIZE=15000;
int MemoryPointer=0;
int CellGrid[MEMORY_SIZE][3];
int MemoryMode = 0;
//0 - Char Mode, 1 - String Mode, 2 - Bool Mode

void Error(string message);
void REPL();
void Execute(string raw); // call function with raw qwerty code
int AccessMemory(int r, int c);
void UpdateMemory(int r, int c, bool increment=true);
void UpdateMemory(int r, int c, int value);
void HandleCharacter(char c);

int main(int argc, char *argv[]){
    if (argc != 2){
        Error("Expected file path argument");
    }
    if (argv[1]=="-i"){
        REPL();
    }
    File.open(argv[1]);

    //dump file contents to string
    string fileContents="";
    char tmp;
    while (!File.eof()){
        File.get(tmp);
        fileContents+=tmp;
    }
    fileContents=fileContents.substr(0,fileContents.length()-1);
    File.close();
    cout << "Beginning Execution...\n\n";

    Execute(fileContents);

    return 0;
}

void Error(string msg){
    cout << "Error :> " << msg << endl;
    exit(0);
}

void Execute(string raw){
    //cout << "EXECUTING " << raw << endl;
    for (int i=0; i<raw.length(); i++){
        //cout << "EXL > " << i << " : " << raw.at(i) <<  endl;
        switch (raw.at(i)){
            case ';':
                //cout << "Comment\n";
                while (raw.at(i)!='\n'){
                    //cout << "-"<< raw.at(i);
                    i++;
                }
                break;
            case ' ' :
            case '\t':
            case '\r':
            case '\n':
                break;
            case TT_START_LOOP:
            {
                string subraw="";
                //Goto next non white space character
                i++;
                while (isspace(raw.at(i)))
                    i++;
                //cout << "CBIT " << raw.at(i) << endl;
                bool runTo0;
                if (raw.at(i)==TT_RUN_TO_0)
                    runTo0=true;
                else if (raw.at(i)==TT_RUN_NUM)
                    runTo0=false;
                else
                    Error("Expected q or t following loop");
                int depth = 1;
                while (i < raw.length()){
                    i++;
                    if (raw.at(i) == TT_CLOSE_LOOP){
                        depth--;
                        if (depth <= 0)
                            break;
                    }
                    else if (raw.at(i)== TT_START_LOOP)
                        depth++;
                    subraw+=raw.at(i);
                }
                //cout << "Entering loop subraw : " << subraw << endl;
                if (runTo0){
                    while (AccessMemory(Row,Col)){
                        Execute(subraw);
                    }
                }
                else{
                    //cout << "Execute " << AccessMemory(Row, Col) << " Times\n";
                    int count = AccessMemory(Row,Col);
                    for (int i = 0;i<count;i++){
                        Execute(subraw);
                    }
                }
                //cout << "Loop Complete\n";
                break;
            }
            default:
                //cout << "Handling : " << raw.at(i) << endl;
                HandleCharacter(raw.at(i));

        }
    }
}
void REPL(){

}

void HandleCharacter(char c){
    switch (c){
        case TT_OUTPUT:
            //cout << "O: " << AccessMemory(Row,Col) << endl;
            if (MemoryMode==0){
                //Character Mode
                char tmp;
                tmp = (char) AccessMemory(Row,Col);
                cout << tmp;
            }
            else if (MemoryMode==1){
                //Integer Mode
                int tmp;
                tmp = AccessMemory(Row,Col);
                cout << tmp;
            }
            else if (MemoryMode==2){
                //Boolean mode
                int tmp;
                tmp=(bool) AccessMemory(Row,Col);
                cout << tmp;
            }
            break;
        case TT_INPUT:
            //cout << "I\n";
            char tmp;
            cin >> tmp;
            UpdateMemory(Row,Col,tmp);
            break;
        
        case TT_CHAR:
            MemoryMode=0;
            break;
        case TT_INT:
            MemoryMode=1;
            break;
        case TT_BIN:
            MemoryMode=2;
            break;
        
        case TT_MLEFT:
            Col--;
            break;
        case TT_MRIGHT:
            Col++;
            break;
        case TT_MDOWN:
            Row++;
            break;
        case TT_MUP:
            Row--;
            break;
        
        case TT_DECREMENT:
            UpdateMemory(Row,Col,false);
            break;
        case TT_INCREMENT:
            UpdateMemory(Row,Col);
            break;
        case TT_AND:
            //AND between current cell and cell to right -> cell to left
            UpdateMemory(Row,Col-1,(AccessMemory(Row,Col) && AccessMemory(Row, Col + 1)));
            break;
        case TT_OR:
            //OR between current cell and cell to right -> cell to left
            UpdateMemory(Row,Col-1,(AccessMemory(Row,Col) || AccessMemory(Row, Col + 1)));
            break;
        case TT_INVERT:
            //INVERT Cell
            UpdateMemory(Row,Col,!(AccessMemory(Row,Col)));
            break;

        case TT_COLUMN:
            UpdateMemory(Row,Col,Col);
            break;
        case TT_ROW:
            UpdateMemory(Row,Col,Row);
            break;
        case TT_GOTO_COL:
            Col = AccessMemory(Row,Col);
            break;
        case TT_GOTO_ROW:
            Row = AccessMemory(Row,Col);
            break;
            

    }
}

int AccessMemory(int r, int c){
    //return 53;
    for (int i=0; i<MEMORY_SIZE;i++){
        if (CellGrid[i][0]==r && CellGrid[i][1]==c){
            return CellGrid[i][2];
        }
    }
    return 0;
}
void UpdateMemory(int r, int c, bool increment){
    bool found = false;
    //cout << "I : " << c << " " << r << "\n";
    for (int i=0; i<MEMORY_SIZE;i++){
        if (CellGrid[i][0]==r && CellGrid[i][1]==c){
            if (increment){
                CellGrid[i][2]++;
                
            }
            else {
                CellGrid[i][2]--;
                
            }
            found=true;
            break;
        }
    }
    if (!found){
        int value = increment * 2 - 1;
        CellGrid[MemoryPointer][0]=r;
        CellGrid[MemoryPointer][1]=c;
        CellGrid[MemoryPointer][2]=value;
        MemoryPointer++;
    }

}
void UpdateMemory(int r, int c, int value){
    bool found = false;
    for (int i=0; i<MEMORY_SIZE;i++){
        if (CellGrid[i][0]==r && CellGrid[i][1]==c){
            CellGrid[i][2]=value;
            found=true;
            break;
        }
    }
    if (!found){
        CellGrid[MemoryPointer][0]=r;
        CellGrid[MemoryPointer][1]=c;
        CellGrid[MemoryPointer][2]=value;
        MemoryPointer++;
    }

}
