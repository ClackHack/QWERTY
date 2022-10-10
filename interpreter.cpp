#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <iomanip>

using namespace std;

ifstream File;

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
const char TT_EQ         ='K';
const char TT_LT         ='P';

const char TT_COLUMN     ='X';
const char TT_ROW=        'Y';
const char TT_GOTO_COL   ='G';
const char TT_GOTO_ROW   ='H';
const char TT_DEBUG      ='Z';

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
    if (argc < 2){
        Error("Expected file path argument");
    }
    //cout <<"Start";
    //cout << argv[1];
    string arg1 = argv[1];
    if (arg1 ==  "i"){
       // cout << "REPL";
        REPL();
        File.close();
        exit(0);
    }
    
    File.open(argv[1]);
    if (File.fail()){
        cout << "Cannot Locate File Specified...\n";
        File.close();
        exit(1);
    }

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
    //cout << "V: " << Row << " " << Col << endl;
    if (argc==3){
        string arg2 = argv[2];
        if (arg2=="i"){
            REPL();
        }
    }

    return 0;
}

void Error(string msg){
    cout << "Error :> " << msg << endl;
    exit(0);
}

void Execute(string raw){
    //cout << "EXECUTING " << raw << endl;
    for (int i=0; i<raw.length(); i++){
        //cout << "EXL > " << i << " : " << raw.at(i)<<   endl;
        switch (raw.at(i)){
            case ';':
                //cout << "Comment\n";
                while (raw.at(i)!='\n'){
                    //cout << "C : " << i << " " << raw.length() << endl;
                    //cout << "-"<< raw.at(i);
                    i++;
                    if (i >= raw.length())
                        break;
                }
                //cout << "CF : " << i << " " << raw.length() << endl;
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
                //cout << "Entering loop subraw : " << subraw << " : " << Row << " " << Col << endl;
                if (runTo0){

                    while ((bool) AccessMemory(Row,Col)){
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
    //cout << "Entering REPL...\n\nEnter '!' to exit\n\n";
    string line;
    
    while (true){
        //cout << "V: " << Row << " " << Col << endl;
        cout <<setfill('0');
        cout << "QW ("<<setw(4)<< Row<<", "<<setw(4)<<Col<<"): > " << setw(0);
        cout << setfill(' ');
        getline(cin,line);
        if (line == "!")
            break;
        Execute(line);
    }
    cout << "\nClosing REPL...\n";

}

void HandleCharacter(char c){
    switch (c){
        case TT_OUTPUT:
            //cout << "VOut: " << Row << " " << Col << endl;
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
            if (MemoryMode==0){
                //Char mode
                char tmp;
                cin >> tmp;
                UpdateMemory(Row,Col,tmp);
            }
            else if (MemoryMode==1){
                //integer mode
                int tmp;
                cin >> tmp;
                UpdateMemory(Row,Col,tmp);

            }
            else if (MemoryMode==2){
                //integer mode
                bool tmp;
                cin >> tmp;
                UpdateMemory(Row,Col,tmp);

            }
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
            Row--;
            break;
        case TT_MUP:
            Row++;
            break;
        
        case TT_DECREMENT:
            UpdateMemory(Row,Col,false);
            break;
        case TT_INCREMENT:
           // cout << "INC : " << Row << " " << Col << endl;
            UpdateMemory(Row,Col,true);
            //cout << "Ac " << AccessMemory(0, 0) << endl;
            break;

        case TT_AND:
            //AND between current cell and cell to right -> cell to left
            //cout << "MEM " << AccessMemory(Row, Col) << endl;
            //cout << ((bool) AccessMemory(Row,Col) && (bool) AccessMemory(Row, Col + 1)) << endl;
            UpdateMemory(Row,Col-1,(int) ((bool) AccessMemory(Row,Col) && (bool) AccessMemory(Row, Col + 1)));
            break;
        case TT_OR:
            //OR between current cell and cell to right -> cell to left
            UpdateMemory(Row,Col-1,(int)((bool)AccessMemory(Row,Col) || (bool) AccessMemory(Row, Col + 1)));
            break;
        case TT_INVERT:
            //INVERT Cell
            UpdateMemory(Row,Col,!(AccessMemory(Row,Col)));
            break;
        case TT_EQ:
            //cout << "EQ " << (int)((bool)(AccessMemory(Row,Col) ==  AccessMemory(Row, Col + 1)));
            UpdateMemory(Row,Col-1,(int)((bool)(AccessMemory(Row,Col) ==  AccessMemory(Row, Col + 1))));
            break;
        case TT_LT:
            UpdateMemory(Row,Col-1,(int)((bool)(AccessMemory(Row,Col) <  AccessMemory(Row, Col + 1))));
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
        case TT_DEBUG:
            cout << "DBG\n";
            cout << "   ";
            cout << setw(5);
            cout << "R"<< setw(5) << "C"<< setw(5) <<  "V" << endl;
            for (int i=0;i<MemoryPointer+1;i++){
                cout << "|> " << setw(5) << CellGrid[i][0] << " " << setw(5)<< CellGrid[i][1] 
                << " "<< setw(5) << CellGrid[i][2] << endl; 
            }
            break;
            

    }
}

int AccessMemory(int r, int c){
    //return 53;
    for (int i=0; i<MemoryPointer+1;i++){
        if (CellGrid[i][0]==r && CellGrid[i][1]==c){
            return CellGrid[i][2];
        }
    }
    return 0;
}
void UpdateMemory(int r, int c, bool increment){
    //cout << "Bool Up\n";
    bool found = false;
    //cout << "I : " << c << " " << r << "\n";
    for (int i=0; i<MemoryPointer+1;i++){
        if (CellGrid[i][0]==r && CellGrid[i][1]==c){
            if (increment){
                CellGrid[i][2]++;
                //cout << "ININC> " << r << " " << c << endl;
                
            }
            else {
                CellGrid[i][2]--;
                
            }
            found=true;
            break;
        }
    }
    if (!found){
        MemoryPointer++;
        int value = increment * 2 - 1;
        //cout << "MAKENEW " << value << " " << r << " " << c << endl ;
        CellGrid[MemoryPointer][0]=r;
        CellGrid[MemoryPointer][1]=c;
        CellGrid[MemoryPointer][2]=value;
        
    }

}
void UpdateMemory(int r, int c, int value){
    bool found = false;
    for (int i=0; i<MemoryPointer+1;i++){
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
