#include "Evaluator.h"
uint8_t tokenIndex = 0;


int16_t valueOfExpr(int16_t tokens[]){
    int16_t value = valueOfTerm(tokens);
    while(tokens[tokenIndex]== PLUS || tokens[tokenIndex] == MINUS){
        if(tokens[tokenIndex] == PLUS){
            tokenIndex++;
            value += valueOfTerm(tokens);
        } else {
            tokenIndex++;
            value -= valueOfTerm(tokens);
        }
    }
    tokenIndex = 0;
    return value;
}

int16_t valueOfTerm(int16_t tokens[]){
    int16_t value = valueOfFactor(tokens);
    
    while(tokens[tokenIndex]== TIMES || tokens[tokenIndex] == DIVIDE){
        if(tokens[tokenIndex] == TIMES){
           tokenIndex++;
           value *= valueOfFactor(tokens);
        } else {
           tokenIndex++;
           value /= valueOfFactor(tokens);
        }
    }
    return value;
}

int16_t valueOfFactor(int16_t tokens[]){
    int16_t value = 0;
    if(tokens[tokenIndex] == LEFTPARENTHESE){
        tokenIndex++;
        value = valueOfExpr(tokens);
        if(tokens[tokenIndex] == RIGHTPARENTHESE){
            tokenIndex++;
        }
    } else if(tokens[tokenIndex] >= 0){
        value = tokens[tokenIndex];
        tokenIndex++;
    }
    return value;
}