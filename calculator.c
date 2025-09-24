#include<stdio.h>
#include<string.h>
#include<ctype.h>

//Step 1: Function to validate string (only a character-level filter)
int validateString(char s[]){
    
    for (int i=0 ; i<strlen(s) ; i++){
        if (s[i] == '\n') continue;
        if (!((s[i]>='0' && s[i]<='9') || s[i]=='+' || s[i]=='-' || s[i]=='*' || s[i]=='/')){
            return 0;
        }
    }
    return 1;
}

//Step 2: Tokenization and Sequence Validation
//Handle whitespaces and new line in isdigit
int tokeniseExpression(char input[], int numbers[], char operators[]){
    int i=0 , n = strlen(input);
    int numCount=0, opCount=0;

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

    tokeniseExpression(input, numbers, operators);

    // printf("Validated String: %s",input);

}
