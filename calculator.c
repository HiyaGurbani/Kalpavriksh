#include<stdio.h>
#include<string.h>

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

int main(){
    char input[1000];

    printf("Enter Expression: ");
    fgets(input, sizeof(input), stdin);

    if (!validateString(input)){
        printf("Error: Invalid Expression");
        return 0;
    }

    printf("Validated String: %s",input);
}
