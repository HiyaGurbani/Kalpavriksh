#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>
#include<stdbool.h>

#define MAX_SIZE 1000


int isValidExpression(char expression[]){  
    for (int charIndex=0 ; charIndex<strlen(expression) ; charIndex++){
        if (expression[charIndex] == '\n') continue;
        if (!((expression[charIndex]>='0' && expression[charIndex]<='9') || 
               expression[charIndex]=='+' || expression[charIndex]=='-' || 
               expression[charIndex]=='*' || expression[charIndex]=='/' || 
               expression[charIndex]==' ')){
            printf("Error: Invalid Expression");
            return 0;
        }
    }
    return 1;
}

int isValidSequence(char expression[]){
    int length = strlen(expression);
    int hasDigit = 0;

    //Checking if expression is empty or has only whitespaces
    for (int i = 0; i < length; i++){
        if (isdigit((unsigned char)expression[i])){
            hasDigit = 1;
            break;
        }
    }
    if (!hasDigit){
        printf("Error: Expression must contain at least one number\n");
        return 0;
    }

    int lastWasOperator = 1;
    for (int charIndex=0 ; charIndex<length ; charIndex++){
        char currentChar = expression[charIndex];

        if (currentChar==' ' || currentChar=='\n') continue;

        if (isdigit((unsigned char)currentChar)){
            lastWasOperator = 0;

            while(charIndex+1 < length && isdigit((unsigned char) expression[charIndex+1])) 
                charIndex++;
        }
        else {
            if (lastWasOperator){
                printf("Error: Invalid Sequence near %c\n", currentChar);
                return 0;
            }
            lastWasOperator = 1;
        }
    }

    if (lastWasOperator){
        printf("Error: Expression cannot end with an operator\n");
        return 0;
    }

    return 1;
}

int getPrecedence(char operator){
    if (operator == '*' || operator == '/') return 2;
    if (operator == '+' || operator == '-') return 1;
    return 0;
}

bool divisionByZero = false; 
long long applyOperation(long long leftOperand , long long rightOperand, char operator){
    switch(operator){
        case '+': return leftOperand+rightOperand;
        case '-': return leftOperand-rightOperand;
        case '*': return leftOperand*rightOperand;
        case '/':
            if (rightOperand==0){
                printf("Error: Division by Zero\n");
                divisionByZero = true;
                return 0;
            }
            return leftOperand/rightOperand;
    }
    return 0;
}

long long evaluateExpression(char expression[]){
    long long operandStack[MAX_SIZE];
    int operandTop = -1;
    char operatorStack[MAX_SIZE];
    int operatorTop = -1;

    int position = 0, size = strlen(expression);

    //Stack-based Single Pass Evaluation
    while(position < size){
        char currentChar = expression[position];

        if (currentChar == ' ' || currentChar == '\n'){
            position++;
            continue;
        }

        if (isdigit((unsigned char)currentChar)){
            long long number = 0;
            while(position<size && isdigit((unsigned char)expression[position])){
                number = number*10 + (expression[position]-'0');
                position++;
            }
            operandStack[++operandTop] = number;
        } else {
            while(operatorTop >=0 &&
                  getPrecedence(operatorStack[operatorTop]) >= getPrecedence(currentChar)){
                    long long rightOperand = operandStack[operandTop--];
                    long long leftOperand = operandStack[operandTop--];
                    char operator = operatorStack[operatorTop--];
                    long long result = applyOperation(leftOperand, rightOperand, operator);
                    if (divisionByZero) return 0; 
                    operandStack[++operandTop] = result;
                  }
                  operatorStack[++operatorTop] = currentChar;
                  position++; 
        }
    }

    while (operatorTop >= 0){
        long long rightOperand = operandStack[operandTop--];
        long long leftOperand = operandStack[operandTop--];
        char operator = operatorStack[operatorTop--];
        long long result = applyOperation(leftOperand, rightOperand, operator);
        if (divisionByZero) return 0; 
        operandStack[++operandTop] = result;
    }

    return operandStack[operandTop];
}

int main(){
    char input[MAX_SIZE];

    printf("Enter Expression: ");
    fgets(input, sizeof(input), stdin);

    if (!isValidExpression(input) || !isValidSequence(input)){
        return 0;
    }

    long long result = evaluateExpression(input);

    if (divisionByZero) {
        return 0;
    }

    if (result > INT_MAX || result < INT_MIN){
        printf("Error: Integer Overflow\n");
        return 0;
    }

    printf("Result: %d\n", (int)result);
    return 0;
}
