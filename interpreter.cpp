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

void Execute(string raw){
    for (int i; i=raw.length(); i++){
        switch (raw.at(i)){
            case ';':
                while (raw.at(i)!='\n'){
                    i++;
                }
                break;
            case ' ' :
            case '\t':
            case '\r':
            case '\n':
                break;
            case TT_START_LOOP:
                string subraw="";
                //Goto next non white space character
                while (!isspace(raw.at(i)))
                    i++;
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
                if (runTo0){
                    while (AccessMemory(Row,Col)){
                        Execute(subraw);
                    }
                }
                else{
                    for (int i = 0;i<AccessMemory(Row,Col);i++){
                        Execute(subraw);
                    }
                }

        }
    }
}
void REPL(){

}

void HandleCharacter(char c){
    switch (c){
        case TT_OUTPUT:
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
    return 53;
    for (int i=0; i<MEMORY_SIZE;i++){
        if (CellGrid[i][0]==r && CellGrid[i][1]==c){
            return CellGrid[i][2];
        }
    }
    return 0;
}
void UpdateMemory(int r, int c, bool increment){
    bool found = false;
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
