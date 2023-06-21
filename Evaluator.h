#pragma once
#include <stdint.h>
//Recursive descent parser tha follows this CFG
// expr -> term {add-op term}
// term -> factor { mult-op factor }
// factor -> ( expr ) | number
// add-op -> + | -
// mult-op -> * | /
// number -> any unsigned number in the range of int16_t
//Value that keeps track of what token is currenltly on


//Token values that represent +,-,*,/,(,)
#define PLUS -1
#define MINUS -2
#define TIMES -3
#define DIVIDE -4
#define LEFTPARENTHESE -5
#define RIGHTPARENTHESE -6

//Parser function prototypes
int16_t valueOfExpr(int16_t tokens[]);
int16_t valueOfTerm(int16_t tokens[]);
int16_t valueOfFactor(int16_t tokens[]);

