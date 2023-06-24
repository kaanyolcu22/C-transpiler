#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_INPUT_SIZE 256
#define MAX_LINE_LENGTH 256
#define MAX_TOKENS 256

// created char input array with MAX_INPUT_SIZE, it declared in line 8 as 256
char input[MAX_INPUT_SIZE];
// created values array with MAX_INPUT_SIZE, it declared in line 8 as 256
long long int values[MAX_INPUT_SIZE];
// created char variables array with MAX_INPUT_SIZE, it declared in line 8 as 256
char *variables[MAX_INPUT_SIZE];
// created int numVariables for a counter
int numVariables = 0;
// created a tokentype for enums
int number_of_expr=1; // yeni

// created a bool flag for checking if the input is valid or not
bool flag = false;
// output file
FILE *outputFile;


typedef enum
{
    NULL_TOKEN,
    LONGLONG,
    INTEGER,
    VARIABLE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULUS,
    OR,
    AND,
    EQUALS,
    XOR,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    LEFT_ROTATE,
    RIGHT_ROTATE,
    BITWISE_NOT,
    LEFT_PAREN,
    RIGHT_PAREN,
    COMMA,
} TokenType;

// created a struct for expressions
typedef struct
{
    TokenType type;
    long long int value;
    char *var;
    struct Expression *left;
    struct Expression *right;
    int expression_num ;


} Expression;

// created a struct for tokens
typedef struct
{
    TokenType type;
    char *string;

} Token;

// created a struct for 5 which includes Token struct
typedef struct
{
    Token *tokens;
    int numTokens;
    int currentToken;
    bool hasError;
    bool hasEqual;
} TokenList;

// createExpression function, this function called to create expressions
Expression *createExpression(TokenType type, long long int value, char *var, Expression *left, Expression *right,int expression_num)
{
    Expression *expression = (Expression *)malloc(sizeof(Expression));
    // if expression type is equals we assign the right value to left value
    if (type == EQUALS)
    {
        expression->type = type;
        expression->left = left;
        expression->right = right;
        expression->value = right->value;
        expression->var = left->var;
        expression->expression_num=expression_num; // yeni
    }
    else
    {
        expression->type = type;
        expression->left = left;
        expression->right = right;
        expression->value = value;
        expression->var = var;
        expression->expression_num=expression_num;// yeni
    }
    return expression;
}

// Declaring functions in the code

// checks is a varible already assigned
bool IsAlreadyVariable(char *var);
// getting the value of an assigned value
long long int getValue(char *var);
// checks if the input is valid or not. (Some \ ; chars.)
bool isStringValid(char *string);
// Parse Base functions. These functions parsing the tokenlist and make a Parse Tree.
Expression *parseBase(TokenList *tokenlist);
Expression *parseBase2(TokenList *tokenlist);
Expression *parseBase3(TokenList *tokenlist);
Expression *parseBase4(TokenList *tokenlist);
Expression *parseBase5(TokenList *tokenList);
// Parse Expression function. Make the expressions.
Expression *parseExpression(TokenList *tokenlist);

// createToken function, creating and return a token
Token *createToken(TokenType type, char *string, int startPos)
{
    Token *token = malloc(sizeof(Token));
    token->type = type;
    token->string = string;
    return token;
}

// createTokenList function, creating a token list of tokens
TokenList *createTokenList()
{
    TokenList *list = malloc(sizeof(TokenList));
    list->tokens = malloc(MAX_TOKENS * sizeof(Token));
    list->numTokens = 0;
    list->currentToken = 0;
    list->hasError = false;
    return list;
}

