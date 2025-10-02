//CRUD Operations on File

#include<stdio.h>
#include<string.h>
#include <stdlib.h>

struct User {
    int id;
    char name[100];
    int age;
};

//Operation 1 : Creating a New User and checking the unique ID constraint
void createUser(){
    struct User u;

    FILE *fp;
    char line[200];
    int idExists = 0;

    printf("Enter ID: ");
    scanf("%d", &u.id);
    getchar();

    fp = fopen("users.txt", "r");
    if (fp != NULL){
        while (fgets(line, sizeof(line), fp)){
            int currentId, age;
            char name[100];
            char *lastSpace = strrchr(line, ' ');

            if (lastSpace != NULL){
                *lastSpace = '\0';
                age = atoi(lastSpace+1);
                sscanf(line, "%d %[^\n]", &currentId, name);

                if (currentId == u.id){
                    idExists = 1;
                    break;
                }
            }
        }
        fclose(fp);
    }

    if (idExists){
        printf("Error: User with ID %d already exists!\n", u.id);
        return;
    }

    fp = fopen("users.txt", "a");
    if (fp == NULL){
        printf("Error creating file\n");
        return;
    }

    printf("Enter Name: ");
    fgets(u.name, sizeof(u.name), stdin);
    //Used to remove extra line with fgets
    u.name[strcspn(u.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &u.age);

    fprintf(fp, "%d %s %d\n", u.id, u.name, u.age);

    fclose(fp);
    printf("User Created Successfully\n");
}

//Operation 2 : Reading all the present users
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

//Operation 3 : Updating the user on the basis of ID
void updateUser(){
    FILE *fp, *temp;
    char line[200];
    int id, found=0;

    printf("Enter the ID of the user: ");
    scanf("%d", &id);

    fp = fopen("users.txt", "r");
    if (fp == NULL){
        printf("Error Opening File. Try creating a User!\n");
        return;
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

//Operation 4 : Deleting the user by id 
void deleteUser(){
    FILE *fp, *temp;
    char line[200];
    int id, found=0;

    printf("Enter the ID of the user to delete: ");
    scanf("%d", &id);

    fp = fopen("users.txt", "r");
    if (fp == NULL){
        printf("Error Opening File. Try creating a user!\n");
        return;

    }

    temp = fopen("temp.txt", "w");
    if (temp == NULL){
        printf("Error Opening Temp File!\n");
        fclose(fp);
        return;
    }

    while(fgets(line, sizeof(line), fp)){
        int currentId, age;
        char name[100];
        char *lastSpace = strrchr(line, ' ');

        if (lastSpace != NULL){
            *lastSpace = '\0';
            age = atoi(lastSpace+1);
            sscanf(line, "%d %[^\n]", &currentId, name);

            if (currentId ==id){
                found = 1;
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
        printf("User deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("User with ID %d not found.\n", id);
    }
}


int main(){
    int choice;

    while(1){
        printf("\nList of Operations: \n");
        printf("1. Create User \n2. Read Users \n3. Update User \n4. Delete User \n5. Exit\n");
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
            printf("Delete User chosen\n");
            deleteUser();
        }

        else if (choice==5){
            printf("Exiting");
            break;
        }

        else{
            printf("Wrong Choice Selected");
        }
    }

    return 0;
}