#include<stdio.h>
#include<string.h>
#include<ctype.h>

//Step 1: Function to validate string (only a character-level filter)
int validateString(char s[]){
    
    for (int i=0 ; i<strlen(s) ; i++){
        if (s[i] == '\n') continue;
        if (!((s[i]>='0' && s[i]<='9') || s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/' || s[i]==' ')){
            return 0;
        }
    }
    return 1;
}

int numCount, opCount;

//Step 2: Seperation of numbers & operators and Sequence Validation
int tokenizeExpression(char input[], int numbers[], char operators[]){
    int i=0 , n = strlen(input);
    numCount=0; opCount=0;

    while(i<n){
        if (isdigit(input[i])){
            int num=0;
            while(i<n && isdigit(input[i])){
                num = num * 10 + (input[i] - '0');
                i++;
            }
            numbers[numCount++] = num;
        } else if (input[i]=='+' || input[i]=='-' || input[i]=='*' || input[i]=='/'){

            int prev = i - 1, next = i + 1;
            while (prev >= 0 && (input[prev] == ' ' || input[prev] == '\n')) prev--;
            while (next < n && (input[next] == ' ' || input[next] == '\n')) next++;

            if (prev<0 || next>=n || !(isdigit(input[next])) || !(isdigit(input[prev]))){
                printf("Error: Invalid Expression (Sequence is not valid)"); //Sequence isn't valid
                return 0;
            } operators[opCount++] = input[i];
            i++; 
        } else if (input[i] == ' ' || input[i] == '\n') i++;
        else return 0; //Edge case: Won't happen as validateString function ensures that
    }
    return 1;
}

//Step 3: Calculation
void calculateResult(int numbers[], char operators[]){

    //Higher Precedence : * and /
    for (int i=0 ; i<opCount ; i++){
        if (operators[i] == '*' || operators[i] == '/'){
            if (operators[i] == '*'){
                numbers[i] = numbers[i]*numbers[i+1];
            } 
            else if (operators[i] == '/'){
                //Edge Case: Division By Zero
                if (numbers[i+1] == 0){
                    printf("Error: Division by Zero");
                    return;
                }
                numbers[i] = numbers[i]/numbers[i+1];
            }

            //Shift the numbers to left
            for (int j=i+1 ; j<numCount ; j++){
                numbers[j] = numbers[j+1];
            } numCount--;

            //Shift the operators to left
            for(int j=i ; j<opCount ; j++){
                operators[j] = operators[j+1];
            } opCount--;

            i--;
        } 
    }
    
    //Lower Precedence: + and -
    int result = numbers[0];
    for (int i=0 ; i<opCount ; i++){
        if (operators[i] == '+'){
            result += numbers[i+1];
        }
        else if (operators[i] == '-'){
            result -= numbers[i+1];
        }
    }

    printf("%d", result);

}

int main(){
    char input[1000];
    int numbers[1000];
    char operators[1000];

    printf("Enter Expression: ");
    fgets(input, sizeof(input), stdin);

    if (!validateString(input)){
        printf("Error: Invalid Expression");
        return 0;
    }

    if (!tokenizeExpression(input, numbers, operators)){
        return 0;
    }

    // printf("Validated String: %s",input);
    calculateResult(numbers, operators);

}
