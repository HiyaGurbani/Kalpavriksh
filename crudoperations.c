//CRUD Operations on File

#include<stdio.h>
#include<string.h>
#include <stdlib.h>

struct User {
    int id;
    char name[100];
    int age;
};

void createUser(){
    struct User u;

    FILE *fp;
    fp = fopen("users.txt", "a");
    if (fp == NULL){
        printf("Error creating file\n");
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &u.id);
    getchar();

    printf("Enter Name: ");
    fgets(u.name, sizeof(u.name), stdin);
    //Used to remove extra line with fgets
    u.name[strcspn(u.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &u.age);

    //Check for unique

    fprintf(fp, "%d %s %d\n", u.id, u.name, u.age);

    fclose(fp);
    printf("User Created Successfully\n");
}

void readUser(){
    FILE *fp;
    char line[200];
    
    fp = fopen("users.txt", "r");
    if (fp == NULL){
        printf("Error Opening File. Try creating a User!\n");
        return;
    }

    if (fgets(line, sizeof(line), fp) == NULL){
        printf("There are no users. Kindly create a user!");
        fclose(fp);
        return;
    }

    printf("Users's List\n");
    printf("%s", line);
    while(fgets(line, sizeof(line), fp)){
        printf("%s",line);
    }

    fclose(fp);
}

void updateUser(){
    FILE *fp, *temp;
    char line[200];
    int id, found=0;

    printf("Enter the ID of the user: ");
    scanf("%d", &id);

    fp = fopen("users.txt", "r");
    if (fp == NULL){
        printf("Error Opening File. Try creating a User!\n");
    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL){
        printf("Error Opening Temp File!\n");
        fclose(fp);
        return;
    }

    while(fgets(line, sizeof(line), fp)){
        int currentId;
        char name[100];
        int age;
        char *lastSpace = strrchr(line, ' ');

        if (lastSpace != NULL){
            *lastSpace = '\0';
            age = atoi(lastSpace+1);
            sscanf(line,  "%d %[^\n]", &currentId, name);

            if (currentId == id){
                found = 1;
                char newName[100];
                int newAge;

                printf("Enter new name: ");
                getchar();
                fgets(newName, sizeof(newName), stdin);
                newName[strcspn(newName, "\n")] = '\0';

                printf("Enter new age: ");
                scanf("%d", &newAge);

                fprintf(temp, "%d %s %d\n", currentId, newName, newAge);
            } else {
                fprintf(temp, "%d %s %d\n", currentId, name, age);
            }
        }
    }

    fclose(fp);
    fclose(temp);

    if (found){
        remove("users.txt");
        rename("temp.txt", "users.txt");
        printf("User updated successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", id);
    }
}

int main(){
    int choice;

    printf("List of Operations: \n");
    printf("1. Create User \n2. Read Users \n3. Update User \n4. Delete User \n");
    printf("Enter the number of operation you want to perform: ");
    scanf("%d", &choice);

    if (choice==1){
        printf("Create User chosen\n");
        createUser();
    }

    else if (choice==2){
        printf("Read User chosen\n");
        readUser();
    }

    else if (choice==3){
        printf("Update User chosen\n");
        updateUser();
    }

    else if (choice==4){
        printf("Delete User chosen");
        deleteUser();
    }

    else printf("Wrong Choice Selected");

    return 0;
}