// void addToken function, add tokens to the tokenlist
void addToken(TokenList *list, Token *token)
{
    list->tokens[list->numTokens++] = *token;
}
// declaring what is multiply function
// LEXER function, this function called to check the input and add tokens to the tokenlist. This function is the one of the main functions in the program.
void lexer(char *input, TokenList *tokenList)
{
    int i = 0;
    int startPos = 0;
    // if the input[i] is '\0' code stops the working
    while (input[i] != '\0')
    {
        // checking if the input[i] is a space
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        // checking if the input[i] is a digit
        if (isdigit(input[i]))
        {
            int j = i;
            while (isdigit(input[j]))
            {
                j++;
            }
            char *num = malloc((j - i + 1) * sizeof(char));
            memcpy(num, input + i, j - i);
            num[j - i] = '\0';
            addToken(tokenList, createToken(LONGLONG, num, i));
            i = j;
            continue;
        }
        // checking if the input[i] is a EQUALS and create and add a EQUALS token to the tokenlist
        if (input[i] == '=')
        {
            addToken(tokenList, createToken(EQUALS, "=", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a | and create and add a OR token to the tokenlist
        if (input[i] == '|')
        {
            addToken(tokenList, createToken(OR, "|", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a % and break the loop because this sign is for the comments
        /*if (input[i] == '%')
        {
            break;
        }*/
        // checking if the input[i] is a + and create and add a PLUS token to the tokenlist
        if (input[i] == '+')
        {
            addToken(tokenList, createToken(PLUS, "+", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a - and create and add a MINUS token to the tokenlist
        if (input[i] == '-')
        {
            addToken(tokenList, createToken(MINUS, "-", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a * and create and add a MULTIPLY token to the tokenlist
        if (input[i] == '*')
        {
            addToken(tokenList, createToken(MULTIPLY, "*", startPos));
            i++;
            continue;
        }
        if (input[i] == '/')
        {
            addToken(tokenList, createToken(DIVIDE, "/", startPos));
            i++;
            continue;
        }
        if (input[i] == '%'){
            addToken(tokenList, createToken(MODULUS, "%", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a , and create and add a COMMA token to the tokenlist
        if (input[i] == ',')
        {
            addToken(tokenList, createToken(COMMA, ",", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a ( and create and add a LEFT_PAREN token to the tokenlist
        if (input[i] == '(')
        {
            addToken(tokenList, createToken(LEFT_PAREN, "(", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a ) and create and add a RIGHT_PAREN token to the tokenlist
        if (input[i] == ')')
        {
            addToken(tokenList, createToken(RIGHT_PAREN, ")", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a & and create and add an AND token to the tokenlist
        if (input[i] == '&')
        {
            addToken(tokenList, createToken(AND, "&", startPos));
            i++;
            continue;
        }
        // checking if the input[i] is a alphanumeric
        if (isalpha(input[i]))
        {
            int j = i;
            while (isalpha(input[j]))
            {
                j++;
            }
            char *var = malloc((j - i + 1) * sizeof(char));
            memcpy(var, input + i, j - i);
            var[j - i] = '\0';
            // checking if it is a varible
            if (strcmp(var, "xor") != 0 && strcmp(var, "ls") != 0 && strcmp(var, "rs") != 0 && strcmp(var, "lr") != 0 &&
                strcmp(var, "rr") != 0 && strcmp(var, "not") != 0)
            {
                // adding a VARIABLE token with var to tokenlist
                addToken(tokenList, createToken(VARIABLE, var, startPos));
                i = j;
                continue;
            }
            else
            {
                // checking if the var is an xor and create and add a XOR token to the tokenlist
                if (strcmp(var, "xor") == 0)
                {
                    addToken(tokenList, createToken(XOR, "xor", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an ls and create and add a LEFT_SHIFT token to the tokenlist
                if (strcmp(var, "ls") == 0)
                {
                    addToken(tokenList, createToken(LEFT_SHIFT, "ls", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an rs and create and add a RIGHT_SHIFT token to the tokenlist
                if (strcmp(var, "rs") == 0)
                {
                    addToken(tokenList, createToken(RIGHT_SHIFT, "rs", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an lr and create and add a LEFT_ROTATE token to the tokenlist
                if (strcmp(var, "lr") == 0)
                {
                    addToken(tokenList, createToken(LEFT_ROTATE, "lr", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an lr and create and add a RIGHT_ROTATE token to the tokenlist
                if (strcmp(var, "rr") == 0)
                {
                    addToken(tokenList, createToken(RIGHT_ROTATE, "rr", startPos));
                    i = j;
                    continue;
                }
                // checking if the var is an not and create and add a BITWISE_NOT token to the tokenlist
                if (strcmp(var, "not") == 0)
                {
                    addToken(tokenList, createToken(BITWISE_NOT, "not", startPos));
                    i = j;
                    continue;
                }
            }
        }
    }
}

// the operator functions
int multiply(int a, int b)
{
    return a * b;
}

int divide(int a, int b,TokenList *tokenList)
{
    //if b is 0 , then we have an error and to not get a segmentation fault we return 0
    if (b == 0)
    {
        tokenList->hasError=true;
        return 0;
    }
    return a / b;
}

int modulus(int a, int b,TokenList *tokenList)
{
    //if b is 0 , then we have an error and to not get a segmentation fault we return 0
    if (b == 0)
    {
        tokenList->hasError=true;
        return 0;
    }
    return a % b;
}
// declaring what is minus function
int minus(int a, int b)
{
    return a - b;
}
// declaring what is plus function
int plus(int a, int b)
{
    return a + b;
}
// declaring what is or function
int or (int a, int b)
{
    return a | b;
}
// declaring what is and function
int and (int a, int b)
{
    return a & b;
}
// declaring what is xor function
int xor (int a, int b) {
    return a ^ b;
}
// declaring what is leftShift function
int leftShift(int a, int b)
{
    return a << b;
}
// declaring what is rightShift function
int rightShift(int a, int b)
{
    return a >> b;
}
// declaring what is leftRotate function
int leftRotate(int a, int b)
{
    return (a << b) | (a >> (32 - b));
}
// declaring what is rightRotate function
int rightRotate(int a, int b)
{
    return (a >> b) | (a << (32 - b));
}
// declaring what is bitwiseNot function
int bitwiseNot(int a)
{
    return ~a;
}



Expression *parseExpression(TokenList *tokenList)
{
    Expression *expression = parseBase5(tokenList);
    while (tokenList->tokens[tokenList->currentToken].type == EQUALS)
    {
        flag = true;
        if (expression->type != VARIABLE)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *next_expr = parseBase5(tokenList);
        if(next_expr->type == LONGLONG){
            // if the next expression is a number, then we need to store it in a variable and write the corresponding LLVM code .
            fprintf(outputFile,"store i32 %d, i32* %%%s\n", next_expr->value, expression->var);
        }
        else{
            // if the next expression is a variable, then we need to store it in a variable and write the corresponding LLVM code .
            fprintf(outputFile,"store i32 %%%d, i32* %%%s\n", next_expr->expression_num, expression->var);
        }

        expression = createExpression(EQUALS, 0, NULL, expression, next_expr, number_of_expr);
    }

    if (tokenList->tokens[tokenList->currentToken].type == VARIABLE || tokenList->tokens[tokenList->currentToken].type == INTEGER || tokenList->tokens[tokenList->currentToken].type == LONGLONG || tokenList->tokens[tokenList->currentToken].type == XOR || tokenList->tokens[tokenList->currentToken].type == LEFT_SHIFT || tokenList->tokens[tokenList->currentToken].type == RIGHT_SHIFT || tokenList->tokens[tokenList->currentToken].type == LEFT_ROTATE || tokenList->tokens[tokenList->currentToken].type == RIGHT_ROTATE || tokenList->tokens[tokenList->currentToken].type == BITWISE_NOT)
    {

        tokenList->hasError = true;
    }

    return expression;
}

Expression *parseBase5(TokenList *tokenList)
{

    Expression *expression = parseBase4(tokenList);
    // if the current token is a OR token, then we need to create an expression and write the corresponding LLVM code .
    // check for the type of the expression and the next expression and write the corresponding LLVM code .
    while (tokenList->tokens[tokenList->currentToken].type == OR)
    {

        tokenList->currentToken++;

        Expression *next_expr = parseBase4(tokenList);
        if(expression->type==LONGLONG && next_expr->type != LONGLONG){
            fprintf(outputFile,"%%%d = or i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
        }
        else if(expression->type!=LONGLONG && next_expr->type == LONGLONG){
            fprintf(outputFile,"%%%d = or i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
        }
        else if(expression->type==LONGLONG && next_expr->type == LONGLONG){
            fprintf(outputFile,"%%%d = or i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
        }
        else{
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
        }
        expression = createExpression(OR, or (expression->value, next_expr->value), NULL, expression, next_expr, number_of_expr);
        number_of_expr++;
    }
    return expression;
}

Expression *parseBase4(TokenList *tokenList)
{

    Expression *expression = parseBase3(tokenList);
    // if the next token is AND, then we need to store it in a variable and write the corresponding LLVM code .
    while (tokenList->tokens[tokenList->currentToken].type == AND)
    {

        tokenList->currentToken++;

        Expression *next_expr = parseBase3(tokenList);
        if(expression->type==LONGLONG && next_expr->type != LONGLONG){
            fprintf(outputFile,"%%%d = and i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
        }
        else if(expression->type!=LONGLONG && next_expr->type == LONGLONG){
            fprintf(outputFile,"%%%d = and i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
        }
        else if(expression->type==LONGLONG && next_expr->type == LONGLONG){
            fprintf(outputFile,"%%%d = and i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
        }
        else{

            fprintf(outputFile,"%%%d = and i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
        }
        expression = createExpression(AND, and(expression->value, next_expr->value), NULL, expression, next_expr, number_of_expr);
        number_of_expr++;
    }
    return expression;
}

Expression *parseBase3(TokenList *tokenList)
{

    Expression *expression = parseBase2(tokenList);
    while (tokenList->tokens[tokenList->currentToken].type == PLUS || tokenList->tokens[tokenList->currentToken].type == MINUS)
    {

        TokenType temp = tokenList->tokens[tokenList->currentToken].type;

        tokenList->currentToken++;

        Expression *next_expr = parseBase2(tokenList);

        if (temp == MINUS)
        {
            if(expression->type==LONGLONG && next_expr->type != LONGLONG){
                fprintf(outputFile,"%%%d = sub i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
            }
            else if(expression->type!=LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = sub i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
            }
            else if(expression->type==LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = sub i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
            }
            else {
                fprintf(outputFile,"%%%d = sub i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
            }
            expression = createExpression(MINUS, minus(expression->value, next_expr->value), NULL, expression, next_expr, number_of_expr);
            number_of_expr++;
        }
        else
        {
            if(expression->type==LONGLONG && next_expr->type != LONGLONG){
                fprintf(outputFile,"%%%d = add i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
            }
            else if(expression->type!=LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = add i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
            }
            else if(expression->type==LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = add i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
            }
            else {
                fprintf(outputFile,"%%%d = add i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
            }

            expression = createExpression(PLUS, plus(expression->value, next_expr->value), NULL, expression, next_expr, number_of_expr);
            number_of_expr++;
        }
    }
    return expression;
}

Expression *parseBase2(TokenList *tokenList)
{
    Expression *expression = parseBase(tokenList);

    while (tokenList->tokens[tokenList->currentToken].type == MULTIPLY || tokenList->tokens[tokenList->currentToken].type == DIVIDE ||
           tokenList->tokens[tokenList->currentToken].type == MODULUS)
    {
        TokenType temp = tokenList->tokens[tokenList->currentToken].type;

        tokenList->currentToken++;

        Expression *next_expr = parseBase(tokenList);
        // we look for the operations : modulus, divide and multiply and write the corresponding LLVM code .
        // check for the type of the expression and the next expression and write the corresponding LLVM code .
        if (temp == MULTIPLY){
            if(expression->type==LONGLONG && next_expr->type != LONGLONG){
                fprintf(outputFile,"%%%d = mul i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
            }
            else if(expression->type!=LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = mul i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
            }
            else if(expression->type==LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = mul i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
            }
            else {
                fprintf(outputFile,"%%%d = mul i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
            }
            expression = createExpression(MULTIPLY, multiply(expression->value, next_expr->value), NULL, expression, next_expr, number_of_expr);
        } else if( temp == MODULUS){
            if(expression->type==LONGLONG && next_expr->type != LONGLONG){
                fprintf(outputFile,"%%%d = srem i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
            }
            else if(expression->type!=LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = srem i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
            }
            else if(expression->type==LONGLONG && next_expr->type == LONGLONG){
                fprintf(outputFile,"%%%d = srem i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
            }
            else {
                fprintf(outputFile,"%%%d = srem i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
            }
            expression = createExpression(MODULUS, modulus(expression->value, next_expr->value,tokenList), NULL, expression, next_expr, number_of_expr);
        }
        else{
            if (expression->type == LONGLONG && next_expr->type != LONGLONG) {

                fprintf(outputFile,"%%%d = sdiv i32 %d, %%%d\n", number_of_expr, expression->value, next_expr->expression_num);
            } else if (expression->type != LONGLONG && next_expr->type == LONGLONG) {

                fprintf(outputFile,"%%%d = sdiv i32 %%%d, %d\n", number_of_expr, expression->expression_num, next_expr->value);
            } else if (expression->type == LONGLONG && next_expr->type == LONGLONG) {

                fprintf(outputFile,"%%%d = sdiv i32 %d, %d\n", number_of_expr, expression->value, next_expr->value);
            } else {

                fprintf(outputFile,"%%%d = sdiv i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, next_expr->expression_num);
            }
            expression = createExpression(DIVIDE, divide(expression->value, next_expr->value, tokenList), NULL, expression, next_expr, number_of_expr);

        }
        number_of_expr++;

    }
    return expression;
}

Expression *parseBase(TokenList *tokenList)
{

    if (tokenList->tokens[tokenList->currentToken].type == LONGLONG)
    {
        // if the token is a long long we create an expression with the value of the long long and return it
        Expression *expression = createExpression(LONGLONG, atoll(tokenList->tokens[tokenList->currentToken].string), NULL, NULL, NULL, number_of_expr);

        fprintf(outputFile,"%%%d = add i32 %d, %d\n", number_of_expr, 0, expression->value);
        number_of_expr++;
        tokenList->currentToken++;


        return expression;
    }

    if (tokenList->tokens[tokenList->currentToken].type == VARIABLE)
    {
        Expression *expression;

        if (IsAlreadyVariable(tokenList->tokens[tokenList->currentToken].string))
        {
            // if when we the next token is equals we need to give false or true to the expression whether is_assignment is true or false
            if(tokenList->tokens[tokenList->currentToken + 1].type == EQUALS){
                expression = createExpression(VARIABLE, getValue(tokenList->tokens[tokenList->currentToken].string), tokenList->tokens[tokenList->currentToken].string, NULL, NULL,number_of_expr);
            }
            else{
                expression = createExpression(VARIABLE, getValue(tokenList->tokens[tokenList->currentToken].string), tokenList->tokens[tokenList->currentToken].string, NULL, NULL,number_of_expr);
                // we load the variable from the memory
                fprintf(outputFile,"%%%d = load i32, i32* %%%s\n", number_of_expr, tokenList->tokens[tokenList->currentToken].string);
                number_of_expr++;
            }

        }
        else
        {
            // if the  undefined variable is in right side of the equals sign it is an error
            if(tokenList->tokens[tokenList->currentToken + 1].type != EQUALS ){
                tokenList->hasError = true;
            }
            expression = createExpression(VARIABLE, 0, tokenList->tokens[tokenList->currentToken].string, NULL, NULL,number_of_expr);

            // we allocate memory for the variable when we first see it and if is in left side of the equals sign
            fprintf(outputFile,"%%%s = alloca i32\n", tokenList->tokens[tokenList->currentToken].string);


        }
        tokenList->currentToken++;
        // if the next token is left throw error
        if (tokenList->tokens[tokenList->currentToken].type == LEFT_PAREN)
        {
            tokenList->hasError = true;
        }
        return expression;
    }

    if (tokenList->tokens[tokenList->currentToken].type == XOR)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        // if the next token is not comma throw error
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        // if the next token is not right parenthesis throw error
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        // we should look for the cases seperately if the first expression is a number or a variable and the same for the second expression
        if(expression->type==LONGLONG && expression2->type != LONGLONG){
            fprintf(outputFile,"%%%d = xor i32 %d, %%%d\n", number_of_expr, expression->value, expression2->expression_num);
        }
        else if(expression->type!=LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = xor i32 %%%d, %d\n", number_of_expr, expression->expression_num, expression2->value);
        }
        else if(expression->type==LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = xor i32 %d, %d\n", number_of_expr, expression->value, expression2->value);
        }
        else {
            fprintf(outputFile,"%%%d = xor i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, expression2->expression_num);
        }
        return createExpression(XOR, xor(expression->value, expression2->value), NULL, expression, expression2,number_of_expr++);
    }
     // if the next token is left rotate
    if (tokenList->tokens[tokenList->currentToken].type == LEFT_ROTATE)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        // first we should left shift our first expression by the second expression
        // then we should substrat 32 by the second expression
        // then we should right shift the first expression by the result of the substraction
        // then we should or the result of the right shift with the result of the left shift
        if(expression->type==LONGLONG && expression2->type != LONGLONG){
            fprintf(outputFile,"%%%d = shl i32 %d, %%%d\n", number_of_expr, expression->value, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %%%d\n", number_of_expr,32, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }
        else if(expression->type!=LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = shl i32 %%%d, %d\n", number_of_expr, expression->expression_num, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %d\n", number_of_expr,32, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }
        else if(expression->type==LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = shl i32 %d, %d\n", number_of_expr, expression->value, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %d\n", number_of_expr,32, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }

        else {
            fprintf(outputFile,"%%%d = shl i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %%%d\n", number_of_expr,32, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }
        return createExpression(LEFT_ROTATE, leftRotate(expression->value, expression2->value), NULL, expression, expression2,number_of_expr++);
    }
    // if the next token is right rotate
    if (tokenList->tokens[tokenList->currentToken].type == RIGHT_ROTATE)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        // if the next token is not a comma, then we have an error
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        // we need to parse the second expression
        Expression *expression2 = parseExpression(tokenList);
        // if the next token is not a right parenthesis, then we have an error
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        // first we should right shift our first expression by the second expression
        // then we should substrat 32 by the second expression
        // then we should left shift the first expression by the result of the substraction
        // then we should or the result of the right shift with the result of the left shift
        if(expression->type==LONGLONG && expression2->type != LONGLONG){
            fprintf(outputFile,"%%%d = ashr i32 %d, %%%d\n", number_of_expr, expression->value, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %%%d\n", number_of_expr,32, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = shl i32 %d, %%%d\n", number_of_expr, expression->value, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }

        else if(expression->type!=LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %d\n", number_of_expr, expression->expression_num, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %d\n", number_of_expr,32, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = shl i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }
        else if(expression->type==LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = ashr i32 %d, %d\n", number_of_expr, expression->value, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %d\n", number_of_expr,32, expression2->value);
            number_of_expr++;
            fprintf(outputFile,"%%%d = shl i32 %d, %%%d\n", number_of_expr, expression->value, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }
        else {
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = sub  i32 %d , %%%d\n", number_of_expr,32, expression2->expression_num);
            number_of_expr++;
            fprintf(outputFile,"%%%d = shl i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, number_of_expr-1);
            number_of_expr++;
            fprintf(outputFile,"%%%d = or i32 %%%d, %%%d\n", number_of_expr, number_of_expr-3, number_of_expr-1);
        }
        return createExpression(RIGHT_ROTATE, rightRotate(expression->value, expression2->value), NULL, expression, expression2,number_of_expr++);
    }
    // if the next token is bitwise not

    if (tokenList->tokens[tokenList->currentToken].type == BITWISE_NOT)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        if(expression->type==LONGLONG){
            fprintf(outputFile,"%%%d = xor i32 %d, -1\n", number_of_expr, expression->value);
        }
        else {
            fprintf(outputFile,"%%%d = xor i32 %%%d, -1\n", number_of_expr, expression->expression_num);
        }
        return createExpression(BITWISE_NOT, bitwiseNot(expression->value), NULL, expression, NULL,number_of_expr++);
    }
    // if the next token is left shift
    if (tokenList->tokens[tokenList->currentToken].type == RIGHT_SHIFT)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;

        if(expression->type==LONGLONG && expression2->type != LONGLONG){
            fprintf(outputFile,"%%%d = ashr i32 %d, %%%d\n", number_of_expr, expression->value, expression2->expression_num);
        }
        else if(expression->type!=LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %d\n", number_of_expr, expression->expression_num, expression2->value);
        }
        else if(expression->type==LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = ashr i32 %d, %d\n", number_of_expr, expression->value, expression2->value);
        }
        else {
            fprintf(outputFile,"%%%d = ashr i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, expression2->expression_num);
        }
        return createExpression(RIGHT_SHIFT, rightShift(expression->value, expression2->value), NULL, expression, expression2,number_of_expr++);
    }
    // if the next token is left shift
    if (tokenList->tokens[tokenList->currentToken].type == LEFT_SHIFT)
    {
        tokenList->currentToken++;
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != COMMA)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        Expression *expression2 = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        if(expression->type==LONGLONG && expression2->type != LONGLONG){
            fprintf(outputFile,"%%%d = shl i32 %d, %%%d\n", number_of_expr, expression->value, expression2->expression_num);
        }
        else if(expression->type!=LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = shl i32 %%%d, %d\n", number_of_expr, expression->expression_num, expression2->value);
        }
        else if(expression->type==LONGLONG && expression2->type == LONGLONG){
            fprintf(outputFile,"%%%d = shl i32 %d, %d\n", number_of_expr, expression->value, expression2->value);
        }
        else {
            fprintf(outputFile,"%%%d = shl i32 %%%d, %%%d\n", number_of_expr, expression->expression_num, expression2->expression_num);
        }
        return createExpression(LEFT_SHIFT, leftShift(expression->value, expression2->value), NULL, expression, expression2,number_of_expr++);
    }
    if (tokenList->tokens[tokenList->currentToken].type == LEFT_PAREN)
    {
        tokenList->currentToken++;
        Expression *expression = parseExpression(tokenList);
        if (tokenList->tokens[tokenList->currentToken].type != RIGHT_PAREN)
        {
            tokenList->hasError = true;
        }
        tokenList->currentToken++;
        return expression;
    }
    tokenList->hasError = true;
}




// if the current variable is already assigned, return true
bool IsAlreadyVariable(char *var)
{
    for (int i = 0; i < numVariables; i++)
    {
        if (strcmp(var, variables[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

// return the value of an assigned variable
long long int getValue(char *var) // checking if the var is an lr and create and add a LEFT_ROTATE token to the tokenlist
{
    for (int i = 0; i < numVariables; i++)
    {
        if (strcmp(var, variables[i]) == 0)
        {
            return values[i];
        }
    }
}

// when we take input, we will check if all chars are valid or not
bool isStringValid(char *string)
{
    for (int i = 0; i < strlen(string); i++)
    {
        // this is the end of the string
        if (string[i] == '\n' || string[i] == '%')
        {
            return true;
        }
        // checking the chars
        if (string[i] >= 'a' && string[i] <= 'z' || string[i] >= 'A' && string[i] <= 'Z' || string[i] >= '0' && string[i] <= '9' || string[i] == ' ' || string[i] == '(' || string[i] == ')' || string[i] == ',' || string[i] == '=' || string[i] == '+' || string[i] == '-' || string[i] == '*'|| string[i] == '/' || string[i] == '%' || string[i] == '&' || string[i] == '|')
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}
// checking if the parentheses are valid or not
bool isParenthesesValid(char *string)
{
    int leftParentheses = 0;
    int rightParentheses = 0;
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == '(')
        {
            leftParentheses++;
        }
        if (string[i] == ')')
        {
            rightParentheses++;
        }
    }
    if (leftParentheses == rightParentheses)
    {
        return true;
    }
    return false;
}

int main(int argc, char *argv[])
{
    // opening the input file
    FILE  *textfile;
    bool isInputValid = true;
    char    *filename = argv[1];
    char    line[MAX_LINE_LENGTH];
    textfile = fopen(filename, "r");
    // opening the output file
    // output file should be argv[1] - ".adv" + ".ll"
    char *outputFileName = malloc(strlen(filename) - 3 + 3);
    strcpy(outputFileName, filename);
    outputFileName[strlen(filename) - 3] = '\0';
    strcat(outputFileName, "ll");
    //initializing the line number
    int line_number = 1;
    outputFile = fopen(outputFileName, "w");
    // print the header of the output file
    fprintf(outputFile,"; ModuleID = 'advcalc2ir'\n");
    fprintf(outputFile,"declare i32 @printf(i8*, ...)\n");
    fprintf(outputFile,"@print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n");
    fprintf(outputFile,"define i32 @main() {\n");
    while (fgets(line, MAX_LINE_LENGTH, textfile))
    {
        // if input is null, it will break the loop and stops the program
        if (line ==NULL){
            break;
        }

        // deleting spaces front of the input
        int i = 0;
        while (line[i] == ' ')
        {
            i++;
        }
        strcpy(line, line + i);

        // if the first variable of input continued
        if (line[0] == '\n')
        {
            line_number++;
            continue;
        }
        // if the all line is a comment line continued
        /*if (input[0] == '%')
        {
            printf("> ");
            continue;
        }*/

        // checking if the input is valid
        if (!isStringValid(line))
        {
            printf("Error on line %d!\n",line_number);
            line_number++;
            isInputValid = false;
            continue;
        }
        // checking if the parentheses are valid
        if (!isParenthesesValid(line))
        {
            printf("Error on line %d!\n",line_number);

            line_number++;
            isInputValid = false;
            continue;
        }
        // checking if the input has equal sign or not
        bool hasEqualSign = false;
        for (int i = 0; i < strlen(line); i++)
        {
            if (line[i] == '=')
            {
                hasEqualSign = true;
            }
        }

        // created a token list
        TokenList *tokenList = createTokenList();
        // lexed the input
        lexer(line, tokenList);


        tokenList->hasEqual = hasEqualSign;

        // parsed the input
        Expression *expression = parseExpression(tokenList);

        // if we logged the error, program says error and back to the taking input
        if (tokenList->hasError)
        {
            printf("Error on line %d!\n",line_number);
            line_number++;
            isInputValid = false;
            continue;
        }

        // if the expression is an assignment expression
        if (expression->type == EQUALS)
        {
            if (IsAlreadyVariable(expression->var))
            {
                for (int i = 0; i < numVariables; i++)
                {
                    if (strcmp(expression->var, variables[i]) == 0)
                    {
                        values[i] = expression->value;
                    }
                }
            }
            else
            {
                variables[numVariables] = expression->var;
                values[numVariables] = expression->value;
                numVariables++;
            }
            variables[numVariables] = expression->var;
            values[numVariables] = expression->value;
            numVariables++;
        }
        else
        {
            // if the expression is not an assignment expression, it will print it
            fprintf(outputFile,"call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %%%d )\n", expression->expression_num);
            number_of_expr++;
            //printf("%lld\n", expression->value);
        }
        line_number++;
    }
    // footer of the output file
    fprintf(outputFile,"ret i32 0\n");
    fprintf(outputFile,"}\n");


    // closing the files
    fclose(textfile);
    fclose(outputFile);
    // if the input is not valid, it will delete the output file
    if(!isInputValid)
    {
        remove(outputFileName);
    }

    return 0;
}
