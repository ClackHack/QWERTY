C++ language
QWERTY
Only letters can be used
State can be changed between binary, char, and int
Maybe add customizable key mapping 
Add interpreter function
Write BF to QWERTY Converter

o - output current cell
i- input current cell

c- char mode
b-bin mode
n - integer mode

E - begin loop
(If followed by a ‘q’ run till cell is 0
If followed by a ‘t’ run the cell’s value # times
(Runs n time’s absolutely, regardless of pointer location)
L - close loop

A- move left
D - move right
W - move up
S - move down 

v - decrement 
U- increment

F- And cell and cell to right -> cell to left
R - Or cell and cell to left -> cell to left
J - invert cell

K - Equivalence between cell and cell to right -> Cell to left
P - '<' Between cell and cell to right -> cell to left


X-write column number
Y- write row number 

G - go to current cell # column
H - go to current cell # row

Z - Debug Character